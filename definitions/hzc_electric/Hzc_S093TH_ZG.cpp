// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HzcElectric S093TH-ZG — graduated from generated/.
// Temperature and humidity sensor
// z2m-source: hzc_electric.ts #S093TH-ZG.
//
// Bug fixed: the generated port wired the generic kFzOnOff + a phantom
// `state` expose (and a kTzOnOff write path) — but z2m decodes this sensor
// via fz.temperature + fz.humidity only. The real temperature/humidity
// channels were dropped and a dead on/off switch was synthesised. Re-pointed
// at the generic kFzTemperature (msTemperatureMeasurement 0x0402, /100) and
// kFzHumidity (msRelativeHumidity 0x0405, /100), restored the temperature +
// humidity exposes and 0x0402 / 0x0405 bindings, dropped the toZigbee path.
// (z2m note: battery percentage is not reported by this device.)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hzc_electric {
namespace {
const FzConverter* const kFz_S093TH_ZG[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
constexpr const char* kModels_S093TH_ZG[] = { "TempAndHumSensor-ZB3.0" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0402},
    {1, 0x0405},
};

extern const PreparedDefinition kDef_S093TH_ZG{
    .zigbee_models=kModels_S093TH_ZG, .zigbee_models_count=sizeof(kModels_S093TH_ZG)/sizeof(kModels_S093TH_ZG[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S093TH-ZG", .vendor="HzcElectric",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_S093TH_ZG, .from_zigbee_count=sizeof(kFz_S093TH_ZG)/sizeof(kFz_S093TH_ZG[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hzc_electric
