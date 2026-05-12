// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Avatto ZDMS16-2 — auto-generated.
// Zigbee Module 2 channels Dimmer
// z2m-source: avatto.ts #ZDMS16-2.
#include "definitions/_generic/_shared.hpp"
#include "definitions/avatto/_shared.hpp"

namespace zhc::devices::avatto {
namespace {


constexpr const char* kModels_ZDMS16_2[] = { "TS0601" };
constexpr const char* kManus_ZDMS16_2[] = { "_TZE204_o9gyszw2", "_TZE204_jtbgusdc", "_TZE284_jtbgusdc" };
}  // namespace

extern const PreparedDefinition kDef_ZDMS16_2{
    .zigbee_models=kModels_ZDMS16_2, .zigbee_models_count=sizeof(kModels_ZDMS16_2)/sizeof(kModels_ZDMS16_2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZDMS16_2, .manufacturer_names_count=sizeof(kManus_ZDMS16_2)/sizeof(kManus_ZDMS16_2[0]),
    .model="ZDMS16-2", .vendor="Avatto",
    .meta=nullptr, .exposes=kExposesAvattoLight, .exposes_count=kExposesAvattoLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAvattoLight, .from_zigbee_count=kFzAvattoLightCount,
    .to_zigbee=kTzAvattoLight, .to_zigbee_count=kTzAvattoLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsAvattoLight,
    .bindings_count           = kBindingsAvattoLightCount,
};

}  // namespace zhc::devices::avatto
