// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ynoa 8718801528334 — hand-curated (was wrong-bundled).
// Remote control one-button dimmer — battery-powered (single endpoint), NOT
// a settable on/off device. The auto-generator emitted kFzOnOff + kTzOnOff +
// a writable `state`, so every press was dead and it falsely advertised a
// relay. The remote drives a bound light via genOnOff / genLevelCtrl cluster
// commands, surfaced on `action`.
//
// z2m fromZigbee: command_on, command_off, command_move, command_stop,
//   battery.
// z2m toZigbee: [] (cannot be controlled).
// z2m exposes: e.action(["on","off","brightness_move_up",
//   "brightness_move_down","brightness_stop"]), e.battery().
//
// Action label mapping (generic decoders):
//   kFzCommandOn   → "on"
//   kFzCommandOff  → "off"
//   kFzCommandMove → "brightness_move_up" / "brightness_move_down"
//   kFzCommandStop → "brightness_stop"
// Single endpoint → bare `action` (no suffix).
//
// z2m-source: ynoa.ts #8718801528334.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ynoa {
namespace {
const FzConverter* const kFz_D8718801528334[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D8718801528334[] = { "ZBT-DIMSwitch-D0000" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg (z2m configure binds genPowerCfg only)
};

extern const PreparedDefinition kDef_D8718801528334{
    .zigbee_models=kModels_D8718801528334, .zigbee_models_count=sizeof(kModels_D8718801528334)/sizeof(kModels_D8718801528334[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="8718801528334", .vendor="Ynoa",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D8718801528334, .from_zigbee_count=sizeof(kFz_D8718801528334)/sizeof(kFz_D8718801528334[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ynoa
