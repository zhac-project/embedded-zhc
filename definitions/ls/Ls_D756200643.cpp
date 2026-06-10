// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ls 756200643 — graduated from generated/ (LS parity, 2026-06-10).
// Zigbee remote (SEMOTE)
// z2m-source: ls.ts #756200643.
//
// z2m extend chain:
//   m.battery()
//   lsModernExtend.groupIdExpose()       — numeric `action_group`
//   lsModernExtend.commandsOnOffDouble() — fz command_on_double / command_off_double
//   m.commandsOnOff({commands: ["on", "off"]})
//   m.commandsLevelCtrl({commands: ["brightness_step_up", "brightness_step_down", "brightness_stop"]})
//   m.commandsColorCtrl({commands: ["color_temperature_step_up", "color_temperature_step_down", "stop_move_step"]})
//
// Action surface (from those filters): on, off, on_double, off_double,
// brightness_step_up, brightness_step_down, brightness_stop,
// color_temperature_step_up, color_temperature_step_down, stop_move_step.
//
// Coverage:
//   on / off                              — kFzCommandOn / kFzCommandOff (genOnOff cmd 0x01/0x00)
//   on_double / off_double                — local kFzLsCommandOnDouble (cmd 0x07) /
//                                           kFzLsCommandOffDouble (cmd 0x40)
//   brightness_step_up/_down              — kFzCommandStep (genLevelCtrl cmd 0x02)
//   brightness_stop                       — kFzCommandStop (genLevelCtrl cmd 0x03)
//   color_temperature_step_up/_down       — kFzCommandStepColorTemp (lightingColorCtrl cmd 0x4C)
//   stop_move_step                        — local kFzLsCommandStopMoveStep (lightingColorCtrl cmd 0x47)
//   battery / voltage                     — kFzBattery (genPowerCfg attr 0x0020/0x0021)
//
// `action_group` (the bind group id from the inbound APS frame) is
// declared as an expose for shape parity with z2m, but ZHC's frame
// dispatcher does not currently surface the group id, so the value
// will not populate at runtime — see LS_PARITY.md "Open runtime gaps".
//
// As a Zigbee end-device remote, SEMOTE has no Tz: every interaction
// is an outbound cluster command FROM the device. The original port
// included kTzOnOff which would have routed an inbound `state` write
// to a non-existent on/off attribute on the remote — it is dropped.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ls/_shared.hpp"

namespace zhc::devices::ls {
namespace {
const FzConverter* const kFz_D756200643[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::ls::kFzLsCommandOnDouble,
    &::zhc::ls::kFzLsCommandOffDouble,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::ls::kFzLsCommandStopMoveStep,
};
constexpr const char* kModels_D756200643[] = { "SEMOTE" };

constexpr const char* kActionEnum_D756200643[] = {
    "on", "off", "on_double", "off_double",
    "brightness_step_up", "brightness_step_down", "brightness_stop",
    "color_temperature_step_up", "color_temperature_step_down",
    "stop_move_step",
};

constexpr Expose kExposes_D756200643[] = {
    {"battery",      ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",      ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"action",       ExposeType::Enum,    Access::State, nullptr, nullptr,
        kActionEnum_D756200643,
        sizeof(kActionEnum_D756200643)/sizeof(kActionEnum_D756200643[0])},
    {"action_group", ExposeType::Numeric, Access::State, nullptr,
        "Group where the action was triggered on", nullptr, 0},
};

// Output bindings — z2m setupConfigureForBinding("genOnOff", "output")
// plus implicit bindings from m.commandsLevelCtrl / m.commandsColorCtrl.
// The matching genPowerCfg bind keeps battery reports flowing.
constexpr BindingSpec kBind_D756200643[] = {
    {1, 0x0001},   // genPowerCfg (battery)
    {1, 0x0006},   // genOnOff (output → coordinator)
    {1, 0x0008},   // genLevelCtrl (output → coordinator)
    {1, 0x0300},   // lightingColorCtrl (output → coordinator)
};

}  // namespace

extern const PreparedDefinition kDef_D756200643{
    .zigbee_models=kModels_D756200643, .zigbee_models_count=sizeof(kModels_D756200643)/sizeof(kModels_D756200643[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="756200643", .vendor="Ls",
    .meta=nullptr, .exposes=kExposes_D756200643, .exposes_count=sizeof(kExposes_D756200643)/sizeof(kExposes_D756200643[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D756200643, .from_zigbee_count=sizeof(kFz_D756200643)/sizeof(kFz_D756200643[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_D756200643, .bindings_count=sizeof(kBind_D756200643)/sizeof(kBind_D756200643[0]),
};

}  // namespace zhc::devices::ls
