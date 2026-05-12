// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Owon THS317 — auto-generated.
// Temperature and humidity sensor
// z2m-source: owon.ts #THS317.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {
const FzConverter* const kFz_THS317[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_THS317[] = { "THS317" };

}  // namespace


// z2m: e.battery(), e.temperature(), e.humidity(); reporting on endpoint 2.
constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {2, 0x0001},  // genPowerCfg
    {2, 0x0402},  // msTemperatureMeasurement
    {2, 0x0405},  // msRelativeHumidity
};

extern const PreparedDefinition kDef_THS317{
    .zigbee_models=kModels_THS317, .zigbee_models_count=sizeof(kModels_THS317)/sizeof(kModels_THS317[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="THS317", .vendor="Owon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_THS317, .from_zigbee_count=sizeof(kFz_THS317)/sizeof(kFz_THS317[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::owon
