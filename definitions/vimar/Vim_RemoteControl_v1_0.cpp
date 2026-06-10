// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Vimar RemoteControl_v1.0 — graduated 2026-06-10.
// Remote control IoT — battery wall/handheld transmitter (action-only).
//
// z2m wires `[m.commandsOnOff(), m.commandsWindowCovering(),
// m.commandsLevelCtrl()]` → the device is an ACTION emitter, not a
// controllable on/off load. The auto-port mis-mapped it as a generic
// on/off switch (kFzOnOff + kTzOnOff + a `state` expose) whose state
// never updates (it sends commands, it has no on/off attribute). Now
// wired to the generic command converters that emit `action`:
//
//   m.commandsOnOff()          → on / off / toggle
//     kFzCommandOn (genOnOff 0x01), kFzCommandOff (0x00),
//     kFzCommandOffWithEffect (0x40), kFzCommandToggle (0x02)
//   m.commandsWindowCovering() → open / close / stop
//     kFzCommandCoverOpen/Close/Stop (closuresWindowCovering 0x00/0x01/0x02)
//   m.commandsLevelCtrl()      → brightness_move_to_level / _move_up /
//                                _move_down / _step_up / _step_down / _stop
//     kFzCommandMoveToLevel (genLevelCtrl 0x00), kFzCommandMove (0x01),
//     kFzCommandStep (0x02), kFzCommandStop (0x03)
//
// toZigbee dropped (z2m toZigbee:[] — nothing to set on a transmitter).
// Bindings target the three OUTPUT clusters the remote drives.
// z2m-source: vimar.ts #RemoteControl_v1.0.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vimar {
namespace {
const FzConverter* const kFz_RemoteControl_v1_0[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandOffWithEffect,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
};
constexpr const char* kModels_RemoteControl_v1_0[] = { "RemoteControl_v1.0" };

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff (output)
    {1, 0x0008},  // genLevelCtrl (output)
    {1, 0x0102},  // closuresWindowCovering (output)
};

}  // namespace

extern const PreparedDefinition kDef_RemoteControl_v1_0{
    .zigbee_models=kModels_RemoteControl_v1_0, .zigbee_models_count=sizeof(kModels_RemoteControl_v1_0)/sizeof(kModels_RemoteControl_v1_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RemoteControl_v1.0", .vendor="Vimar",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RemoteControl_v1_0, .from_zigbee_count=sizeof(kFz_RemoteControl_v1_0)/sizeof(kFz_RemoteControl_v1_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::vimar
