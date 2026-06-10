// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Halemeier HA-ZSM-MW2 — hand-rewritten command-style remote.
// S-Mitter MultiWhite2 smart remote control.
// z2m: fromZigbee=[battery, command_step, command_step_color_temperature,
//                  command_recall, command_off, command_on],
// toZigbee=[battery_percentage_remaining], exposes=[action_group, battery,
// action(recall_*/on/off/color_temperature_step_*/brightness_step_*)].
// The auto-generator emitted kFzOnOff + kTzOnOff (a controllable on/off
// switch with a `state` attribute), but this is a battery REMOTE: it emits
// genOnOff / genLevelCtrl / lightingColorCtrl / genScenes cluster commands,
// it has no on/off state. Fix → command_* converters + `action` expose.
// z2m-source: halemeier.ts #HA-ZSM-MW2.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::halemeier {
namespace {
const FzConverter* const kFz_HA_ZSM_MW2[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzBattery,
};

constexpr Expose kExposes_HA_ZSM_MW2[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_HA_ZSM_MW2[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (step color temp)
    {1, 0x0005},  // genScenes (recall)
    {1, 0x0001},  // genPowerCfg (battery)
};

constexpr const char* kModels_HA_ZSM_MW2[] = { "HA-ZSM-MW2" };

}  // namespace

extern const PreparedDefinition kDef_HA_ZSM_MW2{
    .zigbee_models=kModels_HA_ZSM_MW2, .zigbee_models_count=sizeof(kModels_HA_ZSM_MW2)/sizeof(kModels_HA_ZSM_MW2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HA-ZSM-MW2", .vendor="Halemeier",
    .meta=nullptr,
    .exposes=kExposes_HA_ZSM_MW2, .exposes_count=sizeof(kExposes_HA_ZSM_MW2)/sizeof(kExposes_HA_ZSM_MW2[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HA_ZSM_MW2, .from_zigbee_count=sizeof(kFz_HA_ZSM_MW2)/sizeof(kFz_HA_ZSM_MW2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_HA_ZSM_MW2, .bindings_count=sizeof(kBindings_HA_ZSM_MW2)/sizeof(kBindings_HA_ZSM_MW2[0]),
};

}  // namespace zhc::devices::halemeier
