// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Honyar U86K31ND6 — graduated from generated/.
// 3 gang switch
// z2m-source: honyar.ts #U86K31ND6.
//
// Fix (gap class f — incomplete multi-gang surface): z2m uses
// m.deviceEndpoints({left:1, center:2, right:3}) + m.onOff with all
// three endpointNames, i.e. a switch on every endpoint with genOnOff
// bound per endpoint. The generated def already carried the
// endpoint_map but exposed only a single bare `state` and bound genOnOff
// only on EP1, so the SPA surfaced one control and EP2/EP3 were never
// bound. Restored per-gang suffixed exposes + per-endpoint genOnOff
// bindings (mirrors the lonsonho S04-2C-LN multi-gang fix).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::honyar {
namespace {
const FzConverter* const kFz_U86K31ND6[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_U86K31ND6[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_U86K31ND6[] = { "00500c35" };
// z2m deviceEndpoints: {left:1, center:2, right:3}
constexpr ::zhc::EndpointLabel kEndpoints_U86K31ND6[] = { {"left", 1}, {"center", 2}, {"right", 3} };

}  // namespace


// --- per-endpoint surface (matches z2m onOff on left/center/right) ---
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

extern const PreparedDefinition kDef_U86K31ND6{
    .zigbee_models=kModels_U86K31ND6, .zigbee_models_count=sizeof(kModels_U86K31ND6)/sizeof(kModels_U86K31ND6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="U86K31ND6", .vendor="Honyar",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_U86K31ND6, .from_zigbee_count=sizeof(kFz_U86K31ND6)/sizeof(kFz_U86K31ND6[0]),
    .to_zigbee=kTz_U86K31ND6, .to_zigbee_count=sizeof(kTz_U86K31ND6)/sizeof(kTz_U86K31ND6[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_U86K31ND6,
    .endpoint_map_count = sizeof(kEndpoints_U86K31ND6)/sizeof(kEndpoints_U86K31ND6[0]),
};

}  // namespace zhc::devices::honyar
