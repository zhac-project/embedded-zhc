// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Nordtronic 98426061 — graduated for the action-remote remap.
// Remote Control
// z2m-source: nordtronic.ts #98426061
//             (m.battery + m.identify + m.commandsOnOff + m.commandsLevelCtrl
//              + m.commandsColorCtrl).
//
// z2m's role for this device is a *command client*: it emits action events from
// the genOnOff / genLevelCtrl / genColorCtrl cluster-specific commands it sends,
// plus a battery channel. The auto-port mis-ported it as a controllable on/off
// switch — kFzOnOff + a StateSet `state` expose + kTzOnOff — which is a dead
// state (the remote has no on/off attribute to read or write) and dropped the
// whole action stream. Rewired the generic kFzCommand* converters (each emits
// the z2m `action` string) + an `action` String expose; kept battery (correct).
// Single endpoint -> no endpoint_map -> bare `action` key (no suffix).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nordtronic {
namespace {
const FzConverter* const kFz_D98426061[] = {
    // m.commandsOnOff() -> action on / off / toggle
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    // m.commandsLevelCtrl() -> brightness move / step / stop / move_to_level
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzCommandMoveToLevel,
    // m.commandsColorCtrl() -> colour move/step commands
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzCommandMoveColorTemperature,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D98426061[] = { "WSZ 98426061", "98426061" };

}  // namespace


// --- exposes / bindings (Tier-2 hand-maintained) ---
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
    {1, 0x0001},
};
// --- end Tier-2 block ---

extern const PreparedDefinition kDef_D98426061{
    .zigbee_models=kModels_D98426061, .zigbee_models_count=sizeof(kModels_D98426061)/sizeof(kModels_D98426061[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="98426061", .vendor="Nordtronic",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D98426061, .from_zigbee_count=sizeof(kFz_D98426061)/sizeof(kFz_D98426061[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nordtronic
