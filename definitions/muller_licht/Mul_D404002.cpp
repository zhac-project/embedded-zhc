// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: MullerLicht 404002 — hand-rewritten from a wrong on/off bundle.
// Tint dim remote control.
// z2m-source: muller_licht.ts #404002.
//
// z2m fromZigbee: command_on, command_off, command_step, command_move,
// command_stop, command_recall, command_store. Single-zone dim remote —
// every command now has a generic decoder. `store_1` ← kFzCommandStore
// (genScenes commandStore cmd 0x04); previously the store action was a
// dead enum entry while recall_1 already worked via kFzCommandRecall.
//
// Battery-only remote — no to_zigbee path. Bindings cover on/off,
// level control, and scenes, matching z2m's `configure(...)` block.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::muller_licht {
namespace {
const FzConverter* const kFz_D404002[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandStore,   // store_1
};

constexpr const char* kModels_D404002[] = { "ZBT-DIMController-D0800" };
constexpr const char* kActionValues_D404002[] = {
    "on", "off",
    "brightness_step_up", "brightness_step_down",
    "brightness_move_up", "brightness_move_down", "brightness_stop",
    "recall_1", "store_1",
};
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActionValues_D404002,
     sizeof(kActionValues_D404002)/sizeof(kActionValues_D404002[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0005},
};

extern const PreparedDefinition kDef_D404002{
    .zigbee_models=kModels_D404002, .zigbee_models_count=sizeof(kModels_D404002)/sizeof(kModels_D404002[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="404002", .vendor="MullerLicht",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D404002, .from_zigbee_count=sizeof(kFz_D404002)/sizeof(kFz_D404002[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::muller_licht
