// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Leviton DL15S-1BZ — m.onOff() defaults add power_on_behavior.
// Lumina RF 15A switch, 120/277V
// z2m-source: leviton.ts #DL15S-1BZ.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leviton {
namespace {
const FzConverter* const kFz_DL15S_1BZ[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_DL15S_1BZ[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels_DL15S_1BZ[] = { "DL15S" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_DL15S_1BZ{
    .zigbee_models=kModels_DL15S_1BZ, .zigbee_models_count=sizeof(kModels_DL15S_1BZ)/sizeof(kModels_DL15S_1BZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DL15S-1BZ", .vendor="Leviton",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DL15S_1BZ, .from_zigbee_count=sizeof(kFz_DL15S_1BZ)/sizeof(kFz_DL15S_1BZ[0]),
    .to_zigbee=kTz_DL15S_1BZ, .to_zigbee_count=sizeof(kTz_DL15S_1BZ)/sizeof(kTz_DL15S_1BZ[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::leviton
