// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Rti-Tek STH1Z temperature and humidity sensor.
// New vendor upstream in z2m v26.100.0. Reports as zigbeeModel "STHZB".
//
// Temperature, humidity and battery all come off the standard measurement
// clusters, so the generic converters cover the sensing surface outright.
//
// DEFERRED: the vendor cluster 0xFD22 upstream declares — temperature unit,
// a 32-bit fault code, and internal temperature/humidity calibration offsets
// (signed, min -128). Those are settings rather than readings; wiring them
// needs a decode map plus signed attribute writes against a manufacturer
// cluster, with no generic helper to lean on. The sensor's actual output is
// fully covered without them.
//
// z2m-source: rti_tek.ts #STH1Z.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::rti_tek {
namespace {

const FzConverter* const kFz_STH1Z[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_STH1Z[] = { "STHZB" };

constexpr Expose kExposes_STH1Z[] = {
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

constexpr BindingSpec kBindings_STH1Z[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0405},   // msRelativeHumidity
};

}  // namespace

extern const PreparedDefinition kDef_STH1Z{
    .zigbee_models=kModels_STH1Z, .zigbee_models_count=sizeof(kModels_STH1Z)/sizeof(kModels_STH1Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="STH1Z", .vendor="Rti-Tek",
    .meta=nullptr,
    .exposes=kExposes_STH1Z, .exposes_count=sizeof(kExposes_STH1Z)/sizeof(kExposes_STH1Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_STH1Z, .from_zigbee_count=sizeof(kFz_STH1Z)/sizeof(kFz_STH1Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_STH1Z, .bindings_count=sizeof(kBindings_STH1Z)/sizeof(kBindings_STH1Z[0]),
};

}  // namespace zhc::devices::rti_tek
