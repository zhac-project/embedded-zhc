// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Xyzroe ZigUSB — hand-rewritten (was wrong-bundle: state/power/voltage/current only).
// Zigbee USB power monitor and switch (CC2530, 4 endpoints).
// z2m-source: xyzroe.ts #ZigUSB.
//
// Endpoints (l1=1, l2=2, l4=4, l5=5):
//   l1 (1): on/off switch (inverted) + restart timer + button config + action
//   l2 (2): current / voltage / power (genAnalogInput-encoded) + interval
//   l4 (4): cpu_temperature (msTemperatureMeasurement / fz.temperature)
//   l5 (5): uptime (genAnalogInput on ep5)
//
// Custom converters (zigusb_on_off_invert / zigusb_analog_input /
// ptvo_multistate_action / zigusb_button_config) are not part of the
// generic library; only the static expose / binding layout is portable.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::xyzroe {
namespace {
const FzConverter* const kFz_ZigUSB[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzTemperature,
};
const TzConverter* const kTz_ZigUSB[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_ZigUSB[] = { "ZigUSB" };

constexpr const char* kButtonModes[]  = { "input", "output", "toggle" };
constexpr const char* kLinkToOutput[] = { "no", "same", "inverted" };
constexpr const char* kBindCommand[]  = { "on", "off", "toggle" };
constexpr const char* kActionVals[]   = { "single", "double", "triple" };

constexpr Expose kExp_ZigUSB[] = {
    // ep1 — switch + restart + button config + action
    {"state_l1",         ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"restart_l1",       ExposeType::Numeric, ::zhc::Access::Set,      "s",     nullptr, nullptr, 0},
    {"button_mode_l1",   ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, kButtonModes,  3},
    {"link_to_outpu_l1", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, kLinkToOutput, 3},
    {"bind_command_l1",  ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, kBindCommand,  3},
    {"action",           ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, kActionVals,   3},
    // ep2 — analog inputs (encoded as genAnalogInput.presentValue + description)
    {"current_l2",       ExposeType::Numeric, ::zhc::Access::State,    "A",     nullptr, nullptr, 0},
    {"voltage_l2",       ExposeType::Numeric, ::zhc::Access::State,    "V",     nullptr, nullptr, 0},
    {"power_l2",         ExposeType::Numeric, ::zhc::Access::State,    "W",     nullptr, nullptr, 0},
    {"interval_l2",      ExposeType::Numeric, ::zhc::Access::Set,      "s",     nullptr, nullptr, 0},
    // ep4 — cpu_temperature (mapped from msTemperatureMeasurement)
    {"temperature_l4",   ExposeType::Numeric, ::zhc::Access::State,    "\xc2\xb0""C", nullptr, nullptr, 0},
    // ep5 — uptime (genAnalogInput on ep5)
    {"uptime_l5",        ExposeType::Numeric, ::zhc::Access::State,    "s",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_ZigUSB[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0007},  // genOnOffSwitchCfg (button config)
    {1, 0x0012},  // genMultistateInput (ptvo action)
    {2, 0x000C},  // genAnalogInput (current/voltage/power, ptvo-encoded)
    {4, 0x0402},  // msTemperatureMeasurement (cpu_temperature)
    {5, 0x000C},  // genAnalogInput (uptime)
};
}  // namespace

extern const PreparedDefinition kDef_ZigUSB{
    .zigbee_models=kModels_ZigUSB, .zigbee_models_count=sizeof(kModels_ZigUSB)/sizeof(kModels_ZigUSB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZigUSB", .vendor="Xyzroe",
    .meta=nullptr, .exposes=kExp_ZigUSB, .exposes_count=sizeof(kExp_ZigUSB)/sizeof(kExp_ZigUSB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZigUSB, .from_zigbee_count=sizeof(kFz_ZigUSB)/sizeof(kFz_ZigUSB[0]),
    .to_zigbee=kTz_ZigUSB, .to_zigbee_count=sizeof(kTz_ZigUSB)/sizeof(kTz_ZigUSB[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_ZigUSB, .bindings_count=sizeof(kBind_ZigUSB)/sizeof(kBind_ZigUSB[0]),
};

}  // namespace zhc::devices::xyzroe
