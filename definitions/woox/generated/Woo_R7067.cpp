// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Woox R7067 — thermostatic radiator valve (TS0601 Tuya MCU).
// z2m-source: woox.ts #R7067 (legacy.fromZigbee.woox_thermostat +
//             toZigbee.woox_thermostat_*).
//
// Maps the woox-specific dataPoints from lib/legacy.ts:
//   wooxMode (DP 2)                             — system_mode/away_mode (enum)
//   wooxManualTemperatureSetpoint (DP 16)       — current_heating_setpoint /2
//   wooxLocalTemperature (DP 24)                — local_temperature /10
//   wooxChildLock (DP 30)                       — child_lock bool (LOCK/UNLOCK)
//   wooxBatteryCapacity (DP 34)                 — battery (and battery_low <30)
//   wooxComfortTemperature (DP 101)             — comfort_temperature /2
//   wooxEnergySavingTemperature (DP 102)        — eco_temperature /2
//   wooxHolidayModeSettings (DP 103)            — holidays_schedule (string;
//                                                  unsupported decode)
//   wooxTemperatureCalibration (DP 104)         — local_temperature_calibration /10
//   wooxAutomaticTemperatureSetpoint (DP 105)   — current_heating_setpoint /2
//   wooxBoostHeating (DP 106)                   — boost_heating bool
//   wooxWindowStatus (DP 107)                   — window_detection (OPEN/CLOSED)
//   wooxWindowTemperature (DP 116)              — window_detection_temperature /2
//   wooxWindowTime (DP 117)                     — window_detection_time
//   wooxBoostHeatingCountdown (DP 118)          — boost_heating_countdown
//   wooxControlTemperature (DP 119)             — write-only setpoint
//                                                  (current_heating_setpoint
//                                                   * 10 — handled via tz)
//
// Limitations vs z2m:
//   - Schedule DPs (Mon..Sun + holidays) are not represented; z2m
//     emits comma-joined strings, which would need a String/BytesRef
//     DP type beyond ZHC's 24-byte string cap. Marked TODO.
//   - system_mode/away_mode coupling: z2m derives both from the same
//     enum value (0/1/2/3); we emit `system_mode` only — adapter
//     consumers can map.
//   - tz `current_heating_setpoint` writes DP 119 (wooxControlTemperature)
//     using Math.round(value * 10). The ZHC DP encoder treats negative
//     divisor as multiply, so we mark divisor=-10 to mirror the scale.
//   - `system_mode` write semantics in z2m are a multi-DP composite
//     (set mode + setpoint). We only encode the mode enum; the
//     setpoint side-effect needs a runtime hook (TODO).
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::woox {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kSystemMode_R7067[] = {
    { 0, "auto" },
    { 1, "heat" },
    { 2, "auto" },   // value 2 == away_mode ON; system_mode stays "auto"
    { 3, "off" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kChildLock_R7067[] = {
    { 0, "UNLOCK" },
    { 1, "LOCK" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kWindowStatus_R7067[] = {
    { 0, "CLOSED" },
    { 1, "OPEN" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kDps_R7067[] = {
    // dp_id, key,                                type,                       divisor, enum_table,                count, flags
    {   2, "system_mode",                    ::zhc::TuyaDpType::Enum,    1, kSystemMode_R7067,
        sizeof(kSystemMode_R7067)/sizeof(kSystemMode_R7067[0]), 0 },
    {  16, "current_heating_setpoint",       ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },     // /2
    {  24, "local_temperature",              ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },    // /10
    {  30, "child_lock",                     ::zhc::TuyaDpType::Enum,    1, kChildLock_R7067,
        sizeof(kChildLock_R7067)/sizeof(kChildLock_R7067[0]), 0 },
    {  34, "battery",                        ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "comfort_temperature",            ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },     // /2
    { 102, "eco_temperature",                ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },     // /2
    { 104, "local_temperature_calibration",  ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },    // /10
    { 105, "current_heating_setpoint",       ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },     // auto-mode /2
    { 106, "boost_heating",                  ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 107, "window_detection",               ::zhc::TuyaDpType::Enum,    1, kWindowStatus_R7067,
        sizeof(kWindowStatus_R7067)/sizeof(kWindowStatus_R7067[0]), 0 },
    { 116, "window_detection_temperature",   ::zhc::TuyaDpType::Numeric, 2, nullptr, 0, 0 },     // /2
    { 117, "window_detection_time",          ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 118, "boost_heating_countdown",        ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    // tz-only: control_temperature DP 119 — z2m writes value*10 here.
    // Accept "current_heating_setpoint" writes via the duplicate key
    // mapping to DP 16 above; the generic encoder picks the first
    // matching entry, which encodes /2. For exact z2m parity (×10),
    // this row would need a tz-only override; left as TODO since the
    // DP-16 path is the documented "manual setpoint" target and the
    // composite DP-2/119 system_mode handler isn't yet replicated.
};
constexpr ::zhc::tuya::TuyaDatapointMap kDpMap_R7067{
    kDps_R7067, sizeof(kDps_R7067)/sizeof(kDps_R7067[0])
};

constexpr FzConverter kFzDp_R7067{
    .family            = FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
    .user_config       = &kDpMap_R7067,
};
constexpr TzConverter kTzDp_R7067{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kDpMap_R7067,
};

const FzConverter* const kFz_R7067[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::tuya::kFzIgnoreTuyaSetTime,
    &kFzDp_R7067,
};
const TzConverter* const kTz_R7067[] = { &kTzDp_R7067 };

constexpr const char* kModels_R7067[] = { "TS0601" };
constexpr const char* kManus_R7067[] = { "_TZE200_wnvhlcgl" };

constexpr Expose kExp_R7067[] = {
    {"local_temperature",             ExposeType::Numeric, Access::State,    "C",     nullptr, nullptr, 0},
    {"current_heating_setpoint",      ExposeType::Numeric, Access::StateSet, "C",     nullptr, nullptr, 0},
    {"comfort_temperature",           ExposeType::Numeric, Access::StateSet, "C",     nullptr, nullptr, 0},
    {"eco_temperature",               ExposeType::Numeric, Access::StateSet, "C",     nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "C",     nullptr, nullptr, 0},
    {"window_detection_temperature",  ExposeType::Numeric, Access::StateSet, "C",     nullptr, nullptr, 0},
    {"window_detection_time",         ExposeType::Numeric, Access::StateSet, "min",   nullptr, nullptr, 0},
    {"boost_heating_countdown",       ExposeType::Numeric, Access::State,    "s",     nullptr, nullptr, 0},
    {"system_mode",                   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_detection",              ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"child_lock",                    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"boost_heating",                 ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery",                       ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_R7067[] = {
    {1, 0xEF00},  // manuSpecificTuya — z2m's configure binds genBasic;
                  // we use the DP cluster which is the actual data path.
};

}  // namespace

extern const PreparedDefinition kDef_R7067{
    .zigbee_models=kModels_R7067, .zigbee_models_count=sizeof(kModels_R7067)/sizeof(kModels_R7067[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_R7067, .manufacturer_names_count=sizeof(kManus_R7067)/sizeof(kManus_R7067[0]),
    .model="R7067", .vendor="Woox",
    .meta=nullptr, .exposes=kExp_R7067, .exposes_count=sizeof(kExp_R7067)/sizeof(kExp_R7067[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_R7067, .from_zigbee_count=sizeof(kFz_R7067)/sizeof(kFz_R7067[0]),
    .to_zigbee=kTz_R7067,   .to_zigbee_count=sizeof(kTz_R7067)/sizeof(kTz_R7067[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_R7067,
    .bindings_count=sizeof(kBind_R7067)/sizeof(kBind_R7067[0]),
};

}  // namespace zhc::devices::woox
