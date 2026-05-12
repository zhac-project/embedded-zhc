// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: MullerLicht 404000/404005/404012/404019 — auto-generated.
// Tint LED bulb GU10/E14/E27 350/470/806 lumen, dimmable, color, opal white
// z2m-source: muller_licht.ts #404000/404005/404012/404019.
#include "definitions/_generic/_shared.hpp"
#include "definitions/muller_licht/_shared.hpp"

namespace zhc::devices::muller_licht {
namespace {


constexpr const char* kModels_D404000_404005_404012_404019[] = { "ZBT-ExtendedColor", "Bulb white+color" };

}  // namespace

extern const PreparedDefinition kDef_D404000_404005_404012_404019{
    .zigbee_models=kModels_D404000_404005_404012_404019, .zigbee_models_count=sizeof(kModels_D404000_404005_404012_404019)/sizeof(kModels_D404000_404005_404012_404019[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="404000/404005/404012/404019", .vendor="MullerLicht",
    .meta=nullptr, .exposes=kExposesMullerLichtColorCTLight, .exposes_count=kExposesMullerLichtColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzMullerLichtColorCTLight, .from_zigbee_count=kFzMullerLichtColorCTLightCount,
    .to_zigbee=kTzMullerLichtColorCTLight, .to_zigbee_count=kTzMullerLichtColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsMullerLichtColorCTLight,
    .bindings_count           = kBindingsMullerLichtColorCTLightCount,
};

}  // namespace zhc::devices::muller_licht
