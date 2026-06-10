// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Peq 3300-P — graduated parent override.
// Door & window contact sensor
// z2m-source: peq.ts #3300-P — fromZigbee:[fz.temperature,
//   fz.ias_contact_alarm_1, fz.battery]; exposes temperature/contact/
//   battery_low/tamper/battery.
//
// Bug fixed (two real gaps vs z2m):
//   (1) IAS dead-key: the generated def lowered the generic kFzIasZone
//       converter, which emits the bare key "alarm" — never matching the
//       declared expose, so the primary contact state never reached the
//       shadow. Swapped in the typed kFzIasContactAlarm (emits "contact",
//       z2m-inverted: contact = !(zoneStatus bit0)).
//   (2) Dropped temperature channel: z2m carries fz.temperature
//       (msTemperatureMeasurement 0x0402, /100) but the port dropped the
//       decoder, the `temperature` expose, and the 0x0402 binding. All
//       three restored.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::peq {
namespace {
const FzConverter* const kFz_D3300_P[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasContactAlarm,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_D3300_P[] = { "3300" };

}  // namespace


// exposes: temperature/contact/battery_low/tamper/battery (z2m parity).
constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg (battery)
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0500},   // ssIasZone (contact)
};

extern const PreparedDefinition kDef_D3300_P{
    .zigbee_models=kModels_D3300_P, .zigbee_models_count=sizeof(kModels_D3300_P)/sizeof(kModels_D3300_P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3300-P", .vendor="Peq",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3300_P, .from_zigbee_count=sizeof(kFz_D3300_P)/sizeof(kFz_D3300_P[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::peq
