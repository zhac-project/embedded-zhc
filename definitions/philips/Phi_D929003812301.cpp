// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Philips Hue Runner single spotlight (white) (z2m v26.92.0 parity).
// z2m-source: philips.ts #929003812301 (added v26.91.0)
// Upstream colorTemp range [153, 454] mireds.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::philips {
namespace {
const FzConverter* const kFz_D929003812301[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_D929003812301[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_D929003812301[] = { "929003812301" };

constexpr Expose kExposes_D929003812301[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D929003812301[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_D929003812301{
    .zigbee_models=kModels_D929003812301, .zigbee_models_count=sizeof(kModels_D929003812301)/sizeof(kModels_D929003812301[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="929003812301", .vendor="Philips",
    .meta=nullptr, .exposes=kExposes_D929003812301, .exposes_count=sizeof(kExposes_D929003812301)/sizeof(kExposes_D929003812301[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D929003812301, .from_zigbee_count=sizeof(kFz_D929003812301)/sizeof(kFz_D929003812301[0]),
    .to_zigbee=kTz_D929003812301, .to_zigbee_count=sizeof(kTz_D929003812301)/sizeof(kTz_D929003812301[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D929003812301, .bindings_count=sizeof(kBindings_D929003812301)/sizeof(kBindings_D929003812301[0]),
};

}  // namespace zhc::devices::philips
