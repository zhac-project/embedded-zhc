// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecosmart D1531 — uses shared Light bundle.
// A19 bright white bulb
// z2m-source: ecosmart.ts #D1531 — m.light().
#include "definitions/_generic/_shared.hpp"
#include "definitions/ecosmart/_shared.hpp"

namespace zhc::devices::ecosmart {
namespace {
constexpr const char* kModels_D1531[] = { "\\u0000\\u0002\\u0000\\u0004\\u0000\\f^I\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u000e", "\\u0000\\u0002\\u0000\\u0004^" "\xEF" "\xBF" "\xBD" "\xEF" "\xBF" "\xBD" "&\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u0000\\u000e" };

}  // namespace

extern const PreparedDefinition kDef_D1531{
    .zigbee_models=kModels_D1531,
    .zigbee_models_count=sizeof(kModels_D1531)/sizeof(kModels_D1531[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="D1531", .vendor="Ecosmart",
    .meta=nullptr,
    .exposes=kExposesEcosmartLight, .exposes_count=kExposesEcosmartLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzEcosmartLight, .from_zigbee_count=kFzEcosmartLightCount,
    .to_zigbee=kTzEcosmartLight,   .to_zigbee_count=kTzEcosmartLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsEcosmartLight, .bindings_count=kBindingsEcosmartLightCount,
};

}  // namespace zhc::devices::ecosmart
