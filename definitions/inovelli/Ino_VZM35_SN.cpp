// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Inovelli VZM35-SN — uses shared inovelli converters.
// Fan controller (mains-powered; NOT battery-operated)
// z2m-source: inovelli.ts #VZM35-SN.
// Graduated from generated/ to wire the scene/button-tap `action`
// decoder (z2m `supportsButtonTaps: true`): manuSpec raw frame on
// endpoint 2 → action `<button>_<click>`.
#include "definitions/_generic/_shared.hpp"
#include "definitions/inovelli/_shared.hpp"

namespace zhc::devices::inovelli {
namespace {

constexpr const char* kModels_VZM35_SN[] = { "VZM35-SN" };

const FzConverter* const kFz_VZM35_SN[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,  // genLevelCtrl drives fan_mode in z2m
    &kFzInovelliSceneAction,  // z2m supportsButtonTaps → action <button>_<click>
};
const TzConverter* const kTz_VZM35_SN[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    // Inovelli manuSpec writeable parameters (cluster 0xFC31, mfg 0x122f).
    // Subset of COMMON_DIMMER_ATTRIBUTES that VZM35 uses for fan
    // ramp/min/max/output-mode (3-speed vs exhaust on-off).
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

constexpr Expose kAutoExposes[] = {
    {"state",     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // z2m e.action(BUTTON_TAP_SEQUENCES) — scene/button taps.
    {"action", ExposeType::Enum, Access::State, nullptr, "<button>_<click>", nullptr, 0},
    // VZM35 fan-specific parameter exposes (subset of COMMON_DIMMER).
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
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl (fan_mode)
    {1, 0xFC31},   // manuSpecificInovelli — parameter writes/reports.
};

}  // namespace

extern const PreparedDefinition kDef_VZM35_SN{
    .zigbee_models=kModels_VZM35_SN, .zigbee_models_count=sizeof(kModels_VZM35_SN)/sizeof(kModels_VZM35_SN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VZM35-SN", .vendor="Inovelli",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VZM35_SN, .from_zigbee_count=sizeof(kFz_VZM35_SN)/sizeof(kFz_VZM35_SN[0]),
    .to_zigbee=kTz_VZM35_SN, .to_zigbee_count=sizeof(kTz_VZM35_SN)/sizeof(kTz_VZM35_SN[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,
    .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::inovelli
