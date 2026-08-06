// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Innr RCL 242 C round ceiling light 42cm, white and colour (z2m v26.92.0 parity).
// z2m-source: innr.ts #RCL 242 C (added v26.78.0)
// Upstream colorTemp range [153, 556] mireds.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::innr {
namespace {
const FzConverter* const kFz_RCL_242_C[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_RCL_242_C[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_RCL_242_C[] = { "RCL 242 C" };

constexpr Expose kExposes_RCL_242_C[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_RCL_242_C[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_RCL_242_C{
    .zigbee_models=kModels_RCL_242_C, .zigbee_models_count=sizeof(kModels_RCL_242_C)/sizeof(kModels_RCL_242_C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RCL 242 C", .vendor="Innr",
    .meta=nullptr, .exposes=kExposes_RCL_242_C, .exposes_count=sizeof(kExposes_RCL_242_C)/sizeof(kExposes_RCL_242_C[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RCL_242_C, .from_zigbee_count=sizeof(kFz_RCL_242_C)/sizeof(kFz_RCL_242_C[0]),
    .to_zigbee=kTz_RCL_242_C, .to_zigbee_count=sizeof(kTz_RCL_242_C)/sizeof(kTz_RCL_242_C[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_RCL_242_C, .bindings_count=sizeof(kBindings_RCL_242_C)/sizeof(kBindings_RCL_242_C[0]),
};

}  // namespace zhc::devices::innr
