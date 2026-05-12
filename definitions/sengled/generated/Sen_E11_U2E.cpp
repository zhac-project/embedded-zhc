// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sengled E11-U2E — hand-rewritten 2026-04-28.
// Element color plus E27
// z2m-source: sengled.ts #E11-U2E.
// Mapping: sengledLight({colorTemp: undefined, color: {modes: ["xy"]}}) -> ColorLight bundle.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {
namespace {


constexpr const char* kModels_E11_U2E[] = { "E11-U2E" };

}  // namespace

extern const PreparedDefinition kDef_E11_U2E{
    .zigbee_models=kModels_E11_U2E, .zigbee_models_count=sizeof(kModels_E11_U2E)/sizeof(kModels_E11_U2E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E11-U2E", .vendor="Sengled",
    .meta=nullptr, .exposes=kExposesSengledColorLight, .exposes_count=kExposesSengledColorLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSengledColorLight, .from_zigbee_count=kFzSengledColorLightCount,
    .to_zigbee=kTzSengledColorLight, .to_zigbee_count=kTzSengledColorLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsSengledColorLight,
    .bindings_count           = kBindingsSengledColorLightCount,
};

}  // namespace zhc::devices::sengled
