// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sylvania 70552 — auto-generated.
// Smart+ LED A19 dimmable soft white
// Bundle: Dim (dimmable).
// z2m-source: sylvania.ts #70552.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sylvania/_shared.hpp"

namespace zhc::devices::sylvania {
namespace {

constexpr const char* kModels_D70552[] = { "A19 W non CEC" };

}  // namespace

extern const PreparedDefinition kDef_D70552{
    .zigbee_models=kModels_D70552, .zigbee_models_count=sizeof(kModels_D70552)/sizeof(kModels_D70552[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="70552", .vendor="Sylvania",
    .meta=nullptr, .exposes=kExposesSylvaniaDim, .exposes_count=kExposesSylvaniaDimCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSylvaniaDim, .from_zigbee_count=kFzSylvaniaDimCount,
    .to_zigbee=kTzSylvaniaDim, .to_zigbee_count=kTzSylvaniaDimCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsSylvaniaDim,
    .bindings_count           = kBindingsSylvaniaDimCount,
};

}  // namespace zhc::devices::sylvania
