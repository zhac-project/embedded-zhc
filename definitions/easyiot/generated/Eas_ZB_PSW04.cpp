// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Easyiot ZB-PSW04 — auto-generated.
// Zigbee 4-channel relay (endpoints l1..l4 → ep1..ep4)
// z2m-source: easyiot.ts #ZB-PSW04.
//
// TODO(easyiot): per-endpoint state exposes (state_l1..state_l4)
// require runtime endpoint-name plumbing; current ZHC port exposes a
// single "state" handled wildcard across all endpoints.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
const FzConverter* const kFz_ZB_PSW04[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_ZB_PSW04[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_ZB_PSW04[] = { "ZB-PSW04" };

constexpr Expose kExposes_ZB_PSW04[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_PSW04[] = {
    {1, 0x0006}, {2, 0x0006}, {3, 0x0006}, {4, 0x0006},
};

constexpr ::zhc::EndpointLabel kEndpoints_ZB_PSW04[] = { {"l1", 1}, {"l2", 2}, {"l3", 3}, {"l4", 4} };

}  // namespace

extern const PreparedDefinition kDef_ZB_PSW04{
    .zigbee_models=kModels_ZB_PSW04, .zigbee_models_count=sizeof(kModels_ZB_PSW04)/sizeof(kModels_ZB_PSW04[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-PSW04", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_PSW04, .exposes_count=sizeof(kExposes_ZB_PSW04)/sizeof(kExposes_ZB_PSW04[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB_PSW04, .from_zigbee_count=sizeof(kFz_ZB_PSW04)/sizeof(kFz_ZB_PSW04[0]),
    .to_zigbee=kTz_ZB_PSW04, .to_zigbee_count=sizeof(kTz_ZB_PSW04)/sizeof(kTz_ZB_PSW04[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_PSW04, .bindings_count=sizeof(kBindings_ZB_PSW04)/sizeof(kBindings_ZB_PSW04[0]),
    .endpoint_map       = kEndpoints_ZB_PSW04,
    .endpoint_map_count = sizeof(kEndpoints_ZB_PSW04)/sizeof(kEndpoints_ZB_PSW04[0]),
};

}  // namespace zhc::devices::easyiot
