// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Paulmann 371222402 — hand-rewritten to use shared Light bundle.
// Puric pane pendant light 6x6W dimmable
// z2m-source: paulmann.ts #371222402.
#include "definitions/_generic/_shared.hpp"
#include "definitions/paulmann/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
constexpr const char* kModels_D371222402[] = { "371222402" };
}  // namespace

extern const PreparedDefinition kDef_D371222402{
    .zigbee_models=kModels_D371222402, .zigbee_models_count=sizeof(kModels_D371222402)/sizeof(kModels_D371222402[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="371222402", .vendor="Paulmann",
    .meta=nullptr, .exposes=kExposesPaulmannLight, .exposes_count=kExposesPaulmannLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulmannLight, .from_zigbee_count=kFzPaulmannLightCount,
    .to_zigbee=kTzPaulmannLight, .to_zigbee_count=kTzPaulmannLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsPaulmannLight, .bindings_count=kBindingsPaulmannLightCount,
};

}  // namespace zhc::devices::paulmann
