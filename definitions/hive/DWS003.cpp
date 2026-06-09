// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Hive DWS003 — hand-tuned (graduated for typed IAS decode).
// Contact sensor — z2m: fz.temperature + fz.ias_contact_alarm_1 + fz.battery.
// Endpoint 6 binds msTemperatureMeasurement + genPowerCfg.
//
// Parity fix: the auto-port wired the generic `kFzIasZone`, which emits
// `alarm_1` / `alarm_2` — so the device's `contact` expose was dead.
// z2m uses `fz.ias_contact_alarm_1` (zoneType 0x0015), so this wires the
// typed `kFzIasContactAlarm` (zoneStatus bit 0 → `contact`; tamper +
// battery_low preserved).
// z2m-source: hive.ts #DWS003 + fromZigbee.ts fz.ias_contact_alarm_1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hive {
namespace {
const FzConverter* const kFz_DWS003[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_DWS003[] = { "DWS003" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {6, 0x0402},  // msTemperatureMeasurement
    {6, 0x0001},  // genPowerCfg
    {6, 0x0500},  // ssIasZone
};

extern const PreparedDefinition kDef_DWS003{
    .zigbee_models=kModels_DWS003, .zigbee_models_count=sizeof(kModels_DWS003)/sizeof(kModels_DWS003[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DWS003", .vendor="Hive",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DWS003, .from_zigbee_count=sizeof(kFz_DWS003)/sizeof(kFz_DWS003[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hive
