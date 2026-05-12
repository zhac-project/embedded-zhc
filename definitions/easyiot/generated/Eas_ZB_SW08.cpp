// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Easyiot ZB-SW08 — auto-generated.
// Zigbee 8-channel relay (endpoints l1..l8 → ep1..ep8)
// z2m-source: easyiot.ts #ZB-SW08.
//
// TODO(easyiot): per-endpoint state exposes (state_l1..state_l8)
// require runtime endpoint-name plumbing; current ZHC port exposes a
// single "state" handled wildcard across all endpoints.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
const FzConverter* const kFz_ZB_SW08[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_ZB_SW08[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_ZB_SW08[] = { "ZB-SW08" };

constexpr Expose kExposes_ZB_SW08[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_SW08[] = {
    {1, 0x0006}, {2, 0x0006}, {3, 0x0006}, {4, 0x0006},
    {5, 0x0006}, {6, 0x0006}, {7, 0x0006}, {8, 0x0006},
};

constexpr ::zhc::EndpointLabel kEndpoints_ZB_SW08[] = { {"l1", 1}, {"l2", 2}, {"l3", 3}, {"l4", 4}, {"l5", 5}, {"l6", 6}, {"l7", 7}, {"l8", 8} };

}  // namespace

extern const PreparedDefinition kDef_ZB_SW08{
    .zigbee_models=kModels_ZB_SW08, .zigbee_models_count=sizeof(kModels_ZB_SW08)/sizeof(kModels_ZB_SW08[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-SW08", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_SW08, .exposes_count=sizeof(kExposes_ZB_SW08)/sizeof(kExposes_ZB_SW08[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB_SW08, .from_zigbee_count=sizeof(kFz_ZB_SW08)/sizeof(kFz_ZB_SW08[0]),
    .to_zigbee=kTz_ZB_SW08, .to_zigbee_count=sizeof(kTz_ZB_SW08)/sizeof(kTz_ZB_SW08[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_SW08, .bindings_count=sizeof(kBindings_ZB_SW08)/sizeof(kBindings_ZB_SW08[0]),
    .endpoint_map       = kEndpoints_ZB_SW08,
    .endpoint_map_count = sizeof(kEndpoints_ZB_SW08)/sizeof(kEndpoints_ZB_SW08[0]),
};

}  // namespace zhc::devices::easyiot
