// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo CM10ZW.
// Multi-functional 3 gang relay
// z2m-source: orvibo.ts #CM10ZW.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_CM10ZW[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_CM10ZW[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_CM10ZW[] = { "396483ce8b3f4e0d8e9d79079a35a420" };

constexpr ::zhc::EndpointLabel kEndpoints_CM10ZW[] = { {"l1", 1}, {"l2", 2}, {"l3", 3} };

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

extern const PreparedDefinition kDef_CM10ZW{
    .zigbee_models=kModels_CM10ZW, .zigbee_models_count=sizeof(kModels_CM10ZW)/sizeof(kModels_CM10ZW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CM10ZW", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CM10ZW, .from_zigbee_count=sizeof(kFz_CM10ZW)/sizeof(kFz_CM10ZW[0]),
    .to_zigbee=kTz_CM10ZW, .to_zigbee_count=sizeof(kTz_CM10ZW)/sizeof(kTz_CM10ZW[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_CM10ZW,
    .endpoint_map_count = sizeof(kEndpoints_CM10ZW)/sizeof(kEndpoints_CM10ZW[0]),
};

}  // namespace zhc::devices::orvibo
