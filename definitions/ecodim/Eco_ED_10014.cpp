// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecodim ED-10014 — wall remote (hand-rewritten, 2026-04-28q).
// Zigbee 8 button wall switch - white (4 endpoints, button-pair per EP).
// z2m-source: ecodim.ts #ED-10014 — fz.command_on/off/move/stop + fz.battery;
//   e.action(on_<n>/off_<n>/brightness_move_up_<n>/brightness_move_down_<n>/
//   brightness_stop_<n> for n in 1..4); meta:{multiEndpoint:true}.
//
// PARITY FIX (lost per-button identity): the command decoders emit a bare
// `action` (a kAlwaysGlobalKey), collapsing all four endpoints onto one key —
// z2m distinguishes them per endpoint (on_1 .. brightness_stop_4). Set
// endpoint_map + endpoint_action_suffix so the dispatcher rewrites the key to
// action_1 .. action_4 (same convention as robb ROB_200-007-0).
// No to_zigbee path — battery-powered genOnOff/genLevelCtrl client.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ecodim {
namespace {
const FzConverter* const kFz_ED_10014[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ED_10014[] = { "ED-10014" };

constexpr Expose kExposes_ED_10014[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ED_10014[] = {
    {1, 0x0001},
    {1, 0x0006}, {1, 0x0008},
    {2, 0x0006}, {2, 0x0008},
    {3, 0x0006}, {3, 0x0008},
    {4, 0x0006}, {4, 0x0008},
};

constexpr ::zhc::EndpointLabel kEndpoints_ED_10014[] = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4} };

}  // namespace

extern const PreparedDefinition kDef_ED_10014{
    .zigbee_models=kModels_ED_10014, .zigbee_models_count=sizeof(kModels_ED_10014)/sizeof(kModels_ED_10014[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ED-10014", .vendor="Ecodim",
    .meta=nullptr, .exposes=kExposes_ED_10014, .exposes_count=sizeof(kExposes_ED_10014)/sizeof(kExposes_ED_10014[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ED_10014, .from_zigbee_count=sizeof(kFz_ED_10014)/sizeof(kFz_ED_10014[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ED_10014, .bindings_count=sizeof(kBindings_ED_10014)/sizeof(kBindings_ED_10014[0]),
    .endpoint_map       = kEndpoints_ED_10014,
    .endpoint_map_count = sizeof(kEndpoints_ED_10014)/sizeof(kEndpoints_ED_10014[0]),
    .endpoint_action_suffix = true,  // per-button: action_1..action_4 (z2m on_<n>/off_<n>/...)
};

}  // namespace zhc::devices::ecodim
