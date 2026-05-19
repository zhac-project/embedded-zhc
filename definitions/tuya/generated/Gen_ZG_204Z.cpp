// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Manual override (was auto-generated): HOBEIAN ZG-204Z PIR — IAS Zone
// + sensitivity / keep_time configuration.
//
// z2m-source: tuya.ts zigbeeModel "ZG-204Z" → IH012-RT01 entry.
// Uses m.iasZoneAlarm({zoneType:"occupancy", zoneAttributes:
// ["alarm_1","battery_low"]}) + fz.ZM35HQ_attr (read) +
// tz.ZM35HQ_attr (write) + fz.battery.
//
// Wire layout:
//   genPowerCfg (0x0001) attrs
//     → battery %, battery voltage      via kFzBattery
//   ssIasZone   (0x0500) cmd 0x00
//     → occupancy + tamper + battery_low via kFzIasMotionAlarm
//   ssIasZone   (0x0500) attr report / read response
//     attr 0x0013 currentZoneSensitivityLevel (ENUM8)
//       → "sensitivity" "low"|"medium"|"high"
//     attr 0xF001 (61441, manuSpec, ENUM8)
//       → "keep_time"   "30"|"60"|"120" (seconds)
//   Set-path writes those same attrs via tz_zcl_write_attr.
//
// Generator previously bound 0xEF00/0x0006 (wrong); real device speaks
// pure IAS Zone — no manuSpecificTuya, no genOnOff. Generator follow-up
// is in private zhac-tools.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tuya {
namespace {

using ::zhc::generic::ZclWriteLookup;
using ::zhc::generic::ZclWriteSpec;

const FzConverter* const kFzGen_ZG_204Z[] = {
    &::zhc::generic::kFzBattery,         // genPowerCfg → battery + voltage
    &::zhc::generic::kFzIasMotionAlarm,  // ssIasZone cmd 0x00 → occupancy/tamper/battery_low
    &::zhc::generic::kFzIasZoneConfig,   // ssIasZone attr 0x0013/0xF001 → sensitivity/keep_time
};
constexpr const char* kModelsGen_ZG_204Z[] = { "ZG-204Z" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"sensitivity", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"keep_time",   ExposeType::Enum,    Access::StateSet, "s",  nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
};

// ── Reporting setup ─────────────────────────────────────────────────
//
// Mirrors z2m's configure block:
//   reporting.bind(endpoint, coordinatorEndpoint, ["genPowerCfg"]);
//   reporting.batteryPercentageRemaining(endpoint);
//   reporting.batteryVoltage(endpoint);
//
// Defaults match z2m reporting defaults: min 1h, max 65535 (off, only
// on change), reportable change 1 unit. Battery devices send rarely
// to preserve life.
constexpr ::zhc::ReportingSpec kReports_ZG_204Z[] = {
    {1, 0x0001, 0x0021, 0x20 /* u8 */, 3600, 65535, 1, 0},  // batteryPercentageRemaining
    {1, 0x0001, 0x0020, 0x20 /* u8 */, 3600, 65535, 1, 0},  // batteryVoltage
};

// ── Read-on-join: seed sensitivity + keep_time + zoneStatus ────────
//
// z2m's tz.ZM35HQ_attr.convertGet reads exactly this triple. We do
// the same on join so the shadow has values before the user opens
// the device-detail page. attr ids packed LE per the ConfigStep
// payload contract — count is bytes / 2 inside run_configure.
//
// manu_code = 0 (profile-wide). Tuya firmwares ignore the manu code
// on attr 0xF001 reads — z2m gets away with the same.
constexpr std::uint8_t kReadAttrs_ZG_204Z[] = {
    0x13, 0x00,   // 0x0013 currentZoneSensitivityLevel
    0x01, 0xF0,   // 0xF001 keep_time (manuSpec range, profile-wide read)
    0x02, 0x00,   // 0x0002 zoneStatus (re-seed after configure)
};
constexpr ::zhc::ConfigStep kConfigSteps_ZG_204Z[] = {
    {
        .op          = ::zhc::ConfigStepOp::Read,
        .endpoint    = 1,
        .cluster_id  = 0x0500,
        .cmd_id      = 0,
        .flags       = 0,
        .payload     = kReadAttrs_ZG_204Z,
        .payload_len = sizeof(kReadAttrs_ZG_204Z),
        .wait_ms     = 0,
        .manu_code   = 0,
    },
};

// ── ssIasZone writes for sensitivity + keep_time ────────────────────
//
// String labels match what the FZ emits and what the SPA enum picker
// surfaces; the device-side ENUM8 value is the lookup index.

constexpr ZclWriteLookup kLookup_Sensitivity[] = {
    {"low",    0},
    {"medium", 1},
    {"high",   2},
};
constexpr ZclWriteLookup kLookup_KeepTime[] = {
    {"30",  0},
    {"60",  1},
    {"120", 2},
};

// std attr 0x0013 currentZoneSensitivityLevel (ENUM8).
constexpr ZclWriteSpec kSpec_Sensitivity{
    "sensitivity", 0x0013, 0x30 /* ENUM8 */, 0,
    kLookup_Sensitivity, sizeof(kLookup_Sensitivity) / sizeof(kLookup_Sensitivity[0]),
};
// manuSpec attr 0xF001 (61441), ENUM8.
constexpr ZclWriteSpec kSpec_KeepTime{
    "keep_time", 0xF001, 0x30 /* ENUM8 */, 0,
    kLookup_KeepTime, sizeof(kLookup_KeepTime) / sizeof(kLookup_KeepTime[0]),
};

}  // namespace

extern const TzConverter kTz_ZG_204Z_Sensitivity{
    .key         = "sensitivity",
    .cluster     = "ssIasZone",
    .cluster_id  = 0x0500,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_Sensitivity,
};
extern const TzConverter kTz_ZG_204Z_KeepTime{
    .key         = "keep_time",
    .cluster     = "ssIasZone",
    .cluster_id  = 0x0500,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_KeepTime,
};

namespace {
const TzConverter* const kTzGen_ZG_204Z[] = {
    &kTz_ZG_204Z_Sensitivity,
    &kTz_ZG_204Z_KeepTime,
};
}  // namespace

extern const PreparedDefinition kDefGen_ZG_204Z{
    .zigbee_models=kModelsGen_ZG_204Z, .zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG-204Z", .vendor="HOBEIAN",
    .meta=nullptr, .exposes=kAutoExposes,
    .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzGen_ZG_204Z,
    .from_zigbee_count=sizeof(kFzGen_ZG_204Z)/sizeof(kFzGen_ZG_204Z[0]),
    .to_zigbee=kTzGen_ZG_204Z,
    .to_zigbee_count=sizeof(kTzGen_ZG_204Z)/sizeof(kTzGen_ZG_204Z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,
    .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .reports=kReports_ZG_204Z,
    .reports_count=sizeof(kReports_ZG_204Z)/sizeof(kReports_ZG_204Z[0]),
    .config_steps=kConfigSteps_ZG_204Z,
    .config_steps_count=sizeof(kConfigSteps_ZG_204Z)/sizeof(kConfigSteps_ZG_204Z[0]),
};

}  // namespace zhc::devices::tuya
