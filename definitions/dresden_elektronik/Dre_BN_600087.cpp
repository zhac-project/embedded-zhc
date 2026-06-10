// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: DresdenElektronik BN-600087 (Lighting Switch) — hand-edited 2026-06-10.
// 2-part Zigbee-powered light switch (battery-powered remote, 2 endpoints).
// z2m: m.deviceEndpoints({1:1, 2:2}) +
//      m.commandsOnOff(["1","2"]) + m.commandsLevelCtrl(["1","2"])
//      + m.commandsColorCtrl(["1","2"]) + m.battery().
//
// z2m's commandsOnOff/LevelCtrl/ColorCtrl with endpointNames postfix the
// action per button (on_1 / on_2 / brightness_step_up_2 …) so the two
// rockers keep identity. `action` is a kAlwaysGlobalKey, so the endpoint_map
// alone is NOT enough — without endpoint_action_suffix the dispatcher leaves
// `action` bare and both buttons collapse onto one key (a 2-button remote
// looks like 1 button). Set endpoint_action_suffix so the dispatcher rewrites
// the key to action_<label> (action_1 / action_2). No m.commandsScenes()/Tz here.
// z2m-source: dresden_elektronik.ts #BN-600087.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::dresden_elektronik {
namespace {
const FzConverter* const kFz_BN_600087[] = {
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
constexpr const char* kModels_BN_600087[] = { "Lighting Switch" };

constexpr ::zhc::EndpointLabel kEndpoints_BN_600087[] = { {"1", 1}, {"2", 2} };

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_BN_600087{
    .zigbee_models=kModels_BN_600087, .zigbee_models_count=sizeof(kModels_BN_600087)/sizeof(kModels_BN_600087[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BN-600087", .vendor="DresdenElektronik",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_BN_600087, .from_zigbee_count=sizeof(kFz_BN_600087)/sizeof(kFz_BN_600087[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_BN_600087,
    .endpoint_map_count = sizeof(kEndpoints_BN_600087)/sizeof(kEndpoints_BN_600087[0]),
    .endpoint_action_suffix = true,  // action → action_1 / action_2 (per-button identity)
};

}  // namespace zhc::devices::dresden_elektronik
