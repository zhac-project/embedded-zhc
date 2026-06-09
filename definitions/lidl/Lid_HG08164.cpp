// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lidl HG08164 — hand-rewritten command-style smart button.
// Silvercrest smart button (TS004F, scene-switch firmware).
//
// Graduated from generated/Lid_HG08164.cpp: the auto-generator emitted
// kFzOnOff + kTzOnOff (a controllable `state` endpoint) which is wrong —
// this is a battery wireless button. z2m wires
//   fz.command_on / command_off / command_step / command_stop → action
//   (on / off / brightness_step_up|down / brightness_stop),
// plus fz.battery and tuya.fz.on_off_action (the press-count handler that
// adds action="single"/"double"). Swapped to the generic kFzCommand*
// converters (each emits action="…") + kFzBattery, replacing the `state`
// expose with an `action` enum. Mirrors the icasa ICZB-RM11S port.
//
// tuya.fz.on_off_action is a manuspecific genOnOff variant with no
// generic converter; the `action` expose still admits single/double
// (deferred — bespoke Tuya press-count frame). genPowerCfg/genOnOff
// binds preserved.
// z2m-source: lidl.ts #HG08164.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lidl {
namespace {
const FzConverter* const kFz_HG08164[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_HG08164[] = { "TS004F" };
constexpr const char* kManus_HG08164[] = { "_TZ3000_rco1yzb1" };
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0001},  // genPowerCfg
};

extern const PreparedDefinition kDef_HG08164{
    .zigbee_models=kModels_HG08164, .zigbee_models_count=sizeof(kModels_HG08164)/sizeof(kModels_HG08164[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_HG08164, .manufacturer_names_count=sizeof(kManus_HG08164)/sizeof(kManus_HG08164[0]),
    .model="HG08164", .vendor="Lidl",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HG08164, .from_zigbee_count=sizeof(kFz_HG08164)/sizeof(kFz_HG08164[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lidl
