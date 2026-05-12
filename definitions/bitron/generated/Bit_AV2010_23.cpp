// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bitron AV2010/23 — hand-rewritten 2026-04-28.
// 4-button Zigbee remote control.
// z2m-source: bitron.ts #AV2010/23.
//
// z2m bundle: fz.ias_no_alarm + fz.command_on + fz.command_off
//             + fz.command_step + fz.command_recall
//   exposes [action(on/off/brightness_step_*/recall_*), battery_low].
//
// Mapped:
//   kFzCommandOn     — genOnOff cmd 0x01 → action="on".
//   kFzCommandOff    — genOnOff cmd 0x00 → action="off".
//   kFzCommandStep   — genLevelCtrl cmd 0x02 → action="brightness_step_{up,down}".
//   kFzCommandRecall — genScenes cmd 0x05 → action="recall_<n>".
// NOTE: fz.ias_no_alarm is a no-op in z2m (drops IAS reports); not wired.
//   Battery_low surfaces via genPowerCfg low-voltage event normally — there
//   is no kFzBatteryLow generic, so this is a residual gap (see
//   docs/BITRON_PARITY.md).
// NOTE: previous port wired kFzOnOff/kTzOnOff and exposed `state`. AV2010/23
//   is a sender (button), not a sink, so it has no on/off state to control.
//   Removed.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bitron {
namespace {
constexpr const char* kModels_AV2010_23[] = { "AV2010/23", "902010/23" };

constexpr const char* kActions_AV2010_23[] = {
    "on", "off",
    "brightness_step_up", "brightness_step_down",
    "recall_0",  "recall_1",  "recall_2",  "recall_3",
    "recall_4",  "recall_5",  "recall_6",  "recall_7",
    "recall_8",  "recall_9",  "recall_10", "recall_11",
    "recall_12", "recall_13", "recall_14", "recall_15",
};

const FzConverter* const kFz_AV2010_23[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandRecall,
};

}  // namespace


constexpr Expose kAutoExposes[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr,
      kActions_AV2010_23,
      sizeof(kActions_AV2010_23)/sizeof(kActions_AV2010_23[0]) },
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0005},  // genScenes
};

extern const PreparedDefinition kDef_AV2010_23{
    .zigbee_models=kModels_AV2010_23, .zigbee_models_count=sizeof(kModels_AV2010_23)/sizeof(kModels_AV2010_23[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AV2010/23", .vendor="Bitron",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AV2010_23, .from_zigbee_count=sizeof(kFz_AV2010_23)/sizeof(kFz_AV2010_23[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bitron
