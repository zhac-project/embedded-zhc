// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HOBEIAN ZG-204ZL-z luminance motion sensor (pvvx/ZigbeeTLc), z2m v26.133.0.
// z2m-source: zigbeetlc.ts #ZG-204ZL-z (moved from custom_devices_diy.ts, vendor HOBEIAN).
//
// z2m extend = m.battery({voltage}), m.illuminance(), m.occupancy(),
//   m.onOff({powerOnBehavior: false}) (state = remote On/Off binding enable),
//   m.enumLookup(power_on_behavior, genOnOff/startUpOnOff),
//   m.numeric(pir_timeout, msOccupancySensing/pirOToUDelay 0x0010 u16 s),
//   msIlluminanceLevelSensing (0x0401): light_level enum (levelStatus 0x0000,
//   reported MIN..1 h / 1), illuminance_threshold 0x5003 u24 lx,
//   illuminance_measurement_interval 0x5000 u8 3..60 s, illuminance_offset
//   0x5001 u16, illuminance_maximum 0x5002 u16 lx,
//   m.commandsOnOff() (action on / off / toggle; bound on genOnOff).
// Not ported: the illuminance_threshold WRITE (u24: ZclWriteSpec cannot encode
// it, so the key is read-only), battery percentage reporting (house rule), the
// occupancy / illuminance reporting rows upstream configures (pre-existing).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {

// msOccupancySensing pirOToUDelay.
constexpr ::zhc::generic::ZclAttrRow kPirRows_ZG_204ZL_z[] = { { 0x0010, "pir_timeout" } };
constexpr ::zhc::generic::ZclAttrMap kPirMap_ZG_204ZL_z{
    kPirRows_ZG_204ZL_z, sizeof(kPirRows_ZG_204ZL_z)/sizeof(kPirRows_ZG_204ZL_z[0]) };
constexpr FzConverter kFzPir_ZG_204ZL_z = ::zhc::generic::zcl_attr_fz("msOccupancySensing", &kPirMap_ZG_204ZL_z);
constexpr ::zhc::generic::ZclWriteSpec kPirSpec_ZG_204ZL_z{ "pir_timeout", 0x0010, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzPir_ZG_204ZL_z = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kPirSpec_ZG_204ZL_z);

// msIlluminanceLevelSensing (0x0401).
constexpr ::zhc::generic::ZclWriteLookup kLightLevel_ZG_204ZL_z[] = {
    {"on_target", 0}, {"below_target", 1}, {"above_target", 2}, {"unknown", 255},
};
constexpr ::zhc::generic::ZclAttrRow kLevelRows_ZG_204ZL_z[] = {
    { 0x0000, "light_level", 1, kLightLevel_ZG_204ZL_z,
      sizeof(kLightLevel_ZG_204ZL_z)/sizeof(kLightLevel_ZG_204ZL_z[0]) },
    { 0x5000, "illuminance_measurement_interval" },
    { 0x5001, "illuminance_offset" },
    { 0x5002, "illuminance_maximum" },
    { 0x5003, "illuminance_threshold" },
};
constexpr ::zhc::generic::ZclAttrMap kLevelMap_ZG_204ZL_z{
    kLevelRows_ZG_204ZL_z, sizeof(kLevelRows_ZG_204ZL_z)/sizeof(kLevelRows_ZG_204ZL_z[0]) };
constexpr FzConverter kFzLevel_ZG_204ZL_z =
    ::zhc::generic::zcl_attr_fz("msIlluminanceLevelSensing", &kLevelMap_ZG_204ZL_z);
