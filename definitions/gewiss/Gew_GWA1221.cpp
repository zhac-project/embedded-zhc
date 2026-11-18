// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Gewiss GWA1221 Chorus axial dimmer module (zigbeeModel
// GWA1221_DIMMER), new in z2m v26.126.0..v26.133.0. Without it the substring
// pass handed the module to whichever "DIMMER" def came first (CTM 4503145).
// m.deviceTemperature + m.light (power_on_behavior, no effect) +
// m.lightingBallast (lightingBallastCfg minLevel 0x0010 / maxLevel 0x0011, uint8) +
// m.electricityMeter current only (rmsCurrent / 1000).
// Not ported: the gewissLed cluster 0xFD79 led_standby_brightness /
// led_moving_brightness (percent written as value * 255 / 100), identify.
// z2m-source: gewiss.ts #GWA1221.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::gewiss {
namespace {

constexpr ::zhc::generic::ZclAttrRow kCurrentRows[] = { { 0x0508, "current", 1000 } };
constexpr ::zhc::generic::ZclAttrMap kCurrentMap{ kCurrentRows, 1, 0 };
constexpr FzConverter kFzCurrent = ::zhc::generic::zcl_attr_fz("haElectricalMeasurement", &kCurrentMap);

constexpr ::zhc::generic::ZclAttrRow kBallastRows[] = {
    { 0x0010, "ballast_minimum_level" },
    { 0x0011, "ballast_maximum_level" },
};
constexpr ::zhc::generic::ZclAttrMap kBallastMap{ kBallastRows, 2, 0 };
constexpr FzConverter kFzBallast = ::zhc::generic::zcl_attr_fz("lightingBallastCfg", &kBallastMap);
constexpr ::zhc::generic::ZclWriteSpec kBallastMinSpec{ "ballast_minimum_level", 0x0010, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kBallastMaxSpec{ "ballast_maximum_level", 0x0011, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzBallastMin = ::zhc::generic::zcl_write_tz("lightingBallastCfg", 0x0301, &kBallastMinSpec);
constexpr TzConverter kTzBallastMax = ::zhc::generic::zcl_write_tz("lightingBallastCfg", 0x0301, &kBallastMaxSpec);

const FzConverter* const kFz_GWA1221[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzDeviceTemperature,
    &kFzCurrent,
    &kFzBallast,
};
const TzConverter* const kTz_GWA1221[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzPowerOnBehavior1,
    &kTzBallastMin,
    &kTzBallastMax,
};

constexpr const char* kModels_GWA1221[] = { "GWA1221_DIMMER" };

constexpr Expose kExposes_GWA1221[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"device_temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"ballast_minimum_level", ExposeType::Numeric, Access::StateSet, nullptr,
     "Specifies the minimum light output of the ballast", nullptr, 0, ExposeCategory::Config, 1, 254, 0},
    {"ballast_maximum_level", ExposeType::Numeric, Access::StateSet, nullptr,
     "Specifies the maximum light output of the ballast", nullptr, 0, ExposeCategory::Config, 1, 254, 0},
};

constexpr BindingSpec kBindings_GWA1221[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0B04},
};
constexpr ReportingSpec kReports_GWA1221[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 3600, 0, 0},
    {1, 0x0008, 0x0000, 0x20, 0, 3600, 1, 0},
    {1, 0x0B04, 0x0508, 0x21, 10, 65000, 5, 0},
};

}  // namespace

extern const PreparedDefinition kDef_GWA1221{
    .zigbee_models=kModels_GWA1221, .zigbee_models_count=sizeof(kModels_GWA1221)/sizeof(kModels_GWA1221[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GWA1221", .vendor="Gewiss",
    .meta=nullptr, .exposes=kExposes_GWA1221, .exposes_count=sizeof(kExposes_GWA1221)/sizeof(kExposes_GWA1221[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_GWA1221, .from_zigbee_count=sizeof(kFz_GWA1221)/sizeof(kFz_GWA1221[0]),
    .to_zigbee=kTz_GWA1221, .to_zigbee_count=sizeof(kTz_GWA1221)/sizeof(kTz_GWA1221[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_GWA1221, .bindings_count=sizeof(kBindings_GWA1221)/sizeof(kBindings_GWA1221[0]),
    .reports=kReports_GWA1221, .reports_count=sizeof(kReports_GWA1221)/sizeof(kReports_GWA1221[0]),
};

}  // namespace zhc::devices::gewiss
