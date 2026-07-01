// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Eglo 901471 (z2m v26.76.0 parity).
// z2m-source: eglo.ts #901471
//
// EGLO ROVITO-Z ceiling light (EBF-RGB-ZMB-CLB). z2m models it with
// m.light({colorTemp:{range:[153,370]}, color:{modes:["xy","hs"],
// enhancedHue:true}}) — full RGBW on/off + brightness + color_temp + colour.
// Wired to the eglo/_shared full-RGBW bundle, same as sibling ROVITO-Z 900091
// and MASSIGNANO-Z 300686.
//
// The z2m entry also declares m.deviceEndpoints({1:1, 3:3}) plus
// m.commandsOnOff/LevelCtrl/ColorCtrl. Those build an unsuffixed `action`
// enum from EP3 command frames (a secondary controller surface). The light
// itself is single-endpoint: m.light() passes no endpointNames, so its
// exposes are NOT endpoint-suffixed (EP1 only). The command/action
// transmitter side is DEFERRED — the primary light function is fully covered
// here (matches the 300686 precedent, which likewise defers commandsOnOff).
#include "definitions/eglo/_shared.hpp"

namespace zhc::devices::eglo {
namespace {
constexpr const char* kModels_D901471[] = { "EBF-RGB-ZMB-CLB" };
}  // namespace

extern const PreparedDefinition kDef_D901471{
    .zigbee_models=kModels_D901471, .zigbee_models_count=sizeof(kModels_D901471)/sizeof(kModels_D901471[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="901471", .vendor="Eglo",
    .meta=nullptr, .exposes=kExposesEgloColorCTLight, .exposes_count=kExposesEgloColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzEgloColorCTLight, .from_zigbee_count=kFzEgloColorCTLightCount,
    .to_zigbee=kTzEgloColorCTLight, .to_zigbee_count=kTzEgloColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindingsEgloColorCTLight,.bindings_count=kBindingsEgloColorCTLightCount,
};

}  // namespace zhc::devices::eglo
