// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Leviton ZS057-D0Z — m.light() defaults add power_on_behavior;
// z2m also exposes ballast_minimum_level / ballast_maximum_level via
// closuresLightingBallastCfg (cluster 0x0301), no generic encoder yet.
// Wall switch, 0-10V dimmer, 120-277V, Lumina™ RF
// z2m-source: leviton.ts #ZS057-D0Z.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::leviton {
namespace {
const FzConverter* const kFz_ZS057_D0Z[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_ZS057_D0Z[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels_ZS057_D0Z[] = { "ZS057" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"ballast_minimum_level", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"ballast_maximum_level", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_ZS057_D0Z{
    .zigbee_models=kModels_ZS057_D0Z, .zigbee_models_count=sizeof(kModels_ZS057_D0Z)/sizeof(kModels_ZS057_D0Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZS057-D0Z", .vendor="Leviton",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZS057_D0Z, .from_zigbee_count=sizeof(kFz_ZS057_D0Z)/sizeof(kFz_ZS057_D0Z[0]),
    .to_zigbee=kTz_ZS057_D0Z, .to_zigbee_count=sizeof(kTz_ZS057_D0Z)/sizeof(kTz_ZS057_D0Z[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::leviton
