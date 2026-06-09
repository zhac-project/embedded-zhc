// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Awox ERCU_WS_Zm — wall-mount light remote, not an on/off switch.
//
// Connect-Z magnetic wall mountable light RCU. z2m models it as
//   m.deviceEndpoints({endpoints:{"1":1,"3":3}}), meta:{multiEndpoint:true}
//   m.commandsOnOff() + m.commandsLevelCtrl() + m.commandsColorCtrl()
// i.e. a battery remote that only *sends* genOnOff / genLevelCtrl /
// lightingColorCtrl client->server commands and exposes `action`. It has
// no local relay. The generated def wired kFzOnOff + a controllable
// on/off TZ (both dead — kFzOnOff decodes attribute *reports*, which a
// pure command transmitter never sends) and exposed a meaningless
// `state`. Re-wired to the generic command-action decoders; the action
// stream surfaces the button presses. endpoint_map mirrors z2m's
// endpoints:{"1":1,"3":3} postfix, so the two button banks surface as
// action_1 (main) and action_3 (secondary).
//
// z2m-source: awox.ts #ERCU_WS_Zm.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::awox {
namespace {
const FzConverter* const kFz_ERCU_WS_Zm[] = {
    // m.commandsOnOff()
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    // m.commandsLevelCtrl()
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    // m.commandsColorCtrl()
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzCommandMoveHue,
    &::zhc::generic::kFzCommandMoveColorTemperature,
};
constexpr const char* kModels_ERCU_WS_Zm[] = { "ERCU_WS_Zm" };

constexpr ::zhc::EndpointLabel kEndpoints_ERCU_WS_Zm[] = { {"1", 1}, {"3", 3} };

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006}, {1, 0x0008}, {1, 0x0300},
    {3, 0x0006}, {3, 0x0008}, {3, 0x0300},
};

}  // namespace


constexpr WhiteLabel kWhiteLabels_ERCU_WS_Zm[] = {
    {"EGLO","900116"},
};
extern const PreparedDefinition kDef_ERCU_WS_Zm{
    .zigbee_models=kModels_ERCU_WS_Zm, .zigbee_models_count=sizeof(kModels_ERCU_WS_Zm)/sizeof(kModels_ERCU_WS_Zm[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ERCU_WS_Zm", .vendor="Awox",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_ERCU_WS_Zm, .white_labels_count=sizeof(kWhiteLabels_ERCU_WS_Zm)/sizeof(kWhiteLabels_ERCU_WS_Zm[0]),
    .from_zigbee=kFz_ERCU_WS_Zm, .from_zigbee_count=sizeof(kFz_ERCU_WS_Zm)/sizeof(kFz_ERCU_WS_Zm[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_ERCU_WS_Zm,
    .endpoint_map_count = sizeof(kEndpoints_ERCU_WS_Zm)/sizeof(kEndpoints_ERCU_WS_Zm[0]),
    .endpoint_action_suffix = true,
};

}  // namespace zhc::devices::awox
