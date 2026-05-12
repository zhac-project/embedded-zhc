// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: DresdenElektronik BN-600085 (Scene Switch) — hand-edited 2026-04-29c.
// 3-part Zigbee-powered scene switch (battery-powered remote).
// z2m: m.commandsOnOff() + m.commandsLevelCtrl() + m.commandsColorCtrl()
//      + m.commandsScenes() + m.battery().
//
// PARTIAL: ZHC's generic kFzCommand* covers OnOff / LevelCtrl / ColorCtrl;
// `commandsScenes` (cluster 0x0005 commandRecall / commandStore) has no
// generic decoder yet — scene button presses will not surface as
// `action` events until a generic kFzCommandRecallScene lands.
// Bound clusters still expose state/battery; the runtime gap is
// documented in DRESDEN_ELEKTRONIK_PARITY.md.
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
