// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Avatto ZWPM16-2 — auto-generated.
// Zigbee smart energy meter 80A/2CH
// z2m-source: avatto.ts #ZWPM16-2.
#include "definitions/_generic/_shared.hpp"
#include "definitions/avatto/_shared.hpp"

namespace zhc::devices::avatto {
namespace {


constexpr const char* kModels_ZWPM16_2[] = { "TS0601" };
constexpr const char* kManus_ZWPM16_2[] = { "_TZE204_jrcfsaa3" };
}  // namespace

extern const PreparedDefinition kDef_ZWPM16_2{
    .zigbee_models=kModels_ZWPM16_2, .zigbee_models_count=sizeof(kModels_ZWPM16_2)/sizeof(kModels_ZWPM16_2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZWPM16_2, .manufacturer_names_count=sizeof(kManus_ZWPM16_2)/sizeof(kManus_ZWPM16_2[0]),
    .model="ZWPM16-2", .vendor="Avatto",
    .meta=nullptr, .exposes=kExposesAvattoLight, .exposes_count=kExposesAvattoLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAvattoLight, .from_zigbee_count=kFzAvattoLightCount,
    .to_zigbee=kTzAvattoLight, .to_zigbee_count=kTzAvattoLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsAvattoLight,
    .bindings_count           = kBindingsAvattoLightCount,
};

}  // namespace zhc::devices::avatto
