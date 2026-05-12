// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: MullerLicht 45391 — auto-generated.
// Smart power strip
// z2m-source: muller_licht.ts #45391.
#include "definitions/_generic/_shared.hpp"
#include "definitions/muller_licht/_shared.hpp"

namespace zhc::devices::muller_licht {
namespace {

constexpr const char* kModels_D45391[] = { "tint smart power strip" };

}  // namespace

extern const PreparedDefinition kDef_D45391{
    .zigbee_models=kModels_D45391, .zigbee_models_count=sizeof(kModels_D45391)/sizeof(kModels_D45391[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="45391", .vendor="MullerLicht",
    .meta=nullptr, .exposes=kExposesMullerLichtOnOff, .exposes_count=kExposesMullerLichtOnOffCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzMullerLichtOnOff, .from_zigbee_count=kFzMullerLichtOnOffCount,
    .to_zigbee=kTzMullerLichtOnOff, .to_zigbee_count=kTzMullerLichtOnOffCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsMullerLichtOnOff, .bindings_count=kBindingsMullerLichtOnOffCount,
};

}  // namespace zhc::devices::muller_licht
