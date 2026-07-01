// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Custom devices (DiY) ZG-204ZL-z luminance motion sensor (z2m v26.76.0 parity).
// z2m-source: custom_devices_diy.ts #ZG-204ZL-z
//
// Luminance motion sensor (pvvx/ZigbeeTLc). z2m extend =
//   m.battery(), m.occupancy(), m.illuminance(),
//   m.enumLookup(power_on_behavior, genOnOff/startUpOnOff),
//   m.numeric(pir_timeout, msOccupancySensing/pirOToUDelay).
// Battery (genPowerCfg 0x0001), occupancy (msOccupancySensing 0x0406 bit-0),
// illuminance (msIlluminanceMeasurement 0x0400 raw lux) and power_on_behavior
// (genOnOff 0x4003 startUpOnOff enum: off/on/toggle/previous) route through the
// matching generic converters. The pir_timeout config numeric
// (msOccupancySensing pirOToUDelay 0x0010, access ALL) has no generic converter
// and is DROPPED — tracked as PARTIAL, mirrors the "port the standard sensor
// stream, defer the vendor config numeric" precedent (cf. sonoff SNZB-02M).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
const FzConverter* const kFz_ZG_204ZL_z[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_ZG_204ZL_z[] = {
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels_ZG_204ZL_z[] = { "ZG-204ZL-z" };

constexpr Expose kAutoExposes[] = {
    {"battery",           ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",           ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"occupancy",         ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"illuminance",       ExposeType::Numeric, Access::State,    "lx", nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0406},  // msOccupancySensing
    {1, 0x0400},  // msIlluminanceMeasurement
    {1, 0x0006},  // genOnOff (startUpOnOff)
};

}  // namespace

extern const PreparedDefinition kDef_ZG_204ZL_z{
    .zigbee_models=kModels_ZG_204ZL_z, .zigbee_models_count=sizeof(kModels_ZG_204ZL_z)/sizeof(kModels_ZG_204ZL_z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG-204ZL-z", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZG_204ZL_z, .from_zigbee_count=sizeof(kFz_ZG_204ZL_z)/sizeof(kFz_ZG_204ZL_z[0]),
    .to_zigbee=kTz_ZG_204ZL_z, .to_zigbee_count=sizeof(kTz_ZG_204ZL_z)/sizeof(kTz_ZG_204ZL_z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::custom_devices_diy
