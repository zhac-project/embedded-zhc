// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS0052 dimmer modules, hand-ported (the generated stubs had no
// light: TS0052 exposed only an `action`, the 2-channel ones battery + on/off).
//   TS0052    zigbeeModel, 1 channel (white label Tuya FS-05R, _TZ3000_mgusv51k)
//   TS0052_2  fingerprint TS0052 + _TZ3000_zjtxnoft / _kvwrdf47 / _sfibawtr,
//             channels l1 = endpoint 1, l2 = endpoint 2, Tuya magic packet
// Both: tuyaLight({powerOnBehavior: true, configureReporting: true,
// switchType: true, minBrightness: "attribute"}) — state, brightness 0-254
// (moveToLevelWithOnOff), min_brightness (genLevelCtrl 0xFC00), effect,
// do_not_disturb, power_on_behavior + switch_type (manuSpecificTuya3 0xE001),
// Tuya's 0xF000 brightness reports; genOnOff + genLevelCtrl bound and reported.
// TS0052_2 has one do_not_disturb and one switch_type, sent to endpoint 1;
// switch_type is write-only there: z2m publishes its report without an
// endpoint, which the dispatcher would file under switch_type_l1.
// Not ported: brightness_move / brightness_step and level_config (no z2m
// expose), power source "when unknown".
// z2m-source: tuya.ts #TS0052 / #TS0052_2 (v26.115.1).
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"   // kReportsDimmer_1ep, tuyaLight extras
namespace zhc::devices::tuya {
namespace {
const FzConverter* const kFzGen_TS0052[] = {
    &::zhc::generic::kFzOnOff, &::zhc::generic::kFzBrightness,
    &::zhc::tuya::kFzTuyaBrightness, &::zhc::tuya::kFzTuyaMinBrightness,
    &::zhc::tuya::kFzTuyaPowerOnBehavior2, &::zhc::tuya::kFzTuyaSwitchType,
};
const FzConverter* const kFzGen_TS0052_2[] = {
    &::zhc::generic::kFzOnOff, &::zhc::generic::kFzBrightness,
    &::zhc::tuya::kFzTuyaBrightness, &::zhc::tuya::kFzTuyaMinBrightness,
    &::zhc::tuya::kFzTuyaPowerOnBehavior2,
};
const TzConverter* const kTzGen_TS0052[] = {
    &::zhc::generic::kTzOnOff, &::zhc::generic::kTzBrightness, &::zhc::generic::kTzEffect,
    &::zhc::tuya::kTzTuyaMinBrightness, &::zhc::tuya::kTzTuyaDoNotDisturb,
    &::zhc::tuya::kTzTuyaPowerOnBehavior2, &::zhc::tuya::kTzTuyaSwitchType,
};
constexpr const char* kModelsGen_TS0052[] = { "TS0052" };
constexpr const char* kManusGen_TS0052_2[] = {
    "_TZ3000_zjtxnoft", "_TZ3000_kvwrdf47", "_TZ3000_sfibawtr" };

constexpr const char* kEffects[] = {
    "blink", "breathe", "okay", "channel_change", "finish_effect", "stop_effect" };
constexpr const char* kPowerOn[] = { "off", "previous", "on" };
constexpr const char* kSwitchType[] = { "toggle", "state", "momentary" };

// brightness carries z2m's 0-254 range: the hub's name-based default does not
// cover the _l1 / _l2 names. do_not_disturb is a command the light never
// echoes, so Set, as TS0505B.
constexpr Expose kExposesGen_TS0052[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, "Brightness of this light",
     nullptr, 0, ExposeCategory::State, 0, 254, 1},
    {"min_brightness", ExposeType::Numeric, Access::StateSet, nullptr, "Minimum light brightness",
     nullptr, 0, ExposeCategory::Config, 1, 255, 1},
    {"effect", ExposeType::Enum, Access::Set, nullptr, "Triggers an effect on the light",
     kEffects, 6},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss", kPowerOn, 3,
     ExposeCategory::Config},
    {"do_not_disturb", ExposeType::Binary, Access::Set, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Config},
    {"switch_type", ExposeType::Enum, Access::StateSet, nullptr, "Type of the switch",
     kSwitchType, 3, ExposeCategory::Config},
};
constexpr Expose kExposesGen_TS0052_2[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, "Brightness of this light",
     nullptr, 0, ExposeCategory::State, 0, 254, 1},
    {"min_brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, "Minimum light brightness",
     nullptr, 0, ExposeCategory::Config, 1, 255, 1},
    {"effect_l1", ExposeType::Enum, Access::Set, nullptr, "Triggers an effect on the light",
     kEffects, 6},
    {"power_on_behavior_l1", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss", kPowerOn, 3,
     ExposeCategory::Config},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l2", ExposeType::Numeric, Access::StateSet, nullptr, "Brightness of this light",
     nullptr, 0, ExposeCategory::State, 0, 254, 1},
    {"min_brightness_l2", ExposeType::Numeric, Access::StateSet, nullptr, "Minimum light brightness",
     nullptr, 0, ExposeCategory::Config, 1, 255, 1},
    {"effect_l2", ExposeType::Enum, Access::Set, nullptr, "Triggers an effect on the light",
     kEffects, 6},
    {"power_on_behavior_l2", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss", kPowerOn, 3,
     ExposeCategory::Config},
    {"do_not_disturb", ExposeType::Binary, Access::Set, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Config},
    {"switch_type", ExposeType::Enum, Access::Set, nullptr, "Type of the switch",
     kSwitchType, 3, ExposeCategory::Config},
};

