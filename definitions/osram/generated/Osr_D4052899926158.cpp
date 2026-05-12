// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Osram 4052899926158 — auto-generated.
// LIGHTIFY Surface Light TW
// z2m-source: osram.ts #4052899926158.
#include "definitions/_generic/_shared.hpp"
#include "definitions/osram/_shared.hpp"

namespace zhc::devices::osram {
namespace {


constexpr const char* kModels_D4052899926158[] = { "Surface Light W " "\xEF" "\xBF" "\xBD" "C LIGHTIFY" };

}  // namespace

extern const PreparedDefinition kDef_D4052899926158{
    .zigbee_models=kModels_D4052899926158, .zigbee_models_count=sizeof(kModels_D4052899926158)/sizeof(kModels_D4052899926158[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4052899926158", .vendor="Osram",
    .meta=nullptr, .exposes=kExposesOsramLight, .exposes_count=kExposesOsramLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzOsramLight, .from_zigbee_count=kFzOsramLightCount,
    .to_zigbee=kTzOsramLight, .to_zigbee_count=kTzOsramLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsOsramLight,
    .bindings_count           = kBindingsOsramLightCount,
};

}  // namespace zhc::devices::osram
