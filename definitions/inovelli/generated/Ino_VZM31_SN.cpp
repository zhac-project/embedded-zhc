// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Inovelli VZM31-SN — uses shared inovelli converters.
// 2-in-1 switch + dimmer
// z2m-source: inovelli.ts #VZM31-SN.
#include "definitions/_generic/_shared.hpp"
#include "definitions/inovelli/_shared.hpp"

namespace zhc::devices::inovelli {
namespace {
const FzConverter* const kFz_VZM31_SN[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz_VZM31_SN[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    // Inovelli manuSpec writeable parameters (cluster 0xFC31, mfg 0x122f).
    &kTzInoDimmingSpeedUpRemote,
    &kTzInoDimmingSpeedUpLocal,
    &kTzInoRampRateOffToOnRemote,
    &kTzInoRampRateOffToOnLocal,
    &kTzInoDimmingSpeedDownRemote,
    &kTzInoDimmingSpeedDownLocal,
    &kTzInoRampRateOnToOffRemote,
    &kTzInoRampRateOnToOffLocal,
    &kTzInoInvertSwitch,
    &kTzInoAutoTimerOff,
    &kTzInoDefaultLevelLocal,
    &kTzInoDefaultLevelRemote,
    &kTzInoStateAfterPowerRestored,
    &kTzInoMinimumLevel,
    &kTzInoMaximumLevel,
    &kTzInoOutputMode,
    &kTzInoSmartBulbMode,
};
constexpr const char* kModels_VZM31_SN[] = { "VZM31-SN" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    // Inovelli COMMON_DIMMER + COMMON_DIMMER_ON_OFF parameter exposes.
    {"dimmingSpeedUpRemote",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"dimmingSpeedUpLocal",     ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"rampRateOffToOnRemote",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"rampRateOffToOnLocal",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"dimmingSpeedDownRemote",  ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"dimmingSpeedDownLocal",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"rampRateOnToOffRemote",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"rampRateOnToOffLocal",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"invertSwitch",            ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"autoTimerOff",            ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0},
    {"defaultLevelLocal",       ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"defaultLevelRemote",      ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"stateAfterPowerRestored", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"minimumLevel",            ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"maximumLevel",            ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"outputMode",              ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"smartBulbMode",           ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0702},
    {1, 0xFC31},  // manuSpecificInovelli — for parameter writes/reports.
};

extern const PreparedDefinition kDef_VZM31_SN{
    .zigbee_models=kModels_VZM31_SN, .zigbee_models_count=sizeof(kModels_VZM31_SN)/sizeof(kModels_VZM31_SN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VZM31-SN", .vendor="Inovelli",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VZM31_SN, .from_zigbee_count=sizeof(kFz_VZM31_SN)/sizeof(kFz_VZM31_SN[0]),
    .to_zigbee=kTz_VZM31_SN, .to_zigbee_count=sizeof(kTz_VZM31_SN)/sizeof(kTz_VZM31_SN[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::inovelli
