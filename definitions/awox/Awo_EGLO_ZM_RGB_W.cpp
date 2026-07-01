// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Awox EGLO_ZM_RGB_W — RGB bulb with dedicated 3000 K white LED,
// uses awox/_shared ColorCTLight bundle (on/off + brightness + color_temp + color).
// z2m: m.light({colorTemp:{range:[333,333], startup:false}, color:{modes:["xy","hs"]}}).
// z2m-source: awox.ts #EGLO_ZM_RGB_W.
#include "definitions/_generic/_shared.hpp"
#include "definitions/awox/_shared.hpp"

namespace zhc::devices::awox {
namespace {

constexpr const char* kModels_EGLO_ZM_RGB_W[] = { "EGLO_ZM_RGB_W" };

}  // namespace


constexpr WhiteLabel kWhiteLabels_EGLO_ZM_RGB_W[] = {
    {"EGLO","113135"},
};
extern const PreparedDefinition kDef_EGLO_ZM_RGB_W{
    .zigbee_models=kModels_EGLO_ZM_RGB_W,
    .zigbee_models_count=sizeof(kModels_EGLO_ZM_RGB_W)/sizeof(kModels_EGLO_ZM_RGB_W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EGLO_ZM_RGB_W", .vendor="Awox",
    .meta=nullptr,
    .exposes=kExposesAwoxColorCTLight, .exposes_count=kExposesAwoxColorCTLightCount,
    .white_labels=kWhiteLabels_EGLO_ZM_RGB_W, .white_labels_count=sizeof(kWhiteLabels_EGLO_ZM_RGB_W)/sizeof(kWhiteLabels_EGLO_ZM_RGB_W[0]),
    .from_zigbee=kFzAwoxColorCTLight, .from_zigbee_count=kFzAwoxColorCTLightCount,
    .to_zigbee=kTzAwoxColorCTLight, .to_zigbee_count=kTzAwoxColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAwoxColorCTLight, .bindings_count=kBindingsAwoxColorCTLightCount,
};

}  // namespace zhc::devices::awox
