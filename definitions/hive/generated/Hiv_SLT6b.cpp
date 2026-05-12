// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Hive SLT6b — hand-tuned.
// Heating thermostat remote control. Unlike SLT6 (battery only), SLT6b
// uses `extend: [m.battery(), m.temperature()]` upstream, so it also
// reports msTemperatureMeasurement.
// z2m-source: hive.ts #SLT6b.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hive {
namespace {
const FzConverter* const kFz_SLT6b[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
};

constexpr const char* kModels_SLT6b[] = { "SLT6b" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
};

extern const PreparedDefinition kDef_SLT6b{
    .zigbee_models=kModels_SLT6b, .zigbee_models_count=sizeof(kModels_SLT6b)/sizeof(kModels_SLT6b[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SLT6b", .vendor="Hive",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SLT6b, .from_zigbee_count=sizeof(kFz_SLT6b)/sizeof(kFz_SLT6b[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hive
