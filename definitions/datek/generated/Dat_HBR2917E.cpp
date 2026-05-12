// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Datek HBR2917E — hand-upgraded for full z2m parity.
// Eva scene selector — battery + temperature + scene/dim command stream
// (recall, on/off, level move/stop). z2m exposes the action set
// ["recall_1", "recall_2", "recall_3", "recall_4", "on", "off",
//  "brightness_move_down", "brightness_move_up", "brightness_stop"].
// `tz.on_off` is in z2m's toZigbee[] for read-back symmetry; we mirror
// it via `kTzOnOff`.
// z2m-source: datek.ts #HBR2917E.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::datek {
namespace {
const FzConverter* const kFz_HBR2917E[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
};
const TzConverter* const kTz_HBR2917E[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_HBR2917E[] = { "Scene Selector", "SSDS" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0005},  // genScenes
    {1, 0x0402},  // msTemperatureMeasurement
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_HBR2917E{
    .zigbee_models=kModels_HBR2917E, .zigbee_models_count=sizeof(kModels_HBR2917E)/sizeof(kModels_HBR2917E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HBR2917E", .vendor="Datek",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HBR2917E, .from_zigbee_count=sizeof(kFz_HBR2917E)/sizeof(kFz_HBR2917E[0]),
    .to_zigbee=kTz_HBR2917E, .to_zigbee_count=sizeof(kTz_HBR2917E)/sizeof(kTz_HBR2917E[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::datek
