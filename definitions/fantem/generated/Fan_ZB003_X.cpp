// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Fantem ZB003-X — hand-rewritten 2026-04-29g sweep.
// 4 in 1 multi sensor.
//
// z2m-source: fantem.ts #ZB003-X.
//
// Wire format notes:
//   * Battery (% + voltage) rides standard genPowerCfg via kFzBattery.
//   * Occupancy + tamper + battery_low ride ssIasZone
//     commandStatusChangeNotification (cmd 0x00) decoded by
//     kFzIasZoneStatusChange — the device sends this on motion events.
//   * Tuya DP stream on manuSpecificTuya (0xEF00):
//       DP 102 fantemReportingTime         u32 → reporting_time (min)
//       DP 104 fantemTempCalibration       i32 → temperature_calibration
//       DP 105 fantemHumidityCalibration   i32 → humidity_calibration
//       DP 106 fantemLuxCalibration        i32 → illuminance_calibration
//       DP 107 fantemTemp                  s32 / 10 → temperature (°C)
//       DP 108 fantemHumidity              u16 → humidity (%)
//       DP 109 fantemMotionEnable          bool → pir_enable
//       DP 110 fantemBattery               u8  → battery2 (%)
//       DP 111 fantemLedEnable             bool → led_enable (inverted)
//       DP 112 fantemReportingEnable       bool → reporting_enable
//   * Sensitivity / keep_time settings are written to ssIasZone:
//       attr 0x0013 currentZoneSensitivityLevel (ENUM8) → sensitivity
//       attr 0xF001 (61441) custom (ENUM8)            → keep_time
//
// Surface gap (PARTIAL): illuminance reading itself is reported via a
// Tuya-private ssIasZone attr 0xF002 (61442) that the generic ZHC IAS
// runtime doesn't claim. Calibration knob is wired (DP 106) and the
// occupancy/tamper path is full; only the raw lux value is missing.

