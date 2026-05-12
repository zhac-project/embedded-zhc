// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Leviton DG6HD-1BW — m.light() defaults add power_on_behavior.
// Zigbee in-wall smart dimmer
// z2m-source: leviton.ts #DG6HD-1BW.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leviton {
namespace {
const FzConverter* const kFz_DG6HD_1BW[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_DG6HD_1BW[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels_DG6HD_1BW[] = { "DG6HD" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_DG6HD_1BW{
    .zigbee_models=kModels_DG6HD_1BW, .zigbee_models_count=sizeof(kModels_DG6HD_1BW)/sizeof(kModels_DG6HD_1BW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DG6HD-1BW", .vendor="Leviton",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DG6HD_1BW, .from_zigbee_count=sizeof(kFz_DG6HD_1BW)/sizeof(kFz_DG6HD_1BW[0]),
    .to_zigbee=kTz_DG6HD_1BW, .to_zigbee_count=sizeof(kTz_DG6HD_1BW)/sizeof(kTz_DG6HD_1BW[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::leviton
