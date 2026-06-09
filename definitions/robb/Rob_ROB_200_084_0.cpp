// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Robb ROB_200-084-0 — hand-rewritten from a wrong on/off bundle.
// 4-button wireless Zigbee switch (Black, dual button front).
// z2m-source: robb.ts #ROB_200-084-0 — m.battery() + m.commandsOnOff() +
//   m.commandsLevelCtrl() over endpoints 1..4.
//
// Bug fixed: same generator misread as ROB_200-081-0 — m.commandsOnOff/
// commandsLevelCtrl became a controllable m.onOff() with a writable `state`
// + kTzOnOff. This is a battery-powered wireless wall switch (genOnOff /
// genLevelCtrl CLIENT); it only emits cluster commands. We wire the command
// decoders and expose `action`; the `_<n>` per-button suffix comes from the
// endpoint_map downstream.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::robb {
namespace {
const FzConverter* const kFz_ROB_200_084_0[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ROB_200_084_0[] = { "ROB_200-084-0" };

constexpr ::zhc::EndpointLabel kEndpoints_ROB_200_084_0[] = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4} };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0008},
};

extern const PreparedDefinition kDef_ROB_200_084_0{
    .zigbee_models=kModels_ROB_200_084_0, .zigbee_models_count=sizeof(kModels_ROB_200_084_0)/sizeof(kModels_ROB_200_084_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ROB_200-084-0", .vendor="Robb",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ROB_200_084_0, .from_zigbee_count=sizeof(kFz_ROB_200_084_0)/sizeof(kFz_ROB_200_084_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_ROB_200_084_0,
    .endpoint_map_count = sizeof(kEndpoints_ROB_200_084_0)/sizeof(kEndpoints_ROB_200_084_0[0]),
    .endpoint_action_suffix = true,  // per-button: action_1..action_4 (z2m on_<n>/off_<n>/...)
};

}  // namespace zhc::devices::robb
