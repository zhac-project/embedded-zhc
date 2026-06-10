// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Airam CTR.UBX — graduated from generated/ for the action remap.
// CTR.U remote BX
// z2m-source: airam.ts #CTR.UBX.
//
// CTR.UBX is the BX revision of the CTR.U command-style dimmer remote,
// NOT a controllable on/off switch. z2m wires fz.command_on /
// command_off / command_step / command_move / command_stop /
// command_recall and exposes a single composite `action`. The auto-
// generated port mis-ported it as a controllable on/off (kFzOnOff +
// kTzOnOff + a dead `state` Binary expose + a genOnOff write path), so
// every button press was dropped. Fixed by wiring the generic genOnOff
// / genLevelCtrl / genScenes command converters and publishing
// `action`. z2m's configure binds genBasic/genOnOff/genLevelCtrl/
// genScenes on ep1; mirrored here (genBasic has no action source but is
// kept for parity). Single endpoint → one global action.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::airam {
namespace {
const FzConverter* const kFz_CTR_UBX[] = {
    &::zhc::generic::kFzCommandOn,      // genOnOff cmd 0x01 → action "on"
    &::zhc::generic::kFzCommandOff,     // genOnOff cmd 0x00 → action "off"
    &::zhc::generic::kFzCommandStep,    // genLevelCtrl 0x02 → brightness_step_<up|down>
    &::zhc::generic::kFzCommandMove,    // genLevelCtrl 0x01 → brightness_move_<up|down>
    &::zhc::generic::kFzCommandStop,    // genLevelCtrl 0x03 → brightness_stop
    &::zhc::generic::kFzCommandRecall,  // genScenes   0x05 → recall_<scene>
};
constexpr const char* kModels_CTR_UBX[] = { "ZBT-Remote-EU-DIMV2A2" };

// z2m e.action([...]); converters emit canonical action strings at
// runtime. String/State, no toZigbee.
constexpr Expose kExposes_CTR_UBX[] = {
    {"action", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_CTR_UBX[] = {
    {1, 0x0000},  // genBasic   (z2m configure binds it; no action source)
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0005},  // genScenes
};

}  // namespace

extern const PreparedDefinition kDef_CTR_UBX{
    .zigbee_models=kModels_CTR_UBX, .zigbee_models_count=sizeof(kModels_CTR_UBX)/sizeof(kModels_CTR_UBX[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CTR.UBX", .vendor="Airam",
    .meta=nullptr, .exposes=kExposes_CTR_UBX, .exposes_count=sizeof(kExposes_CTR_UBX)/sizeof(kExposes_CTR_UBX[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CTR_UBX, .from_zigbee_count=sizeof(kFz_CTR_UBX)/sizeof(kFz_CTR_UBX[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_CTR_UBX, .bindings_count=sizeof(kBindings_CTR_UBX)/sizeof(kBindings_CTR_UBX[0]),
};

}  // namespace zhc::devices::airam
