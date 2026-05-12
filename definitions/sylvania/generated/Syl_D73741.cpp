// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sylvania 73741 — auto-generated.
// LIGHTIFY LED adjustable color RT 5/6
// Bundle: ColorCTLight (RGB + tunable white).
// z2m-source: sylvania.ts #73741.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sylvania/_shared.hpp"

namespace zhc::devices::sylvania {
namespace {

constexpr const char* kModels_D73741[] = { "RT RGBW" };

}  // namespace

extern const PreparedDefinition kDef_D73741{
    .zigbee_models=kModels_D73741, .zigbee_models_count=sizeof(kModels_D73741)/sizeof(kModels_D73741[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="73741", .vendor="Sylvania",
    .meta=nullptr, .exposes=kExposesSylvaniaColorCTLight, .exposes_count=kExposesSylvaniaColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSylvaniaColorCTLight, .from_zigbee_count=kFzSylvaniaColorCTLightCount,
    .to_zigbee=kTzSylvaniaColorCTLight, .to_zigbee_count=kTzSylvaniaColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsSylvaniaColorCTLight,
    .bindings_count           = kBindingsSylvaniaColorCTLightCount,
};

}  // namespace zhc::devices::sylvania
