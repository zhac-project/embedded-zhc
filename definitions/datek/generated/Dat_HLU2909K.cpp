// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Datek HLU2909K — hand-upgraded for full z2m parity.
// APEX smart plug 16A — on/off + electrical_measurement + temperature
//                       + power_on_behavior write.
// z2m-source: datek.ts #HLU2909K.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::datek {
namespace {
const FzConverter* const kFz_HLU2909K[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_HLU2909K[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels_HLU2909K[] = { "PoP" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0B04},  // haElectricalMeasurement
    {1, 0x0402},  // msTemperatureMeasurement
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_HLU2909K{
    .zigbee_models=kModels_HLU2909K, .zigbee_models_count=sizeof(kModels_HLU2909K)/sizeof(kModels_HLU2909K[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HLU2909K", .vendor="Datek",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HLU2909K, .from_zigbee_count=sizeof(kFz_HLU2909K)/sizeof(kFz_HLU2909K[0]),
    .to_zigbee=kTz_HLU2909K, .to_zigbee_count=sizeof(kTz_HLU2909K)/sizeof(kTz_HLU2909K[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::datek
