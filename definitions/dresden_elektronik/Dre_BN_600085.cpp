// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: DresdenElektronik BN-600085 (Scene Switch) — hand-edited 2026-06-10.
// 3-part Zigbee-powered scene switch (battery-powered remote).
// z2m: m.commandsOnOff() + m.commandsLevelCtrl() + m.commandsColorCtrl()
//      + m.commandsScenes() + m.battery().
//
// FULL on the command stream: ZHC's generic kFzCommand* covers OnOff /
// LevelCtrl / ColorCtrl, and the generic genScenes (0x0005) decoders
// kFzCommandRecall (cmd 0x05 → action "recall_<scene>") + kFzCommandStore
// (cmd 0x04 → action "store_<scene>") cover m.commandsScenes()'s recall /
// store buttons. (The earlier "no generic decoder" note was stale — both
// converters already live in _generic/_shared; they were simply not wired.)
// z2m's add / remove / remove_all scene commands have no generic decoder
// yet — rarely surfaced by a scene remote, deferred (INFRA).
// All actions stay bare (single-endpoint remote, no endpoint_map).
// z2m-source: dresden_elektronik.ts #BN-600085.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::dresden_elektronik {
namespace {
const FzConverter* const kFz_BN_600085[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandRecall,   // genScenes recall → action "recall_<scene>"
    &::zhc::generic::kFzCommandStore,    // genScenes store  → action "store_<scene>"
    &::zhc::generic::kFzBattery,
};
// Remote control sends commands; no toZigbee converters needed.
constexpr const char* kModels_BN_600085[] = { "Scene Switch" };

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_BN_600085{
    .zigbee_models=kModels_BN_600085, .zigbee_models_count=sizeof(kModels_BN_600085)/sizeof(kModels_BN_600085[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BN-600085", .vendor="DresdenElektronik",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_BN_600085, .from_zigbee_count=sizeof(kFz_BN_600085)/sizeof(kFz_BN_600085[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::dresden_elektronik
