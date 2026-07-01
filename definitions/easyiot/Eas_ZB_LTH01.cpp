// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Easyiot ZB-LTH01 — light + temperature/humidity sensor.
// z2m-source: easyiot.ts #ZB-LTH01
//   (extend: [m.temperature(), m.humidity(), m.illuminance(), m.battery()]).
//
// Standard-ZCL sensor, no vendor-specific frames: msTemperatureMeasurement
// (0x0402, /100), msRelativeHumidity (0x0405, /100), msIlluminanceMeasurement
// (0x0400), genPowerCfg (0x0001) battery — all decode via the shared generic
// converters. z2m matches by fingerprint {modelID:"ZB-LTH01",
// manufacturerName:"easyiot"}; modelID alone is unique here (lumi-style match).
#include "definitions/easyiot/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
const FzConverter* const kFz_ZB_LTH01[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ZB_LTH01[] = { "ZB-LTH01" };

constexpr Expose kExposes_ZB_LTH01[] = {
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_LTH01[] = {
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0400},  // msIlluminanceMeasurement
    {1, 0x0001},  // genPowerCfg
};

}  // namespace

extern const PreparedDefinition kDef_ZB_LTH01{
    .zigbee_models=kModels_ZB_LTH01, .zigbee_models_count=sizeof(kModels_ZB_LTH01)/sizeof(kModels_ZB_LTH01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-LTH01", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_LTH01, .exposes_count=sizeof(kExposes_ZB_LTH01)/sizeof(kExposes_ZB_LTH01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB_LTH01, .from_zigbee_count=sizeof(kFz_ZB_LTH01)/sizeof(kFz_ZB_LTH01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_LTH01, .bindings_count=sizeof(kBindings_ZB_LTH01)/sizeof(kBindings_ZB_LTH01[0]),
};

}  // namespace zhc::devices::easyiot
