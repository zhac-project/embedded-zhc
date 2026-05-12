// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Vesternet VES-ZB-REM-013 — hand-curated.
// Zigbee 12-button remote — battery-powered, NO on/off state. The
// auto-generator wrong-bundled this as a generic on/off switch; reality
// is that the device only emits commandOn/Off/Move/Stop/Recall ZCL
// cluster commands which the action-decoders translate into "on_N",
// "off_N", "brightness_move_up_N", "brightness_move_down_N",
// "brightness_stop_N", "recall_N_M" semantic action labels.
//
// z2m fromZigbee: fz.command_on, fz.command_off, fz.command_move,
//                 fz.command_stop, fz.command_recall, fzLocal.battery
// z2m toZigbee: [] (it cannot be controlled).
// z2m exposes: e.battery(), e.action([72 labels]).
//
// Action-key suffix routing per source endpoint (1..6 for the 12 keys
// = 6 rocker pairs) is delivered by `kFzCommandOn/Off/Move/Stop/Recall`
// via their default endpoint suffixing. There is no per-endpoint state
// because the device just routes button presses to its own EPs.
//
// z2m-source: vesternet.ts #VES-ZB-REM-013.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vesternet {
namespace {
const FzConverter* const kFz_VES_ZB_REM_013[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzBattery,
};
// Toolbox is empty — the remote has no controllable state.
constexpr const char* kModels_VES_ZB_REM_013[] = { "ZGRC-KEY-013" };

}  // namespace


// --- hand-curated exposes / bindings ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    // `action` is the conventional event label key; values are emitted
    // by kFzCommand* converters. We expose it as Enum-typed; valid
    // labels from z2m: on_1..on_6, off_1..off_6, stop_1..stop_6,
    // brightness_move_up_1..6, brightness_move_down_1..6,
    // brightness_stop_1..6, recall_1_1..6_5 — total 72 distinct strings.
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

// z2m configure: bind genOnOff/genLevelCtrl/genPowerCfg + battery%.
// fz.command_recall implies genScenes binding too. Configure callbacks
// are deferred to vendor-specific configure() in v2; for now we only
// declare the binding rows so the adapter can emit ZDO bind requests.
constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0001},   // genPowerCfg
    {1, 0x0005},   // genScenes (for fz.command_recall)
};
// --- end ---

extern const PreparedDefinition kDef_VES_ZB_REM_013{
    .zigbee_models=kModels_VES_ZB_REM_013, .zigbee_models_count=sizeof(kModels_VES_ZB_REM_013)/sizeof(kModels_VES_ZB_REM_013[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VES-ZB-REM-013", .vendor="Vesternet",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VES_ZB_REM_013, .from_zigbee_count=sizeof(kFz_VES_ZB_REM_013)/sizeof(kFz_VES_ZB_REM_013[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::vesternet
