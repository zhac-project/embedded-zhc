// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Perenio PEHPL0X — hand-rewritten (vendor parity sweep).
// Power link smart plug — adds the perenioSpecific (0xFC7B / 0x007B)
// metering + alarm cluster on top of the generic OnOff + Metering set.
// z2m-source: perenio.ts #PEHPL0X.
#include "definitions/_generic/_shared.hpp"
#include "definitions/perenio/_shared.hpp"

namespace zhc::devices::perenio {
namespace {
const FzConverter* const kFz_PEHPL0X[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &kFzPerenioSmartPlug,
};
const TzConverter* const kTz_PEHPL0X[] = {
    &::zhc::generic::kTzOnOff,
    &kTzPerenioDefaultState,
    &kTzPerenioVoltageMin,
    &kTzPerenioVoltageMax,
    &kTzPerenioPowerMax,
    &kTzPerenioEnergyLimit,
    &kTzPerenioAlarmsResetVMin,
    &kTzPerenioAlarmsResetVMax,
    &kTzPerenioAlarmsResetPMax,
    &kTzPerenioAlarmsResetEnergy,
};
constexpr const char* kModels_PEHPL0X[] = { "PEHPL0X" };

constexpr const char* kDefaultOnOffStateEnum[] = { "on", "off", "previous" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"state",                 ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"default_on_off_state",  ExposeType::Enum,    Access::StateSet, nullptr, nullptr,
        kDefaultOnOffStateEnum, sizeof(kDefaultOnOffStateEnum)/sizeof(kDefaultOnOffStateEnum[0])},
    {"energy",                ExposeType::Numeric, Access::State,    "kWh", nullptr, nullptr, 0},
    {"power",                 ExposeType::Numeric, Access::State,    "W",   nullptr, nullptr, 0},
    {"rms_voltage",           ExposeType::Numeric, Access::State,    "V",   nullptr, nullptr, 0},
    {"active_power",          ExposeType::Numeric, Access::State,    "W",   nullptr, nullptr, 0},
    {"consumed_energy",       ExposeType::Numeric, Access::State,    "Wh",  nullptr, nullptr, 0},
    {"alarm_voltage_min",     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alarm_voltage_max",     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alarm_power_max",       ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alarm_consumed_energy", ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"voltage_min",           ExposeType::Numeric, Access::StateSet, "V",   nullptr, nullptr, 0},
    {"voltage_max",           ExposeType::Numeric, Access::StateSet, "V",   nullptr, nullptr, 0},
    {"power_max",             ExposeType::Numeric, Access::StateSet, "W",   nullptr, nullptr, 0},
    {"consumed_energy_limit", ExposeType::Numeric, Access::StateSet, "kWh", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0702},   // seMetering
    {1, 0xFC7B},   // perenioSpecific (manuf 0x007B)
};

extern const PreparedDefinition kDef_PEHPL0X{
    .zigbee_models=kModels_PEHPL0X, .zigbee_models_count=sizeof(kModels_PEHPL0X)/sizeof(kModels_PEHPL0X[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PEHPL0X", .vendor="Perenio",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PEHPL0X, .from_zigbee_count=sizeof(kFz_PEHPL0X)/sizeof(kFz_PEHPL0X[0]),
    .to_zigbee=kTz_PEHPL0X, .to_zigbee_count=sizeof(kTz_PEHPL0X)/sizeof(kTz_PEHPL0X[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::perenio
