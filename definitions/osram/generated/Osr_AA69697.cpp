// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram AA69697 — auto-generated.
// Classic A60 RGBW
// z2m-source: osram.ts #AA69697.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_AA69697[] = { "Classic A60 RGBW" };

}  // namespace

extern const PreparedDefinition kDef_AA69697{
    .zigbee_models=kModels_AA69697, .zigbee_models_count=sizeof(kModels_AA69697)/sizeof(kModels_AA69697[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AA69697", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramColorCTLight, .exposes_count=kExposesOsramColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramColorCTLight, .from_zigbee_count=kFzOsramColorCTLightCount,
    .to_zigbee=kTzOsramColorCTLight, .to_zigbee_count=kTzOsramColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramColorCTLight,
    .bindings_count           = kBindingsOsramColorCTLightCount,
};

}  // namespace zhc::devices::osram
