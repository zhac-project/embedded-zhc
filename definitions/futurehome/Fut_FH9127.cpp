// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Futurehome FH9127 in-wall dimmer (z2m v26.125.0 window).
// m.light({configureReporting: true}), the same surface as FH9130 (4509243).
// z2m-source: futurehome.ts #FH9127.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::futurehome {
namespace {
const FzConverter* const kFz_FH9127[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_FH9127[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_FH9127[] = { "FH9127" };

constexpr Expose kExposes_FH9127[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_FH9127[] = {
    {1, 0x0006},
    {1, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_FH9127{
    .zigbee_models=kModels_FH9127, .zigbee_models_count=sizeof(kModels_FH9127)/sizeof(kModels_FH9127[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FH9127", .vendor="Futurehome",
    .meta=nullptr, .exposes=kExposes_FH9127, .exposes_count=sizeof(kExposes_FH9127)/sizeof(kExposes_FH9127[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_FH9127, .from_zigbee_count=sizeof(kFz_FH9127)/sizeof(kFz_FH9127[0]),
    .to_zigbee=kTz_FH9127, .to_zigbee_count=sizeof(kTz_FH9127)/sizeof(kTz_FH9127[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_FH9127, .bindings_count=sizeof(kBindings_FH9127)/sizeof(kBindings_FH9127[0]),
};

}  // namespace zhc::devices::futurehome
