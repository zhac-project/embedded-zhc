// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Awox 12226 — hand-rewritten to use awox/_shared Light bundle.
// Dimmable filament lamp (m.light()).
// z2m-source: awox.ts #12226.
#include "definitions/_generic/_shared.hpp"
#include "definitions/awox/_shared.hpp"

namespace zhc::devices::awox {
namespace {

constexpr const char* kModels_D12226[] = { "ESMLFzm_w6_Dimm" };

}  // namespace

extern const PreparedDefinition kDef_D12226{
    .zigbee_models=kModels_D12226, .zigbee_models_count=sizeof(kModels_D12226)/sizeof(kModels_D12226[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="12226", .vendor="Awox",
    .meta=nullptr,
    .exposes=kExposesAwoxLight, .exposes_count=kExposesAwoxLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAwoxLight, .from_zigbee_count=kFzAwoxLightCount,
    .to_zigbee=kTzAwoxLight, .to_zigbee_count=kTzAwoxLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAwoxLight, .bindings_count=kBindingsAwoxLightCount,
};

}  // namespace zhc::devices::awox
