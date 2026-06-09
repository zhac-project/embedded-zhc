// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lidl FB20-002 — hand-rewritten command-style remote control.
// Livarno Lux switch and dimming light remote control.
//
// Graduated from generated/Lid_FB20_002.cpp: the auto-generator emitted
// kFzOnOff + kTzOnOff (a controllable on/off endpoint with a `state`
// expose) which is wrong — this is a battery-less wall remote. It emits
// cluster-specific commands and z2m surfaces them as `action`:
//   fz.command_on / off / step / move / stop → on / off /
//   brightness_step_up|down / brightness_move_up|down / brightness_stop.
// Swapped to the generic kFzCommand* converters (each emits action="…")
// and replaced the `state` expose with an `action` enum. Mirrors the
// trust ZYCT-202 and icasa ICZB-RM11S remote ports.
//
// z2m also lists fzLocal.FB20002_on, a bespoke handler re-emitting
// action="on" for a non-standard on frame this remote sends; the plain
// kFzCommandOn already covers the standard genOnOff On command and the
// vendor-specific frame has no generic converter (deferred).
// z2m-source: lidl.ts #FB20-002.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lidl {
namespace {
const FzConverter* const kFz_FB20_002[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
};
constexpr const char* kModels_FB20_002[] = { "TS1001" };
constexpr const char* kManus_FB20_002[] = { "_TYZB01_bngwdjsr" };
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
};

extern const PreparedDefinition kDef_FB20_002{
    .zigbee_models=kModels_FB20_002, .zigbee_models_count=sizeof(kModels_FB20_002)/sizeof(kModels_FB20_002[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_FB20_002, .manufacturer_names_count=sizeof(kManus_FB20_002)/sizeof(kManus_FB20_002[0]),
    .model="FB20-002", .vendor="Lidl",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_FB20_002, .from_zigbee_count=sizeof(kFz_FB20_002)/sizeof(kFz_FB20_002[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lidl
