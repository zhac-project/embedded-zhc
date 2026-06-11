// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: BrunHolding Fire Fence — graduated from generated/ for real fixes.
// Stove guard for safe cooking. Dual relay (main_switch EP1 + short_override
// EP2) + power metering on EP1 + ambient temperature + battery.
//
// z2m-source: brun_holding.ts #Fire Fence.
//
// Fixes vs the auto-port:
//   * endpoint_map had short_override on EP1 (collides with main_switch);
//     z2m deviceEndpoints maps {main_switch:1, short_override:2} — restored
//     EP2 so the two relays no longer share the bare `state` key.
//   * Missing temperature: z2m ships m.temperature() (msTemperatureMeasurement
//     0x0402) but the port dropped kFzTemperature, the expose, and the bind.
//     Re-added. Sensor reports on EP1 -> suffixed `temperature_main_switch`
//     (multi-EP suffix convention, matches diyRuz/xyzroe).
//   * Phantom `voltage` expose: z2m m.electricityMeter sets voltage:false +
//     current:false -> only power + energy (seMetering 0x0702). The generic
//     kFzMetering emits energy + power (no voltage), so the `voltage` expose
//     was dead. Removed; no 0x0B04 half exists on this device.
//   * Per-EP metering: z2m tags electricityMeter to main_switch, so power +
//     energy land under `power_main_switch` / `energy_main_switch`.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::brun_holding {
namespace {
const FzConverter* const kFz_Fire_Fence[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzTemperature,
};
const TzConverter* const kTz_Fire_Fence[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_Fire_Fence[] = { "Power Control Unit" };

// z2m m.deviceEndpoints({main_switch:1, short_override:2}).
constexpr ::zhc::EndpointLabel kEndpoints_Fire_Fence[] = {
    {"main_switch", 1}, {"short_override", 2},
};

}  // namespace


// Exposes mirror the z2m endpoint-tagged keys: per-relay state, metering on
// the main_switch endpoint, ambient temperature (EP1), and battery.
constexpr Expose kAutoExposes[] = {
    {"state_main_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_short_override", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_main_switch", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"energy_main_switch", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"temperature_main_switch", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff    — main_switch
    {2, 0x0006},   // genOnOff    — short_override
    {1, 0x0001},   // genPowerCfg — battery
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0702},   // seMetering  — energy/power on main_switch
};

extern const PreparedDefinition kDef_Fire_Fence{
    .zigbee_models=kModels_Fire_Fence, .zigbee_models_count=sizeof(kModels_Fire_Fence)/sizeof(kModels_Fire_Fence[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Fire Fence", .vendor="BrunHolding",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Fire_Fence, .from_zigbee_count=sizeof(kFz_Fire_Fence)/sizeof(kFz_Fire_Fence[0]),
    .to_zigbee=kTz_Fire_Fence, .to_zigbee_count=sizeof(kTz_Fire_Fence)/sizeof(kTz_Fire_Fence[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_Fire_Fence,
    .endpoint_map_count = sizeof(kEndpoints_Fire_Fence)/sizeof(kEndpoints_Fire_Fence[0]),
};

}  // namespace zhc::devices::brun_holding
