// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Elko 4523430 — hand-rewritten (was wrong-bundle: battery+state stub).
// ESH Plus Super TR RF PH (mains heater thermostat).
// z2m-source: elko.ts #4523430 — addElkoToHvacThermostatCluster +
//             elkoThermostat. Elko-private attrs live on the standard
//             hvacThermostat cluster (0x0201) at IDs 0x0401..0x0419 with
//             NO manufacturer code (write fc=0x10).
//
// elko attr id table (z2m: elkoExtend.addElkoToHvacThermostatCluster):
//   0x0401 elkoLoad           u16  → "load" (W)
//   0x0402 elkoDisplayText    str  → "display_text" (R/W via tz_text)
//   0x0403 elkoSensor         enum → "sensor" {air,floor,supervisor_floor}
//   0x0404 elkoRegulatorTime  u8   → "regulator_time" (min)
//   0x0405 elkoRegulatorMode  bool → "regulator_mode" {regulator,thermostat}
//   0x0406 elkoPowerStatus    bool → "system_mode" {off,heat}  (z2m alias)
//   0x0408 elkoMeanPower      u16  → "mean_power" (W)
//   0x0409 elkoExternalTemp   i16  → "floor_temp" (°C, scale 100)
//   0x0411 elkoNightSwitching bool → "night_switching"
//   0x0412 elkoFrostGuard     bool → "frost_guard"
//   0x0413 elkoChildLock      bool → "child_lock"
//   0x0414 elkoMaxFloorTemp   u8   → "max_floor_temp" (°C)
//   0x0415 elkoRelayState     bool → "running_state" {idle,heat}
//   0x0417 elkoCalibration    i8   → "local_temperature_calibration"
//                                    (z2m: round(value/10))
//
// NOTE on z2m PARTIAL parity:
//   * z2m's "system_mode" tz writes elkoPowerStatus (0x0406) — handled
//     here via kTzElkoSystemMode (overrides the generic kTzThermostat
//     system_mode path which writes the standard 0x001C).
//   * z2m's "local_temperature_calibration" tz writes elkoCalibration
//     (0x0417) scaled by 10 — handled here by writing the i8 raw; the
//     adapter must scale on the input side. UNPORTED: the *10 scaling.
//   * supervisor_floor temperature display fallback (z2m fz_thermostat
//     overrides local_temperature when sensor==supervisor_floor) is NOT
//     replicated — host emits raw local_temperature + floor_temp
//     separately; consumer overlays. UNPORTED.
//   * z2m's derived "power" (load*relay_state) is NOT computed in fz —
//     UNPORTED.
//   * elkoDisplayText is a CHAR_STR write; tz_zcl_write_attr only
//     supports up to u32 scalar values. BLOCKED — see TODO below.

#include "definitions/_generic/_shared.hpp"
#include <cstdio>

