// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Airam AIRAM-CTR.U — graduated from generated/ for the action remap.
// CTR.U remote
// z2m-source: airam.ts #AIRAM-CTR.U.
//
// CTR.U is a command-style dimmer remote, NOT a controllable on/off
// switch. z2m wires fz.command_on / command_off / command_step /
// command_move / command_stop / command_recall and exposes a single
// composite `action`. The auto-generated port mis-ported it as a
// controllable on/off (kFzOnOff + kTzOnOff + a dead `state` Binary
// expose + a genOnOff write path), so every button press was dropped.
// Fixed by wiring the generic genOnOff / genLevelCtrl / genScenes
// command converters and publishing `action`. Single endpoint (z2m
// binds ep1 only) → one global action, no endpoint_action_suffix.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::airam {
namespace {
const FzConverter* const kFz_AIRAM_CTR_U[] = {
    &::zhc::generic::kFzCommandOn,      // genOnOff cmd 0x01 → action "on"
    &::zhc::generic::kFzCommandOff,     // genOnOff cmd 0x00 → action "off"
    &::zhc::generic::kFzCommandStep,    // genLevelCtrl 0x02 → brightness_step_<up|down>
    &::zhc::generic::kFzCommandMove,    // genLevelCtrl 0x01 → brightness_move_<up|down>
    &::zhc::generic::kFzCommandStop,    // genLevelCtrl 0x03 → brightness_stop
    &::zhc::generic::kFzCommandRecall,  // genScenes   0x05 → recall_<scene>
};
constexpr const char* kModels_AIRAM_CTR_U[] = { "ZBT-Remote-EU-DIMV1A2" };

// z2m e.action([...]); the converters emit canonical action strings at
// runtime (on/off/brightness_step_*/brightness_move_*/brightness_stop/
// recall_<n>). String/State here, no toZigbee.
constexpr Expose kExposes_AIRAM_CTR_U[] = {
    {"action", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_AIRAM_CTR_U[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0005},  // genScenes
};

}  // namespace

extern const PreparedDefinition kDef_AIRAM_CTR_U{
    .zigbee_models=kModels_AIRAM_CTR_U, .zigbee_models_count=sizeof(kModels_AIRAM_CTR_U)/sizeof(kModels_AIRAM_CTR_U[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AIRAM-CTR.U", .vendor="Airam",
    .meta=nullptr, .exposes=kExposes_AIRAM_CTR_U, .exposes_count=sizeof(kExposes_AIRAM_CTR_U)/sizeof(kExposes_AIRAM_CTR_U[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AIRAM_CTR_U, .from_zigbee_count=sizeof(kFz_AIRAM_CTR_U)/sizeof(kFz_AIRAM_CTR_U[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_AIRAM_CTR_U, .bindings_count=sizeof(kBindings_AIRAM_CTR_U)/sizeof(kBindings_AIRAM_CTR_U[0]),
};

}  // namespace zhc::devices::airam
