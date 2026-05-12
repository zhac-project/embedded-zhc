// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram ST8AU-CON — auto-generated.
// OSRAM SubstiTUBE T8 Advanced UO Connected
// z2m-source: osram.ts #ST8AU-CON.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_ST8AU_CON[] = { "SubstiTube", "Connected Tube Z3" };

}  // namespace

extern const PreparedDefinition kDef_ST8AU_CON{
    .zigbee_models=kModels_ST8AU_CON, .zigbee_models_count=sizeof(kModels_ST8AU_CON)/sizeof(kModels_ST8AU_CON[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ST8AU-CON", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramLight, .exposes_count=kExposesOsramLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramLight, .from_zigbee_count=kFzOsramLightCount,
    .to_zigbee=kTzOsramLight, .to_zigbee_count=kTzOsramLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramLight,
    .bindings_count           = kBindingsOsramLightCount,
};

}  // namespace zhc::devices::osram
