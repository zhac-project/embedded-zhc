// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Candeo C205 (uses shared candeo switch_type).
// Zigbee switch module
// z2m-source: candeo.ts #C205.
#include "definitions/_generic/_shared.hpp"
#include "definitions/candeo/_shared.hpp"

namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C205[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::candeo::kFzExternalSwitchType
};
const TzConverter* const kTz_C205[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::candeo::kTzExternalSwitchType
};
constexpr const char* kModels_C205[] = { "C205" };

constexpr Expose kAutoExposes[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"external_switch_type", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006}
};

}  // namespace

extern const PreparedDefinition kDef_C205{
    .zigbee_models=kModels_C205, .zigbee_models_count=sizeof(kModels_C205)/sizeof(kModels_C205[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C205", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C205, .from_zigbee_count=sizeof(kFz_C205)/sizeof(kFz_C205[0]),
    .to_zigbee=kTz_C205, .to_zigbee_count=sizeof(kTz_C205)/sizeof(kTz_C205[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
