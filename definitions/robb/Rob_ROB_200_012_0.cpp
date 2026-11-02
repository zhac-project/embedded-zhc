// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: ROBB ROB_200-012-0 dual phase-cut dimmer (z2m v26.125.0 window).
// Two-channel ROB_200-011-0: m.deviceEndpoints({l1: 1, l2: 2}) + m.light on both.
// z2m-source: robb.ts #ROB_200-012-0.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::robb {
namespace {
const FzConverter* const kFz_ROB_200_012_0[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_ROB_200_012_0[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_ROB_200_012_0[] = { "ROB_200-012-0" };

constexpr ::zhc::EndpointLabel kEndpoints_ROB_200_012_0[] = { {"l1", 1}, {"l2", 2} };

constexpr Expose kExposes_ROB_200_012_0[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ROB_200_012_0[] = {
    {1, 0x0006},
    {1, 0x0008},
    {2, 0x0006},
    {2, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_ROB_200_012_0{
    .zigbee_models=kModels_ROB_200_012_0, .zigbee_models_count=sizeof(kModels_ROB_200_012_0)/sizeof(kModels_ROB_200_012_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ROB_200-012-0", .vendor="Robb",
    .meta=nullptr, .exposes=kExposes_ROB_200_012_0, .exposes_count=sizeof(kExposes_ROB_200_012_0)/sizeof(kExposes_ROB_200_012_0[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ROB_200_012_0, .from_zigbee_count=sizeof(kFz_ROB_200_012_0)/sizeof(kFz_ROB_200_012_0[0]),
    .to_zigbee=kTz_ROB_200_012_0, .to_zigbee_count=sizeof(kTz_ROB_200_012_0)/sizeof(kTz_ROB_200_012_0[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ROB_200_012_0, .bindings_count=sizeof(kBindings_ROB_200_012_0)/sizeof(kBindings_ROB_200_012_0[0]),
    .endpoint_map       = kEndpoints_ROB_200_012_0,
    .endpoint_map_count = sizeof(kEndpoints_ROB_200_012_0)/sizeof(kEndpoints_ROB_200_012_0[0]),
};

}  // namespace zhc::devices::robb
