// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sylvania 484719 — auto-generated.
// Dimmable soft white BR30 LED flood light bulb
// Bundle: Dim (dimmable).
// z2m-source: sylvania.ts #484719.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sylvania/_shared.hpp"

namespace zhc::devices::sylvania {
namespace {

constexpr const char* kModels_D484719[] = { "iQBR30" };

}  // namespace

extern const PreparedDefinition kDef_D484719{
    .zigbee_models=kModels_D484719, .zigbee_models_count=sizeof(kModels_D484719)/sizeof(kModels_D484719[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="484719", .vendor="Sylvania",
    .meta=nullptr, .exposes=kExposesSylvaniaDim, .exposes_count=kExposesSylvaniaDimCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSylvaniaDim, .from_zigbee_count=kFzSylvaniaDimCount,
    .to_zigbee=kTzSylvaniaDim, .to_zigbee_count=kTzSylvaniaDimCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsSylvaniaDim,
    .bindings_count           = kBindingsSylvaniaDimCount,
};

}  // namespace zhc::devices::sylvania
