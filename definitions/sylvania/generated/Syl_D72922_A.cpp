// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sylvania 72922-A — auto-generated.
// SMART+ Smart Plug
// Bundle: OnOff (smart plug).
// z2m-source: sylvania.ts #72922-A.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sylvania/_shared.hpp"

namespace zhc::devices::sylvania {
namespace {

constexpr const char* kModels_D72922_A[] = { "PLUG" };

}  // namespace

extern const PreparedDefinition kDef_D72922_A{
    .zigbee_models=kModels_D72922_A, .zigbee_models_count=sizeof(kModels_D72922_A)/sizeof(kModels_D72922_A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="72922-A", .vendor="Sylvania",
    .meta=nullptr, .exposes=kExposesSylvaniaOnOff, .exposes_count=kExposesSylvaniaOnOffCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzSylvaniaOnOff, .from_zigbee_count=kFzSylvaniaOnOffCount,
    .to_zigbee=kTzSylvaniaOnOff, .to_zigbee_count=kTzSylvaniaOnOffCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindingsSylvaniaOnOff,
    .bindings_count           = kBindingsSylvaniaOnOffCount,
};

}  // namespace zhc::devices::sylvania
