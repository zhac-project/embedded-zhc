// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Avatto TRV60_thermostat — auto-generated.
// Screen thermostatic radiator valve
// z2m-source: avatto.ts #TRV60_thermostat.
#include "definitions/_generic/_shared.hpp"
#include "definitions/avatto/_shared.hpp"

namespace zhc::devices::avatto {
namespace {


constexpr const char* kModels_TRV60_thermostat[] = { "TS0601" };
constexpr const char* kManus_TRV60_thermostat[] = { "_TZE284_ty5neqqo" };
}  // namespace

extern const PreparedDefinition kDef_TRV60_thermostat{
    .zigbee_models=kModels_TRV60_thermostat, .zigbee_models_count=sizeof(kModels_TRV60_thermostat)/sizeof(kModels_TRV60_thermostat[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TRV60_thermostat, .manufacturer_names_count=sizeof(kManus_TRV60_thermostat)/sizeof(kManus_TRV60_thermostat[0]),
    .model="TRV60_thermostat", .vendor="Avatto",
    .meta=nullptr, .exposes=kExposesAvattoLight, .exposes_count=kExposesAvattoLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAvattoLight, .from_zigbee_count=kFzAvattoLightCount,
    .to_zigbee=kTzAvattoLight, .to_zigbee_count=kTzAvattoLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsAvattoLight,
    .bindings_count           = kBindingsAvattoLightCount,
};

}  // namespace zhc::devices::avatto