constexpr BindingSpec kBindingsGen_TS0052[] = {
    {1, 0x0006},
    {1, 0x0008},
};
constexpr BindingSpec kBindingsGen_TS0052_2[] = {
    {1, 0x0006}, {1, 0x0008},
    {2, 0x0006}, {2, 0x0008},
};
// kReportsDimmer_1ep on both channels.
constexpr ReportingSpec kReportsGen_TS0052_2[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 3600, 0, 0},
    {1, 0x0008, 0x0000, 0x20, 1, 3600, 1, 0},
    {2, 0x0006, 0x0000, 0x10, 0, 3600, 0, 0},
    {2, 0x0008, 0x0000, 0x20, 1, 3600, 1, 0},
};
constexpr ::zhc::EndpointLabel kEndpointsGen_TS0052_2[] = { {"l1", 1}, {"l2", 2} };

constexpr WhiteLabel kWhiteLabels_Gen_TS0052[] = {
    {"Tuya","FS-05R"},
};
}  // namespace

extern const PreparedDefinition kDefGen_TS0052{
    .zigbee_models=kModelsGen_TS0052,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,
    .manufacturer_names_count=0,
    .model="TS0052",.vendor="Tuya",
    .meta=nullptr,.exposes=kExposesGen_TS0052,.exposes_count=sizeof(kExposesGen_TS0052)/sizeof(kExposesGen_TS0052[0]),
    .white_labels=kWhiteLabels_Gen_TS0052, .white_labels_count=sizeof(kWhiteLabels_Gen_TS0052)/sizeof(kWhiteLabels_Gen_TS0052[0]),
    .from_zigbee=kFzGen_TS0052,.from_zigbee_count=sizeof(kFzGen_TS0052)/sizeof(kFzGen_TS0052[0]),
    .to_zigbee=kTzGen_TS0052,.to_zigbee_count=sizeof(kTzGen_TS0052)/sizeof(kTzGen_TS0052[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBindingsGen_TS0052,.bindings_count=sizeof(kBindingsGen_TS0052)/sizeof(kBindingsGen_TS0052[0]),
    .reports=::zhc::tuya::kReportsDimmer_1ep,
    .reports_count=::zhc::tuya::kReportsDimmer_1ep_count,
};

extern const PreparedDefinition kDefGen_TS0052_2{
    .zigbee_models=kModelsGen_TS0052,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,.manufacturer_names=kManusGen_TS0052_2,
    .manufacturer_names_count=sizeof(kManusGen_TS0052_2)/sizeof(kManusGen_TS0052_2[0]),
    .model="TS0052_2",.vendor="Tuya",
    .meta=nullptr,.exposes=kExposesGen_TS0052_2,.exposes_count=sizeof(kExposesGen_TS0052_2)/sizeof(kExposesGen_TS0052_2[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFzGen_TS0052_2,.from_zigbee_count=sizeof(kFzGen_TS0052_2)/sizeof(kFzGen_TS0052_2[0]),
    .to_zigbee=kTzGen_TS0052,.to_zigbee_count=sizeof(kTzGen_TS0052)/sizeof(kTzGen_TS0052[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBindingsGen_TS0052_2,.bindings_count=sizeof(kBindingsGen_TS0052_2)/sizeof(kBindingsGen_TS0052_2[0]),
    .reports=kReportsGen_TS0052_2,.reports_count=sizeof(kReportsGen_TS0052_2)/sizeof(kReportsGen_TS0052_2[0]),
    .config_steps=::zhc::tuya::kConfigStepsTuyaMagicPacket,
    .config_steps_count=::zhc::tuya::kConfigStepsTuyaMagicPacketCount,
    .endpoint_map=kEndpointsGen_TS0052_2,
    .endpoint_map_count=sizeof(kEndpointsGen_TS0052_2)/sizeof(kEndpointsGen_TS0052_2[0]),
};
}
