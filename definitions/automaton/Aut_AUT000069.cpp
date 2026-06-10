// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Automaton AUT000069 — graduated from generated, multi-endpoint fix.
// Underfloor heating / Irrigation valves controller - 5 zones (TS011F).
//
// z2m wires `tuya.modernExtend.tuyaOnOff({endpoints: ["l1".."l5"]})` with
// `endpoint: {l1:1, l2:2, l3:3, l4:4, l5:5}` and `meta.multiEndpoint`, i.e.
// FIVE independent genOnOff channels. The auto-port collapsed this to a
// single bare `state` expose with one EP1 binding and NO endpoint_map, so
// all five zones collided on the same key and only zone 1 was bound. Added
// the endpoint_map (l1..l5), per-EP state exposes (state_l1..state_l5) and
// genOnOff bindings on EP1..EP5 so each zone routes to its own suffixed key.
//
// Deferred (INFRA, no generic decoder): the tuyaOnOff extras
// `powerOnBehavior2` (Tuya 0x8002), `childLock` (0x8000) and `onOffCountdown`
// are Tuya manuSpecific genOnOff attrs/commands; the generic
// kFzPowerOnBehavior1 only handles standard 0x4003 startUpOnOff. Left off
// pending a Tuya-manuSpecific genOnOff converter.
//
// z2m-source: automaton.ts #AUT000069.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::automaton {
namespace {
const FzConverter* const kFz_AUT000069[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_AUT000069[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_AUT000069[] = { "TS011F" };
constexpr const char* kManus_AUT000069[] = { "_TZ3000_j0ktmul1" };
constexpr ::zhc::EndpointLabel kEndpoints_AUT000069[] = {
    {"l1", 1}, {"l2", 2}, {"l3", 3}, {"l4", 4}, {"l5", 5},
};

constexpr Expose kExp_AUT000069[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l5", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_AUT000069[] = {
    {1, 0x0006},  // genOnOff EP1 (l1)
    {2, 0x0006},  // genOnOff EP2 (l2)
    {3, 0x0006},  // genOnOff EP3 (l3)
    {4, 0x0006},  // genOnOff EP4 (l4)
    {5, 0x0006},  // genOnOff EP5 (l5)
};
}  // namespace

extern const PreparedDefinition kDef_AUT000069{
    .zigbee_models=kModels_AUT000069, .zigbee_models_count=sizeof(kModels_AUT000069)/sizeof(kModels_AUT000069[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_AUT000069, .manufacturer_names_count=sizeof(kManus_AUT000069)/sizeof(kManus_AUT000069[0]),
    .model="AUT000069", .vendor="Automaton",
    .meta=nullptr, .exposes=kExp_AUT000069, .exposes_count=sizeof(kExp_AUT000069)/sizeof(kExp_AUT000069[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AUT000069, .from_zigbee_count=sizeof(kFz_AUT000069)/sizeof(kFz_AUT000069[0]),
    .to_zigbee=kTz_AUT000069, .to_zigbee_count=sizeof(kTz_AUT000069)/sizeof(kTz_AUT000069[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_AUT000069, .bindings_count=sizeof(kBind_AUT000069)/sizeof(kBind_AUT000069[0]),
    .endpoint_map=kEndpoints_AUT000069, .endpoint_map_count=sizeof(kEndpoints_AUT000069)/sizeof(kEndpoints_AUT000069[0]),
};

}  // namespace zhc::devices::automaton
