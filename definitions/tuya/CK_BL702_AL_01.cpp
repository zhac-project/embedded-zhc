// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya CK-BL702-AL-01(7008_Z102LG01-1) LED bulb, graduated from the
// generated stub (which wired Tuya on/off actions onto a ZCL light).
// z2m v26.115.1 adds zigbeeModel CK-BL702-AL-02(7008)-1. z2m: tuyaLight
// ({colorTemp: [142, 500], color}) + fz/tz.power_on_behavior; meta
// moveToLevelWithOnOffDisable -> kTzBrightnessMoveToLevel.
// z2m-source: tuya.ts #CK-BL702-AL-01(7008_Z102LG01-1).
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightnessMoveToLevel,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kOpts7[] = { "off", "on", "toggle", "previous" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 142, 500, 1},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, "Controls the behavior when the device is powered on after power loss", kOpts7, 4, ExposeCategory::Config},
};
constexpr const char* kM[] = { "CK-BL702-AL-01(7008_Z102LG01-1)", "CK-BL702-AL-02(7008)-1" };
constexpr BindingSpec kBind[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};
constexpr ConfigStep kSteps[] = {
    { ConfigStepOp::Read, 1, 0x0000, 0x00, 0, ::zhc::tuya::kTuyaMagicPacketAttrs, sizeof(::zhc::tuya::kTuyaMagicPacketAttrs), 0 },
};
}  // namespace

extern const PreparedDefinition kDef_CK_BL702_AL_01_Z102{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CK-BL702-AL-01(7008_Z102LG01-1)", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .config_steps=kSteps, .config_steps_count=sizeof(kSteps)/sizeof(kSteps[0]),
};

}  // namespace zhc::devices::tuya
