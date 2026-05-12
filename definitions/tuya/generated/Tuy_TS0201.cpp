// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0201 — temperature + humidity + battery sensor.
// z2m-source: tuya.ts #TS0201.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tuya {
namespace {

const FzConverter* const kFz_TS0201[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_TS0201[] = { "TS0201", "SNTZ003", "TY0201" };

constexpr Expose kExposes_TS0201[] = {
    { "temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_TS0201[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
};

}  // namespace

extern const PreparedDefinition kDef_TS0201{
    .zigbee_models            = kModels_TS0201,
    .zigbee_models_count      = sizeof(kModels_TS0201)/sizeof(kModels_TS0201[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "TS0201",
    .vendor                   = "Tuya",
    .meta                     = nullptr,
    .exposes                  = kExposes_TS0201,
    .exposes_count            = sizeof(kExposes_TS0201)/sizeof(kExposes_TS0201[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_TS0201,
    .from_zigbee_count        = sizeof(kFz_TS0201)/sizeof(kFz_TS0201[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_TS0201,
    .bindings_count           = sizeof(kBindings_TS0201)/sizeof(kBindings_TS0201[0]),
};

}  // namespace zhc::devices::tuya
