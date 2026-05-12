// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Awox 33943/33944/33946 — hand-rewritten to use awox/_shared ColorCTLight.
// LED RGB & brightness — m.light({colorTemp:{range:[153,370]}, color:{modes:["xy","hs"]}}).
// z2m-source: awox.ts #33943/33944/33946.
#include "definitions/_generic/_shared.hpp"
#include "definitions/awox/_shared.hpp"

namespace zhc::devices::awox {
namespace {

constexpr const char* kModels_D33943_33944_33946[] = { "ESMLzm_c5_GU10", "TLSR82xx" };

}  // namespace

extern const PreparedDefinition kDef_D33943_33944_33946{
    .zigbee_models=kModels_D33943_33944_33946,
    .zigbee_models_count=sizeof(kModels_D33943_33944_33946)/sizeof(kModels_D33943_33944_33946[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="33943/33944/33946", .vendor="Awox",
    .meta=nullptr,
    .exposes=kExposesAwoxColorCTLight, .exposes_count=kExposesAwoxColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAwoxColorCTLight, .from_zigbee_count=kFzAwoxColorCTLightCount,
    .to_zigbee=kTzAwoxColorCTLight, .to_zigbee_count=kTzAwoxColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAwoxColorCTLight, .bindings_count=kBindingsAwoxColorCTLightCount,
};

}  // namespace zhc::devices::awox
