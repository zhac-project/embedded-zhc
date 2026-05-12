// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram AA70155 — auto-generated.
// LIGHTIFY LED A19 tunable white / Classic A60 TW
// z2m-source: osram.ts #AA70155.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_AA70155[] = { "LIGHTIFY A19 Tunable White", "Classic A60 TW" };

}  // namespace

extern const PreparedDefinition kDef_AA70155{
    .zigbee_models=kModels_AA70155, .zigbee_models_count=sizeof(kModels_AA70155)/sizeof(kModels_AA70155[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AA70155", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramCTLight, .exposes_count=kExposesOsramCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramCTLight, .from_zigbee_count=kFzOsramCTLightCount,
    .to_zigbee=kTzOsramCTLight, .to_zigbee_count=kTzOsramCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramCTLight,
    .bindings_count           = kBindingsOsramCTLightCount,
};

}  // namespace zhc::devices::osram
