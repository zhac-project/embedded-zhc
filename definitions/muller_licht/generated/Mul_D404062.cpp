// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: MullerLicht 404062 — auto-generated.
// Kea RGB+CCT
// z2m-source: muller_licht.ts #404062.
#include "definitions/_generic/_shared.hpp"
#include "definitions/muller_licht/_shared.hpp"

namespace zhc::devices::muller_licht {
namespace {


constexpr const char* kModels_D404062[] = { "TS0505B" };
constexpr const char* kManus_D404062[] = { "_TZ3210_mntza0sw", "_TZ3210_r0vzq1oj" };
}  // namespace

extern const PreparedDefinition kDef_D404062{
    .zigbee_models=kModels_D404062, .zigbee_models_count=sizeof(kModels_D404062)/sizeof(kModels_D404062[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_D404062, .manufacturer_names_count=sizeof(kManus_D404062)/sizeof(kManus_D404062[0]),
    .model="404062", .vendor="MullerLicht",
    .meta=nullptr, .exposes=kExposesMullerLichtColorCTLight, .exposes_count=kExposesMullerLichtColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzMullerLichtColorCTLight, .from_zigbee_count=kFzMullerLichtColorCTLightCount,
    .to_zigbee=kTzMullerLichtColorCTLight, .to_zigbee_count=kTzMullerLichtColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsMullerLichtColorCTLight,
    .bindings_count           = kBindingsMullerLichtColorCTLightCount,
};

}  // namespace zhc::devices::muller_licht
