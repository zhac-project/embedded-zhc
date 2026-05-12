// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo R20W2Z.
// In wall switch - 2 gang
// z2m-source: orvibo.ts #R20W2Z.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_R20W2Z[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_R20W2Z[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_R20W2Z[] = { "9ea4d5d8778d4f7089ac06a3969e784b", "83b9b27d5ffb4830bf35be5b1023623e", "2810c2403b9c4a5db62cc62d1030d95e" };

constexpr ::zhc::EndpointLabel kEndpoints_R20W2Z[] = { {"l1", 1}, {"l2", 2} };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
};

extern const PreparedDefinition kDef_R20W2Z{
    .zigbee_models=kModels_R20W2Z, .zigbee_models_count=sizeof(kModels_R20W2Z)/sizeof(kModels_R20W2Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="R20W2Z", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_R20W2Z, .from_zigbee_count=sizeof(kFz_R20W2Z)/sizeof(kFz_R20W2Z[0]),
    .to_zigbee=kTz_R20W2Z, .to_zigbee_count=sizeof(kTz_R20W2Z)/sizeof(kTz_R20W2Z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_R20W2Z,
    .endpoint_map_count = sizeof(kEndpoints_R20W2Z)/sizeof(kEndpoints_R20W2Z[0]),
};

}  // namespace zhc::devices::orvibo
