// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram AC08562 — auto-generated.
// SMART+ Candle E14 Dimmable White
// z2m-source: osram.ts #AC08562.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_AC08562[] = { "B40 DIM Z3" };

}  // namespace

extern const PreparedDefinition kDef_AC08562{
    .zigbee_models=kModels_AC08562, .zigbee_models_count=sizeof(kModels_AC08562)/sizeof(kModels_AC08562[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AC08562", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramLight, .exposes_count=kExposesOsramLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramLight, .from_zigbee_count=kFzOsramLightCount,
    .to_zigbee=kTzOsramLight, .to_zigbee_count=kTzOsramLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramLight,
    .bindings_count           = kBindingsOsramLightCount,
};

}  // namespace zhc::devices::osram
