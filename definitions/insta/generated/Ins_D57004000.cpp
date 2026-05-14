// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Insta 57004000 — hand-rewritten 2026-04-28s.
// Pushbutton Interface 2-gang 230V (action-only, no relay back-end).
// z2m-source: insta.ts #57004000.
//   zigbeeModel: ["NEXENTRO Pushbutton Interface", "57004000"]
//   fingerprint: manufacturerName "Insta GmbH" + modelID "Generic UP Device"
//                EP4 + EP5 (deviceID 261, dimmer-style outputs) + EP7
//                (deviceID 515, cover output) + EP242 (Green Power)
//   fromZigbee:  [command_on, command_off, command_toggle, command_recall,
//                 command_move, command_stop, command_cover_open,
//                 command_cover_close, command_cover_stop]
//   toZigbee:    []
//   exposes:     action(["on_e1","off_e1","toggle_e1","recall_*_e1",
//                        "brightness_stop_e1","brightness_move_*_e1",
//                        "on_e2","off_e2", ..., "open_cover","close_cover",
//                        "stop_cover"])
//   meta:        multiEndpoint
//   endpoint:    {e1: 4, e2: 5, cover: 7}
//
// Mapped:
//   kFzCommandOn          — genOnOff cmd 0x01 → action="on"
//   kFzCommandOff         — genOnOff cmd 0x00 → action="off"
//   kFzCommandToggle      — genOnOff cmd 0x02 → action="toggle"
//   kFzCommandRecall      — genScenes cmd 0x05 → action="recall_<n>"
//   kFzCommandMove        — genLevelCtrl cmd 0x01 → action="brightness_move_<dir>"
//   kFzCommandStop        — genLevelCtrl cmd 0x03 → action="brightness_stop"
//   kFzCommandCoverOpen   — closuresWindowCovering cmd 0x00 → action="open"
//   kFzCommandCoverClose  — cmd 0x01 → action="close"
//   kFzCommandCoverStop   — cmd 0x02 → action="stop"
//
// Multi-endpoint:
//   endpoint_map labels EP4→"e1", EP5→"e2", EP7→"cover".
//   endpoint_action_suffix=true so action keys gain `_e1` / `_e2` /
//   `_cover` suffix matching z2m's enumeration. The cover commands
//   on EP7 surface as `action="open_cover"` / `action="close_cover"`
//   / `action="stop_cover"`.
//
// No `state` expose / no `to_zigbee` — this is a pure pushbutton
// interface; the actual switching happens upstream (typically a
// 57005000 actuator bound to the same group).

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::insta {
namespace {
const FzConverter* const kFz_D57004000[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
};
constexpr const char* kModels_D57004000[] = { "NEXENTRO Pushbutton Interface", "57004000" };

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr ::zhc::EndpointLabel kEndpoints_D57004000[] = {
    {"e1",    4},
    {"e2",    5},
    {"cover", 7},
};

}  // namespace

extern const PreparedDefinition kDef_D57004000{
    .zigbee_models=kModels_D57004000, .zigbee_models_count=sizeof(kModels_D57004000)/sizeof(kModels_D57004000[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="57004000", .vendor="Insta",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D57004000, .from_zigbee_count=sizeof(kFz_D57004000)/sizeof(kFz_D57004000[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_D57004000,
    .endpoint_map_count = sizeof(kEndpoints_D57004000)/sizeof(kEndpoints_D57004000[0]),
    .endpoint_action_suffix = true,
};

}  // namespace zhc::devices::insta
