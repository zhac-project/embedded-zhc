// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: PirogovX ZB-MIDEA-AC — Zigbee air-conditioner controller for
// Midea / Royal Clima / Hommyn / Neoline (ESP32-H2/C6 firmware).
// New vendor upstream in z2m v26.99.0.
//
// The controller puts its ENTIRE surface on the standard Analog Input
// cluster (genAnalogInput, 0x000C) as manufacturer attributes
// 0xF000..0xF009 — there is no hvacThermostat traffic for anything but
// the optional bind. So one codec reads every attribute and one
// write-attribute tz per settable key covers the control surface.
//
//   0xF000 bool  power           -> state ON/OFF
//   0xF001 u8    mode            -> system_mode  off/auto/cool/heat/dry/fan_only
//   0xF002 u8    fan mode        -> fan_mode     auto/low/medium/high/quiet
//   0xF003 u8    swing mode      -> swing_mode   off/horizontal/vertical/both
//   0xF004 u8    preset          -> preset       none/sleep/turbo
//   0xF005 bool  display         -> display ON/OFF
//   0xF006 num   indoor temp     -> local_temperature
//   0xF007 num   outdoor temp    -> outdoor_temperature
//   0xF008 num   target temp     -> occupied_heating_setpoint (16..30 step 1)
//   0xF009 str   firmware        -> firmware_version
//
// z2m also lists `tz.thermostat_system_mode` and
// `tz.thermostat_occupied_heating_setpoint` — the stock hvacThermostat
// writers — alongside its own analog-input ones. The device's own
// converters are the ones that actually reach the firmware, so those
// two are wired here to the analog-input path rather than to
// kTzThermostat, which would write cluster 0x0201 the firmware does not
// serve.
//
// z2m-source: pirogovx.ts #ZB-MIDEA-AC.
#include "definitions/_generic/_shared.hpp"

#include <cstring>

