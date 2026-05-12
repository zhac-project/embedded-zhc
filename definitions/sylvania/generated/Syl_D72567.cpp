// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sylvania 72567 — auto-generated.
// SMART+ Zigbee adjustable white edge-lit flush mount light
// Bundle: CTLight (tunable white).
// z2m-source: sylvania.ts #72567.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sylvania/_shared.hpp"

namespace zhc::devices::sylvania {
namespace {

constexpr const char* kModels_D72567[] = { "Flushmount TW" };

}  // namespace

extern const PreparedDefinition kDef_D72567{
    .zigbee_models=kModels_D72567, .zigbee_models_count=sizeof(kModels_D72567)/sizeof(kModels_D72567[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="72567", .vendor="Sylvania",
    .meta=nullptr, .exposes=kExposesSylvaniaLight, .exposes_count=kExposesSylvaniaLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSylvaniaLight, .from_zigbee_count=kFzSylvaniaLightCount,
    .to_zigbee=kTzSylvaniaLight, .to_zigbee_count=kTzSylvaniaLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsSylvaniaLight,
    .bindings_count           = kBindingsSylvaniaLightCount,
};

}  // namespace zhc::devices::sylvania
