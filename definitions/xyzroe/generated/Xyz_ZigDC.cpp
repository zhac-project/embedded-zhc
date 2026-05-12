// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Xyzroe ZigDC — hand-rewritten (was wrong-bundle: state/power/voltage/current only).
// ZigDC — INA3221 dual-rail multi-channel DC monitor (8 endpoints).
// z2m-source: xyzroe.ts #ZigDC.
//
// Endpoints (l1=1, l2=2, l3=3, l5=5, l6=6, l7=7, l8=8):
//   l1 (1): BTN button config + action + uptime + interval (top-level)
//   l2 (2): cpu_temperature
//   l3 (3): INA3221 chip 0x41 (ch1..ch3)  — current/voltage/power
//   l5 (5): INA3221 chip 0x44 (ch4..ch6)  — current/voltage/power
//   l6 (6): temperature + humidity
//   l7 (7): IN1 button config (genOnOffSwitchCfg)
//   l8 (8): IN2 button config (genOnOffSwitchCfg)
//
// Channel readings come through fzLocal.ZigDC_ina3221 (custom; not in
// generic lib) — only the static expose / binding layout is portable.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::xyzroe {
namespace {
const FzConverter* const kFz_ZigDC[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
// No state-set path on ZigDC; to_zigbee is null on the PreparedDefinition.
constexpr const char* kModels_ZigDC[] = { "ZigDC" };

constexpr const char* kButtonModes[]  = { "input", "output", "toggle" };
constexpr const char* kLinkToOutput[] = { "no", "same", "inverted" };
constexpr const char* kBindCommand[]  = { "on", "off", "toggle" };
constexpr const char* kActionVals[]   = { "single", "double", "triple", "hold", "release" };

constexpr Expose kExp_ZigDC[] = {
    // INA3221 ch1..ch6 (chip 0x41 → ch1-3 on ep3, chip 0x44 → ch4-6 on ep5)
    {"current_ch1", ExposeType::Numeric, ::zhc::Access::State, "A",  nullptr, nullptr, 0},
    {"voltage_ch1", ExposeType::Numeric, ::zhc::Access::State, "V",  nullptr, nullptr, 0},
    {"power_ch1",   ExposeType::Numeric, ::zhc::Access::State, "W",  nullptr, nullptr, 0},
    {"current_ch2", ExposeType::Numeric, ::zhc::Access::State, "A",  nullptr, nullptr, 0},
    {"voltage_ch2", ExposeType::Numeric, ::zhc::Access::State, "V",  nullptr, nullptr, 0},
    {"power_ch2",   ExposeType::Numeric, ::zhc::Access::State, "W",  nullptr, nullptr, 0},
    {"current_ch3", ExposeType::Numeric, ::zhc::Access::State, "A",  nullptr, nullptr, 0},
    {"voltage_ch3", ExposeType::Numeric, ::zhc::Access::State, "V",  nullptr, nullptr, 0},
    {"power_ch3",   ExposeType::Numeric, ::zhc::Access::State, "W",  nullptr, nullptr, 0},
    {"current_ch4", ExposeType::Numeric, ::zhc::Access::State, "A",  nullptr, nullptr, 0},
    {"voltage_ch4", ExposeType::Numeric, ::zhc::Access::State, "V",  nullptr, nullptr, 0},
    {"power_ch4",   ExposeType::Numeric, ::zhc::Access::State, "W",  nullptr, nullptr, 0},
    {"current_ch5", ExposeType::Numeric, ::zhc::Access::State, "A",  nullptr, nullptr, 0},
    {"voltage_ch5", ExposeType::Numeric, ::zhc::Access::State, "V",  nullptr, nullptr, 0},
    {"power_ch5",   ExposeType::Numeric, ::zhc::Access::State, "W",  nullptr, nullptr, 0},
    {"current_ch6", ExposeType::Numeric, ::zhc::Access::State, "A",  nullptr, nullptr, 0},
    {"voltage_ch6", ExposeType::Numeric, ::zhc::Access::State, "V",  nullptr, nullptr, 0},
    {"power_ch6",   ExposeType::Numeric, ::zhc::Access::State, "W",  nullptr, nullptr, 0},
    // ep6 — environment
    {"temperature_l6", ExposeType::Numeric, ::zhc::Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"humidity_l6",    ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0},
    // ep2 — cpu_temperature
    {"temperature_l2", ExposeType::Numeric, ::zhc::Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    // ptvo multistate action (top-level)
    {"action",         ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, kActionVals, 5},
    // BTN/IN1/IN2 button config (l1, l7, l8)
    {"button_mode_l1",   ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kButtonModes,  3},
    {"link_to_outpu_l1", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kLinkToOutput, 3},
    {"bind_command_l1",  ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kBindCommand,  3},
    {"button_mode_l7",   ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kButtonModes,  3},
    {"link_to_outpu_l7", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kLinkToOutput, 3},
    {"bind_command_l7",  ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kBindCommand,  3},
    {"button_mode_l8",   ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kButtonModes,  3},
    {"link_to_outpu_l8", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kLinkToOutput, 3},
    {"bind_command_l8",  ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kBindCommand,  3},
    // top-level diagnostics & config
    {"uptime",   ExposeType::Numeric, ::zhc::Access::State, "s", nullptr, nullptr, 0},
    {"interval", ExposeType::Numeric, ::zhc::Access::Set,   "s", nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_ZigDC[] = {
    {1, 0x0007},  // genOnOffSwitchCfg (BTN config)
    {1, 0x0012},  // genMultistateInput (ptvo action)
    {1, 0x000C},  // genAnalogInput (uptime)
    {2, 0x0402},  // msTemperatureMeasurement (cpu_temperature)
    {3, 0x000C},  // genAnalogInput (INA3221 chip 0x41)
    {5, 0x000C},  // genAnalogInput (INA3221 chip 0x44)
    {6, 0x0402},  // msTemperatureMeasurement
    {6, 0x0405},  // msRelativeHumidity
    {7, 0x0007},  // genOnOffSwitchCfg (IN1)
    {8, 0x0007},  // genOnOffSwitchCfg (IN2)
};
}  // namespace

extern const PreparedDefinition kDef_ZigDC{
    .zigbee_models=kModels_ZigDC, .zigbee_models_count=sizeof(kModels_ZigDC)/sizeof(kModels_ZigDC[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZigDC", .vendor="Xyzroe",
    .meta=nullptr, .exposes=kExp_ZigDC, .exposes_count=sizeof(kExp_ZigDC)/sizeof(kExp_ZigDC[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZigDC, .from_zigbee_count=sizeof(kFz_ZigDC)/sizeof(kFz_ZigDC[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_ZigDC, .bindings_count=sizeof(kBind_ZigDC)/sizeof(kBind_ZigDC[0]),
};

}  // namespace zhc::devices::xyzroe