namespace zhc::devices::elko {
namespace {

// ── Elko hvacThermostat attribute → state-key decoder ──────────────
//
// Tiny in-file map decoder. Walks every elko-private attr id reported
// on hvacThermostat (0x0201) and emits the corresponding state key.
//
// Why a per-device decoder: only Super TR uses these attrs; promoting
// to definitions/elko/_shared.{hpp,cpp} buys nothing today (single
// consumer, <120 LOC).

enum class ElkoFzKind : std::uint8_t {
    Raw,    // pass through Value as-is
    Bool,   // numeric → bool
    Enum,   // numeric → label
    Mode,   // numeric → "heat"/"off" (PowerStatus) or "idle"/"heat" (Relay)
};

struct ElkoEnumPair {
    std::uint64_t value;
    const char*   label;
};

struct ElkoFzEntry {
    std::uint16_t       attr_id;
    const char*         out_key;
    ElkoFzKind          kind;
    const ElkoEnumPair* enums;
    std::uint8_t        enum_count;
};

constexpr ElkoEnumPair kEnumSensor[] = {
    { 0, "air" }, { 1, "floor" }, { 3, "supervisor_floor" },
};
constexpr ElkoEnumPair kEnumRegulatorMode[] = {
    { 0, "thermostat" }, { 1, "regulator" },
};
constexpr ElkoEnumPair kEnumOnOff[] = {
    { 0, "off" }, { 1, "on" },
};
constexpr ElkoEnumPair kEnumPowerStatus[] = {
    { 0, "off" }, { 1, "heat" },
};
constexpr ElkoEnumPair kEnumRelayState[] = {
    { 0, "idle" }, { 1, "heat" },
};

constexpr ElkoFzEntry kElkoFzMap[] = {
    { 0x0401, "load",                ElkoFzKind::Raw,  nullptr,             0 },
    { 0x0403, "sensor",              ElkoFzKind::Enum, kEnumSensor,         3 },
    { 0x0404, "regulator_time",      ElkoFzKind::Raw,  nullptr,             0 },
    { 0x0405, "regulator_mode",      ElkoFzKind::Enum, kEnumRegulatorMode,  2 },
    { 0x0406, "system_mode",         ElkoFzKind::Enum, kEnumPowerStatus,    2 },
    { 0x0408, "mean_power",          ElkoFzKind::Raw,  nullptr,             0 },
    { 0x0409, "floor_temp",          ElkoFzKind::Raw,  nullptr,             0 },
    { 0x0411, "night_switching",     ElkoFzKind::Enum, kEnumOnOff,          2 },
    { 0x0412, "frost_guard",         ElkoFzKind::Enum, kEnumOnOff,          2 },
    { 0x0413, "child_lock",          ElkoFzKind::Bool, nullptr,             0 },
    { 0x0414, "max_floor_temp",      ElkoFzKind::Raw,  nullptr,             0 },
    { 0x0415, "running_state",       ElkoFzKind::Enum, kEnumRelayState,     2 },
    { 0x0417, "local_temperature_calibration",
                                     ElkoFzKind::Raw,  nullptr,             0 },
};

bool elko_decode_value(const ElkoFzEntry& e,
                        const Value& v,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value o{};
    switch (e.kind) {
        case ElkoFzKind::Raw:
            o = v;
            break;
        case ElkoFzKind::Bool: {
            bool b;
            if      (v.type == ValueType::Bool) b = v.b;
            else if (v.type == ValueType::Uint) b = v.u != 0;
            else if (v.type == ValueType::Int)  b = v.i != 0;
            else return false;
            o.type = ValueType::Bool; o.b = b;
            break;
        }
        case ElkoFzKind::Enum:
        case ElkoFzKind::Mode: {
            if (!e.enums) return false;
            std::uint64_t raw = 0;
            if      (v.type == ValueType::Uint) raw = v.u;
            else if (v.type == ValueType::Int)  raw = static_cast<std::uint64_t>(v.i);
            else if (v.type == ValueType::Bool) raw = v.b ? 1 : 0;
            else return false;
            const char* label = nullptr;
            for (std::uint8_t i = 0; i < e.enum_count; ++i) {
                if (e.enums[i].value == raw) { label = e.enums[i].label; break; }
            }
            if (!label) return false;
            o.type = ValueType::StringRef; o.str = label;
            break;
        }
    }
    out.put(e.out_key, o);
    return true;
}

bool fz_elko_thermostat(const DecodedMessage& msg,
                         const FzConverter&,
                         const PreparedDefinition&,
                         RuntimeContext&,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    constexpr std::size_t kMapCount = sizeof(kElkoFzMap) / sizeof(kElkoFzMap[0]);
    for (std::size_t i = 0; i < kMapCount; ++i) {
        const auto& e = kElkoFzMap[i];
        char keybuf[8];
        std::snprintf(keybuf, sizeof(keybuf), "%u",
                       static_cast<unsigned>(e.attr_id));
        const Value* v = msg.payload.find(keybuf);
        if (!v) continue;
        if (elko_decode_value(e, *v, out)) emitted = true;
    }
    return emitted;
}

}  // namespace

extern const FzConverter kFzElkoThermostat{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport)
                       | ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_elko_thermostat },
    .user_config       = nullptr,
};

