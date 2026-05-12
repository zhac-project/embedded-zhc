// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: UniversalElectronicsInc XHS1-UE — hand-rewritten 2026-04-29a.
// Wireless digital pet resistant PIR detector.
// z2m-source: universal_electronics_inc.ts #XHS1-UE.
//
// z2m bundle:
//   fz.ias_occupancy_alarm_1   — ssIasZone status_change → occupancy + tamper + battery_low
//   fz.temperature             — msTemperatureMeasurement
//   fz.battery                 — genPowerCfg
//
// Mapped:
//   kFzIasMotionAlarm          — typed status-change decoder, alarm_1 → "occupancy".
//   kFzTemperature, kFzBattery — fully covered.
//
// FULL: every z2m converter is mapped; no toZigbee path on this device.
// Previous auto-port wired kFzIasZone (attribute-report decoder for attr 0x0002)
// which the device never reports — the keypad / sensor only emits the
// ssIasZone command 0x00 status-change frame. Replaced with the typed
// motion variant and renamed `alarm` → `occupancy` in the expose list.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::universal_electronics_inc {
namespace {
const FzConverter* const kFz_XHS1_UE[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_XHS1_UE[] = { "URC4470BC0-X-R" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",         nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",        nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr,     nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr,     nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr,     nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0500},  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_XHS1_UE{
    .zigbee_models=kModels_XHS1_UE, .zigbee_models_count=sizeof(kModels_XHS1_UE)/sizeof(kModels_XHS1_UE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="XHS1-UE", .vendor="UniversalElectronicsInc",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_XHS1_UE, .from_zigbee_count=sizeof(kFz_XHS1_UE)/sizeof(kFz_XHS1_UE[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::universal_electronics_inc
