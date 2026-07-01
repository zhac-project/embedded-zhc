// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Heiman HS1SA-E Lover — smoke detector (zigbeeModel "Smokesensor-EF2-3.0").
// Ported (Tier 1, generic converters only): battery/voltage, IAS smoke alarm
// (key "smoke") with battery_low, and the tz.warning siren control.
// Deferred: the "test"/"trouble" sub-flags, and all heimanClusterSpecial (0xFC90)
// extras — device mute/state, IAS-WD warning-device mute, initiate-test-mode,
// sensor fault state, and the reported/rejoined/rebooted counters.
// z2m-source: heiman.ts #HS1SA-E Lover.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HS1SA_E_Lover[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasSmokeAlarm,
};
const TzConverter* const kTz_HS1SA_E_Lover[] = {
    &::zhc::generic::kTzWarning,
};
constexpr const char* kModels_HS1SA_E_Lover[] = { "Smokesensor-EF2-3.0" };

}  // namespace


constexpr Expose kExposes_HS1SA_E_Lover[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"smoke", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_HS1SA_E_Lover{
    .zigbee_models=kModels_HS1SA_E_Lover, .zigbee_models_count=sizeof(kModels_HS1SA_E_Lover)/sizeof(kModels_HS1SA_E_Lover[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS1SA-E Lover", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_HS1SA_E_Lover, .exposes_count=sizeof(kExposes_HS1SA_E_Lover)/sizeof(kExposes_HS1SA_E_Lover[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS1SA_E_Lover, .from_zigbee_count=sizeof(kFz_HS1SA_E_Lover)/sizeof(kFz_HS1SA_E_Lover[0]),
    .to_zigbee=kTz_HS1SA_E_Lover, .to_zigbee_count=sizeof(kTz_HS1SA_E_Lover)/sizeof(kTz_HS1SA_E_Lover[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heiman
