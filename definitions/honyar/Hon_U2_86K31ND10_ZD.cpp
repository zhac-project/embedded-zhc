// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Honyar U2-86K31ND10-ZD — graduated from generated/.
// 3 gang switch
// z2m-source: honyar.ts #U2-86K31ND10-ZD.
//
// Fix (gap class f — missing endpoint_map): z2m declares three switch
// endpoints (e.switch().withEndpoint("left"/"right"/"center"), endpoint
// {left:1, center:2, right:3}, meta.multiEndpoint) and binds genOnOff on
// all three. The generated def carried a single bare `state` expose, a
// single EP1 binding, and NO endpoint_map, so the three gangs collided
// on the bare `state` key at runtime (last-writer-wins) and EP2/EP3 were
// never bound. Restored per-gang suffixed exposes + per-endpoint genOnOff
// bindings + the endpoint_map (mirrors the lonsonho S04-2C-LN fix).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::honyar {
namespace {
const FzConverter* const kFz_U2_86K31ND10_ZD[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_U2_86K31ND10_ZD[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_U2_86K31ND10_ZD[] = { "HY0097" };
// z2m endpoint: {left:1, center:2, right:3}
constexpr ::zhc::EndpointLabel kEndpoints_U2_86K31ND10_ZD[] = { {"left", 1}, {"center", 2}, {"right", 3} };

}  // namespace


// --- per-endpoint surface (matches z2m multiEndpoint left/center/right) ---
constexpr Expose kAutoExposes[] = {
    {"state_left", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_center", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_right", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
};
// --- end block ---

extern const PreparedDefinition kDef_U2_86K31ND10_ZD{
    .zigbee_models=kModels_U2_86K31ND10_ZD, .zigbee_models_count=sizeof(kModels_U2_86K31ND10_ZD)/sizeof(kModels_U2_86K31ND10_ZD[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="U2-86K31ND10-ZD", .vendor="Honyar",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_U2_86K31ND10_ZD, .from_zigbee_count=sizeof(kFz_U2_86K31ND10_ZD)/sizeof(kFz_U2_86K31ND10_ZD[0]),
    .to_zigbee=kTz_U2_86K31ND10_ZD, .to_zigbee_count=sizeof(kTz_U2_86K31ND10_ZD)/sizeof(kTz_U2_86K31ND10_ZD[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_U2_86K31ND10_ZD,
    .endpoint_map_count = sizeof(kEndpoints_U2_86K31ND10_ZD)/sizeof(kEndpoints_U2_86K31ND10_ZD[0]),
};

}  // namespace zhc::devices::honyar
