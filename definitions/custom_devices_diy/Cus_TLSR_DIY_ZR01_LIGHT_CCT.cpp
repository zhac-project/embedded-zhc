// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Custom devices (DiY) TLSR-DIY-ZR01-LIGHT-CCT — CCT LED lamp /
// dimmer running custom firmware. New upstream in z2m v26.97.0.
//
// z2m: `m.light({colorTemp: {range: [130, 560], startup: false},
// configureReporting: true}) + m.identify()`. Three zigbeeModels share
// the entry — the firmware reports a different model id per build
// target (track / strip / custom).
//
// `startup: false` means the firmware has no genLevelCtrl
// startUpCurrentLevel support, so no start-up brightness expose is
// declared. `m.identify()` is a command-only extend with no state.
//
// z2m-source: custom_devices_diy.ts #TLSR-DIY-ZR01-LIGHT-CCT.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {

const FzConverter* const kFz_ZR01_LIGHT_CCT[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_ZR01_LIGHT_CCT[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};

constexpr const char* kModels_ZR01_LIGHT_CCT[] = {
    "LIGHT-CCT-TRK", "LIGHT-CCT-STR", "LIGHT-CCT-CUST",
};

constexpr Expose kExposes_ZR01_LIGHT_CCT[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
     ExposeCategory::State, 130, 560, 1},
};

constexpr BindingSpec kBindings_ZR01_LIGHT_CCT[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_TLSR_DIY_ZR01_LIGHT_CCT{
    .zigbee_models=kModels_ZR01_LIGHT_CCT,
    .zigbee_models_count=sizeof(kModels_ZR01_LIGHT_CCT)/sizeof(kModels_ZR01_LIGHT_CCT[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TLSR-DIY-ZR01-LIGHT-CCT", .vendor="Custom devices (DiY)",
    .meta=nullptr,
    .exposes=kExposes_ZR01_LIGHT_CCT,
    .exposes_count=sizeof(kExposes_ZR01_LIGHT_CCT)/sizeof(kExposes_ZR01_LIGHT_CCT[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZR01_LIGHT_CCT,
    .from_zigbee_count=sizeof(kFz_ZR01_LIGHT_CCT)/sizeof(kFz_ZR01_LIGHT_CCT[0]),
    .to_zigbee=kTz_ZR01_LIGHT_CCT,
    .to_zigbee_count=sizeof(kTz_ZR01_LIGHT_CCT)/sizeof(kTz_ZR01_LIGHT_CCT[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZR01_LIGHT_CCT,
    .bindings_count=sizeof(kBindings_ZR01_LIGHT_CCT)/sizeof(kBindings_ZR01_LIGHT_CCT[0]),
};

}  // namespace zhc::devices::custom_devices_diy
