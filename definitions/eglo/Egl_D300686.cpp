// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Eglo 300686 (EZMB-RGB-TW-CLB) — MASSIGNANO-Z RGBW ceiling light.
//
// z2m models it with m.light({colorTemp:{range:[153,370]},
// color:{modes:["xy","hs"], enhancedHue:true}}). The generated def wired
// on/off + brightness only — the lightingColorCtrl (0x0300) reports for
// color_temp / colour (hue/sat/xy) were dead. Re-wired to the eglo/_shared
// full-RGBW bundle so color_temp + colour decode and set.
//
// z2m-source: eglo.ts #300686.
#include "definitions/eglo/_shared.hpp"

namespace zhc::devices::eglo {
namespace {
constexpr const char* kModels_D300686[] = { "EZMB-RGB-TW-CLB" };
}  // namespace

extern const PreparedDefinition kDef_D300686{
    .zigbee_models=kModels_D300686, .zigbee_models_count=sizeof(kModels_D300686)/sizeof(kModels_D300686[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="300686", .vendor="Eglo",
    .meta=nullptr, .exposes=kExposesEgloColorCTLight, .exposes_count=kExposesEgloColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzEgloColorCTLight, .from_zigbee_count=kFzEgloColorCTLightCount,
    .to_zigbee=kTzEgloColorCTLight, .to_zigbee_count=kTzEgloColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindingsEgloColorCTLight,.bindings_count=kBindingsEgloColorCTLightCount,
};

}  // namespace zhc::devices::eglo
