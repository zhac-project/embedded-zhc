// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lidl FB21-001 — hand-rewritten command-style remote control.
// Livarno Lux switch and dimming light remote control.
//
// Graduated from generated/Lid_FB21_001.cpp: same bug as the sibling
// FB20-002 — the auto-generator emitted kFzOnOff + kTzOnOff (a `state`
// endpoint) for what is a battery-less wall remote. It emits cluster
// commands that z2m surfaces as `action`:
//   fz.command_on / off / step / move / stop → on / off /
//   brightness_step_up|down / brightness_move_up|down / brightness_stop.
// Swapped to the generic kFzCommand* converters and an `action` enum
// expose. Mirrors the trust ZYCT-202 / icasa ICZB-RM11S ports.
//
// z2m additionally lists tuya.fz.switch_scene, a manuspecific genOnOff
// payload variant that emits action="switch_scene"/scene_N. It has no
// generic converter here; the `action` expose still admits those values
// (deferred — bespoke Tuya scene frame).
// z2m-source: lidl.ts #FB21-001.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lidl {
namespace {
const FzConverter* const kFz_FB21_001[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
};
constexpr const char* kModels_FB21_001[] = { "TS1001" };
constexpr const char* kManus_FB21_001[] = { "_TYZB01_hww2py6b" };
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
};

extern const PreparedDefinition kDef_FB21_001{
    .zigbee_models=kModels_FB21_001, .zigbee_models_count=sizeof(kModels_FB21_001)/sizeof(kModels_FB21_001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_FB21_001, .manufacturer_names_count=sizeof(kManus_FB21_001)/sizeof(kManus_FB21_001[0]),
    .model="FB21-001", .vendor="Lidl",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_FB21_001, .from_zigbee_count=sizeof(kFz_FB21_001)/sizeof(kFz_FB21_001[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lidl
