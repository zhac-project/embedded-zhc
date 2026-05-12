// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Terncy TERNCY-WS01 — generic on/off + multi-EP map.
// Smart light switch — 4 gang without neutral wire. z2m wires
// `m.deviceEndpoints({l1:1,l2:2,l3:3,l4:4}) + m.onOff({endpointNames:
// ['l1','l2','l3','l4']})`. The runtime suffixes the `state` key with
// the matching label per incoming endpoint via the endpoint_map.
// Outbound writes (`state_l1`, …) get split + routed by the same
// pipeline.
// z2m-source: terncy.ts #TERNCY-WS01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::terncy {
namespace {

const FzConverter* const kFz_TERNCY_WS01[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_TERNCY_WS01[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_TERNCY_WS01[] = { "TERNCY-WS01-S4" };

constexpr ::zhc::EndpointLabel kEndpoints_TERNCY_WS01[] = {
    {"l1", 1}, {"l2", 2}, {"l3", 3}, {"l4", 4},
};

constexpr Expose kAutoExposes[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
    {4, 0x0006},
};

}  // namespace

extern const PreparedDefinition kDef_TERNCY_WS01{
    .zigbee_models=kModels_TERNCY_WS01,
    .zigbee_models_count=sizeof(kModels_TERNCY_WS01)/sizeof(kModels_TERNCY_WS01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-WS01", .vendor="Terncy",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TERNCY_WS01, .from_zigbee_count=sizeof(kFz_TERNCY_WS01)/sizeof(kFz_TERNCY_WS01[0]),
    .to_zigbee=kTz_TERNCY_WS01, .to_zigbee_count=sizeof(kTz_TERNCY_WS01)/sizeof(kTz_TERNCY_WS01[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_TERNCY_WS01,
    .endpoint_map_count = sizeof(kEndpoints_TERNCY_WS01)/sizeof(kEndpoints_TERNCY_WS01[0]),
};

}  // namespace zhc::devices::terncy
