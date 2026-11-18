// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: ROBB ROB_200-082-0 4-button wireless Zigbee switch, new in z2m
// v26.126.0..v26.133.0 (same extends as ROB_200-081-0).
// m.deviceEndpoints 1-4 + m.battery() + m.commandsOnOff / m.commandsLevelCtrl
// over endpoints 1-4: genOnOff on / off / offWithEffect / toggle and
// genLevelCtrl moveToLevel / move / step / stop (+ stopWithOnOff) -> action,
// battery percentage (/2) + voltage; genPowerCfg, genOnOff and genLevelCtrl
// bound on every endpoint. The button rides on the key (action_1..action_4),
// as on ROB_200-081-0; z2m puts it in the value (on_1).
// Not ported: the battery percentage reporting row (house rule);
// moveWithOnOff / stepWithOnOff / moveToLevelWithOnOff (no generic decoder).
// z2m-source: robb.ts #ROB_200-082-0.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::robb {
namespace {

const FzConverter* const kFz_ROB_200_082_0[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandOffWithEffect,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_ROB_200_082_0[] = { "ROB_200-082-0" };

constexpr ::zhc::EndpointLabel kEndpoints_ROB_200_082_0[] = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4} };

constexpr const char* kActionValues[] = {
    "on", "off", "toggle",
    "brightness_move_to_level", "brightness_move_up", "brightness_move_down",
    "brightness_step_up", "brightness_step_down", "brightness_stop" };

constexpr Expose kExposes_ROB_200_082_0[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, "Triggered action (e.g. a button click)",
     kActionValues, sizeof(kActionValues)/sizeof(kActionValues[0])},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ROB_200_082_0[] = {
    {1, 0x0001},
    {1, 0x0006}, {2, 0x0006}, {3, 0x0006}, {4, 0x0006},
    {1, 0x0008}, {2, 0x0008}, {3, 0x0008}, {4, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_ROB_200_082_0{
    .zigbee_models=kModels_ROB_200_082_0, .zigbee_models_count=sizeof(kModels_ROB_200_082_0)/sizeof(kModels_ROB_200_082_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ROB_200-082-0", .vendor="Robb",
    .meta=nullptr, .exposes=kExposes_ROB_200_082_0, .exposes_count=sizeof(kExposes_ROB_200_082_0)/sizeof(kExposes_ROB_200_082_0[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ROB_200_082_0, .from_zigbee_count=sizeof(kFz_ROB_200_082_0)/sizeof(kFz_ROB_200_082_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ROB_200_082_0, .bindings_count=sizeof(kBindings_ROB_200_082_0)/sizeof(kBindings_ROB_200_082_0[0]),
    .endpoint_map       = kEndpoints_ROB_200_082_0,
    .endpoint_map_count = sizeof(kEndpoints_ROB_200_082_0)/sizeof(kEndpoints_ROB_200_082_0[0]),
    .endpoint_action_suffix = true,
};

}  // namespace zhc::devices::robb
