// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: SM0001 — auto-generated stub.
// z2m-source: tuya.ts #SM0001.
#include "definitions/_generic/_shared.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr const char* kModels[] = { "SM0001" };
}

constexpr WhiteLabel kWhiteLabels_SM0001[] = {
    {"Zemismart","ZM-H7"},
};
extern const PreparedDefinition kDefSM0001{
    .zigbee_models=kModels,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="SM0001",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_SM0001, .white_labels_count=sizeof(kWhiteLabels_SM0001)/sizeof(kWhiteLabels_SM0001[0]),
    // no FzConverters in stub — register for match only
    .from_zigbee=nullptr,.from_zigbee_count=0,
    .to_zigbee=nullptr,.to_zigbee_count=0,
    .configure=nullptr,.on_event=nullptr };
}
