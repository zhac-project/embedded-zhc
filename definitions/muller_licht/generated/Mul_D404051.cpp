// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: MullerLicht 404051 — auto-generated.
// Tint LED-Spotlights, white+color 3x (1800-6500K+RGB) 19W
// z2m-source: muller_licht.ts #404051.
#include "definitions/_generic/_shared.hpp"
#include "definitions/muller_licht/_shared.hpp"

namespace zhc::devices::muller_licht {
namespace {


constexpr const char* kModels_D404051[] = { "tint-Spotlights" };

}  // namespace

extern const PreparedDefinition kDef_D404051{
    .zigbee_models=kModels_D404051, .zigbee_models_count=sizeof(kModels_D404051)/sizeof(kModels_D404051[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="404051", .vendor="MullerLicht",
    .meta=nullptr, .exposes=kExposesMullerLichtColorCTLight, .exposes_count=kExposesMullerLichtColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzMullerLichtColorCTLight, .from_zigbee_count=kFzMullerLichtColorCTLightCount,
    .to_zigbee=kTzMullerLichtColorCTLight, .to_zigbee_count=kTzMullerLichtColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsMullerLichtColorCTLight,
    .bindings_count           = kBindingsMullerLichtColorCTLightCount,
};

}  // namespace zhc::devices::muller_licht
