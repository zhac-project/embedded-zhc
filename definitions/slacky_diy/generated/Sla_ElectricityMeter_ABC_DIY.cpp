// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: SlackyDiy ElectricityMeter-ABC-DIY — auto-generated.
// Three phase Electricity Meter via optical port
// z2m-source: slacky_diy.ts #ElectricityMeter-ABC-DIY.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {
const FzConverter* const kFz_ElectricityMeter_ABC_DIY[] = {
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};

constexpr const char* kModels_ElectricityMeter_ABC_DIY[] = { "ElectricityMeterABC_DIY" };

}  // namespace


// z2m wires m.electricityMeter({threePhase: true}) which generates per-phase voltage/current/power.
// Runtime kFzElectricalMeasurement only emits the canonical voltage/power/current keys (single
// phase). Three-phase split (voltage_phase_a/b/c) requires runtime extension we do not add here.
constexpr Expose kAutoExposes[] = {
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0702}, {1, 0x0B04},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_ElectricityMeter_ABC_DIY{
    .zigbee_models=kModels_ElectricityMeter_ABC_DIY, .zigbee_models_count=sizeof(kModels_ElectricityMeter_ABC_DIY)/sizeof(kModels_ElectricityMeter_ABC_DIY[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ElectricityMeter-ABC-DIY", .vendor="SlackyDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ElectricityMeter_ABC_DIY, .from_zigbee_count=sizeof(kFz_ElectricityMeter_ABC_DIY)/sizeof(kFz_ElectricityMeter_ABC_DIY[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::slacky_diy
