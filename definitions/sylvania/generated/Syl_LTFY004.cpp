// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sylvania LTFY004 — auto-generated.
// LIGHTIFY LED gardenspot mini RGB
// Bundle: ColorLight (RGB only).
// z2m-source: sylvania.ts #LTFY004.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sylvania/_shared.hpp"

namespace zhc::devices::sylvania {
namespace {

constexpr const char* kModels_LTFY004[] = { "LIGHTIFY Gardenspot RGB" };

}  // namespace

extern const PreparedDefinition kDef_LTFY004{
    .zigbee_models=kModels_LTFY004, .zigbee_models_count=sizeof(kModels_LTFY004)/sizeof(kModels_LTFY004[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LTFY004", .vendor="Sylvania",
    .meta=nullptr, .exposes=kExposesSylvaniaColorLight, .exposes_count=kExposesSylvaniaColorLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSylvaniaColorLight, .from_zigbee_count=kFzSylvaniaColorLightCount,
    .to_zigbee=kTzSylvaniaColorLight, .to_zigbee_count=kTzSylvaniaColorLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsSylvaniaColorLight,
    .bindings_count           = kBindingsSylvaniaColorLightCount,
};

}  // namespace zhc::devices::sylvania
