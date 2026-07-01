// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Awox 110308 — Connect.Z GU10 tunable white, uses awox/_shared CTLight bundle.
// LED light with color temperature — m.light({colorTemp:{range:[153,370]}}).
// z2m-source: awox.ts #110308 (zigbeeModel ESMLzm_TW_GU10).
#include "definitions/_generic/_shared.hpp"
#include "definitions/awox/_shared.hpp"

namespace zhc::devices::awox {
namespace {

constexpr const char* kModels_D110308[] = { "ESMLzm_TW_GU10" };

}  // namespace

extern const PreparedDefinition kDef_D110308{
    .zigbee_models=kModels_D110308, .zigbee_models_count=sizeof(kModels_D110308)/sizeof(kModels_D110308[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="110308", .vendor="Awox",
    .meta=nullptr,
    .exposes=kExposesAwoxCTLight, .exposes_count=kExposesAwoxCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAwoxCTLight, .from_zigbee_count=kFzAwoxCTLightCount,
    .to_zigbee=kTzAwoxCTLight, .to_zigbee_count=kTzAwoxCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAwoxCTLight, .bindings_count=kBindingsAwoxCTLightCount,
};

}  // namespace zhc::devices::awox
