// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HEIMAN S2-E smart smoke alarm with temperature.
// New upstream in z2m v26.100.0.
//
// Same family as S1-TL-AI, one sensor fewer (no humidity) and a longer list of
// vendor extends. The detection surface — IAS Zone smoke alarm with the test
// and low-battery bits, temperature, battery, and the siren command — is all
// stock and wired here.
//
// DEFERRED — the eight `heimanExtend.*` blocks upstream layers on: mute state,
// indicator light, sensor fault state, interconnect, mutability, test mode and
// link-available. They read and write attributes on heimanClusterSpecial;
// embedded-zhc carries tz converters for several (definitions/heiman/
// _shared.hpp) but no decode map for this model, and inventing one for a
// life-safety device is not appropriate. Same call as S1-TL-AI.
//
// z2m-source: heiman.ts #S2-E.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {

const FzConverter* const kFz_S2_E[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasSmokeAlarm,
    &::zhc::generic::kFzTemperature,
};
const TzConverter* const kTz_S2_E[] = {
    &::zhc::generic::kTzWarning,
};

constexpr const char* kModels_S2_E[] = { "S2-E" };

constexpr Expose kExposes_S2_E[] = {
    {"smoke",       ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"test",        ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

constexpr BindingSpec kBindings_S2_E[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0500},   // ssIasZone
    {1, 0x0502},   // ssIasWd — siren
};

}  // namespace

extern const PreparedDefinition kDef_S2_E{
    .zigbee_models=kModels_S2_E, .zigbee_models_count=sizeof(kModels_S2_E)/sizeof(kModels_S2_E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S2-E", .vendor="Heiman",
    .meta=nullptr,
    .exposes=kExposes_S2_E, .exposes_count=sizeof(kExposes_S2_E)/sizeof(kExposes_S2_E[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_S2_E, .from_zigbee_count=sizeof(kFz_S2_E)/sizeof(kFz_S2_E[0]),
    .to_zigbee=kTz_S2_E, .to_zigbee_count=sizeof(kTz_S2_E)/sizeof(kTz_S2_E[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_S2_E, .bindings_count=sizeof(kBindings_S2_E)/sizeof(kBindings_S2_E[0]),
};

}  // namespace zhc::devices::heiman
