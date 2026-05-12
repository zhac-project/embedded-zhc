// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecosmart A9A19A60WESDZ02 — uses shared CTLight bundle.
// Tuneable white (A19)
// z2m-source: ecosmart.ts #A9A19A60WESDZ02 — m.light({colorTemp: {range: [153, 370]}}).
#include "definitions/_generic/_shared.hpp"
#include "definitions/ecosmart/_shared.hpp"

namespace zhc::devices::ecosmart {
namespace {
constexpr const char* kModels_A9A19A60WESDZ02[] = { "Ecosmart-ZBT-A19-CCT-Bulb" };
}  // namespace

extern const PreparedDefinition kDef_A9A19A60WESDZ02{
    .zigbee_models=kModels_A9A19A60WESDZ02,
    .zigbee_models_count=sizeof(kModels_A9A19A60WESDZ02)/sizeof(kModels_A9A19A60WESDZ02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="A9A19A60WESDZ02", .vendor="Ecosmart",
    .meta=nullptr,
    .exposes=kExposesEcosmartCTLight, .exposes_count=kExposesEcosmartCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzEcosmartCTLight, .from_zigbee_count=kFzEcosmartCTLightCount,
    .to_zigbee=kTzEcosmartCTLight,   .to_zigbee_count=kTzEcosmartCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsEcosmartCTLight, .bindings_count=kBindingsEcosmartCTLightCount,
};

}  // namespace zhc::devices::ecosmart
