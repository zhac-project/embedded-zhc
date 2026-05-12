// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo RL804QZB.
// Multi-functional 3 gang relay
// z2m-source: orvibo.ts #RL804QZB.
// z2m ports use deviceEndpoints {l1:1,l2:2,l3:3}. Per-endpoint state
// currently decoded as separate keys; full per-endpoint dispatch lives in
// the adapter.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_RL804QZB[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_RL804QZB[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_RL804QZB[] = { "82c167c95ed746cdbd21d6817f72c593", "8762413da99140cbb809195ff40f8c51" };

constexpr ::zhc::EndpointLabel kEndpoints_RL804QZB[] = { {"l1", 1}, {"l2", 2}, {"l3", 3} };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
};

extern const PreparedDefinition kDef_RL804QZB{
    .zigbee_models=kModels_RL804QZB, .zigbee_models_count=sizeof(kModels_RL804QZB)/sizeof(kModels_RL804QZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RL804QZB", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RL804QZB, .from_zigbee_count=sizeof(kFz_RL804QZB)/sizeof(kFz_RL804QZB[0]),
    .to_zigbee=kTz_RL804QZB, .to_zigbee_count=sizeof(kTz_RL804QZB)/sizeof(kTz_RL804QZB[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_RL804QZB,
    .endpoint_map_count = sizeof(kEndpoints_RL804QZB)/sizeof(kEndpoints_RL804QZB[0]),
};

}  // namespace zhc::devices::orvibo