namespace zhc::devices::pirogovx {
namespace {

// genAnalogInput manufacturer attributes, decimal-string keys as the
// parser formats unknown ids.
constexpr const char* kKeyPower    = "61440";   // 0xF000
constexpr const char* kKeyMode     = "61441";   // 0xF001
constexpr const char* kKeyFan      = "61442";   // 0xF002
constexpr const char* kKeySwing    = "61443";   // 0xF003
constexpr const char* kKeyPreset   = "61444";   // 0xF004
constexpr const char* kKeyDisplay  = "61445";   // 0xF005
constexpr const char* kKeyIndoor   = "61446";   // 0xF006
constexpr const char* kKeyOutdoor  = "61447";   // 0xF007
constexpr const char* kKeyTarget   = "61448";   // 0xF008
constexpr const char* kKeyFirmware = "61449";   // 0xF009

constexpr const char* kModeValues[]   = {"off", "auto", "cool", "heat", "dry", "fan_only"};
constexpr const char* kFanValues[]    = {"auto", "low", "medium", "high", "quiet"};
constexpr const char* kSwingValues[]  = {"off", "horizontal", "vertical", "both"};
constexpr const char* kPresetValues[] = {"none", "sleep", "turbo"};

template <std::size_t N>
const char* lookup(const char* const (&table)[N], std::int64_t id, const char* fallback) {
    if (id < 0 || static_cast<std::size_t>(id) >= N) return fallback;
    return table[static_cast<std::size_t>(id)];
}

template <std::size_t N>
bool reverse_lookup(const char* const (&table)[N], const char* name, std::uint8_t& id) {
    if (!name) return false;
    for (std::size_t i = 0; i < N; ++i) {
        if (std::strcmp(table[i], name) == 0) { id = static_cast<std::uint8_t>(i); return true; }
    }
    return false;
}

bool as_int(const Value* v, std::int64_t& out) {
    if (!v) return false;
    if      (v->type == ValueType::Uint)  out = static_cast<std::int64_t>(v->u);
    else if (v->type == ValueType::Int)   out = v->i;
    else if (v->type == ValueType::Bool)  out = v->b ? 1 : 0;
    else if (v->type == ValueType::Float) out = static_cast<std::int64_t>(v->f);
    else return false;
    return true;
}

// Temperatures arrive as plain values (no 0.01 scaling — the firmware
// reports whole degrees on an analog-input attribute, not the
// hvacThermostat 0.01 °C convention), so they are forwarded as-is.
void put_number(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
                 const char* key, const Value* v) {
    if (!v) return;
    Value n{};
    if (v->type == ValueType::Float) { n.type = ValueType::Float; n.f = v->f; }
    else {
        std::int64_t raw = 0;
        if (!as_int(v, raw)) return;
        n.type = ValueType::Int; n.i = raw;
    }
    out.put(key, n);
}

bool fz_ac_analog(const DecodedMessage& msg,
                   const FzConverter&,
                   const PreparedDefinition&,
                   RuntimeContext&,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    std::int64_t raw = 0;

    if (as_int(msg.payload.find(kKeyPower), raw)) {
        Value s{}; s.type = ValueType::Bool; s.b = (raw != 0);
        out.put("state", s);
        emitted = true;
    }
    if (as_int(msg.payload.find(kKeyMode), raw)) {
        Value s{}; s.type = ValueType::StringRef;
        s.str = lookup(kModeValues, raw, "auto");
        out.put("system_mode", s);
        emitted = true;
    }
    if (as_int(msg.payload.find(kKeyFan), raw)) {
        Value s{}; s.type = ValueType::StringRef;
        s.str = lookup(kFanValues, raw, "auto");
        out.put("fan_mode", s);
        emitted = true;
    }
    if (as_int(msg.payload.find(kKeySwing), raw)) {
        Value s{}; s.type = ValueType::StringRef;
        s.str = lookup(kSwingValues, raw, "off");
        out.put("swing_mode", s);
        emitted = true;
    }
    if (as_int(msg.payload.find(kKeyPreset), raw)) {
        Value s{}; s.type = ValueType::StringRef;
        s.str = lookup(kPresetValues, raw, "none");
        out.put("preset", s);
        emitted = true;
    }
    if (as_int(msg.payload.find(kKeyDisplay), raw)) {
        Value s{}; s.type = ValueType::Bool; s.b = (raw != 0);
        out.put("display", s);
        emitted = true;
    }
    if (const Value* v = msg.payload.find(kKeyIndoor))  { put_number(out, "local_temperature", v); emitted = true; }
    if (const Value* v = msg.payload.find(kKeyOutdoor)) { put_number(out, "outdoor_temperature", v); emitted = true; }
    if (const Value* v = msg.payload.find(kKeyTarget))  { put_number(out, "occupied_heating_setpoint", v); emitted = true; }
    if (const Value* v = msg.payload.find(kKeyFirmware)) {
        if (v->type == ValueType::StringRef && v->str) {
            Value s{}; s.type = ValueType::StringRef; s.str = v->str;
            out.put("firmware_version", s);
            emitted = true;
        }
    }
    return emitted;
}

// ── write path ───────────────────────────────────────────────────────
//
// Every setter is a plain ZCL writeAttributes on genAnalogInput:
//   FC 0x10 (profile-wide) | TSN (adapter fills) | cmd 0x02
//   attrId LE (2) | dataType (1) | value (1)
// Boolean attributes use type 0x10, the enums 0x20 (uint8) — matching
// z2m's `dataType` table.
struct AcAttrSpec {
    std::uint16_t attr_id;
    std::uint8_t  data_type;      // 0x10 boolean, 0x20 uint8
    const char* const* table;     // enum table, or nullptr for bool/raw
    std::size_t   table_count;
};

constexpr AcAttrSpec kSpecState   {0xF000, 0x10, nullptr, 0};
constexpr AcAttrSpec kSpecMode    {0xF001, 0x20, kModeValues,   sizeof(kModeValues)/sizeof(kModeValues[0])};
constexpr AcAttrSpec kSpecFan     {0xF002, 0x20, kFanValues,    sizeof(kFanValues)/sizeof(kFanValues[0])};
constexpr AcAttrSpec kSpecSwing   {0xF003, 0x20, kSwingValues,  sizeof(kSwingValues)/sizeof(kSwingValues[0])};
constexpr AcAttrSpec kSpecPreset  {0xF004, 0x20, kPresetValues, sizeof(kPresetValues)/sizeof(kPresetValues[0])};
constexpr AcAttrSpec kSpecDisplay {0xF005, 0x10, nullptr, 0};
constexpr AcAttrSpec kSpecTarget  {0xF008, 0x20, nullptr, 0};

bool encode_value(const AcAttrSpec& spec, const Value& input, std::uint8_t& encoded) {
    if (spec.table) {
        if (input.type == ValueType::StringRef && input.str) {
            for (std::size_t i = 0; i < spec.table_count; ++i) {
                if (std::strcmp(spec.table[i], input.str) == 0) {
                    encoded = static_cast<std::uint8_t>(i);
                    return true;
                }
            }
            return false;   // z2m throws on an unsupported label
        }
        // The SPA's device.attr.set path can forward the ordinal directly.
        if (input.type == ValueType::Uint && input.u < spec.table_count) {
            encoded = static_cast<std::uint8_t>(input.u);
            return true;
        }
        return false;
    }

    if (spec.data_type == 0x10) {
        if (input.type == ValueType::Bool) { encoded = input.b ? 1 : 0; return true; }
        if (input.type == ValueType::StringRef && input.str) {
            if (std::strcmp(input.str, "ON")  == 0) { encoded = 1; return true; }
            if (std::strcmp(input.str, "OFF") == 0) { encoded = 0; return true; }
            return false;
        }
        if (input.type == ValueType::Uint) { encoded = input.u ? 1 : 0; return true; }
        if (input.type == ValueType::Int)  { encoded = input.i ? 1 : 0; return true; }
        return false;
    }

    // Plain uint8 (the setpoint). Clamp to the exposed 16..30 range so a
    // stray value cannot drive the AC somewhere it will reject.
    std::int64_t raw = 0;
    if (!as_int(&input, raw)) return false;
    if (raw < 16) raw = 16;
    if (raw > 30) raw = 30;
    encoded = static_cast<std::uint8_t>(raw);
    return true;
}

bool tz_ac_write(std::string_view,
                  const Value& input,
                  const TzConverter& self,
                  const PreparedDefinition&,
                  RuntimeContext&,
                  std::span<std::uint8_t> out_frame,
                  std::size_t& out_size) {
    const auto* spec = static_cast<const AcAttrSpec*>(self.user_config);
    if (!spec) return false;

    std::uint8_t encoded = 0;
    if (!encode_value(*spec, input, encoded)) return false;

    if (out_frame.size() < 7) return false;
    out_frame[0] = 0x10;                                          // FC: profile-wide
    out_frame[1] = 0;                                             // TSN (adapter fills)
    out_frame[2] = 0x02;                                          // writeAttributes
    out_frame[3] = static_cast<std::uint8_t>(spec->attr_id & 0xFF);
    out_frame[4] = static_cast<std::uint8_t>(spec->attr_id >> 8);
    out_frame[5] = spec->data_type;
    out_frame[6] = encoded;
    out_size = 7;
    return true;
}

const FzConverter kFzAcAnalog{
    .family            = FrameFamily::Zcl,
    .cluster           = "genAnalogInput",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_ac_analog },
    .user_config       = nullptr,
};

#define ZHC_AC_TZ(sym, keyname, spec)               \
    const TzConverter sym{                          \
        .key         = keyname,                     \
        .cluster     = "genAnalogInput",            \
        .cluster_id  = 0x000C,                      \
        .command_id  = 0x02,                        \
        .fn          = tz_ac_write,                 \
        .user_config = &spec,                       \
    }

ZHC_AC_TZ(kTzAcState,   "state",                     kSpecState);
ZHC_AC_TZ(kTzAcMode,    "system_mode",               kSpecMode);
ZHC_AC_TZ(kTzAcFan,     "fan_mode",                  kSpecFan);
ZHC_AC_TZ(kTzAcSwing,   "swing_mode",                kSpecSwing);
ZHC_AC_TZ(kTzAcPreset,  "preset",                    kSpecPreset);
ZHC_AC_TZ(kTzAcDisplay, "display",                   kSpecDisplay);
ZHC_AC_TZ(kTzAcTarget,  "occupied_heating_setpoint", kSpecTarget);

#undef ZHC_AC_TZ

const FzConverter* const kFz_ZB_MIDEA_AC[] = { &kFzAcAnalog };
const TzConverter* const kTz_ZB_MIDEA_AC[] = {
    &kTzAcState, &kTzAcMode, &kTzAcFan, &kTzAcSwing,
    &kTzAcPreset, &kTzAcDisplay, &kTzAcTarget,
};

constexpr const char* kModels_ZB_MIDEA_AC[] = { "ZB-MIDEA-AC" };
constexpr const char* kManu_ZB_MIDEA_AC[]   = { "PirogovX" };

constexpr Expose kExposes_ZB_MIDEA_AC[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State,    "C",     nullptr, nullptr, 0},
    {"occupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C",
     nullptr, nullptr, 0, ExposeCategory::State, 16, 30, 1},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr,
     kModeValues, sizeof(kModeValues)/sizeof(kModeValues[0])},
    {"fan_mode", ExposeType::Enum, Access::StateSet, nullptr, "Fan speed",
     kFanValues, sizeof(kFanValues)/sizeof(kFanValues[0])},
    {"swing_mode", ExposeType::Enum, Access::StateSet, nullptr, "Swing mode",
     kSwingValues, sizeof(kSwingValues)/sizeof(kSwingValues[0])},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, "Preset mode",
     kPresetValues, sizeof(kPresetValues)/sizeof(kPresetValues[0])},
    {"display", ExposeType::Binary, Access::StateSet, nullptr,
     "AC display and beep control", nullptr, 0, ExposeCategory::Config},
    {"outdoor_temperature", ExposeType::Numeric, Access::State, "C",
     "Outdoor unit temperature", nullptr, 0},
    {"firmware_version", ExposeType::String, Access::State, nullptr,
     "AC controller firmware version", nullptr, 0, ExposeCategory::Diagnostic},
};

