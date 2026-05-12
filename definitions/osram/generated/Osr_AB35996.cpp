// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram AB35996 — auto-generated.
// Smart+ Spot GU10 Multicolor
// z2m-source: osram.ts #AB35996.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_AB35996[] = { "PAR 16 50 RGBW - LIGHTIFY" };

}  // namespace

extern const PreparedDefinition kDef_AB35996{
    .zigbee_models=kModels_AB35996, .zigbee_models_count=sizeof(kModels_AB35996)/sizeof(kModels_AB35996[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AB35996", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramColorCTLight, .exposes_count=kExposesOsramColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramColorCTLight, .from_zigbee_count=kFzOsramColorCTLightCount,
    .to_zigbee=kTzOsramColorCTLight, .to_zigbee_count=kTzOsramColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramColorCTLight,
    .bindings_count           = kBindingsOsramColorCTLightCount,
};

}  // namespace zhc::devices::osram
