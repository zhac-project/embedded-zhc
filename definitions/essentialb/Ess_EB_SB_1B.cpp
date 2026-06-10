// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Essentielb EB-SB-1B — battery remote (smart button), not a switch.
//
// z2m models it as a pure command transmitter:
//   fromZigbee: [fz.battery, fz.command_on, fz.command_off, fz.command_step,
//                fz.command_stop, fz.command_step_color_temperature]
//   exposes: [e.battery(), e.action(["on","off",
//             "color_temperature_step_up","color_temperature_step_down",
//             "brightness_step_up","brightness_step_down","brightness_stop"])]
//   configure binds genBasic, genOnOff, genPowerCfg, lightingColorCtrl,
//             genLevelCtrl + reports batteryPercentageRemaining.
//
// The auto-generated port wired kFzOnOff + an on/off TZ (both dead — the
// remote sends genOnOff *commands*, never attribute reports, and has no
// local relay to control) and exposed a meaningless controllable `state`.
// The button stream (the device's entire purpose) and the battery report
// were the only live signals, and the action surface was dropped entirely.
//
// Re-wired to the generic command-action decoders so the buttons surface as
// `action`: kFzCommandOn/Off → on/off, kFzCommandStep → brightness_step_up/
// down, kFzCommandStop → brightness_stop, kFzCommandStepColorTemp →
// color_temperature_step_up/down. Battery kept via kFzBattery. No TZ.
//
// z2m-source: essentialb.ts #EB-SB-1B.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::essentialb {
namespace {
const FzConverter* const kFz_EB_SB_1B[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStepColorTemp,
};
constexpr const char* kModels_EB_SB_1B[] = { "EB-SB-1B" };

// z2m e.action([...]) — the seven button events this remote emits.
constexpr const char* kActions_EB_SB_1B[] = {
    "on", "off",
    "color_temperature_step_up", "color_temperature_step_down",
    "brightness_step_up", "brightness_step_down",
    "brightness_stop",
};

}  // namespace


// Hand-curated battery-remote surface: battery + action (no controllable
// state). Bindings mirror z2m's configure() binds.
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
        kActions_EB_SB_1B, sizeof(kActions_EB_SB_1B)/sizeof(kActions_EB_SB_1B[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0000},  // genBasic
    {1, 0x0006},  // genOnOff       (command_on/off)
    {1, 0x0001},  // genPowerCfg    (battery)
    {1, 0x0300},  // lightingColorCtrl (command_step_color_temperature)
    {1, 0x0008},  // genLevelCtrl   (command_step/stop)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_EB_SB_1B{
    .zigbee_models=kModels_EB_SB_1B, .zigbee_models_count=sizeof(kModels_EB_SB_1B)/sizeof(kModels_EB_SB_1B[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EB-SB-1B", .vendor="Essentialb",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EB_SB_1B, .from_zigbee_count=sizeof(kFz_EB_SB_1B)/sizeof(kFz_EB_SB_1B[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::essentialb
