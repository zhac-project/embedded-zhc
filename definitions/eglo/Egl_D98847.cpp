// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Eglo 98847 (EGLO_ZM_TW_CLP) — FUEVA-Z tunable-white ceiling light IP44.
//
// z2m models it with m.light({colorTemp:{range:[153,370]}}). The generated
// def wired on/off + brightness only — the lightingColorCtrl (0x0300)
// color_temp report was dead. Re-wired to the eglo/_shared tunable-white
// bundle so color_temp decodes and sets.
//
// z2m-source: eglo.ts #98847.
#include "definitions/eglo/_shared.hpp"

namespace zhc::devices::eglo {
namespace {
constexpr const char* kModels_D98847[] = { "EGLO_ZM_TW_CLP" };
}  // namespace

extern const PreparedDefinition kDef_D98847{
    .zigbee_models=kModels_D98847, .zigbee_models_count=sizeof(kModels_D98847)/sizeof(kModels_D98847[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="98847", .vendor="Eglo",
    .meta=nullptr, .exposes=kExposesEgloCTLight, .exposes_count=kExposesEgloCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzEgloCTLight, .from_zigbee_count=kFzEgloCTLightCount,
    .to_zigbee=kTzEgloCTLight, .to_zigbee_count=kTzEgloCTLightCount,
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindingsEgloCTLight,.bindings_count=kBindingsEgloCTLightCount,
};

}  // namespace zhc::devices::eglo
