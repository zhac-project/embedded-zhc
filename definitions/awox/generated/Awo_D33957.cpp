// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Awox 33957 — hand-rewritten to use awox/_shared CTLight bundle.
// LED light with color temperature — m.light({colorTemp:{range:[153,454]}}).
// Fingerprint-matched only on TLSR82xx — no zigbeeModel slug.
// z2m-source: awox.ts #33957.
#include "definitions/_generic/_shared.hpp"
#include "definitions/awox/_shared.hpp"

namespace zhc::devices::awox {
namespace {

constexpr const char* kModels_D33957[] = { "TLSR82xx" };

}  // namespace

extern const PreparedDefinition kDef_D33957{
    .zigbee_models=kModels_D33957, .zigbee_models_count=sizeof(kModels_D33957)/sizeof(kModels_D33957[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="33957", .vendor="Awox",
    .meta=nullptr,
    .exposes=kExposesAwoxCTLight, .exposes_count=kExposesAwoxCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAwoxCTLight, .from_zigbee_count=kFzAwoxCTLightCount,
    .to_zigbee=kTzAwoxCTLight, .to_zigbee_count=kTzAwoxCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAwoxCTLight, .bindings_count=kBindingsAwoxCTLightCount,
};

}  // namespace zhc::devices::awox
