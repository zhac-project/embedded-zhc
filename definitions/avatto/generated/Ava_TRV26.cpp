// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Avatto TRV26 — auto-generated.
// Thermostatic radiator valve
// z2m-source: avatto.ts #TRV26.
#include "definitions/_generic/_shared.hpp"
#include "definitions/avatto/_shared.hpp"

namespace zhc::devices::avatto {
namespace {


constexpr const char* kModels_TRV26[] = { "TS0601" };
constexpr const char* kManus_TRV26[] = { "_TZE204_xdtnpp1a", "_TZE284_xdtnpp1a" };
}  // namespace

extern const PreparedDefinition kDef_TRV26{
    .zigbee_models=kModels_TRV26, .zigbee_models_count=sizeof(kModels_TRV26)/sizeof(kModels_TRV26[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TRV26, .manufacturer_names_count=sizeof(kManus_TRV26)/sizeof(kManus_TRV26[0]),
    .model="TRV26", .vendor="Avatto",
    .meta=nullptr, .exposes=kExposesAvattoLight, .exposes_count=kExposesAvattoLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAvattoLight, .from_zigbee_count=kFzAvattoLightCount,
    .to_zigbee=kTzAvattoLight, .to_zigbee_count=kTzAvattoLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsAvattoLight,
    .bindings_count           = kBindingsAvattoLightCount,
};

}  // namespace zhc::devices::avatto
