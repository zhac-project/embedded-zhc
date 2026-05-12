// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy MULTI-ZIG-SW — auto-generated.
// Multi switch from Smarthjemmet.dk
// z2m-source: custom_devices_diy.ts #MULTI-ZIG-SW.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: fromZigbee=[fzLocal.multi_zig_sw_switch_buttons (genMultistateInput action),
// fzLocal.multi_zig_sw_battery (custom voltage->% formula),
// fzLocal.multi_zig_sw_switch_config (genOnOffSwitchCfg switchType per channel)],
// toZigbee=[tzLocal.multi_zig_sw_switch_type]; multiEndpoint button_1..button_4 on EP 2..5.
// exposes: switch_type_1..4 enum, action ["single","double","triple","hold","release"],
// battery, battery_voltage.
// TODO(parity): per-EP genMultistateInput action + per-EP switch_type_X enum read/write
// require custom fz/tz converters not in shared helpers. Generic kFzOnOff/kTzOnOff give
// nothing useful here (device exposes no on/off cluster).
const FzConverter* const kFz_MULTI_ZIG_SW[] = {
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_MULTI_ZIG_SW[] = { "MULTI-ZIG-SW" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {2, 0x0007}, {3, 0x0007}, {4, 0x0007}, {5, 0x0007},  // genOnOffSwitchCfg
    {2, 0x0012}, {3, 0x0012}, {4, 0x0012}, {5, 0x0012},  // genMultistateInput
};

extern const PreparedDefinition kDef_MULTI_ZIG_SW{
    .zigbee_models=kModels_MULTI_ZIG_SW, .zigbee_models_count=sizeof(kModels_MULTI_ZIG_SW)/sizeof(kModels_MULTI_ZIG_SW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MULTI-ZIG-SW", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MULTI_ZIG_SW, .from_zigbee_count=sizeof(kFz_MULTI_ZIG_SW)/sizeof(kFz_MULTI_ZIG_SW[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::custom_devices_diy
