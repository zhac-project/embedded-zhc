// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adurosmart BPU3 — hand-rewritten to use Adu_OnOff bundle.
// ERIA smart plug.
// z2m extend: m.onOff().
// z2m-source: adurosmart.ts #BPU3.
#include "definitions/_generic/_shared.hpp"
#include "definitions/adurosmart/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_BPU3[] = { "BPU3" };

}  // namespace

extern const PreparedDefinition kDef_BPU3{
    .zigbee_models=kModels_BPU3,
    .zigbee_models_count=sizeof(kModels_BPU3)/sizeof(kModels_BPU3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BPU3", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kExposesAduOnOff, .exposes_count=kExposesAduOnOffCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAduOnOff, .from_zigbee_count=kFzAduOnOffCount,
    .to_zigbee=kTzAduOnOff, .to_zigbee_count=kTzAduOnOffCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAduOnOff, .bindings_count=kBindingsAduOnOffCount,
};

}  // namespace zhc::devices::adurosmart
