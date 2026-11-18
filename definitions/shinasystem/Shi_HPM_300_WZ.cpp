// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: ShinaSystem HPM-300-WZ SiHAS mmWave presence sensor (zigbeeModel
// HPM-300-WZ), new in z2m v26.126.0..v26.133.0. m.occupancy plus the attributes
// the def adds to the standard msOccupancySensing cluster (0x0406, no
// manufacturer code): presence_status 0x9000 (enum), the 0x9001..0x900C
// sensitivity / hold-time / LED settings (u16, led_brightness u8), the
// last/peak movement and occupancy diagnostics and the write-only peak_clear.
// Reporting as upstream: occupancy 0 s .. 1 h, presence_status 1 s .. 1 h,
// last_* 120 s .. 2 h, peak_* 10 s .. 2 h, all on any change.
// Not ported: nothing beyond the house rules (no identify upstream).
// z2m-source: shinasystem.ts #HPM-300-WZ
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::shinasystem {
namespace {
using ::zhc::generic::ZclWriteLookup;
using ::zhc::generic::ZclWriteSpec;

constexpr ZclWriteLookup kPresenceLut[] = {
    {"inactive", 0}, {"leaving", 1}, {"occupied", 2}, {"moving", 3}, {"offline", 9},
};
constexpr ZclWriteLookup kPeakClearLut[] = { {"clear", 0} };
constexpr ::zhc::generic::ZclAttrRow kOccRows[] = {
    { 0x9000, "presence_status", 1, kPresenceLut, sizeof(kPresenceLut)/sizeof(kPresenceLut[0]) },
    { 0x9001, "movement_sensitivity" },
    { 0x9002, "leaving_hold_time" },
    { 0x9003, "occupancy_sensitivity" },
    { 0x9004, "led_brightness" },
    { 0x9005, "entry_sensitivity" },
    { 0x9006, "last_movement_value" },
    { 0x9007, "last_occupancy_value" },
    { 0x9008, "peak_movement_value" },
    { 0x9009, "peak_occupancy_value" },
    { 0x900A, "peak_clear", 1, kPeakClearLut, sizeof(kPeakClearLut)/sizeof(kPeakClearLut[0]) },
    { 0x900B, "movement_hold_time" },
    { 0x900C, "occupancy_hold_time" },
};
constexpr ::zhc::generic::ZclAttrMap kOccMap{ kOccRows, sizeof(kOccRows)/sizeof(kOccRows[0]) };
constexpr FzConverter kFzSihas = ::zhc::generic::zcl_attr_fz("msOccupancySensing", &kOccMap);

constexpr ZclWriteSpec kMovementSensSpec{ "movement_sensitivity", 0x9001, 0x21, 0, nullptr, 0 };
constexpr ZclWriteSpec kLeavingHoldSpec{ "leaving_hold_time", 0x9002, 0x21, 0, nullptr, 0 };
constexpr ZclWriteSpec kOccupancySensSpec{ "occupancy_sensitivity", 0x9003, 0x21, 0, nullptr, 0 };
constexpr ZclWriteSpec kLedSpec{ "led_brightness", 0x9004, 0x20, 0, nullptr, 0 };
constexpr ZclWriteSpec kEntrySensSpec{ "entry_sensitivity", 0x9005, 0x21, 0, nullptr, 0 };
constexpr ZclWriteSpec kPeakClearSpec{ "peak_clear", 0x900A, 0x21, 0, kPeakClearLut, sizeof(kPeakClearLut)/sizeof(kPeakClearLut[0]) };
constexpr ZclWriteSpec kMovementHoldSpec{ "movement_hold_time", 0x900B, 0x21, 0, nullptr, 0 };
constexpr ZclWriteSpec kOccupancyHoldSpec{ "occupancy_hold_time", 0x900C, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzMovementSens = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kMovementSensSpec);
constexpr TzConverter kTzLeavingHold = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kLeavingHoldSpec);
constexpr TzConverter kTzOccupancySens = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kOccupancySensSpec);
constexpr TzConverter kTzLed = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kLedSpec);
constexpr TzConverter kTzEntrySens = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kEntrySensSpec);
constexpr TzConverter kTzPeakClear = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kPeakClearSpec);
constexpr TzConverter kTzMovementHold = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kMovementHoldSpec);
constexpr TzConverter kTzOccupancyHold = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kOccupancyHoldSpec);

