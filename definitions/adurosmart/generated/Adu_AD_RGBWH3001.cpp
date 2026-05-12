// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adurosmart AD-RGBWH3001 — hand-rewritten to use Adu_ColorCTLight bundle.
// ERIA Colors and White A19 Dimmable LED bulb.
// z2m extend: m.light({colorTemp: {range: [153, 500]}, color: {modes: ["xy", "hs"], enhancedHue: true}}).
// z2m-source: adurosmart.ts #AD-RGBWH3001.
#include "definitions/_generic/_shared.hpp"
#include "definitions/adurosmart/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_AD_RGBWH3001[] = { "AD-RGBWH3001" };

}  // namespace

extern const PreparedDefinition kDef_AD_RGBWH3001{
    .zigbee_models=kModels_AD_RGBWH3001,
    .zigbee_models_count=sizeof(kModels_AD_RGBWH3001)/sizeof(kModels_AD_RGBWH3001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AD-RGBWH3001", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kExposesAduColorCTLight, .exposes_count=kExposesAduColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAduColorCTLight, .from_zigbee_count=kFzAduColorCTLightCount,
    .to_zigbee=kTzAduColorCTLight, .to_zigbee_count=kTzAduColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAduColorCTLight, .bindings_count=kBindingsAduColorCTLightCount,
};

}  // namespace zhc::devices::adurosmart
