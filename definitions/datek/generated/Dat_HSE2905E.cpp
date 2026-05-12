// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Datek HSE2905E — hand-upgraded for full z2m parity.
// Datek Eva AMS HAN power-meter sensor.
// Wires the metering + electrical-measurement + temperature axes
// from the z2m m.electricityMeter() bundles. Three-phase split and
// `producedEnergy` are decoded via the generic clusters; runtime
// callers see the standard energy/power/voltage/current/temperature
// keys from the foundation parser.
// z2m-source: datek.ts #HSE2905E.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::datek {
namespace {
const FzConverter* const kFz_HSE2905E[] = {
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzTemperature,
};

constexpr const char* kModels_HSE2905E[] = { "Meter Reader" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0702},  // seMetering
    {1, 0x0B04},  // haElectricalMeasurement
    {1, 0x0402},  // msTemperatureMeasurement
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_HSE2905E{
    .zigbee_models=kModels_HSE2905E, .zigbee_models_count=sizeof(kModels_HSE2905E)/sizeof(kModels_HSE2905E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HSE2905E", .vendor="Datek",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HSE2905E, .from_zigbee_count=sizeof(kFz_HSE2905E)/sizeof(kFz_HSE2905E[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::datek
