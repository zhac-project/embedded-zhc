// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Heiman HM-5HA-E — heat detector.
// Ported (Tier 1, generic converters only): battery/voltage, temperature,
// IAS generic "alarm" (zoneType "alarm" → key "alarm") with tamper + battery_low,
// and the tz.warning siren control.
// Deferred: heimanClusterSpecial (0xFC90) extras — device mute/state, indicator
// light, initiate-test-mode, sensor-fault-state, interconnectable, temperature
// offset, reported/rejoined/rebooted counters — plus the "test" sub-flag.
// z2m-source: heiman.ts #HM-5HA-E.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HM_5HA_E[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasGenericAlarm,
};
const TzConverter* const kTz_HM_5HA_E[] = {
    &::zhc::generic::kTzWarning,
};
constexpr const char* kModels_HM_5HA_E[] = { "HM-5HA-E", "HM-5HA-EF-3.0" };

}  // namespace


constexpr Expose kExposes_HM_5HA_E[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"alarm", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_HM_5HA_E{
    .zigbee_models=kModels_HM_5HA_E, .zigbee_models_count=sizeof(kModels_HM_5HA_E)/sizeof(kModels_HM_5HA_E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HM-5HA-E", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_HM_5HA_E, .exposes_count=sizeof(kExposes_HM_5HA_E)/sizeof(kExposes_HM_5HA_E[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HM_5HA_E, .from_zigbee_count=sizeof(kFz_HM_5HA_E)/sizeof(kFz_HM_5HA_E[0]),
    .to_zigbee=kTz_HM_5HA_E, .to_zigbee_count=sizeof(kTz_HM_5HA_E)/sizeof(kTz_HM_5HA_E[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heiman
