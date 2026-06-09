// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Legrand 067774 — dual-gang battery action remote, not a switch.
//
// Two-button wireless wall remote, no local relay. The generated def
// wired kFzOnOff + an on/off TZ (both dead) and carried no endpoint_map,
// so the two gangs could not be told apart. The device emits genOnOff /
// genLevelCtrl client->server commands on endpoints 1 (left) and 2
// (right). Re-wired to the generic command-action decoders; the
// endpoint_map suffixes the emitted `action` key per gang.
//
// z2m-source: legrand.ts #067774 —
//   meta: {multiEndpoint: true}, endpoint: {left:1, right:2}
//   fromZigbee: [fz.identify, fz.command_on, fz.command_off,
//                fz.command_toggle, fz.command_move, fz.command_stop,
//                fz.battery]
//   exposes: e.action([...]), e.battery()
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::legrand {
namespace {
const FzConverter* const kFz_D067774[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D067774[] = { " Double gangs remote switch" };

constexpr ::zhc::EndpointLabel kEndpoints_D067774[] = { {"left", 1}, {"right", 2} };

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0001},
    {2, 0x0006},
    {2, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_D067774{
    .zigbee_models=kModels_D067774, .zigbee_models_count=sizeof(kModels_D067774)/sizeof(kModels_D067774[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="067774", .vendor="Legrand",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D067774, .from_zigbee_count=sizeof(kFz_D067774)/sizeof(kFz_D067774[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_D067774,
    .endpoint_map_count = sizeof(kEndpoints_D067774)/sizeof(kEndpoints_D067774[0]),
    .endpoint_action_suffix = true,
};

}  // namespace zhc::devices::legrand
