// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Avatto ME168_AVATTO — auto-generated.
// Thermostatic radiator valve
// z2m-source: avatto.ts #ME168_AVATTO.
#include "definitions/_generic/_shared.hpp"
#include "definitions/avatto/_shared.hpp"

namespace zhc::devices::avatto {
namespace {


constexpr const char* kModels_ME168_AVATTO[] = { "TS0601" };
constexpr const char* kManus_ME168_AVATTO[] = { "_TZE200_ybsqljjg", "_TZE200_cxakecfo", "_TZE200_4aijvczq", "_TZE200_r5ksy7qo" };
}  // namespace

extern const PreparedDefinition kDef_ME168_AVATTO{
    .zigbee_models=kModels_ME168_AVATTO, .zigbee_models_count=sizeof(kModels_ME168_AVATTO)/sizeof(kModels_ME168_AVATTO[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ME168_AVATTO, .manufacturer_names_count=sizeof(kManus_ME168_AVATTO)/sizeof(kManus_ME168_AVATTO[0]),
    .model="ME168_AVATTO", .vendor="Avatto",
    .meta=nullptr, .exposes=kExposesAvattoLight, .exposes_count=kExposesAvattoLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAvattoLight, .from_zigbee_count=kFzAvattoLightCount,
    .to_zigbee=kTzAvattoLight, .to_zigbee_count=kTzAvattoLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsAvattoLight,
    .bindings_count           = kBindingsAvattoLightCount,
};

}  // namespace zhc::devices::avatto