constexpr ::zhc::generic::ZclWriteSpec kIntervalSpec_ZG_204ZL_z{
    "illuminance_measurement_interval", 0x5000, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kOffsetSpec_ZG_204ZL_z{ "illuminance_offset", 0x5001, 0x21, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kMaximumSpec_ZG_204ZL_z{ "illuminance_maximum", 0x5002, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzInterval_ZG_204ZL_z =
    ::zhc::generic::zcl_write_tz("msIlluminanceLevelSensing", 0x0401, &kIntervalSpec_ZG_204ZL_z);
constexpr TzConverter kTzOffset_ZG_204ZL_z =
    ::zhc::generic::zcl_write_tz("msIlluminanceLevelSensing", 0x0401, &kOffsetSpec_ZG_204ZL_z);
constexpr TzConverter kTzMaximum_ZG_204ZL_z =
    ::zhc::generic::zcl_write_tz("msIlluminanceLevelSensing", 0x0401, &kMaximumSpec_ZG_204ZL_z);

const FzConverter* const kFz_ZG_204ZL_z[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &kFzPir_ZG_204ZL_z,
    &kFzLevel_ZG_204ZL_z,
};
const TzConverter* const kTz_ZG_204ZL_z[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
    &kTzPir_ZG_204ZL_z,
    &kTzInterval_ZG_204ZL_z,
    &kTzOffset_ZG_204ZL_z,
    &kTzMaximum_ZG_204ZL_z,
};
constexpr const char* kModels_ZG_204ZL_z[] = { "ZG-204ZL-z" };

constexpr const char* kActions_ZG_204ZL_z[] = { "on", "off", "toggle" };
constexpr const char* kLightLevels_ZG_204ZL_z[] = { "on_target", "below_target", "above_target", "unknown" };

constexpr Expose kAutoExposes[] = {
    {"battery",           ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",           ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"occupancy",         ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"illuminance",       ExposeType::Numeric, Access::State,    "lx", nullptr, nullptr, 0},
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr,
     "Enable remote On/Off binding. ON: occupancy occupied sends action \"on\", clear sends action \"off\". "
     "OFF: no occupancy actions.", nullptr, 0},
    {"power_on_behavior", ExposeType::Enum,    Access::StateSet, nullptr,
     "Power-on behavior for state (remote On/Off enable)", nullptr, 0},
    {"pir_timeout",       ExposeType::Numeric, Access::StateSet, "s",
     "Occupancy hold time, 0 disables motion detection", nullptr, 0, ExposeCategory::Config, 0, 65535, 0},
    {"light_level",       ExposeType::Enum,    Access::State,    nullptr,
     "Illuminance relative to the illuminance threshold", kLightLevels_ZG_204ZL_z,
     sizeof(kLightLevels_ZG_204ZL_z)/sizeof(kLightLevels_ZG_204ZL_z[0])},
    {"illuminance_threshold", ExposeType::Numeric, Access::State, "lx",
     "On motion, \"on\" is only sent if the illuminance is not above this, 0 disables the check",
     nullptr, 0, ExposeCategory::Config, 0, 3576000, 0},
    {"illuminance_measurement_interval", ExposeType::Numeric, Access::StateSet, "s",
     "Illuminance measurement interval", nullptr, 0, ExposeCategory::Config, 3, 60, 0},
    {"illuminance_offset", ExposeType::Numeric, Access::StateSet, nullptr,
     "Illuminance sensor zero calibration in raw ADC units", nullptr, 0, ExposeCategory::Config, 0, 65535, 0},
    {"illuminance_maximum", ExposeType::Numeric, Access::StateSet, "lx",
     "Reported illuminance at full scale", nullptr, 0, ExposeCategory::Config, 0, 65535, 0},
    {"action",            ExposeType::Enum,    Access::State,    nullptr, "Triggered action (e.g. a button click)",
     kActions_ZG_204ZL_z, sizeof(kActions_ZG_204ZL_z)/sizeof(kActions_ZG_204ZL_z[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0406},  // msOccupancySensing
    {1, 0x0400},  // msIlluminanceMeasurement
    {1, 0x0006},  // genOnOff (state, startUpOnOff, on/off commands)
    {1, 0x0401},  // msIlluminanceLevelSensing (light_level)
};
constexpr ReportingSpec kReports_ZG_204ZL_z[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},   // m.onOff: onOff MIN..MAX / 1
    {1, 0x0401, 0x0000, 0x30, 0, 3600, 1, 0},    // light_level: levelStatus MIN..1 h / 1
};

}  // namespace

extern const PreparedDefinition kDef_ZG_204ZL_z{
    .zigbee_models=kModels_ZG_204ZL_z, .zigbee_models_count=sizeof(kModels_ZG_204ZL_z)/sizeof(kModels_ZG_204ZL_z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG-204ZL-z", .vendor="HOBEIAN",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZG_204ZL_z, .from_zigbee_count=sizeof(kFz_ZG_204ZL_z)/sizeof(kFz_ZG_204ZL_z[0]),
    .to_zigbee=kTz_ZG_204ZL_z, .to_zigbee_count=sizeof(kTz_ZG_204ZL_z)/sizeof(kTz_ZG_204ZL_z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .reports=kReports_ZG_204ZL_z, .reports_count=sizeof(kReports_ZG_204ZL_z)/sizeof(kReports_ZG_204ZL_z[0]),
};

}  // namespace zhc::devices::custom_devices_diy
