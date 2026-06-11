// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tci "TCI - Mini ZLL I" — graduated from generated/ for the action remap.
// Dali 1-10V driver.
//
// The real z2m<->ez gap: this driver is BOTH a controllable light AND a
// controller that emits genOnOff / genLevelCtrl commands. z2m wires
//   m.light()            → on/off + brightness (state + brightness)
//   m.commandsOnOff()    → action "on"/"off"/"toggle" (fz on genOnOff cmds)
//   m.commandsLevelCtrl()→ action brightness_move_to_level / move_<dir> /
//                          step_<dir> / stop (fz on genLevelCtrl cmds)
// The auto-generated port kept only the light half (on/off + brightness),
// silently dropping the `action` expose and every command converter, so
// the device's controller role was dead. Fixed by ADDITIVELY wiring the
// generic genOnOff / genLevelCtrl command converters and publishing
// `action` alongside the existing settable state/brightness + write path.
// Single endpoint (z2m binds ep1 only) → one global action, no suffix.
//
// The light-half attribute converters (kFzOnOff/kFzBrightness read attr
// 0x0000) and the command converters (MessageType::Command) match
// disjoint frame types on the same clusters, so they never collide.
//
// z2m-source: tci.ts #"TCI - Mini ZLL I" (m.light + m.commandsOnOff + m.commandsLevelCtrl).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tci {
namespace {
const FzConverter* const kFz_TCI___Mini_ZLL_I[] = {
    &::zhc::generic::kFzOnOff,              // genOnOff attr 0x0000 → state
    &::zhc::generic::kFzBrightness,         // genLevelCtrl attr 0x0000 → brightness
    &::zhc::generic::kFzCommandOn,          // genOnOff   cmd 0x01 → action "on"
    &::zhc::generic::kFzCommandOff,         // genOnOff   cmd 0x00 → action "off"
    &::zhc::generic::kFzCommandToggle,      // genOnOff   cmd 0x02 → action "toggle"
    &::zhc::generic::kFzCommandMoveToLevel, // genLevelCtrl 0x00 → action "brightness_move_to_level"
    &::zhc::generic::kFzCommandMove,        // genLevelCtrl 0x01 → action "brightness_move_<up|down>"
    &::zhc::generic::kFzCommandStep,        // genLevelCtrl 0x02 → action "brightness_step_<up|down>"
    &::zhc::generic::kFzCommandStop,        // genLevelCtrl 0x03 → action "brightness_stop"
};
const TzConverter* const kTz_TCI___Mini_ZLL_I[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_TCI___Mini_ZLL_I[] = { "TCI - Mini ZLL I" };

}  // namespace


// state + brightness are the settable light half; `action` mirrors z2m
// e.action([...]) — the command converters emit canonical action strings.
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
};

extern const PreparedDefinition kDef_TCI___Mini_ZLL_I{
    .zigbee_models=kModels_TCI___Mini_ZLL_I, .zigbee_models_count=sizeof(kModels_TCI___Mini_ZLL_I)/sizeof(kModels_TCI___Mini_ZLL_I[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TCI - Mini ZLL I", .vendor="Tci",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TCI___Mini_ZLL_I, .from_zigbee_count=sizeof(kFz_TCI___Mini_ZLL_I)/sizeof(kFz_TCI___Mini_ZLL_I[0]),
    .to_zigbee=kTz_TCI___Mini_ZLL_I, .to_zigbee_count=sizeof(kTz_TCI___Mini_ZLL_I)/sizeof(kTz_TCI___Mini_ZLL_I[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::tci
