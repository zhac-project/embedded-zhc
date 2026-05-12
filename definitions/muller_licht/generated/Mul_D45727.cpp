// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: MullerLicht 45727 — auto-generated.
// Tint Amela 42cm, white+ambiance (1800-6500K)
// z2m-source: muller_licht.ts #45727.
#include "definitions/_generic/_shared.hpp"
#include "definitions/muller_licht/_shared.hpp"

namespace zhc::devices::muller_licht {
namespace {


constexpr const char* kModels_D45727[] = { "Bulb white" };

}  // namespace

extern const PreparedDefinition kDef_D45727{
    .zigbee_models=kModels_D45727, .zigbee_models_count=sizeof(kModels_D45727)/sizeof(kModels_D45727[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="45727", .vendor="MullerLicht",
    .meta=nullptr, .exposes=kExposesMullerLichtCTLight, .exposes_count=kExposesMullerLichtCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzMullerLichtCTLight, .from_zigbee_count=kFzMullerLichtCTLightCount,
    .to_zigbee=kTzMullerLichtCTLight, .to_zigbee_count=kTzMullerLichtCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsMullerLichtCTLight,
    .bindings_count           = kBindingsMullerLichtCTLightCount,
};

}  // namespace zhc::devices::muller_licht
