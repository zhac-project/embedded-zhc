// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Eglo 900024/12253 (EGLO_ZM_RGB_TW) — RGBW light.
//
// z2m models it with m.light({colorTemp:{range:[153,370]},
// color:{modes:["xy","hs"]}}). The generated def wired on/off + brightness
// only — the lightingColorCtrl (0x0300) color_temp + colour reports were
// dead. Re-wired to the eglo/_shared full-RGBW bundle.
//
// z2m-source: eglo.ts #900024/12253.
#include "definitions/eglo/_shared.hpp"

namespace zhc::devices::eglo {
namespace {
constexpr const char* kModels_D900024_12253[] = { "EGLO_ZM_RGB_TW" };
}  // namespace

extern const PreparedDefinition kDef_D900024_12253{
    .zigbee_models=kModels_D900024_12253, .zigbee_models_count=sizeof(kModels_D900024_12253)/sizeof(kModels_D900024_12253[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="900024/12253", .vendor="Eglo",
    .meta=nullptr, .exposes=kExposesEgloColorCTLight, .exposes_count=kExposesEgloColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzEgloColorCTLight, .from_zigbee_count=kFzEgloColorCTLightCount,
    .to_zigbee=kTzEgloColorCTLight, .to_zigbee_count=kTzEgloColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindingsEgloColorCTLight,.bindings_count=kBindingsEgloColorCTLightCount,
};

}  // namespace zhc::devices::eglo
