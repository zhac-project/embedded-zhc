// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara SP-P05D Power plug H2 EU (RGB) (lumi.plug.aeu005) and SP-P06D
// Outdoor power plug H2 EU (RGB) (lumi.plug.aeu007), new in z2m v26.126.0..v26.133.0.
// Same extend list as SP-P06E (see SP_P06E.cpp) plus the RGB indicator:
// lumiIndicatorLight's on/off (0x0203) as `state_led`, and led_color_mode
// (0x02DF). Before this file both plugs fell through the substring pass onto
// ZNCZ02LM ("lumi.plug").
// Not ported: brightness_led / color_xy_led (lumiIndicatorLight's local
// gamut-clamped tz), led_max_power / led_max_energy and overload_protection
// (float32 writes), identify.
// z2m-source: lumi.ts #SP-P05D, #SP-P06D.
#include "definitions/lumi/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {

constexpr std::uint16_t kLumiManu = 0x115F;

constexpr ::zhc::generic::ZclWriteLookup kChildLock[] = { {"UNLOCK", 0}, {"LOCK", 1} };
constexpr ::zhc::generic::ZclWriteLookup kPowerOnBehavior[] = {
    {"on", 0}, {"previous", 1}, {"off", 2}, {"inverted", 3} };

constexpr ::zhc::generic::ZclWriteLookup kLedColorMode[] = {
    {"fixed", 0}, {"power", 1}, {"daily_energy", 2}, {"monthly_energy", 3} };

constexpr ::zhc::generic::ZclAttrRow kSettingRows[] = {
    { 0x0203, "state_led", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x02DF, "led_color_mode", 1, kLedColorMode, 4 },
    { 0x0285, "child_lock", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x0202, "charging_protection", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x0517, "power_on_behavior", 1, kPowerOnBehavior, 4 },
};
constexpr ::zhc::generic::ZclAttrMap kSettingMap{
    kSettingRows, sizeof(kSettingRows)/sizeof(kSettingRows[0]), kLumiManu };
constexpr FzConverter kFzSettings = ::zhc::generic::zcl_attr_fz("manuSpecificLumi", &kSettingMap);

constexpr ::zhc::generic::ZclWriteSpec kLedSpec{ "state_led", 0x0203, 0x10, kLumiManu, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kLedModeSpec{ "led_color_mode", 0x02DF, 0x20, kLumiManu, kLedColorMode, 4 };
constexpr ::zhc::generic::ZclWriteSpec kChildLockSpec{ "child_lock", 0x0285, 0x20, kLumiManu, kChildLock, 2 };
constexpr ::zhc::generic::ZclWriteSpec kChargingSpec{ "charging_protection", 0x0202, 0x10, kLumiManu, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kPowerOnSpec{ "power_on_behavior", 0x0517, 0x20, kLumiManu, kPowerOnBehavior, 4 };
constexpr TzConverter kTzLed = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kLedSpec);
constexpr TzConverter kTzLedMode = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kLedModeSpec);
constexpr TzConverter kTzChildLock = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kChildLockSpec);
constexpr TzConverter kTzCharging = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kChargingSpec);
constexpr TzConverter kTzPowerOn = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kPowerOnSpec);

const FzConverter* const kFz_SP_P05D[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::lumi::kFzLumiElectricityMeter,
    &::zhc::lumi::kFzLumiHeartbeat,
    &::zhc::lumi::kFzLumiSettings,
    &kFzSettings,
};
const TzConverter* const kTz_SP_P05D[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::lumi::kTzLumiFlipIndicatorLight,
    &kTzLed,
    &kTzLedMode,
    &kTzChildLock,
    &kTzCharging,
    &kTzPowerOn,
};

constexpr const char* kModels_SP_P05D[] = { "lumi.plug.aeu005" };
constexpr const char* kModels_SP_P06D[] = { "lumi.plug.aeu007" };
constexpr const char* kLedModeOpts[] = { "fixed", "power", "daily_energy", "monthly_energy" };
constexpr const char* kPowerOnOpts[] = { "on", "previous", "off", "inverted" };

