// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Insta InstaRemote — hand-rewritten 2026-04-28s.
// Zigbee Light Link wall/handheld transmitter (battery-powered remote).
// White-labels: Gira 2430-100 / 2435-10, Jung ZLLCD5004M / ZLLLS5004M /
// ZLLA5004M / ZLLHS4. Rebrand of the same SoC.
// z2m-source: insta.ts #InstaRemote.
//   zigbeeModel: [" Remote"]   (note the leading space)
//   fromZigbee:  [command_recall, command_on, command_off_with_effect,
//                 command_step, command_step, command_stop]
//   exposes:     action(["select_0".."select_5","on","off","down","up","stop"])
//   toZigbee:    []
//
// Mapped:
//   kFzCommandRecall          — genScenes cmd 0x05 → "recall_<n>" (select_*)
//   kFzCommandOn              — genOnOff cmd 0x01 → "on"
//   kFzCommandOffWithEffect   — genOnOff cmd 0x40 → "off"  (NEW converter)
//   kFzCommandStep            — genLevelCtrl cmd 0x02 → "brightness_step_up/_down"
//   kFzCommandMove            — genLevelCtrl cmd 0x01 → "brightness_move_up/_down"
//   kFzCommandStop            — genLevelCtrl cmd 0x03 → "brightness_stop"
//
// Known parity gap (documented in INSTA_PARITY.md):
//   * z2m emits action labels `select_0..5` for genScenes recalls and
//     `up`/`down`/`stop` for the dim wheel. Our `kFzCommandRecall`
//     emits `recall_<scene>` and `kFzCommandStep` emits
//     `brightness_step_<dir>`. Downstream rule consumers can remap;
//     a vendor-specific converter would be cosmetic-only.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::insta {
namespace {
const FzConverter* const kFz_InstaRemote[] = {
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOffWithEffect,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
};
constexpr const char* kModels_InstaRemote[] = { " Remote" };

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr WhiteLabel kWhiteLabels[] = {
    {"Gira", "2430-100"},
    {"Gira", "2435-10"},
    {"Jung", "ZLLCD5004M"},
    {"Jung", "ZLLLS5004M"},
    {"Jung", "ZLLA5004M"},
    {"Jung", "ZLLHS4"},
};

}  // namespace

extern const PreparedDefinition kDef_InstaRemote{
    .zigbee_models=kModels_InstaRemote, .zigbee_models_count=sizeof(kModels_InstaRemote)/sizeof(kModels_InstaRemote[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="InstaRemote", .vendor="Insta",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels, .white_labels_count=sizeof(kWhiteLabels)/sizeof(kWhiteLabels[0]),
    .from_zigbee=kFz_InstaRemote, .from_zigbee_count=sizeof(kFz_InstaRemote)/sizeof(kFz_InstaRemote[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::insta
