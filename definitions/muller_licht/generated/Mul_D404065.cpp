// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: MullerLicht 404065 — auto-generated.
// tint LED-Globe Retro Gold XXL E27
// z2m-source: muller_licht.ts #404065.
#include "definitions/_generic/_shared.hpp"
#include "definitions/muller_licht/_shared.hpp"

namespace zhc::devices::muller_licht {
namespace {


constexpr const char* kModels_D404065[] = { "Retro Bulb Gold XXL white+ambiance" };

}  // namespace

extern const PreparedDefinition kDef_D404065{
    .zigbee_models=kModels_D404065, .zigbee_models_count=sizeof(kModels_D404065)/sizeof(kModels_D404065[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="404065", .vendor="MullerLicht",
    .meta=nullptr, .exposes=kExposesMullerLichtCTLight, .exposes_count=kExposesMullerLichtCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzMullerLichtCTLight, .from_zigbee_count=kFzMullerLichtCTLightCount,
    .to_zigbee=kTzMullerLichtCTLight, .to_zigbee_count=kTzMullerLichtCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsMullerLichtCTLight,
    .bindings_count           = kBindingsMullerLichtCTLightCount,
};

}  // namespace zhc::devices::muller_licht
