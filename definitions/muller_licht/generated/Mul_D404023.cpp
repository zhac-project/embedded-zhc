// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: MullerLicht 404023 — auto-generated.
// LED bulb E27 470 lumen, dimmable, clear
// z2m-source: muller_licht.ts #404023.
#include "definitions/_generic/_shared.hpp"
#include "definitions/muller_licht/_shared.hpp"

namespace zhc::devices::muller_licht {
namespace {


constexpr const char* kModels_D404023[] = { "ZBT-DIMLight-A4700001" };

}  // namespace

extern const PreparedDefinition kDef_D404023{
    .zigbee_models=kModels_D404023, .zigbee_models_count=sizeof(kModels_D404023)/sizeof(kModels_D404023[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="404023", .vendor="MullerLicht",
    .meta=nullptr, .exposes=kExposesMullerLichtLight, .exposes_count=kExposesMullerLichtLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzMullerLichtLight, .from_zigbee_count=kFzMullerLichtLightCount,
    .to_zigbee=kTzMullerLichtLight, .to_zigbee_count=kTzMullerLichtLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsMullerLichtLight,
    .bindings_count           = kBindingsMullerLichtLightCount,
};

}  // namespace zhc::devices::muller_licht
