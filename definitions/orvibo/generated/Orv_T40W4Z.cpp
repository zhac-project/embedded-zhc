// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo T40W4Z.
// MixSwitch 4 gangs
// z2m-source: orvibo.ts #T40W4Z.
// PARTIAL — z2m uses clusterManuSpecifcOrviboSwitchRewiring (cluster
// 0xFD00) and orviboSwitchRewiring composite expose for scene/relay
// rewiring. Not portable as a static expose; commandsScenes inbound is
// captured via kFzCommandRecall but outbound rewiring config is dropped.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_T40W4Z[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzCommandRecall,
};
const TzConverter* const kTz_T40W4Z[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_T40W4Z[] = { "20513b10079f4cc68cffb8b0dc6d3277", "c2ea8c76f9fe40e5a7de5e8fb8dfb765" };

constexpr ::zhc::EndpointLabel kEndpoints_T40W4Z[] = { {"left_up", 1}, {"left_down", 2}, {"center_up", 3}, {"center_down", 4}, {"right_up", 5}, {"right_down", 6} };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state_left_up", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_left_down", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_center_up", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_center_down", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_right_up", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_right_down", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
    {4, 0x0006},
    {5, 0x0006},
    {6, 0x0006},
};

extern const PreparedDefinition kDef_T40W4Z{
    .zigbee_models=kModels_T40W4Z, .zigbee_models_count=sizeof(kModels_T40W4Z)/sizeof(kModels_T40W4Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T40W4Z", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_T40W4Z, .from_zigbee_count=sizeof(kFz_T40W4Z)/sizeof(kFz_T40W4Z[0]),
    .to_zigbee=kTz_T40W4Z, .to_zigbee_count=sizeof(kTz_T40W4Z)/sizeof(kTz_T40W4Z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_T40W4Z,
    .endpoint_map_count = sizeof(kEndpoints_T40W4Z)/sizeof(kEndpoints_T40W4Z[0]),
};

}  // namespace zhc::devices::orvibo
