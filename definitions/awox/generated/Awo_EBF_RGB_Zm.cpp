// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Awox EBF_RGB_Zm — hand-rewritten to use awox/_shared ColorCTLight bundle.
// LED with adjustable color temp on main ring; extra RGB strip for full colors.
// z2m: m.light({colorTemp:{range:[153,370]}, color:{modes:["xy","hs"]}}) + m.commandsOnOff().
// commandsOnOff() only adds command-action exposes (not modelled here, since
// the SPA does not surface action passthroughs for full lights yet).
// z2m-source: awox.ts #EBF_RGB_Zm.
#include "definitions/_generic/_shared.hpp"
#include "definitions/awox/_shared.hpp"

namespace zhc::devices::awox {
namespace {

constexpr const char* kModels_EBF_RGB_Zm[] = { "EBF_RGB_Zm" };

}  // namespace


constexpr WhiteLabel kWhiteLabels_EBF_RGB_Zm[] = {
    {"EGLO","900566"},
};
extern const PreparedDefinition kDef_EBF_RGB_Zm{
    .zigbee_models=kModels_EBF_RGB_Zm,
    .zigbee_models_count=sizeof(kModels_EBF_RGB_Zm)/sizeof(kModels_EBF_RGB_Zm[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EBF_RGB_Zm", .vendor="Awox",
    .meta=nullptr,
    .exposes=kExposesAwoxColorCTLight, .exposes_count=kExposesAwoxColorCTLightCount,
    .white_labels=kWhiteLabels_EBF_RGB_Zm, .white_labels_count=sizeof(kWhiteLabels_EBF_RGB_Zm)/sizeof(kWhiteLabels_EBF_RGB_Zm[0]),
    .from_zigbee=kFzAwoxColorCTLight, .from_zigbee_count=kFzAwoxColorCTLightCount,
    .to_zigbee=kTzAwoxColorCTLight, .to_zigbee_count=kTzAwoxColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAwoxColorCTLight, .bindings_count=kBindingsAwoxColorCTLightCount,
};

}  // namespace zhc::devices::awox
