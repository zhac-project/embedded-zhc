// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Paulmann 501.40 — hand-curated (was wrong-bundled).
// RGB remote control — 4-button battery remote. z2m builds it with
//   m.deviceEndpoints({endpoints: {"1":1,"2":2,"3":3,"4":4}}) + m.battery()
//   + m.commandsOnOff() + m.commandsLevelCtrl() + m.commandsColorCtrl()
//   + m.commandsScenes()
// i.e. each button (EP1..4) emits genOnOff / genLevelCtrl /
// lightingColorCtrl / genScenes cluster commands, surfaced as `action`.
//
// The auto-generator collapsed all of this into a single settable on/off
// light (kFzOnOff + kTzOnOff + writable `state`) on EP1 with no action
// decode and no per-endpoint identity. Replaced with the full command
// decoder set + an `action` enum + endpoint_map {1,2,3,4} +
// endpoint_action_suffix so each button maps to action_1..action_4.
//
// z2m toZigbee: [] (cannot be controlled).
// z2m exposes (via the commands* modernExtends): action including on/off/
//   toggle, brightness_move_*/step_*/stop, color_temperature_move/
//   color_move, recall_*/store_* — distinguished per endpoint by z2m.
//
// z2m-source: paulmann.ts #501.40.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
const FzConverter* const kFz_D501_40[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveToColor,
    &::zhc::generic::kFzCommandMoveColorTemperature,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandStore,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D501_40[] = { "501.40", "50140" };

// z2m deviceEndpoints {"1":1,"2":2,"3":3,"4":4}: drives action_1..action_4.
constexpr ::zhc::EndpointLabel kEndpoints_D501_40[] = {
    {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4},
};

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg (EP1, battery)
    {1, 0x0006}, {1, 0x0008}, {1, 0x0300}, {1, 0x0005},
    {2, 0x0006}, {2, 0x0008}, {2, 0x0300}, {2, 0x0005},
    {3, 0x0006}, {3, 0x0008}, {3, 0x0300}, {3, 0x0005},
    {4, 0x0006}, {4, 0x0008}, {4, 0x0300}, {4, 0x0005},
};

extern const PreparedDefinition kDef_D501_40{
    .zigbee_models=kModels_D501_40, .zigbee_models_count=sizeof(kModels_D501_40)/sizeof(kModels_D501_40[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="501.40", .vendor="Paulmann",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D501_40, .from_zigbee_count=sizeof(kFz_D501_40)/sizeof(kFz_D501_40[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map=kEndpoints_D501_40,
    .endpoint_map_count=sizeof(kEndpoints_D501_40)/sizeof(kEndpoints_D501_40[0]),
    .endpoint_action_suffix=true,
};

}  // namespace zhc::devices::paulmann
