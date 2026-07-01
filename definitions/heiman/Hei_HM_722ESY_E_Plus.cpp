// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Heiman HM-722ESY-E Plus — carbon monoxide detector.
// Ported (Tier 1, generic converters only): battery/voltage, temperature,
// IAS CO alarm (key "carbon_monoxide") with battery_low, and the tz.warning
// siren control.
// Deferred: the "co" numeric (msCarbonMonoxide 0x040C has no generic decoder in
// _shared.hpp), the "test" sub-flag, and all heimanClusterSpecial (0xFC90)
// extras — device mute/state, indicator light, initiate-test-mode, sensor fault
// state, interconnectable, smoke-concentration level/unit, chamber-contamination
// level, temperature offset, endoflife/alarm_state/preheating enums, and the
// reported/rejoined/rebooted counters.
// z2m-source: heiman.ts #HM-722ESY-E Plus.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HM_722ESY_E_Plus[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasCoAlarm,
};
const TzConverter* const kTz_HM_722ESY_E_Plus[] = {
    &::zhc::generic::kTzWarning,
};
constexpr const char* kModels_HM_722ESY_E_Plus[] = { "HM-722ESY-E-PLUS" };

}  // namespace


constexpr Expose kExposes_HM_722ESY_E_Plus[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"carbon_monoxide", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_HM_722ESY_E_Plus{
    .zigbee_models=kModels_HM_722ESY_E_Plus, .zigbee_models_count=sizeof(kModels_HM_722ESY_E_Plus)/sizeof(kModels_HM_722ESY_E_Plus[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HM-722ESY-E Plus", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_HM_722ESY_E_Plus, .exposes_count=sizeof(kExposes_HM_722ESY_E_Plus)/sizeof(kExposes_HM_722ESY_E_Plus[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HM_722ESY_E_Plus, .from_zigbee_count=sizeof(kFz_HM_722ESY_E_Plus)/sizeof(kFz_HM_722ESY_E_Plus[0]),
    .to_zigbee=kTz_HM_722ESY_E_Plus, .to_zigbee_count=sizeof(kTz_HM_722ESY_E_Plus)/sizeof(kTz_HM_722ESY_E_Plus[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heiman