constexpr BindingSpec kBindings_ZB_MIDEA_AC[] = {
    {1, 0x000C},   // genAnalogInput — carries the whole surface
    {1, 0x0201},   // hvacThermostat — optional in z2m, tolerated if it fails
};

}  // namespace

extern const PreparedDefinition kDef_ZB_MIDEA_AC{
    .zigbee_models=kModels_ZB_MIDEA_AC,
    .zigbee_models_count=sizeof(kModels_ZB_MIDEA_AC)/sizeof(kModels_ZB_MIDEA_AC[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManu_ZB_MIDEA_AC,
    .manufacturer_names_count=sizeof(kManu_ZB_MIDEA_AC)/sizeof(kManu_ZB_MIDEA_AC[0]),
    .model="ZB-MIDEA-AC", .vendor="PirogovX",
    .meta=nullptr,
    .exposes=kExposes_ZB_MIDEA_AC,
    .exposes_count=sizeof(kExposes_ZB_MIDEA_AC)/sizeof(kExposes_ZB_MIDEA_AC[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB_MIDEA_AC, .from_zigbee_count=sizeof(kFz_ZB_MIDEA_AC)/sizeof(kFz_ZB_MIDEA_AC[0]),
    .to_zigbee=kTz_ZB_MIDEA_AC, .to_zigbee_count=sizeof(kTz_ZB_MIDEA_AC)/sizeof(kTz_ZB_MIDEA_AC[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_MIDEA_AC,
    .bindings_count=sizeof(kBindings_ZB_MIDEA_AC)/sizeof(kBindings_ZB_MIDEA_AC[0]),
};

}  // namespace zhc::devices::pirogovx