namespace {

// ── ZclWriteSpec table — manufacturer_code = 0 (standard ZCL fc=0x10) ─

constexpr ::zhc::generic::ZclWriteLookup kLookupSensor[] = {
    {"air", 0}, {"floor", 1}, {"supervisor_floor", 3},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupRegulatorMode[] = {
    {"thermostat", 0}, {"regulator", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupOnOff[] = {
    {"off", 0}, {"on", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupPowerStatus[] = {
    {"off", 0}, {"heat", 1},
};

// attr_type values: 0x10 bool, 0x20 u8, 0x21 u16, 0x28 i8,
// 0x29 i16, 0x30 enum8.
constexpr ::zhc::generic::ZclWriteSpec kSpecLoad{
    "load",          0x0401, 0x21, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecSensor{
    "sensor",        0x0403, 0x30, 0,
    kLookupSensor, sizeof(kLookupSensor)/sizeof(kLookupSensor[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRegulatorTime{
    "regulator_time", 0x0404, 0x20, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRegulatorMode{
    "regulator_mode", 0x0405, 0x10, 0,
    kLookupRegulatorMode,
    sizeof(kLookupRegulatorMode)/sizeof(kLookupRegulatorMode[0]),
};
// system_mode override → elkoPowerStatus (0x0406) bool.
constexpr ::zhc::generic::ZclWriteSpec kSpecSystemMode{
    "system_mode",    0x0406, 0x10, 0,
    kLookupPowerStatus,
    sizeof(kLookupPowerStatus)/sizeof(kLookupPowerStatus[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecNightSwitching{
    "night_switching", 0x0411, 0x10, 0,
    kLookupOnOff, sizeof(kLookupOnOff)/sizeof(kLookupOnOff[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecFrostGuard{
    "frost_guard",    0x0412, 0x10, 0,
    kLookupOnOff, sizeof(kLookupOnOff)/sizeof(kLookupOnOff[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecChildLock{
    "child_lock",     0x0413, 0x10, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMaxFloorTemp{
    "max_floor_temp", 0x0414, 0x20, 0, nullptr, 0,
};
// NOTE: z2m scales calibration by 10 before write. Adapter must
// pre-scale; tz_zcl_write_attr writes raw i8.
constexpr ::zhc::generic::ZclWriteSpec kSpecCalibration{
    "local_temperature_calibration", 0x0417, 0x28, 0, nullptr, 0,
};

}  // namespace

#define ZHC_ELKO_TZ(var, spec_ref, key_str)                          \
    extern const TzConverter var{                                    \
        .key         = key_str,                                      \
        .cluster     = "hvacThermostat",                             \
        .cluster_id  = 0x0201,                                       \
        .command_id  = 0x02,                                         \
        .fn          = &::zhc::generic::tz_zcl_write_attr,           \
        .user_config = &spec_ref,                                    \
    };
ZHC_ELKO_TZ(kTzElkoLoad,            kSpecLoad,            "load")
ZHC_ELKO_TZ(kTzElkoSensor,          kSpecSensor,          "sensor")
ZHC_ELKO_TZ(kTzElkoRegulatorTime,   kSpecRegulatorTime,   "regulator_time")
ZHC_ELKO_TZ(kTzElkoRegulatorMode,   kSpecRegulatorMode,   "regulator_mode")
ZHC_ELKO_TZ(kTzElkoSystemMode,      kSpecSystemMode,      "system_mode")
ZHC_ELKO_TZ(kTzElkoNightSwitching,  kSpecNightSwitching,  "night_switching")
ZHC_ELKO_TZ(kTzElkoFrostGuard,      kSpecFrostGuard,      "frost_guard")
ZHC_ELKO_TZ(kTzElkoChildLock,       kSpecChildLock,       "child_lock")
ZHC_ELKO_TZ(kTzElkoMaxFloorTemp,    kSpecMaxFloorTemp,    "max_floor_temp")
ZHC_ELKO_TZ(kTzElkoCalibration,     kSpecCalibration,     "local_temperature_calibration")
#undef ZHC_ELKO_TZ

// TODO(parity): BLOCKED — display_text (elkoDisplayText 0x0402, CHAR_STR)
// requires a string-aware writeAttributes encoder; tz_zcl_write_attr
// only handles scalar 1-4 byte values. Filed under z2m parity gap.

namespace {

const FzConverter* const kFz_D4523430[] = {
    &::zhc::generic::kFzThermostat,   // standard hvacThermostat attrs
                                      // (occupied_heating_setpoint,
                                      //  local_temperature, system_mode 0x001C)
    &kFzElkoThermostat,               // elko-private 0x04xx attrs
};

const TzConverter* const kTz_D4523430[] = {
    &::zhc::generic::kTzThermostat,   // occupied_heating_setpoint
    &kTzElkoSystemMode,               // overrides system_mode → 0x0406
    &kTzElkoCalibration,              // local_temperature_calibration → 0x0417
    &kTzElkoLoad,
    &kTzElkoSensor,
    &kTzElkoRegulatorTime,
    &kTzElkoRegulatorMode,
    &kTzElkoNightSwitching,
    &kTzElkoFrostGuard,
    &kTzElkoChildLock,
    &kTzElkoMaxFloorTemp,
};

constexpr const char* kModels_D4523430[] = { "Super TR" };

constexpr const char* kEnumSystemModeStr[] = { "off", "heat" };
constexpr const char* kEnumRunningStateStr[] = { "idle", "heat" };
constexpr const char* kEnumSensorStr[] = { "air", "floor", "supervisor_floor" };
constexpr const char* kEnumRegulatorModeStr[] = { "thermostat", "regulator" };
constexpr const char* kEnumOnOffStr[] = { "off", "on" };

constexpr Expose kExp_D4523430[] = {
    { "local_temperature",          ExposeType::Numeric, Access::State,    "°C", nullptr, nullptr, 0 },
    { "occupied_heating_setpoint",  ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0 },
    { "local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0 },
    { "system_mode",                ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kEnumSystemModeStr,    2 },
    { "running_state",              ExposeType::Enum,    Access::State,    nullptr, nullptr, kEnumRunningStateStr,  2 },
    { "load",                       ExposeType::Numeric, Access::StateSet, "W",   nullptr, nullptr, 0 },
    { "mean_power",                 ExposeType::Numeric, Access::State,    "W",   nullptr, nullptr, 0 },
    { "floor_temp",                 ExposeType::Numeric, Access::State,    "°C", nullptr, nullptr, 0 },
    { "max_floor_temp",             ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0 },
    { "sensor",                     ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kEnumSensorStr,        3 },
    { "regulator_mode",             ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kEnumRegulatorModeStr, 2 },
    { "regulator_time",             ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0 },
    { "frost_guard",                ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kEnumOnOffStr,         2 },
    { "child_lock",                 ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,               0 },
    { "night_switching",            ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kEnumOnOffStr,         2 },
};

constexpr BindingSpec kBind_D4523430[] = {
    { 1, 0x0201 },   // hvacThermostat
};

}  // namespace

extern const PreparedDefinition kDef_D4523430{
    .zigbee_models=kModels_D4523430, .zigbee_models_count=sizeof(kModels_D4523430)/sizeof(kModels_D4523430[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4523430", .vendor="Elko",
    .meta=nullptr, .exposes=kExp_D4523430, .exposes_count=sizeof(kExp_D4523430)/sizeof(kExp_D4523430[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D4523430, .from_zigbee_count=sizeof(kFz_D4523430)/sizeof(kFz_D4523430[0]),
    .to_zigbee=kTz_D4523430, .to_zigbee_count=sizeof(kTz_D4523430)/sizeof(kTz_D4523430[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_D4523430, .bindings_count=sizeof(kBind_D4523430)/sizeof(kBind_D4523430[0]),
};

}  // namespace zhc::devices::elko
