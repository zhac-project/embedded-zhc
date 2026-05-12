// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecosmart D1523 — uses shared Light bundle.
// A19 soft white bulb
// z2m-source: ecosmart.ts #D1523 — m.light().
#include "definitions/_generic/_shared.hpp"
#include "definitions/ecosmart/_shared.hpp"

namespace zhc::devices::ecosmart {
namespace {
constexpr const char* kModels_D1523[] = { "\\u0000\\u0002\\u0000\\u0004" "\xEF" "\xBF" "\xBD" "V\\u0000\\n\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u000e", "\xEF" "\xBF" "\xBD", "\\u0000\\u0002\\u0000\\u0004r " "\xEF" "\xBF" "\xBD" "P\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u000e", "\\u0000\\u0002\\u0000\\u0004b " "\xEF" "\xBF" "\xBD" "P\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u000e" };

}  // namespace

extern const PreparedDefinition kDef_D1523{
    .zigbee_models=kModels_D1523,
    .zigbee_models_count=sizeof(kModels_D1523)/sizeof(kModels_D1523[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="D1523", .vendor="Ecosmart",
    .meta=nullptr,
    .exposes=kExposesEcosmartLight, .exposes_count=kExposesEcosmartLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzEcosmartLight, .from_zigbee_count=kFzEcosmartLightCount,
    .to_zigbee=kTzEcosmartLight,   .to_zigbee_count=kTzEcosmartLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsEcosmartLight, .bindings_count=kBindingsEcosmartLightCount,
};

}  // namespace zhc::devices::ecosmart
