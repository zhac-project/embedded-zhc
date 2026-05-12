// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Paulmann 968.93 — hand-rewritten to use shared Light bundle.
// URail rail adapter smart home Zigbee on/off/dimm
// z2m-source: paulmann.ts #968.93.
#include "definitions/_generic/_shared.hpp"
#include "definitions/paulmann/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
constexpr const char* kModels_D968_93[] = { "H036-0500" };
}  // namespace

extern const PreparedDefinition kDef_D968_93{
    .zigbee_models=kModels_D968_93, .zigbee_models_count=sizeof(kModels_D968_93)/sizeof(kModels_D968_93[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="968.93", .vendor="Paulmann",
    .meta=nullptr, .exposes=kExposesPaulmannLight, .exposes_count=kExposesPaulmannLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulmannLight, .from_zigbee_count=kFzPaulmannLightCount,
    .to_zigbee=kTzPaulmannLight, .to_zigbee_count=kTzPaulmannLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsPaulmannLight, .bindings_count=kBindingsPaulmannLightCount,
};

}  // namespace zhc::devices::paulmann
