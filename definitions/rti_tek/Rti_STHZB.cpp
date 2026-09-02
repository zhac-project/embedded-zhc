// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Rti-Tek STHZB temperature and humidity sensor (STH1Z / STH2Z).
// New vendor upstream in z2m v26.100.0 as "STH1Z". z2m v26.103.0 (#13055)
// folded the STH2Z into the same entry and renamed the model to the shared
// zigbeeModel "STHZB": both variants report the same modelID and upstream
// tells them apart at runtime from the FD22 `productName` attribute (0x0003).
// Model renamed here to follow upstream.
//
// Temperature, humidity and battery all come off the standard measurement
// clusters, so the generic converters cover the sensing surface outright.
//
// DEFERRED: the vendor cluster 0xFD22 upstream declares — temperature unit,
// a 32-bit fault code, and internal temperature/humidity calibration offsets
// (signed, min -128). Those are settings rather than readings; wiring them
// needs a decode map plus signed attribute writes against a manufacturer
// cluster, with no generic helper to lean on. The sensor's actual output is
// fully covered without them. The STH2Z adds four comfort-limit attributes
// on the same cluster (0xE014..0xE017, humidity / temperature lower+upper);
// deferred with the rest of FD22.
//
// z2m-source: rti_tek.ts #STHZB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::rti_tek {
namespace {

const FzConverter* const kFz_STHZB[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_STHZB[] = { "STHZB" };

constexpr Expose kExposes_STHZB[] = {
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

constexpr BindingSpec kBindings_STHZB[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0405},   // msRelativeHumidity
};

}  // namespace

extern const PreparedDefinition kDef_STHZB{
    .zigbee_models=kModels_STHZB, .zigbee_models_count=sizeof(kModels_STHZB)/sizeof(kModels_STHZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="STHZB", .vendor="Rti-Tek",
    .meta=nullptr,
    .exposes=kExposes_STHZB, .exposes_count=sizeof(kExposes_STHZB)/sizeof(kExposes_STHZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_STHZB, .from_zigbee_count=sizeof(kFz_STHZB)/sizeof(kFz_STHZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_STHZB, .bindings_count=sizeof(kBindings_STHZB)/sizeof(kBindings_STHZB[0]),
};

}  // namespace zhc::devices::rti_tek
