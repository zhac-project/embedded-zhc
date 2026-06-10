// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Msh msh.bme280psm — multi-sensor restored.
// MSH outdoor thermometer with BME280
// z2m-source: msh.ts #msh.bme280psm
//   extend: [m.battery(), m.temperature(), m.humidity(), m.pressure()]
//
// Bug fixed: the Tier-1 port reduced this 4-channel BME280 sensor to
// battery-only. z2m m.temperature()/m.humidity()/m.pressure() bind the
// standard measurement clusters (msTemperatureMeasurement 0x0402 /100,
// msRelativeHumidity 0x0405 /100, msPressureMeasurement 0x0403). Wired the
// generic kFzTemperature / kFzHumidity / kFzPressure decoders + exposes +
// bindings so all four channels decode.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::msh {
namespace {
const FzConverter* const kFz_msh_bme280psm[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzPressure,
};

constexpr const char* kModels_msh_bme280psm[] = { "msh.bme280psm" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",  nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
    {"pressure",    ExposeType::Numeric, Access::State, "kPa", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0405},
    {1, 0x0403},
};

extern const PreparedDefinition kDef_msh_bme280psm{
    .zigbee_models=kModels_msh_bme280psm, .zigbee_models_count=sizeof(kModels_msh_bme280psm)/sizeof(kModels_msh_bme280psm[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="msh.bme280psm", .vendor="Msh",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_msh_bme280psm, .from_zigbee_count=sizeof(kFz_msh_bme280psm)/sizeof(kFz_msh_bme280psm[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::msh
