// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: DresdenElektronik FLS-H — hand-edited 2026-04-29c.
// Zigbee Light Link wireless electronic ballast (color + CT).
// z2m: m.light({colorTemp:[153,500], color:true})  → ColorCTLight.
// z2m-source: dresden_elektronik.ts #FLS-H.
#include "definitions/dresden_elektronik/_shared.hpp"

namespace zhc::devices::dresden_elektronik {
namespace {
constexpr const char* kModels_FLS_H[] = { "FLS-H3" };
}  // namespace

extern const PreparedDefinition kDef_FLS_H{
    .zigbee_models=kModels_FLS_H, .zigbee_models_count=sizeof(kModels_FLS_H)/sizeof(kModels_FLS_H[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FLS-H", .vendor="DresdenElektronik",
    .meta=nullptr,
    .exposes=kExposesDresdenColorCTLight, .exposes_count=kExposesDresdenColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzDresdenColorCTLight, .from_zigbee_count=kFzDresdenColorCTLightCount,
    .to_zigbee=kTzDresdenColorCTLight, .to_zigbee_count=kTzDresdenColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsDresdenColorCTLight, .bindings_count=kBindingsDresdenColorCTLightCount,
};

}  // namespace zhc::devices::dresden_elektronik
