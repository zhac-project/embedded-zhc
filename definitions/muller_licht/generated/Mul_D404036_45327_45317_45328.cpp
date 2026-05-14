// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: MullerLicht 404036/45327/45317/45328 — auto-generated.
// Tint LED white+color
// z2m-source: muller_licht.ts #404036/45327/45317/45328.
#include "definitions/_generic/_shared.hpp"
#include "definitions/muller_licht/_shared.hpp"

namespace zhc::devices::muller_licht {
namespace {


constexpr const char* kModels_D404036_45327_45317_45328[] = { "tint-ExtendedColor" };

}  // namespace


constexpr WhiteLabel kWhiteLabels_D404036_45327_45317_45328[] = {
    {"Müller Licht","404026"},
    {"Müller Licht","404075"},
};
extern const PreparedDefinition kDef_D404036_45327_45317_45328{
    .zigbee_models=kModels_D404036_45327_45317_45328, .zigbee_models_count=sizeof(kModels_D404036_45327_45317_45328)/sizeof(kModels_D404036_45327_45317_45328[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="404036/45327/45317/45328", .vendor="MullerLicht",
    .meta=nullptr, .exposes=kExposesMullerLichtColorCTLight, .exposes_count=kExposesMullerLichtColorCTLightCount,
    .white_labels=kWhiteLabels_D404036_45327_45317_45328, .white_labels_count=sizeof(kWhiteLabels_D404036_45327_45317_45328)/sizeof(kWhiteLabels_D404036_45327_45317_45328[0]),
    .from_zigbee=kFzMullerLichtColorCTLight, .from_zigbee_count=kFzMullerLichtColorCTLightCount,
    .to_zigbee=kTzMullerLichtColorCTLight, .to_zigbee_count=kTzMullerLichtColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsMullerLichtColorCTLight,
    .bindings_count           = kBindingsMullerLichtColorCTLightCount,
};

}  // namespace zhc::devices::muller_licht