constexpr Expose kExposes_SP_P05D[] = {
    {"state",  ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power",  ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power_outage_count", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss", kPowerOnOpts, 4,
     ExposeCategory::Config},
    {"state_led", ExposeType::Binary, Access::StateSet, nullptr, "On/off state of the indicator light", nullptr, 0},
    {"led_color_mode", ExposeType::Enum, Access::StateSet, nullptr,
     "Indicator light colour: fixed, or a gradient by current power, daily or monthly energy", kLedModeOpts, 4,
     ExposeCategory::Config},
    {"flip_indicator_light", ExposeType::Binary, Access::StateSet, nullptr,
     "After turn on, the indicator light turns on while switch is off, and vice versa", nullptr, 0,
     ExposeCategory::Config},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, "Disables the physical button", nullptr, 0,
     ExposeCategory::Config},
    {"charging_protection", ExposeType::Binary, Access::StateSet, nullptr,
     "Turn off the plug if the power is below 3W for half an hour", nullptr, 0},
};

constexpr BindingSpec kBindings_SP_P05D[] = {
    {1, 0x0006},
    {1, 0x0B04},
};
// m.onOff and m.electricityMeter power defaults (min 10 s, max MAX, change 5).
constexpr ReportingSpec kReports_SP_P05D[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 3600, 0, 0},
    {1, 0x0B04, 0x050B, 0x29, 10, 65000, 5, 0},
};

}  // namespace

extern const PreparedDefinition kDefSP_P05D{
    .zigbee_models=kModels_SP_P05D,
    .zigbee_models_count=sizeof(kModels_SP_P05D)/sizeof(kModels_SP_P05D[0]),
    .model="SP-P05D", .vendor="Aqara",
    .meta=nullptr,
    .exposes=kExposes_SP_P05D,
    .exposes_count=sizeof(kExposes_SP_P05D)/sizeof(kExposes_SP_P05D[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SP_P05D, .from_zigbee_count=sizeof(kFz_SP_P05D)/sizeof(kFz_SP_P05D[0]),
    .to_zigbee=kTz_SP_P05D, .to_zigbee_count=sizeof(kTz_SP_P05D)/sizeof(kTz_SP_P05D[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SP_P05D,
    .bindings_count=sizeof(kBindings_SP_P05D)/sizeof(kBindings_SP_P05D[0]),
    .reports=kReports_SP_P05D,
    .reports_count=sizeof(kReports_SP_P05D)/sizeof(kReports_SP_P05D[0]),
    .power_source_override = 0x01,   // ZCL Basic 0x0007 = Mains (single phase)
};

extern const PreparedDefinition kDefSP_P06D{
    .zigbee_models=kModels_SP_P06D,
    .zigbee_models_count=sizeof(kModels_SP_P06D)/sizeof(kModels_SP_P06D[0]),
    .model="SP-P06D", .vendor="Aqara",
    .meta=nullptr,
    .exposes=kExposes_SP_P05D,
    .exposes_count=sizeof(kExposes_SP_P05D)/sizeof(kExposes_SP_P05D[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SP_P05D, .from_zigbee_count=sizeof(kFz_SP_P05D)/sizeof(kFz_SP_P05D[0]),
    .to_zigbee=kTz_SP_P05D, .to_zigbee_count=sizeof(kTz_SP_P05D)/sizeof(kTz_SP_P05D[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SP_P05D,
    .bindings_count=sizeof(kBindings_SP_P05D)/sizeof(kBindings_SP_P05D[0]),
    .reports=kReports_SP_P05D,
    .reports_count=sizeof(kReports_SP_P05D)/sizeof(kReports_SP_P05D[0]),
    .power_source_override = 0x01,   // ZCL Basic 0x0007 = Mains (single phase)
};

}  // namespace zhc::devices::lumi
