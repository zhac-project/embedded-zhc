// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: D3-DPWK-TY — auto-generated stub.
// z2m-source: tuya.ts #D3-DPWK-TY.
#include "definitions/_generic/_shared.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr const char* kModels[] = { "6dfgetq" };
}
extern const PreparedDefinition kDefD3_DPWK_TY{
    .zigbee_models=kModels,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="D3-DPWK-TY",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    // no FzConverters in stub — register for match only
    .from_zigbee=nullptr,.from_zigbee_count=0,
    .to_zigbee=nullptr,.to_zigbee_count=0,
    .configure=nullptr,.on_event=nullptr };
}
