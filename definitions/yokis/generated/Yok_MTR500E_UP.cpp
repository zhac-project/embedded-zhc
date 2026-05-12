// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yokis MTR500E-UP — parity-extended 2026-04-28.
// Remote power switch with timer 500W (mains, no battery).
// z2m-source: yokis.ts #MTR500E-UP.
//
// z2m extend: YokisOnOff(no powerOnBehavior), m.identify(),
//             YokisSubSystemExtend, yokisLightControlExtend,
//             YokisDeviceExtend, YokisInputExtend, YokisEntryExtend.
// Runtime: standard genOnOff + the manuSpecificYokis{LightControl,
//   SubSystem,Input,Entry} runtime from definitions/yokis/_shared.
//   manuSpecificYokisDevice (reset/openNetwork) still TODO.
// Note: previous port mistakenly carried genPowerCfg/battery — removed (z2m has none).
#include "definitions/_generic/_shared.hpp"
#include "definitions/yokis/_shared.hpp"

namespace zhc::devices::yokis {
namespace {
const FzConverter* const kFz_MTR500E_UP[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::yokis::kFzYokisLightControl,
    &::zhc::yokis::kFzYokisSubSystem,
    &::zhc::yokis::kFzYokisInput,
    &::zhc::yokis::kFzYokisEntry,
};
const TzConverter* const kTz_MTR500E_UP[] = {
    &::zhc::generic::kTzOnOff,
    // LightControl blink + NC + announce knobs.
    &::zhc::yokis::kTzYokisOnTimer,
    &::zhc::yokis::kTzYokisEOnTimer,
    &::zhc::yokis::kTzYokisPulseDuration,
    &::zhc::yokis::kTzYokisOperatingMode,
    &::zhc::yokis::kTzYokisEBlink,
    &::zhc::yokis::kTzYokisBlinkAmount,
    &::zhc::yokis::kTzYokisStateAfterBlink,
    &::zhc::yokis::kTzYokisENcCommand,
    // SubSystem power-failure mode.
    &::zhc::yokis::kTzYokisPowerFailureMode,
    // Input + Entry config.
    &::zhc::yokis::kTzYokisInputMode,
    &::zhc::yokis::kTzYokisContactMode,
    &::zhc::yokis::kTzYokisEShortPress,
    &::zhc::yokis::kTzYokisELongPress,
    &::zhc::yokis::kTzYokisLongPressDuration,
};
constexpr const char* kModels_MTR500E_UP[] = { "MTR500E-UP" };

constexpr Expose kExp_MTR500E_UP[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
constexpr BindingSpec kBind_MTR500E_UP[] = {
    {1, 0x0006},
};
}  // namespace

extern const PreparedDefinition kDef_MTR500E_UP{
    .zigbee_models=kModels_MTR500E_UP, .zigbee_models_count=sizeof(kModels_MTR500E_UP)/sizeof(kModels_MTR500E_UP[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MTR500E-UP", .vendor="Yokis",
    .meta=nullptr, .exposes=kExp_MTR500E_UP, .exposes_count=sizeof(kExp_MTR500E_UP)/sizeof(kExp_MTR500E_UP[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MTR500E_UP, .from_zigbee_count=sizeof(kFz_MTR500E_UP)/sizeof(kFz_MTR500E_UP[0]),
    .to_zigbee=kTz_MTR500E_UP, .to_zigbee_count=sizeof(kTz_MTR500E_UP)/sizeof(kTz_MTR500E_UP[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_MTR500E_UP, .bindings_count=sizeof(kBind_MTR500E_UP)/sizeof(kBind_MTR500E_UP[0]),
};

}  // namespace zhc::devices::yokis
