// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sylvania 71831 — auto-generated.
// Smart Home adjustable white A19 LED bulb
// Bundle: CTLight (tunable white).
// z2m-source: sylvania.ts #71831.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sylvania/_shared.hpp"

namespace zhc::devices::sylvania {
namespace {

constexpr const char* kModels_D71831[] = { "A19 TW 10 year" };

}  // namespace

extern const PreparedDefinition kDef_D71831{
    .zigbee_models=kModels_D71831, .zigbee_models_count=sizeof(kModels_D71831)/sizeof(kModels_D71831[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="71831", .vendor="Sylvania",
    .meta=nullptr, .exposes=kExposesSylvaniaLight, .exposes_count=kExposesSylvaniaLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSylvaniaLight, .from_zigbee_count=kFzSylvaniaLightCount,
    .to_zigbee=kTzSylvaniaLight, .to_zigbee_count=kTzSylvaniaLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsSylvaniaLight,
    .bindings_count           = kBindingsSylvaniaLightCount,
};

}  // namespace zhc::devices::sylvania
