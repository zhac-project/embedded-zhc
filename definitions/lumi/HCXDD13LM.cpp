// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara HCXDD13LM Nebula ceiling lamp H1 (CT) — z2m v26.99.0 parity.
//
// z2m: `addManuSpecificLumiCluster() + m.light({colorTemp: {range:
// [153, 370], startup: false}, effect: false, powerOnBehavior: false})
// + lumiPowerOnBehavior() + lumiZigbeeOTA()`. Same shape as the
// ZNLDP17LM ceiling light, plus the Lumi-flavoured power-on behavior
// (written on the 0xFCC0 manufacturer cluster, not genOnOff
// startUpOnOff — z2m disables the stock one explicitly).
//
// The narrow 153-370 mired colour-temperature range is declared on the
// expose so the UI slider scales to the lamp instead of the generic
// default.
//
// z2m-source: lumi.ts #HCXDD13LM (lumi.light.acn033).
#include "definitions/lumi/_shared.hpp"
#include "definitions/lumi/factories.hpp"

namespace zhc::devices::lumi {
namespace {

using FX = ::zhc::lumi::factory::LumiLight<
    ::zhc::lumi::factory::LumiLightOpts{.level = true, .color_temp = true}>;

constexpr const char* kModels_HCXDD13LM[] = { "lumi.light.acn033" };

constexpr const char* kPowerOnBehaviourValues[] = {
    "on", "previous", "off",
};

constexpr Expose kExposes_HCXDD13LM[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired",  nullptr, nullptr, 0,
     ExposeCategory::State, 153, 370, 1},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kPowerOnBehaviourValues,
     sizeof(kPowerOnBehaviourValues)/sizeof(kPowerOnBehaviourValues[0]),
     ExposeCategory::Config},
    {"device_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"power_outage_count", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_HCXDD13LM[] = {
    {1, 0x0000},   // genBasic (lumi 0xFCC0 rides here)
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDefHCXDD13LM{
    .zigbee_models=kModels_HCXDD13LM,
    .zigbee_models_count=sizeof(kModels_HCXDD13LM)/sizeof(kModels_HCXDD13LM[0]),
    .model="HCXDD13LM", .vendor="Aqara",
    .meta=nullptr,
    .exposes=kExposes_HCXDD13LM,
    .exposes_count=sizeof(kExposes_HCXDD13LM)/sizeof(kExposes_HCXDD13LM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_HCXDD13LM,
    .bindings_count=sizeof(kBindings_HCXDD13LM)/sizeof(kBindings_HCXDD13LM[0]),
};

}  // namespace zhc::devices::lumi
