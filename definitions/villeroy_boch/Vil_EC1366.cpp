// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Villeroy & Boch EC1366 My View mirror cabinet (z2m v26.92.0 parity).
// z2m-source: villeroy_boch.ts #EC1366 (added v26.78.0)
// Upstream colorTemp range [153, 500] mireds.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::villeroy_boch {
namespace {
const FzConverter* const kFz_EC1366[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_EC1366[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_EC1366[] = { "EC1366" };

constexpr Expose kExposes_EC1366[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_EC1366[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_EC1366{
    .zigbee_models=kModels_EC1366, .zigbee_models_count=sizeof(kModels_EC1366)/sizeof(kModels_EC1366[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EC1366", .vendor="Villeroy & Boch",
    .meta=nullptr, .exposes=kExposes_EC1366, .exposes_count=sizeof(kExposes_EC1366)/sizeof(kExposes_EC1366[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EC1366, .from_zigbee_count=sizeof(kFz_EC1366)/sizeof(kFz_EC1366[0]),
    .to_zigbee=kTz_EC1366, .to_zigbee_count=sizeof(kTz_EC1366)/sizeof(kTz_EC1366[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_EC1366, .bindings_count=sizeof(kBindings_EC1366)/sizeof(kBindings_EC1366[0]),
};

}  // namespace zhc::devices::villeroy_boch
