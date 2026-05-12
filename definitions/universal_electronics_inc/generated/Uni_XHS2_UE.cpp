// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: UniversalElectronicsInc XHS2-UE — hand-rewritten 2026-04-29a.
// Magnetic door & window contact sensor.
// z2m-source: universal_electronics_inc.ts #XHS2-UE.
//
// z2m bundle:
//   fz.ias_contact_alarm_1     — ssIasZone status_change → contact + tamper + battery_low
//   fz.temperature             — msTemperatureMeasurement
//   fz.battery                 — genPowerCfg
//
// Mapped:
//   kFzIasContactAlarm         — typed status-change decoder, alarm_1 → "contact".
//   kFzTemperature, kFzBattery — fully covered.
//
// FULL: every z2m converter is mapped; no toZigbee path on this device.
// Previous auto-port wired kFzIasZone (attribute 0x0002 reader); the device
// only emits the ssIasZone command 0x00 status-change frame. Replaced with
// the typed contact variant and renamed `alarm` → `contact` in exposes.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::universal_electronics_inc {
namespace {
const FzConverter* const kFz_XHS2_UE[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_XHS2_UE[] = { "URC4460BC0-X-R" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",         nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",        nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"contact",     ExposeType::Binary,  Access::State, nullptr,     nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr,     nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr,     nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0500},  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_XHS2_UE{
    .zigbee_models=kModels_XHS2_UE, .zigbee_models_count=sizeof(kModels_XHS2_UE)/sizeof(kModels_XHS2_UE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="XHS2-UE", .vendor="UniversalElectronicsInc",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_XHS2_UE, .from_zigbee_count=sizeof(kFz_XHS2_UE)/sizeof(kFz_XHS2_UE[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::universal_electronics_inc
