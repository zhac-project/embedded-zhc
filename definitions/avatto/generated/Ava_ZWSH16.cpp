// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Avatto ZWSH16 — auto-generated.
// Smart temperature and humidity detector
// z2m-source: avatto.ts #ZWSH16.
#include "definitions/_generic/_shared.hpp"
#include "definitions/avatto/_shared.hpp"

namespace zhc::devices::avatto {
namespace {


constexpr const char* kModels_ZWSH16[] = { "TS0601" };
constexpr const char* kManus_ZWSH16[] = { "_TZE204_s139roas" };
}  // namespace

extern const PreparedDefinition kDef_ZWSH16{
    .zigbee_models=kModels_ZWSH16, .zigbee_models_count=sizeof(kModels_ZWSH16)/sizeof(kModels_ZWSH16[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZWSH16, .manufacturer_names_count=sizeof(kManus_ZWSH16)/sizeof(kManus_ZWSH16[0]),
    .model="ZWSH16", .vendor="Avatto",
    .meta=nullptr, .exposes=kExposesAvattoLight, .exposes_count=kExposesAvattoLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAvattoLight, .from_zigbee_count=kFzAvattoLightCount,
    .to_zigbee=kTzAvattoLight, .to_zigbee_count=kTzAvattoLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsAvattoLight,
    .bindings_count           = kBindingsAvattoLightCount,
};

}  // namespace zhc::devices::avatto
