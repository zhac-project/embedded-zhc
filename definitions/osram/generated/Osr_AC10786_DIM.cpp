// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram AC10786-DIM — auto-generated.
// SMART+ classic E27 dimmable
// z2m-source: osram.ts #AC10786-DIM.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_AC10786_DIM[] = { "A60 DIM Z3" };

}  // namespace

extern const PreparedDefinition kDef_AC10786_DIM{
    .zigbee_models=kModels_AC10786_DIM, .zigbee_models_count=sizeof(kModels_AC10786_DIM)/sizeof(kModels_AC10786_DIM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AC10786-DIM", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramLight, .exposes_count=kExposesOsramLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramLight, .from_zigbee_count=kFzOsramLightCount,
    .to_zigbee=kTzOsramLight, .to_zigbee_count=kTzOsramLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramLight,
    .bindings_count           = kBindingsOsramLightCount,
};

}  // namespace zhc::devices::osram
