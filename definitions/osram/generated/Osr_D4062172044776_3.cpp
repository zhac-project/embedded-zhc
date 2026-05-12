// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram 4062172044776_3 — auto-generated.
// Zigbee 3.0 DALI CONV LI dimmer for DALI-based luminaires (with two devices)
// z2m-source: osram.ts #4062172044776_3.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_D4062172044776_3[] = { "Zigbee 3.0 DALI CONV LI" };

constexpr ::zhc::EndpointLabel kEndpoints_D4062172044776_3[] = { {"l1", 10}, {"l2", 11} };

}  // namespace

extern const PreparedDefinition kDef_D4062172044776_3{
    .zigbee_models=kModels_D4062172044776_3, .zigbee_models_count=sizeof(kModels_D4062172044776_3)/sizeof(kModels_D4062172044776_3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4062172044776_3", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramCTLight, .exposes_count=kExposesOsramCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramCTLight, .from_zigbee_count=kFzOsramCTLightCount,
    .to_zigbee=kTzOsramCTLight, .to_zigbee_count=kTzOsramCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramCTLight,
    .bindings_count           = kBindingsOsramCTLightCount,
    .endpoint_map       = kEndpoints_D4062172044776_3,
    .endpoint_map_count = sizeof(kEndpoints_D4062172044776_3)/sizeof(kEndpoints_D4062172044776_3[0]),
};

}  // namespace zhc::devices::osram
