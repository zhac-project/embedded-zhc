// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Modular LWM005 dim module Hue 0-10V/1-10V 150-300W (z2m v26.92.0 parity).
// z2m-source: modular.ts #LWM005 (added v26.91.0)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::modular {
namespace {
const FzConverter* const kFz_LWM005[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_LWM005[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_LWM005[] = { "LWM005" };

constexpr Expose kExposes_LWM005[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_LWM005[] = {
    {1, 0x0006},
    {1, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_LWM005{
    .zigbee_models=kModels_LWM005, .zigbee_models_count=sizeof(kModels_LWM005)/sizeof(kModels_LWM005[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LWM005", .vendor="Modular",
    .meta=nullptr, .exposes=kExposes_LWM005, .exposes_count=sizeof(kExposes_LWM005)/sizeof(kExposes_LWM005[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LWM005, .from_zigbee_count=sizeof(kFz_LWM005)/sizeof(kFz_LWM005[0]),
    .to_zigbee=kTz_LWM005, .to_zigbee_count=sizeof(kTz_LWM005)/sizeof(kTz_LWM005[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_LWM005, .bindings_count=sizeof(kBindings_LWM005)/sizeof(kBindings_LWM005[0]),
};

}  // namespace zhc::devices::modular
