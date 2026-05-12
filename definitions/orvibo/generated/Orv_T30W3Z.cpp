// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Orvibo T30W3Z — uses orvibo/_shared.
// Smart light switch - 3 gang
// z2m-source: orvibo.ts #T30W3Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/orvibo/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_T30W3Z[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_T30W3Z[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::orvibo::kTzOrviboPowerOnBehavior,
};
constexpr const char* kModels_T30W3Z[] = { "9f76c9f31b4c4a499e3aca0977ac4494", "6fd24c0f58a04c848fea837aaa7d6e0f" };

constexpr ::zhc::EndpointLabel kEndpoints_T30W3Z[] = { {"top", 1}, {"center", 2}, {"bottom", 3} };

}  // namespace

constexpr const char* kEnum_T30W3Z_3[] = { "off", "previous" };

constexpr Expose kAutoExposes[] = {
    {"state_top", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_center", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_bottom", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kEnum_T30W3Z_3, 2},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
    {1, 0xFF00},
};

extern const PreparedDefinition kDef_T30W3Z{
    .zigbee_models=kModels_T30W3Z, .zigbee_models_count=sizeof(kModels_T30W3Z)/sizeof(kModels_T30W3Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T30W3Z", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_T30W3Z, .from_zigbee_count=sizeof(kFz_T30W3Z)/sizeof(kFz_T30W3Z[0]),
    .to_zigbee=kTz_T30W3Z, .to_zigbee_count=sizeof(kTz_T30W3Z)/sizeof(kTz_T30W3Z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_T30W3Z,
    .endpoint_map_count = sizeof(kEndpoints_T30W3Z)/sizeof(kEndpoints_T30W3Z[0]),
};

}  // namespace zhc::devices::orvibo
