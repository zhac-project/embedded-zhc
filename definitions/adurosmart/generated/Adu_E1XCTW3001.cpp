// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adurosmart E1XCTW3001 — hand-rewritten to use Adu_CTLight bundle.
// ERIA tunable-white candle bulb (E12).
// z2m extend: m.light({colorTemp: {range: [153, 500]}}).
// z2m-source: adurosmart.ts #E1XCTW3001.
#include "definitions/_generic/_shared.hpp"
#include "definitions/adurosmart/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_E1XCTW3001[] = { "AD-E1XCTW3001" };

}  // namespace

extern const PreparedDefinition kDef_E1XCTW3001{
    .zigbee_models=kModels_E1XCTW3001,
    .zigbee_models_count=sizeof(kModels_E1XCTW3001)/sizeof(kModels_E1XCTW3001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E1XCTW3001", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kExposesAduCTLight, .exposes_count=kExposesAduCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAduCTLight, .from_zigbee_count=kFzAduCTLightCount,
    .to_zigbee=kTzAduCTLight, .to_zigbee_count=kTzAduCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAduCTLight, .bindings_count=kBindingsAduCTLightCount,
};

}  // namespace zhc::devices::adurosmart
