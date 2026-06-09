// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Niko 552-80401 — graduated from generated/ to fix IAS dead-key.
// Wireless motion sensor.
//
// z2m-source: niko.ts #552-80401 — fromZigbee [fz.ias_occupancy_alarm_1,
// fz.battery], exposes [e.occupancy(), e.battery_low(), e.battery()].
//
// FIX: the generated def wired the generic `kFzIasZone`
// (fz_ias_zone_status_change), which emits a neutral `alarm` key and
// therefore left the `occupancy` expose a dead key — the S3 shadow never
// received occupancy. z2m's `fz.ias_occupancy_alarm_1` emits
// occupancy/tamper/battery_low from zoneStatus bits 0/2/3. The generic
// typed converter `kFzIasMotionAlarm` (kLbl_Motion {"occupancy", 0})
// matches that exactly (same commandStatusChangeNotification path, same
// tamper/battery_low bits), so swap to it and rename the expose
// `alarm` → `occupancy`.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::niko {
namespace {
const FzConverter* const kFz_D552_80401[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_D552_80401[] = { "Connectable motion sensor,Zigbee" };

}  // namespace


// --- exposes (hand-edited: alarm → occupancy for parity) ---
constexpr Expose kAutoExposes[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_D552_80401{
    .zigbee_models=kModels_D552_80401, .zigbee_models_count=sizeof(kModels_D552_80401)/sizeof(kModels_D552_80401[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="552-80401", .vendor="Niko",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D552_80401, .from_zigbee_count=sizeof(kFz_D552_80401)/sizeof(kFz_D552_80401[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::niko
