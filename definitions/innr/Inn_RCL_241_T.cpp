// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Innr RCL 241 T round ceiling light 42cm, warm-to-cool white (z2m v26.92.0 parity).
// z2m-source: innr.ts #RCL 241 T (added v26.86.0)
// Upstream colorTemp range [153, 500] mireds.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::innr {
namespace {
const FzConverter* const kFz_RCL_241_T[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_RCL_241_T[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_RCL_241_T[] = { "RCL 241 T" };

constexpr Expose kExposes_RCL_241_T[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_RCL_241_T[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_RCL_241_T{
    .zigbee_models=kModels_RCL_241_T, .zigbee_models_count=sizeof(kModels_RCL_241_T)/sizeof(kModels_RCL_241_T[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RCL 241 T", .vendor="Innr",
    .meta=nullptr, .exposes=kExposes_RCL_241_T, .exposes_count=sizeof(kExposes_RCL_241_T)/sizeof(kExposes_RCL_241_T[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RCL_241_T, .from_zigbee_count=sizeof(kFz_RCL_241_T)/sizeof(kFz_RCL_241_T[0]),
    .to_zigbee=kTz_RCL_241_T, .to_zigbee_count=sizeof(kTz_RCL_241_T)/sizeof(kTz_RCL_241_T[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_RCL_241_T, .bindings_count=sizeof(kBindings_RCL_241_T)/sizeof(kBindings_RCL_241_T[0]),
};

}  // namespace zhc::devices::innr