#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::fantem {
namespace {

using ::zhc::Access;
using ::zhc::BindingSpec;
using ::zhc::Expose;
using ::zhc::ExposeType;
using ::zhc::FzConverter;
using ::zhc::PreparedDefinition;
using ::zhc::TzConverter;
using ::zhc::generic::ZclWriteLookup;
using ::zhc::generic::ZclWriteSpec;

// ── Tuya DP map (fz + tz share the same table) ──────────────────────

constexpr ::zhc::tuya::TuyaDpMapEntry kDpEntries_ZB003_X[] = {
    // DP 102 — reporting_time (minutes), pass-through.
    { 102, "reporting_time",         ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    // DP 104 — temperature_calibration (signed s32, scaled /10).
    { 104, "temperature_calibration", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    // DP 105 — humidity_calibration (signed s32, integer).
    { 105, "humidity_calibration",   ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    // DP 106 — illuminance_calibration (signed s32, integer).
    { 106, "illuminance_calibration", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    // DP 107 — temperature (s32 / 10 → °C).
    { 107, "temperature",            ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    // DP 108 — humidity (u32, integer %).
    { 108, "humidity",               ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    // DP 109 — pir_enable (bool).
    { 109, "pir_enable",             ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    // DP 110 — battery2 (numeric %; second-battery readout).
    { 110, "battery2",               ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    // DP 111 — led_enable (bool, inverted: device 0=led-on, 1=led-off).
    { 111, "led_enable",             ::zhc::TuyaDpType::Bool,    1, nullptr, 0,
      ::zhc::tuya::kTuyaDpFlagInvertBool },
    // DP 112 — reporting_enable (bool).
    { 112, "reporting_enable",       ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kDpMap_ZB003_X{
    kDpEntries_ZB003_X,
    sizeof(kDpEntries_ZB003_X) / sizeof(kDpEntries_ZB003_X[0]),
};

constexpr FzConverter kFzDp_ZB003_X{
    .family            = ::zhc::FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
    .user_config       = &kDpMap_ZB003_X,
};

constexpr TzConverter kTzDp_ZB003_X{
    .key         = nullptr,                       // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kDpMap_ZB003_X,
};

// ── ssIasZone writes for sensitivity + keep_time ───────────────────

constexpr ZclWriteLookup kLookup_Sensitivity[] = {
    {"low",    0},
    {"medium", 1},
    {"high",   2},
};
constexpr ZclWriteLookup kLookup_KeepTime[] = {
    // z2m exposes string labels "0".."480" — match exactly.
    {"0",   0},
    {"30",  1},
    {"60",  2},
    {"120", 3},
    {"240", 4},
    {"480", 5},
};

// std attr 0x0013 currentZoneSensitivityLevel (ENUM8).
constexpr ZclWriteSpec kSpec_Sensitivity{
    "sensitivity", 0x0013, 0x30 /* ENUM8 */, 0,
    kLookup_Sensitivity, sizeof(kLookup_Sensitivity) / sizeof(kLookup_Sensitivity[0]),
};
// custom attr 0xF001 (61441) ENUM8.
constexpr ZclWriteSpec kSpec_KeepTime{
    "keep_time",   0xF001, 0x30 /* ENUM8 */, 0,
    kLookup_KeepTime, sizeof(kLookup_KeepTime) / sizeof(kLookup_KeepTime[0]),
};

extern const TzConverter kTz_Sensitivity{
    .key         = "sensitivity",
    .cluster     = "ssIasZone",
    .cluster_id  = 0x0500,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_Sensitivity,
};
extern const TzConverter kTz_KeepTime{
    .key         = "keep_time",
    .cluster     = "ssIasZone",
    .cluster_id  = 0x0500,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_KeepTime,
};

// ── Fz / Tz tables ──────────────────────────────────────────────────

const FzConverter* const kFz_ZB003_X[] = {
    &::zhc::generic::kFzBattery,             // genPowerCfg → battery + voltage
    &::zhc::generic::kFzIasZoneStatusChange, // ssIasZone cmd 0x00 → occupancy/tamper/battery_low
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_ZB003_X,
};
const TzConverter* const kTz_ZB003_X[] = {
    &kTzDp_ZB003_X,
    &kTz_Sensitivity,
    &kTz_KeepTime,
};

constexpr const char* kModels_ZB003_X[] = { "TS0202" };
constexpr const char* kManus_ZB003_X[]  = {
    "_TZ3210_0aqbrnts", "_TZ3210_rxqls8v0", "_TZ3210_zmy9hjay",
    "_TZ3210_wuhzzfqg", "_TZ3210_ohvnwamm", "_TZ3210_oekbi7o4",
};

constexpr Expose kExposes_ZB003_X[] = {
    {"occupancy",                ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"tamper",                   ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"temperature",              ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity",                 ExposeType::Numeric, Access::State,    "%",   nullptr, nullptr, 0},
    {"battery",                  ExposeType::Numeric, Access::State,    "%",   nullptr, nullptr, 0},
    {"voltage",                  ExposeType::Numeric, Access::State,    "mV",  nullptr, nullptr, 0},
    {"battery2",                 ExposeType::Numeric, Access::State,    "%",   nullptr, nullptr, 0},
    {"battery_low",              ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"illuminance_calibration",  ExposeType::Numeric, Access::StateSet, "lx",  nullptr, nullptr, 0},
    {"temperature_calibration",  ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity_calibration",     ExposeType::Numeric, Access::StateSet, "%",   nullptr, nullptr, 0},
    {"reporting_enable",         ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"reporting_time",           ExposeType::Numeric, Access::StateSet, "min", nullptr, nullptr, 0},
    {"led_enable",               ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"pir_enable",               ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sensitivity",              ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"keep_time",                ExposeType::Enum,    Access::StateSet, "s",   nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB003_X[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
    {1, 0xEF00},  // manuSpecificTuya
};

}  // namespace

extern const PreparedDefinition kDef_ZB003_X{
    .zigbee_models=kModels_ZB003_X, .zigbee_models_count=sizeof(kModels_ZB003_X)/sizeof(kModels_ZB003_X[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZB003_X, .manufacturer_names_count=sizeof(kManus_ZB003_X)/sizeof(kManus_ZB003_X[0]),
    .model="ZB003-X", .vendor="Fantem",
    .meta=nullptr, .exposes=kExposes_ZB003_X, .exposes_count=sizeof(kExposes_ZB003_X)/sizeof(kExposes_ZB003_X[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB003_X, .from_zigbee_count=sizeof(kFz_ZB003_X)/sizeof(kFz_ZB003_X[0]),
    .to_zigbee=kTz_ZB003_X,   .to_zigbee_count=sizeof(kTz_ZB003_X)/sizeof(kTz_ZB003_X[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBindings_ZB003_X, .bindings_count=sizeof(kBindings_ZB003_X)/sizeof(kBindings_ZB003_X[0]),
};

}  // namespace zhc::devices::fantem
