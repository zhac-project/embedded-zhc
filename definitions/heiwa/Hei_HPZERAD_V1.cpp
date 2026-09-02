// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Heiwa HPZERAD-V1 "Ernest" thermostat, OEM Eurevia
// (modelID "Thermostat_RF_Model_00000000000", manufacturerName "Eurevia").
// New vendor upstream in z2m v26.103.0 (#13050).
//
// Everything lives on hvacThermostat (0x0201) of ENDPOINT 25, as vendor
// attributes 0x040F..0x0476 (no manufacturer code). Flat exposes, no
// ExposeType::Climate (project rule). One converter decodes the lot:
//
//   0x0420 displayTemperature  s16 /10 → local_temperature + display_temperature
//   0x046A displayedSetpoint   u16 /10 → current_heating_setpoint
//   0x0422 humidity            u8       0x0424 co2 u16 (0 on variants without a sensor)
//   0x040F batteryVoltage      u16 /100 → battery_voltage (V) + battery (%,
//          the upstream step table: ≥6.00 100, ≥5.90 75, ≥5.80 50, ≥5.70 25, else 0)
//   0x0474 activeProfile       enum    → active_profile + profile + remote_power
//   0x001C systemMode          enum    → system_mode + remote_power (OFF when off)
//   0x0475 / 0x0476 demand icons       → zone_demand_icon
//   config attributes (offsets, display, setpoint limits, language, unit…)
//
// WRITES. `current_heating_setpoint` reproduces upstream exactly: one Write
// Attributes frame carrying occupiedCoolingSetpoint = (v - 1.5) * 100 and
// occupiedHeatingSetpoint = (v + 1.5) * 100 — the thermostat derives its
// displayed setpoint from that pair. 18..27 °C in 0.5 steps, else refused.
// `zone_demand_icon` writes both icon attributes in one frame, cooling
// first when switching to heating (upstream's order). Scaled numerics
// (offsets, brightness, setpoint limits) go through a local writer that
// applies the scale and range before the generic attribute encoder.
//
// NOT reproduced: upstream's read-back confirmation after each write (it
// re-reads the attribute and throws on mismatch), the `refresh` pseudo-key
// (a read burst) and the initial read-all at configure; `room_name` is
// read-only here (no string attribute write path).
//
// z2m-source: heiwa.ts #HPZERAD-V1.
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>
#include <string_view>
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiwa {
namespace {

using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::Value;
using ::zhc::ValueType;

constexpr std::uint8_t kEp = 25;

bool as_int(const Value* v, std::int64_t& out) {
    if (!v) return false;
    switch (v->type) {
        case ValueType::Int:  out = v->i; return true;
        case ValueType::Uint: out = static_cast<std::int64_t>(v->u); return true;
        case ValueType::Bool: out = v->b ? 1 : 0; return true;
        default: return false;
    }
}
void put_f(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key, float f) {
    Value o{}; o.type = ValueType::Float; o.f = f; out.put(key, o);
}
void put_i(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key, std::int64_t i) {
    Value o{}; o.type = ValueType::Int; o.i = i; out.put(key, o);
}
void put_b(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key, bool b) {
    Value o{}; o.type = ValueType::Bool; o.b = b; out.put(key, o);
}
void put_s(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key, const char* s) {
    Value o{}; o.type = ValueType::StringRef; o.str = s; out.put(key, o);
}

// Numeric attributes: raw / scale. Attribute ids as the decimal payload keys.
struct NumAttr { const char* key; const char* attr; float scale; };
constexpr NumAttr kNumeric[] = {
    { "local_temperature",        "1056", 10.f  },   // 0x0420
    { "display_temperature",      "1056", 10.f  },
    { "temperature_offset",       "1057", 10.f  },   // 0x0421
    { "humidity",                 "1058", 1.f   },   // 0x0422
    { "humidity_offset",          "1059", 1.f   },   // 0x0423
    { "co2",                      "1060", 1.f   },   // 0x0424
    { "co2_offset",               "1061", 1.f   },   // 0x0425
    { "display_brightness",       "1062", 5.f   },   // 0x0426
    { "setpoint_central",         "1122", 10.f  },   // 0x0462
    { "setpoint_maximum",         "1127", 10.f  },   // 0x0467
    { "setpoint_minimum",         "1128", 10.f  },   // 0x0468
    { "current_heating_setpoint", "1130", 10.f  },   // 0x046A
    { "battery_voltage",          "1039", 100.f },   // 0x040F
};

struct EnumAttr { const char* key; const char* attr; const char* const* labels; std::uint8_t count; };
constexpr const char* kLanguage[]  = { "french", "english" };
constexpr const char* kUnit[]      = { "celsius", "fahrenheit" };
constexpr const char* kShown[]     = { "hidden", "shown" };
constexpr const char* kSource[]    = { nullptr, "temperature_1", "temperature_2" };   // values 1 / 2
constexpr const char* kProfile[]   = { "off", "eco", "reduced", "comfort" };
constexpr const char* kZoneIcon[]  = { "none", "heating", "cooling" };
constexpr const char* kSystemMode[] = { "off", "auto", nullptr, "cool", "heat",
                                        "emergency_heating", "precooling", "fan_only", "dry", "sleep" };
constexpr EnumAttr kEnums[] = {
    { "language",                   "1044", kLanguage, 2 },   // 0x0414
    { "temperature_unit",           "1045", kUnit,     2 },   // 0x0415
    { "humidity_display",           "1089", kShown,    2 },   // 0x0441
    { "co2_display",                "1090", kShown,    2 },   // 0x0442
    { "display_temperature_source", "1121", kSource,   3 },   // 0x0461
};

bool fz_heiwa_thermostat(const DecodedMessage& msg, const FzConverter&,
                         const PreparedDefinition&, RuntimeContext&,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    std::int64_t raw = 0;

    for (const auto& n : kNumeric) {
        if (!as_int(msg.payload.find(n.attr), raw)) continue;
        if (n.scale == 1.f) put_i(out, n.key, raw);
        else                put_f(out, n.key, static_cast<float>(raw) / n.scale);
        emitted = true;
    }
    for (const auto& e : kEnums) {
        if (!as_int(msg.payload.find(e.attr), raw)) continue;
        if (raw < 0 || raw >= e.count || !e.labels[raw]) continue;
        put_s(out, e.key, e.labels[raw]);
        emitted = true;
    }
    // 0x040F battery voltage → percentage step table (upstream).
    if (as_int(msg.payload.find("1039"), raw)) {
        const float v = static_cast<float>(raw) / 100.f;
        put_i(out, "battery", v >= 6.0f ? 100 : v >= 5.9f ? 75 : v >= 5.8f ? 50 : v >= 5.7f ? 25 : 0);
        emitted = true;
    }
    // 0x0466 setpoint step: 0 → 1.0 °C, anything else → 0.5 °C.
    if (as_int(msg.payload.find("1126"), raw)) {
        put_f(out, "setpoint_step", raw == 0 ? 1.0f : 0.5f);
        emitted = true;
    }
    // 0x0471 setpoint-only UI flag.
    if (as_int(msg.payload.find("1137"), raw)) {
        put_b(out, "setpoint_only_ui", raw != 0);
        emitted = true;
    }
    // 0x0474 active profile → profile + active_profile + remote_power.
    if (as_int(msg.payload.find("1140"), raw) && raw >= 0 && raw < 4) {
        put_s(out, "profile",        kProfile[raw]);
        put_s(out, "active_profile", kProfile[raw]);
        put_s(out, "remote_power",   raw == 0 ? "OFF" : "ON");
        emitted = true;
    }
    // 0x001C systemMode → system_mode + remote_power.
    if (as_int(msg.payload.find("28"), raw) && raw >= 0 && raw < 10 && kSystemMode[raw]) {
        put_s(out, "system_mode",  kSystemMode[raw]);
        put_s(out, "remote_power", raw == 0 ? "OFF" : "ON");
        emitted = true;
    }
    // 0x0475 heating icon / 0x0476 cooling icon → zone_demand_icon.
    {
        std::int64_t heating = 0, cooling = 0;
        const bool h = as_int(msg.payload.find("1141"), heating);
        const bool c = as_int(msg.payload.find("1142"), cooling);
        if (h || c) {
            put_s(out, "zone_demand_icon", heating != 0 ? "heating" : cooling != 0 ? "cooling" : "none");
            emitted = true;
        }
    }
    // 0x0428 room name — read-only string.
    if (const Value* v = msg.payload.find("1064"); v && v->type == ValueType::StringRef && v->str) {
        put_s(out, "room_name", v->str);
        emitted = true;
    }
    return emitted;
}

constexpr FzConverter kFzHeiwaThermostat{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_heiwa_thermostat },
    .user_config       = nullptr,
};
const FzConverter* const kFz[] = { &kFzHeiwaThermostat };

// ── writes ───────────────────────────────────────────────────────────
bool as_float(const Value& in, float& f) {
    switch (in.type) {
        case ValueType::Float: f = in.f; return true;
        case ValueType::Int:   f = static_cast<float>(in.i); return true;
        case ValueType::Uint:  f = static_cast<float>(in.u); return true;
        default: return false;
    }
}

// Scaled numeric attribute: value * scale, range-checked, then the generic
// single-attribute Write Attributes encoder.
struct ScaledSpec { const char* key; std::uint16_t attr_id; std::uint8_t attr_type; float scale; float min; float max; };
constexpr ScaledSpec kScaledTemperatureOffset{ "temperature_offset", 0x0421, 0x29, 10.f, -5.f,    5.f    };
constexpr ScaledSpec kScaledHumidityOffset   { "humidity_offset",    0x0423, 0x29, 1.f,  -20.f,   20.f   };
constexpr ScaledSpec kScaledCo2Offset        { "co2_offset",         0x0425, 0x29, 1.f,  -1000.f, 1000.f };
constexpr ScaledSpec kScaledDisplayBrightness{ "display_brightness", 0x0426, 0x20, 5.f,  0.f,     10.f   };
constexpr ScaledSpec kScaledSetpointCentral  { "setpoint_central",   0x0462, 0x21, 10.f, 18.f,    27.f   };
constexpr ScaledSpec kScaledSetpointMinimum  { "setpoint_minimum",   0x0468, 0x21, 10.f, 18.f,    27.f   };
constexpr ScaledSpec kScaledSetpointMaximum  { "setpoint_maximum",   0x0467, 0x21, 10.f, 18.f,    27.f   };

bool tz_scaled(std::string_view key, const Value& input, const TzConverter& self,
               const PreparedDefinition& def, RuntimeContext& ctx,
               std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    const auto* s = static_cast<const ScaledSpec*>(self.user_config);
    if (!s || key != s->key) return false;
    float f;
    if (!as_float(input, f) || f < s->min || f > s->max) return false;
    Value v{}; v.type = ValueType::Int;
    v.i = static_cast<std::int64_t>(std::lround(f * s->scale));
    const ::zhc::generic::ZclWriteSpec spec{ s->key, s->attr_id, s->attr_type, 0, nullptr, 0 };
    const TzConverter tmp{ s->key, self.cluster, self.cluster_id, self.command_id,
                           &::zhc::generic::tz_zcl_write_attr, &spec };
    return ::zhc::generic::tz_zcl_write_attr(key, v, tmp, def, ctx, out_frame, out_size);
}

// setpoint_step: 1.0 °C → 0, 0.5 °C → 1 (enum8 0x0466).
bool tz_setpoint_step(std::string_view key, const Value& input, const TzConverter& self,
                      const PreparedDefinition& def, RuntimeContext& ctx,
                      std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    if (key != "setpoint_step") return false;
    float f;
    if (!as_float(input, f)) return false;
    Value v{}; v.type = ValueType::Uint;
    if (f == 1.0f)      v.u = 0;
    else if (f == 0.5f) v.u = 1;
    else return false;
    const ::zhc::generic::ZclWriteSpec spec{ "setpoint_step", 0x0466, 0x30, 0, nullptr, 0 };
    const TzConverter tmp{ "setpoint_step", self.cluster, self.cluster_id, self.command_id,
                           &::zhc::generic::tz_zcl_write_attr, &spec };
    return ::zhc::generic::tz_zcl_write_attr(key, v, tmp, def, ctx, out_frame, out_size);
}

// Two-record global Write Attributes: [fc 0x10][tsn][0x02] + records.
struct AttrRecord { std::uint16_t attr; std::uint8_t type; std::int32_t value; std::uint8_t len; };
bool write_records(std::span<std::uint8_t> out_frame, std::size_t& out_size,
                   const AttrRecord* recs, std::size_t n) {
    std::size_t total = 3;
    for (std::size_t i = 0; i < n; ++i) total += 3 + recs[i].len;
    if (out_frame.size() < total) return false;
    std::size_t p = 0;
    out_frame[p++] = 0x10; out_frame[p++] = 0x00; out_frame[p++] = 0x02;
    for (std::size_t i = 0; i < n; ++i) {
        out_frame[p++] = static_cast<std::uint8_t>(recs[i].attr & 0xFF);
        out_frame[p++] = static_cast<std::uint8_t>(recs[i].attr >> 8);
        out_frame[p++] = recs[i].type;
        for (std::uint8_t b = 0; b < recs[i].len; ++b)
            out_frame[p++] = static_cast<std::uint8_t>((static_cast<std::uint32_t>(recs[i].value) >> (8 * b)) & 0xFF);
    }
    out_size = total;
    return true;
}

// current_heating_setpoint: cooling = (v - 1.5) * 100, heating = (v + 1.5) * 100.
bool tz_setpoint(std::string_view key, const Value& input, const TzConverter&,
                 const PreparedDefinition&, RuntimeContext&,
                 std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    if (key != "current_heating_setpoint") return false;
    float f;
    if (!as_float(input, f)) return false;
    if (f < 18.f || f > 27.f) return false;
    if (std::lround(f * 2.f) != static_cast<long>(f * 2.f)) return false;   // 0.5 °C steps
    const AttrRecord recs[] = {
        { 0x0011, 0x29, static_cast<std::int32_t>(std::lround((f - 1.5f) * 100.f)), 2 },
        { 0x0012, 0x29, static_cast<std::int32_t>(std::lround((f + 1.5f) * 100.f)), 2 },
    };
    return write_records(out_frame, out_size, recs, 2);
}

// zone_demand_icon: heating (0x0475) + cooling (0x0476) icons in one frame.
bool tz_zone_demand_icon(std::string_view key, const Value& input, const TzConverter&,
                         const PreparedDefinition&, RuntimeContext&,
                         std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    if (key != "zone_demand_icon") return false;
    if (input.type != ValueType::StringRef || !input.str) return false;
    std::int32_t heating, cooling;
    if      (std::strcmp(input.str, "none")    == 0) { heating = 0; cooling = 0; }
    else if (std::strcmp(input.str, "heating") == 0) { heating = 1; cooling = 0; }
    else if (std::strcmp(input.str, "cooling") == 0) { heating = 0; cooling = 1; }
    else return false;
    // Upstream clears the cooling icon before raising heating, and the
    // reverse otherwise; a single frame keeps that record order.
    const AttrRecord heat_first[] = { { 0x0475, 0x30, heating, 1 }, { 0x0476, 0x30, cooling, 1 } };
    const AttrRecord cool_first[] = { { 0x0476, 0x30, cooling, 1 }, { 0x0475, 0x30, heating, 1 } };
    return write_records(out_frame, out_size, heating ? cool_first : heat_first, 2);
}

constexpr ::zhc::generic::ZclWriteLookup kLanguageLut[] = { {"french", 0}, {"english", 1} };
constexpr ::zhc::generic::ZclWriteLookup kUnitLut[]     = { {"celsius", 0}, {"fahrenheit", 1} };
constexpr ::zhc::generic::ZclWriteLookup kShownLut[]    = { {"hidden", 0}, {"shown", 1} };
constexpr ::zhc::generic::ZclWriteLookup kSourceLut[]   = { {"temperature_1", 1}, {"temperature_2", 2} };
constexpr ::zhc::generic::ZclWriteLookup kOnOffLut[]    = { {"OFF", 0}, {"ON", 1} };
constexpr ::zhc::generic::ZclWriteLookup kProfileLut[]  = { {"off", 0}, {"eco", 1}, {"reduced", 2}, {"comfort", 3} };

constexpr ::zhc::generic::ZclWriteSpec kSpecLanguage   { "language",                   0x0414, 0x30, 0, kLanguageLut, 2 };
constexpr ::zhc::generic::ZclWriteSpec kSpecUnit       { "temperature_unit",           0x0415, 0x30, 0, kUnitLut,     2 };
constexpr ::zhc::generic::ZclWriteSpec kSpecHumDisplay { "humidity_display",           0x0441, 0x30, 0, kShownLut,    2 };
constexpr ::zhc::generic::ZclWriteSpec kSpecCo2Display { "co2_display",                0x0442, 0x30, 0, kShownLut,    2 };
constexpr ::zhc::generic::ZclWriteSpec kSpecSource     { "display_temperature_source", 0x0461, 0x30, 0, kSourceLut,   2 };
constexpr ::zhc::generic::ZclWriteSpec kSpecOnlyUi     { "setpoint_only_ui",           0x0471, 0x30, 0, kOnOffLut,    2 };
constexpr ::zhc::generic::ZclWriteSpec kSpecProfile    { "profile",                    0x0473, 0x30, 0, kProfileLut,  4 };

#define HEIWA_TZ(NAME, KEY, FN, CFG) \
    constexpr TzConverter NAME{ KEY, "hvacThermostat", 0x0201, 0x02, FN, CFG };
HEIWA_TZ(kTzLanguage,          "language",                   &::zhc::generic::tz_zcl_write_attr, &kSpecLanguage)
HEIWA_TZ(kTzUnit,              "temperature_unit",           &::zhc::generic::tz_zcl_write_attr, &kSpecUnit)
HEIWA_TZ(kTzHumDisplay,        "humidity_display",           &::zhc::generic::tz_zcl_write_attr, &kSpecHumDisplay)
HEIWA_TZ(kTzCo2Display,        "co2_display",                &::zhc::generic::tz_zcl_write_attr, &kSpecCo2Display)
HEIWA_TZ(kTzSource,            "display_temperature_source", &::zhc::generic::tz_zcl_write_attr, &kSpecSource)
HEIWA_TZ(kTzOnlyUi,            "setpoint_only_ui",           &::zhc::generic::tz_zcl_write_attr, &kSpecOnlyUi)
HEIWA_TZ(kTzProfile,           "profile",                    &::zhc::generic::tz_zcl_write_attr, &kSpecProfile)
HEIWA_TZ(kTzTemperatureOffset, "temperature_offset",         &tz_scaled, &kScaledTemperatureOffset)
HEIWA_TZ(kTzHumidityOffset,    "humidity_offset",            &tz_scaled, &kScaledHumidityOffset)
HEIWA_TZ(kTzCo2Offset,         "co2_offset",                 &tz_scaled, &kScaledCo2Offset)
HEIWA_TZ(kTzDisplayBrightness, "display_brightness",         &tz_scaled, &kScaledDisplayBrightness)
HEIWA_TZ(kTzSetpointCentral,   "setpoint_central",           &tz_scaled, &kScaledSetpointCentral)
HEIWA_TZ(kTzSetpointMinimum,   "setpoint_minimum",           &tz_scaled, &kScaledSetpointMinimum)
HEIWA_TZ(kTzSetpointMaximum,   "setpoint_maximum",           &tz_scaled, &kScaledSetpointMaximum)
HEIWA_TZ(kTzSetpointStep,      "setpoint_step",              &tz_setpoint_step, nullptr)
HEIWA_TZ(kTzSetpoint,          "current_heating_setpoint",   &tz_setpoint, nullptr)
HEIWA_TZ(kTzZoneDemandIcon,    "zone_demand_icon",           &tz_zone_demand_icon, nullptr)
#undef HEIWA_TZ

const TzConverter* const kTz[] = {
    &kTzSetpoint, &kTzProfile, &kTzZoneDemandIcon,
    &kTzTemperatureOffset, &kTzHumidityOffset, &kTzCo2Offset, &kTzDisplayBrightness,
    &kTzLanguage, &kTzUnit, &kTzHumDisplay, &kTzCo2Display, &kTzSource, &kTzOnlyUi,
    &kTzSetpointCentral, &kTzSetpointMinimum, &kTzSetpointMaximum, &kTzSetpointStep,
};

constexpr const char* kDeg = "\xC2\xB0""C";
constexpr Expose kExposes[] = {
    { "local_temperature",        ExposeType::Numeric, Access::State,    kDeg, "Temperature used by the stock display", nullptr, 0 },
    { "current_heating_setpoint", ExposeType::Numeric, Access::StateSet, kDeg, "Setpoint currently displayed by the thermostat", nullptr, 0, ExposeCategory::State, 18, 27, 1 },
    { "system_mode",              ExposeType::Enum,    Access::State,    nullptr, nullptr, kSystemMode, 10 },
    { "humidity",                 ExposeType::Numeric, Access::State,    "%",  "Measured relative humidity", nullptr, 0 },
    { "co2",                      ExposeType::Numeric, Access::State,    "ppm", "Remains zero on variants without a CO2 sensor", nullptr, 0 },
    { "display_temperature",      ExposeType::Numeric, Access::State,    kDeg, "Temperature used by the stock display", nullptr, 0 },
    { "profile",                  ExposeType::Enum,    Access::StateSet, nullptr, "Request a stock thermostat profile", kProfile, 4, ExposeCategory::Config },
    { "active_profile",           ExposeType::Enum,    Access::State,    nullptr, "Stock thermostat profile currently active", kProfile, 4 },
    { "remote_power",             ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0 },
    { "zone_demand_icon",         ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kZoneIcon, 3 },
    { "temperature_offset",       ExposeType::Numeric, Access::StateSet, kDeg, "Temperature sensor offset", nullptr, 0, ExposeCategory::Config, -5, 5, 1 },
    { "humidity_offset",          ExposeType::Numeric, Access::StateSet, "%",  "Humidity sensor offset", nullptr, 0, ExposeCategory::Config, -20, 20, 1 },
    { "co2_offset",               ExposeType::Numeric, Access::StateSet, "ppm", "CO2 sensor offset", nullptr, 0, ExposeCategory::Config, -1000, 1000, 1 },
    { "display_brightness",       ExposeType::Numeric, Access::StateSet, nullptr, "Stock display brightness", nullptr, 0, ExposeCategory::Config, 0, 10, 1 },
    { "language",                 ExposeType::Enum,    Access::StateSet, nullptr, "Language used by the stock display", kLanguage, 2, ExposeCategory::Config },
    { "temperature_unit",         ExposeType::Enum,    Access::StateSet, nullptr, "Temperature unit used by the stock display", kUnit, 2, ExposeCategory::Config },
    { "humidity_display",         ExposeType::Enum,    Access::StateSet, nullptr, "Whether humidity is shown on the stock display", kShown, 2, ExposeCategory::Config },
    { "co2_display",              ExposeType::Enum,    Access::StateSet, nullptr, "Whether CO2 is shown on the stock display", kShown, 2, ExposeCategory::Config },
    { "display_temperature_source", ExposeType::Enum,  Access::StateSet, nullptr, "Temperature source used by the stock display", kSource + 1, 2, ExposeCategory::Config },
    { "setpoint_only_ui",         ExposeType::Binary,  Access::StateSet, nullptr, "Hide the local M/profile menu while keeping the setpoint arrows", nullptr, 0, ExposeCategory::Config },
    { "setpoint_central",         ExposeType::Numeric, Access::StateSet, kDeg, "Central setpoint", nullptr, 0, ExposeCategory::Config, 18, 27, 1 },
    { "setpoint_minimum",         ExposeType::Numeric, Access::StateSet, kDeg, "Minimum local setpoint", nullptr, 0, ExposeCategory::Config, 18, 27, 1 },
    { "setpoint_maximum",         ExposeType::Numeric, Access::StateSet, kDeg, "Maximum local setpoint", nullptr, 0, ExposeCategory::Config, 18, 27, 1 },
    { "setpoint_step",            ExposeType::Numeric, Access::StateSet, kDeg, "Local setpoint increment (0.5 or 1)", nullptr, 0, ExposeCategory::Config, 0, 1, 1 },
    { "room_name",                ExposeType::String,  Access::State,    nullptr, "Room name shown by the stock UI", nullptr, 0, ExposeCategory::Config },
    { "battery",                  ExposeType::Numeric, Access::State,    "%",  "Estimated remaining battery percentage", nullptr, 0, ExposeCategory::Diagnostic },
    { "battery_voltage",          ExposeType::Numeric, Access::State,    "V",  "Reported battery voltage", nullptr, 0, ExposeCategory::Diagnostic },
};

constexpr BindingSpec kBindings[] = { { kEp, 0x0201 } };
// Upstream reporting: displayTemperature MIN..1h Δ0.1 °C, humidity 10s..1h Δ1,
// displayedSetpoint 1s..MAX Δ0.5 °C, activeProfile 1s..MAX on change.
constexpr ::zhc::ReportingSpec kReports[] = {
    { kEp, 0x0201, 0x0420, 0x29, 0,  3600,  1, 0 },
    { kEp, 0x0201, 0x0422, 0x20, 10, 3600,  1, 0 },
    { kEp, 0x0201, 0x046A, 0x21, 1,  62000, 5, 0 },
    { kEp, 0x0201, 0x0474, 0x30, 1,  62000, 0, 0 },
};

constexpr const char* kModels[] = { "Thermostat_RF_Model_00000000000" };
constexpr const char* kManus[]  = { "Eurevia" };

}  // namespace

extern const PreparedDefinition kDef_HPZERAD_V1{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="HPZERAD-V1", .vendor="Heiwa",
    .meta=nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .reports=kReports, .reports_count=sizeof(kReports)/sizeof(kReports[0]),
    .default_endpoint=kEp,
};

}  // namespace zhc::devices::heiwa
