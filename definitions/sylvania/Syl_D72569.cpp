// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: sylvania 72569 — z2m parity v26.92.0.
// GRADUATED from definitions/sylvania/generated/Syl_D72569.cpp: z2m v26.92.0 folded the LIGHTIFY Conv variant into this definition.
// Otherwise a verbatim copy of the generated file.
// SMART+ Zigbee adjustable white edge-lit under cabinet light
// Bundle: CTLight (tunable white).
// z2m-source: sylvania.ts #72569.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sylvania/_shared.hpp"

namespace zhc::devices::sylvania {
namespace {

constexpr const char* kModels_D72569[] = { "Edge-lit Under Cabinet TW", "LIGHTIFY Conv Under Cabinet TW" };

}  // namespace

extern const PreparedDefinition kDef_D72569{
    .zigbee_models=kModels_D72569, .zigbee_models_count=sizeof(kModels_D72569)/sizeof(kModels_D72569[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="72569", .vendor="Sylvania",
    .meta=nullptr, .exposes=kExposesSylvaniaLight, .exposes_count=kExposesSylvaniaLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSylvaniaLight, .from_zigbee_count=kFzSylvaniaLightCount,
    .to_zigbee=kTzSylvaniaLight, .to_zigbee_count=kTzSylvaniaLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsSylvaniaLight,
    .bindings_count           = kBindingsSylvaniaLightCount,
};

}  // namespace zhc::devices::sylvania
