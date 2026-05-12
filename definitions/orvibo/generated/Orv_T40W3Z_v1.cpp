// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Orvibo T40W3Z_v1 — uses orvibo/_shared.
// MixSwitch 3 gangs
// z2m-source: orvibo.ts #T40W3Z_v1.
#include "definitions/_generic/_shared.hpp"
#include "definitions/orvibo/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_T40W3Z_v1[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_T40W3Z_v1[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::orvibo::kTzOrviboPowerOnBehavior,
};
constexpr const char* kModels_T40W3Z_v1[] = { "e8d667cb184b4a2880dd886c23d00976" };

constexpr ::zhc::EndpointLabel kEndpoints_T40W3Z_v1[] = { {"left", 1}, {"center", 2}, {"right", 3} };

}  // namespace

constexpr const char* kEnum_T40W3Z_v1_3[] = { "off", "previous" };

constexpr Expose kAutoExposes[] = {
    {"state_left", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_center", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_right", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kEnum_T40W3Z_v1_3, 2},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
    {1, 0xFF00},
};

extern const PreparedDefinition kDef_T40W3Z_v1{
    .zigbee_models=kModels_T40W3Z_v1, .zigbee_models_count=sizeof(kModels_T40W3Z_v1)/sizeof(kModels_T40W3Z_v1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T40W3Z_v1", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_T40W3Z_v1, .from_zigbee_count=sizeof(kFz_T40W3Z_v1)/sizeof(kFz_T40W3Z_v1[0]),
    .to_zigbee=kTz_T40W3Z_v1, .to_zigbee_count=sizeof(kTz_T40W3Z_v1)/sizeof(kTz_T40W3Z_v1[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_T40W3Z_v1,
    .endpoint_map_count = sizeof(kEndpoints_T40W3Z_v1)/sizeof(kEndpoints_T40W3Z_v1[0]),
};

}  // namespace zhc::devices::orvibo
