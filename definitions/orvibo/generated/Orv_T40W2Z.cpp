// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Orvibo T40W2Z — uses orvibo/_shared.
// MixSwitch 2 gangs
// z2m-source: orvibo.ts #T40W2Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/orvibo/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_T40W2Z[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_T40W2Z[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::orvibo::kTzOrviboPowerOnBehavior,
};
constexpr const char* kModels_T40W2Z[] = { "2e13af8e17434961be98f055d68c2166" };

constexpr ::zhc::EndpointLabel kEndpoints_T40W2Z[] = { {"left", 1}, {"right", 2} };

}  // namespace

constexpr const char* kEnum_T40W2Z_2[] = { "off", "previous" };

constexpr Expose kAutoExposes[] = {
    {"state_left", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_right", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kEnum_T40W2Z_2, 2},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {1, 0xFF00},
};

extern const PreparedDefinition kDef_T40W2Z{
    .zigbee_models=kModels_T40W2Z, .zigbee_models_count=sizeof(kModels_T40W2Z)/sizeof(kModels_T40W2Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T40W2Z", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_T40W2Z, .from_zigbee_count=sizeof(kFz_T40W2Z)/sizeof(kFz_T40W2Z[0]),
    .to_zigbee=kTz_T40W2Z, .to_zigbee_count=sizeof(kTz_T40W2Z)/sizeof(kTz_T40W2Z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_T40W2Z,
    .endpoint_map_count = sizeof(kEndpoints_T40W2Z)/sizeof(kEndpoints_T40W2Z[0]),
};

}  // namespace zhc::devices::orvibo
