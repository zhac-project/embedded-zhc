// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara SP-P06E — Outdoor plug H2 EU (lumi.plug.aeu006), new in z2m v26.125.0.
// lumiOnOff with the power_on_behavior enum (0xFCC0 0x0517) and no device
// temperature expose, activePower in W (divisor 1), energy / current from the
// 0x00F7 heartbeat (lumiElectricityMeter without voltage), and on
// manuSpecificLumi: led_indicator (0x0203), flip_indicator_light (0x00F0),
// child_lock (0x0285, LOCK 1) and charging_protection (0x0202). Binary settings
// read back as Bool, as ZHAC exposes take them.
// Not ported: overload_protection (a float32 write) and identify.
// z2m-source: lumi.ts #SP-P06E.
#include "definitions/lumi/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {

constexpr std::uint16_t kLumiManu = 0x115F;

constexpr ::zhc::generic::ZclWriteLookup kChildLock[] = { {"UNLOCK", 0}, {"LOCK", 1} };
constexpr ::zhc::generic::ZclWriteLookup kPowerOnBehavior[] = {
    {"on", 0}, {"previous", 1}, {"off", 2}, {"inverted", 3} };

constexpr ::zhc::generic::ZclAttrRow kSettingRows[] = {
    { 0x0203, "led_indicator", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x0285, "child_lock", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x0202, "charging_protection", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x0517, "power_on_behavior", 1, kPowerOnBehavior, 4 },
};
constexpr ::zhc::generic::ZclAttrMap kSettingMap{
    kSettingRows, sizeof(kSettingRows)/sizeof(kSettingRows[0]), kLumiManu };
constexpr FzConverter kFzSettings = ::zhc::generic::zcl_attr_fz("manuSpecificLumi", &kSettingMap);

constexpr ::zhc::generic::ZclWriteSpec kLedSpec{ "led_indicator", 0x0203, 0x10, kLumiManu, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kChildLockSpec{ "child_lock", 0x0285, 0x20, kLumiManu, kChildLock, 2 };
constexpr ::zhc::generic::ZclWriteSpec kChargingSpec{ "charging_protection", 0x0202, 0x10, kLumiManu, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kPowerOnSpec{ "power_on_behavior", 0x0517, 0x20, kLumiManu, kPowerOnBehavior, 4 };
constexpr TzConverter kTzLed = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kLedSpec);
constexpr TzConverter kTzChildLock = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kChildLockSpec);
constexpr TzConverter kTzCharging = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kChargingSpec);
constexpr TzConverter kTzPowerOn = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kPowerOnSpec);

const FzConverter* const kFz_SP_P06E[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::lumi::kFzLumiElectricityMeter,
    &::zhc::lumi::kFzLumiHeartbeat,
    &::zhc::lumi::kFzLumiSettings,
    &kFzSettings,
};
const TzConverter* const kTz_SP_P06E[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::lumi::kTzLumiFlipIndicatorLight,
    &kTzLed,
    &kTzChildLock,
    &kTzCharging,
    &kTzPowerOn,
};

constexpr const char* kModels_SP_P06E[] = { "lumi.plug.aeu006" };
constexpr const char* kPowerOnOpts[] = { "on", "previous", "off", "inverted" };

constexpr Expose kExposes_SP_P06E[] = {
    {"state",  ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power",  ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power_outage_count", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss", kPowerOnOpts, 4,
     ExposeCategory::Config},
    {"led_indicator", ExposeType::Binary, Access::StateSet, nullptr, "LED indicator", nullptr, 0,
     ExposeCategory::Config},
    {"flip_indicator_light", ExposeType::Binary, Access::StateSet, nullptr,
     "After turn on, the indicator light turns on while switch is off, and vice versa", nullptr, 0,
     ExposeCategory::Config},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, "Disables the physical button", nullptr, 0,
     ExposeCategory::Config},
    {"charging_protection", ExposeType::Binary, Access::StateSet, nullptr,
     "Turn off the plug if the power is below 3W for half an hour", nullptr, 0},
};

constexpr BindingSpec kBindings_SP_P06E[] = {
    {1, 0x0006},
    {1, 0x0B04},
};
// m.onOff and m.electricityMeter power defaults (min 10 s, max MAX, change 5).
constexpr ReportingSpec kReports_SP_P06E[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 3600, 0, 0},
    {1, 0x0B04, 0x050B, 0x29, 10, 65000, 5, 0},
};

}  // namespace

extern const PreparedDefinition kDefSP_P06E{
    .zigbee_models=kModels_SP_P06E,
    .zigbee_models_count=sizeof(kModels_SP_P06E)/sizeof(kModels_SP_P06E[0]),
    .model="SP-P06E", .vendor="Aqara",
    .meta=nullptr,
    .exposes=kExposes_SP_P06E,
    .exposes_count=sizeof(kExposes_SP_P06E)/sizeof(kExposes_SP_P06E[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SP_P06E, .from_zigbee_count=sizeof(kFz_SP_P06E)/sizeof(kFz_SP_P06E[0]),
    .to_zigbee=kTz_SP_P06E, .to_zigbee_count=sizeof(kTz_SP_P06E)/sizeof(kTz_SP_P06E[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SP_P06E,
    .bindings_count=sizeof(kBindings_SP_P06E)/sizeof(kBindings_SP_P06E[0]),
    .reports=kReports_SP_P06E,
    .reports_count=sizeof(kReports_SP_P06E)/sizeof(kReports_SP_P06E[0]),
    .power_source_override = 0x01,   // ZCL Basic 0x0007 = Mains (single phase)
};

}  // namespace zhc::devices::lumi
