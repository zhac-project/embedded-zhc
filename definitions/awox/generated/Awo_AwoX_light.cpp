// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Awox AwoX-light — hand-rewritten to use awox/_shared Light bundle.
// Generic light (m.light()), fingerprint-matched on TLSR82xx.
// z2m-source: awox.ts #AwoX-light.
#include "definitions/_generic/_shared.hpp"
#include "definitions/awox/_shared.hpp"

namespace zhc::devices::awox {
namespace {

constexpr const char* kModels_AwoX_light[] = { "TLSR82xx" };

}  // namespace


constexpr WhiteLabel kWhiteLabels_AwoX_light[] = {
    {"EGLO","12229"},
    {"EGLO","12256"},
};
extern const PreparedDefinition kDef_AwoX_light{
    .zigbee_models=kModels_AwoX_light, .zigbee_models_count=sizeof(kModels_AwoX_light)/sizeof(kModels_AwoX_light[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AwoX-light", .vendor="Awox",
    .meta=nullptr,
    .exposes=kExposesAwoxLight, .exposes_count=kExposesAwoxLightCount,
    .white_labels=kWhiteLabels_AwoX_light, .white_labels_count=sizeof(kWhiteLabels_AwoX_light)/sizeof(kWhiteLabels_AwoX_light[0]),
    .from_zigbee=kFzAwoxLight, .from_zigbee_count=kFzAwoxLightCount,
    .to_zigbee=kTzAwoxLight, .to_zigbee_count=kTzAwoxLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAwoxLight, .bindings_count=kBindingsAwoxLightCount,
};

}  // namespace zhc::devices::awox
