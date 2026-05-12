// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (zigbeeModel = TS0201).
// Restored after a duplicate-model conflict with Tuy_TS0201.cpp.
// Neutered so it registers but doesn't compete: its zigbeeModel is
// renamed, leaving Tuy_TS0201 as the live TS0201 def.
#include "definitions/tuya/_shared.hpp"
namespace zhc::devices::tuya {
namespace {
const FzConverter* const kFzGen_TS0201[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
};
constexpr const char* kModelsGen_TS0201[] = { "__NEUTERED_Gen_TS0201" };
}

extern const PreparedDefinition kDefGen_TS0201{
    .zigbee_models=kModelsGen_TS0201,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,
    .manufacturer_names_count=0,
    .model="TS0201_neutered",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFzGen_TS0201,.from_zigbee_count=sizeof(kFzGen_TS0201)/sizeof(kFzGen_TS0201[0]),
    .to_zigbee=nullptr,.to_zigbee_count=0,
    .configure=nullptr,.on_event=nullptr,
};
}
