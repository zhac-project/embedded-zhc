// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lutron Z3-1BRL — hand-curated (was wrong-bundled).
// Aurora smart bulb dimmer — a battery rotary dimmer remote. z2m wires it
// as an action-only device with battery:
//   fromZigbee: [fzLocal.custom_command_move_to_level]
//   extend:     [m.battery()]
//   toZigbee:   []   (cannot be controlled)
//   exposes:    action(["brightness"]), numeric("brightness")
//   configure:  bind genLevelCtrl on EP1
// The rotary turns are reported as genLevelCtrl MoveToLevel commands.
//
// The auto-generator collapsed this into a settable on/off light
// (kFzOnOff + kTzOnOff + a writable `state` expose) — a phantom relay with
// no action decode. The battery half was correct. Replaced the on/off path
// with the genLevelCtrl MoveToLevel command decoder + an `action` enum,
// kept battery, dropped the phantom state and toolbox. Single endpoint
// → no endpoint_map, bare `action`. (z2m's custom_command_move_to_level
// only normalises a NaN level to 255 before delegating to
// fz.command_move_to_level — a JS-side edge case; the generic
// kFzCommandMoveToLevel decode of MoveToLevel/...WithOnOff matches.)
// The generic MoveToLevel decode surfaces the dimmer position as `level`.
//
// z2m-source: lutron.ts #Z3-1BRL.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lutron {
namespace {
const FzConverter* const kFz_Z3_1BRL[] = {
    &::zhc::generic::kFzCommandMoveToLevel,  // genLevelCtrl MoveToLevel → brightness_move_to_level + level
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_Z3_1BRL[] = { "Z3-1BRL" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0008},   // genLevelCtrl (z2m configure binds this)
    {1, 0x0001},   // genPowerCfg (battery)
};

extern const PreparedDefinition kDef_Z3_1BRL{
    .zigbee_models=kModels_Z3_1BRL, .zigbee_models_count=sizeof(kModels_Z3_1BRL)/sizeof(kModels_Z3_1BRL[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Z3-1BRL", .vendor="Lutron",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Z3_1BRL, .from_zigbee_count=sizeof(kFz_Z3_1BRL)/sizeof(kFz_Z3_1BRL[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lutron