const FzConverter* const kFz_HPM_300_WZ[] = {
    &::zhc::generic::kFzOccupancy,
    &kFzSihas,
};
const TzConverter* const kTz_HPM_300_WZ[] = {
    &kTzMovementHold,
    &kTzOccupancyHold,
    &kTzLeavingHold,
    &kTzEntrySens,
    &kTzMovementSens,
    &kTzOccupancySens,
    &kTzLed,
    &kTzPeakClear,
};
constexpr const char* kModels_HPM_300_WZ[] = { "HPM-300-WZ" };
constexpr const char* kPresenceOpts[] = { "inactive", "leaving", "occupied", "moving", "offline" };
constexpr const char* kPeakClearOpts[] = { "clear" };
constexpr Expose kExposes_HPM_300_WZ[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"presence_status", ExposeType::Enum, Access::State, nullptr, "Current presence status",
     kPresenceOpts, sizeof(kPresenceOpts)/sizeof(kPresenceOpts[0])},
    {"movement_hold_time", ExposeType::Numeric, Access::StateSet, "s", "Duration for which the moving state is maintained",
     nullptr, 0, ExposeCategory::Config, 5, 60000, 0},
    {"occupancy_hold_time", ExposeType::Numeric, Access::StateSet, "s", "Duration for which the occupied state is maintained",
     nullptr, 0, ExposeCategory::Config, 5, 60000, 0},
    {"leaving_hold_time", ExposeType::Numeric, Access::StateSet, "s", "Duration for which the leaving state is maintained",
     nullptr, 0, ExposeCategory::Config, 5, 60000, 0},
    {"entry_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr,
     "Sensitivity of the initial detection while inactive (lower is more sensitive)",
     nullptr, 0, ExposeCategory::Config, 15, 6000, 0},
    {"movement_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr,
     "Movement detection sensitivity (lower is more sensitive)", nullptr, 0, ExposeCategory::Config, 10, 6000, 0},
    {"occupancy_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr,
     "Occupancy detection sensitivity (lower is more sensitive)", nullptr, 0, ExposeCategory::Config, 10, 6000, 0},
    {"led_brightness", ExposeType::Numeric, Access::StateSet, "%", "Status LED brightness",
     nullptr, 0, ExposeCategory::Config, 0, 100, 0},
    {"last_movement_value", ExposeType::Numeric, Access::State, nullptr, "Last measured movement value",
     nullptr, 0, ExposeCategory::Diagnostic},
    {"last_occupancy_value", ExposeType::Numeric, Access::State, nullptr, "Last measured occupancy value",
     nullptr, 0, ExposeCategory::Diagnostic},
    {"peak_movement_value", ExposeType::Numeric, Access::State, nullptr, "Peak measured movement value",
     nullptr, 0, ExposeCategory::Diagnostic},
    {"peak_occupancy_value", ExposeType::Numeric, Access::State, nullptr, "Peak measured occupancy value",
     nullptr, 0, ExposeCategory::Diagnostic},
    {"peak_clear", ExposeType::Enum, Access::Set, nullptr, "Reset the peak movement and occupancy values",
     kPeakClearOpts, sizeof(kPeakClearOpts)/sizeof(kPeakClearOpts[0]), ExposeCategory::Config},
};
constexpr BindingSpec kBindings_HPM_300_WZ[] = {
    {1, 0x0406},
};
constexpr ReportingSpec kReports_HPM_300_WZ[] = {
    {1, 0x0406, 0x0000, 0x18, 0, 3600, 0, 0},
    {1, 0x0406, 0x9000, 0x21, 1, 3600, 0, 0},
    {1, 0x0406, 0x9006, 0x21, 120, 7200, 0, 0},
    {1, 0x0406, 0x9007, 0x21, 120, 7200, 0, 0},
    {1, 0x0406, 0x9008, 0x21, 10, 7200, 0, 0},
    {1, 0x0406, 0x9009, 0x21, 10, 7200, 0, 0},
};
}  // namespace

extern const PreparedDefinition kDef_HPM_300_WZ{
    .zigbee_models=kModels_HPM_300_WZ, .zigbee_models_count=sizeof(kModels_HPM_300_WZ)/sizeof(kModels_HPM_300_WZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HPM-300-WZ", .vendor="Shinasystem",
    .meta=nullptr, .exposes=kExposes_HPM_300_WZ, .exposes_count=sizeof(kExposes_HPM_300_WZ)/sizeof(kExposes_HPM_300_WZ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HPM_300_WZ, .from_zigbee_count=sizeof(kFz_HPM_300_WZ)/sizeof(kFz_HPM_300_WZ[0]),
    .to_zigbee=kTz_HPM_300_WZ, .to_zigbee_count=sizeof(kTz_HPM_300_WZ)/sizeof(kTz_HPM_300_WZ[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_HPM_300_WZ, .bindings_count=sizeof(kBindings_HPM_300_WZ)/sizeof(kBindings_HPM_300_WZ[0]),
    .reports=kReports_HPM_300_WZ, .reports_count=sizeof(kReports_HPM_300_WZ)/sizeof(kReports_HPM_300_WZ[0]),
};

}  // namespace zhc::devices::shinasystem
