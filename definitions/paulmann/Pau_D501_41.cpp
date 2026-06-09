// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Paulmann 501.41 — hand-curated (was wrong-bundled).
// Remote control Smart Home Zigbee 3.0 White — single-endpoint battery
// remote. z2m builds it with
//   m.battery({percentageReporting:false})
//   + m.commandsOnOff({commands:["on","off"]})
//   + m.commandsLevelCtrl({commands:["brightness_move_up","..._down",
//        "brightness_stop","brightness_step_up","brightness_step_down"]})
//   + m.commandsColorCtrl({commands:["color_temperature_move_up","..._down",
//        "color_temperature_move_stop","color_temperature_step_up","..._down"]})
//   + m.commandsScenes({commands:["store","recall"]})
//   + fzLocal.paulmann50141ColorTemperatureStopCommand
//
// The auto-generator collapsed this into a settable on/off light (kFzOnOff
// + kTzOnOff + writable `state`) with no action decode. Replaced with the
// genOnOff / genLevelCtrl / lightingColorCtrl(move+step color-temp) /
// genScenes(store,recall) command decoders + an `action` enum.
//
// Coverage caveat: the custom `paulmann50141ColorTemperatureStopCommand`
// only exists to recover `color_temperature_move_stop` from a malformed
// 5-byte raw frame this remote emits on long-press release. The *normal*
// color-temperature move-stop (genLevelCtrl-style 0x4B stop variant) is
// already decoded by kFzCommandMoveColorTemperature; the raw 5-byte quirk
// has no generic ZHC hook and is left for a vendor-specific fz in v2.
// Single endpoint → `action` is emitted bare (no suffix).
//
// z2m toZigbee: [] (cannot be controlled).
//
// z2m-source: paulmann.ts #501.41.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
const FzConverter* const kFz_D501_41[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveColorTemperature,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandStore,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D501_41[] = { "50141" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    // `action` carries on/off/brightness_move_*/brightness_step_*/
    // brightness_stop/color_temperature_move_*/color_temperature_step_*/
    // recall/store (the z2m commands* action set for this remote).
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg (battery)
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl
    {1, 0x0005},   // genScenes (store/recall)
};

extern const PreparedDefinition kDef_D501_41{
    .zigbee_models=kModels_D501_41, .zigbee_models_count=sizeof(kModels_D501_41)/sizeof(kModels_D501_41[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="501.41", .vendor="Paulmann",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D501_41, .from_zigbee_count=sizeof(kFz_D501_41)/sizeof(kFz_D501_41[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::paulmann
