// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: DresdenElektronik FLS-CT — hand-edited 2026-04-29c.
// Zigbee Light Link wireless electronic ballast color temperature
// z2m: m.light({colorTemp: {range: [153, 500]}})  → CTLight bundle.
// z2m-source: dresden_elektronik.ts #FLS-CT.
#include "definitions/dresden_elektronik/_shared.hpp"

namespace zhc::devices::dresden_elektronik {
namespace {
constexpr const char* kModels_FLS_CT[] = { "FLS-CT" };
}  // namespace

extern const PreparedDefinition kDef_FLS_CT{
    .zigbee_models=kModels_FLS_CT, .zigbee_models_count=sizeof(kModels_FLS_CT)/sizeof(kModels_FLS_CT[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FLS-CT", .vendor="DresdenElektronik",
    .meta=nullptr,
    .exposes=kExposesDresdenCTLight, .exposes_count=kExposesDresdenCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzDresdenCTLight, .from_zigbee_count=kFzDresdenCTLightCount,
    .to_zigbee=kTzDresdenCTLight, .to_zigbee_count=kTzDresdenCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsDresdenCTLight, .bindings_count=kBindingsDresdenCTLightCount,
};

}  // namespace zhc::devices::dresden_elektronik
