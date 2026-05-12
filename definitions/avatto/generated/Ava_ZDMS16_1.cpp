// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Avatto ZDMS16-1 — auto-generated.
// Zigbee Module 1 channel Dimmer
// z2m-source: avatto.ts #ZDMS16-1.
#include "definitions/_generic/_shared.hpp"
#include "definitions/avatto/_shared.hpp"

namespace zhc::devices::avatto {
namespace {


constexpr const char* kModels_ZDMS16_1[] = { "TS0601" };
constexpr const char* kManus_ZDMS16_1[] = { "_TZE204_5cuocqty", "_TZE204_nqqylykc", "_TZE204_2cyb66xl", "_TZE204_tgdnh7pw", "_TZE284_nqqylykc" };
}  // namespace

extern const PreparedDefinition kDef_ZDMS16_1{
    .zigbee_models=kModels_ZDMS16_1, .zigbee_models_count=sizeof(kModels_ZDMS16_1)/sizeof(kModels_ZDMS16_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZDMS16_1, .manufacturer_names_count=sizeof(kManus_ZDMS16_1)/sizeof(kManus_ZDMS16_1[0]),
    .model="ZDMS16-1", .vendor="Avatto",
    .meta=nullptr, .exposes=kExposesAvattoLight, .exposes_count=kExposesAvattoLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAvattoLight, .from_zigbee_count=kFzAvattoLightCount,
    .to_zigbee=kTzAvattoLight, .to_zigbee_count=kTzAvattoLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsAvattoLight,
    .bindings_count           = kBindingsAvattoLightCount,
};

}  // namespace zhc::devices::avatto
