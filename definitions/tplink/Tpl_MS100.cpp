// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: TP-Link MS100 — smart motion sensor (parity fix).
// z2m-source: tplink.ts #MS100.
//
// z2m wires `fz.ias_occupancy_alarm_1` (zoneStatus bit0 -> `occupancy`,
// bit2 -> `tamper`, bit3 -> `battery_low`) plus `m.illuminance()`
// (msIlluminanceMeasurement 0x0400 -> `illuminance`) and `fz.battery`.
// The Tier-1 auto-port lowered the generic `kFzIasZone`, which emits the
// bare `alarm` key — the `occupancy` expose was therefore dead — and it
// dropped the illuminance channel entirely. Fixed by lowering the typed
// `kFzIasMotionAlarm` (occupancy from bit0) + `kFzIlluminance`, and the
// exposes now match z2m: occupancy/battery_low/tamper/battery/illuminance.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tplink {
namespace {
const FzConverter* const kFz_MS100[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,   // occupancy/tamper/battery_low (bit0)
    &::zhc::generic::kFzIlluminance,      // m.illuminance() (0x0400)
};

constexpr const char* kModels_MS100[] = { "MS100" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
    {1, 0x0400},
};

extern const PreparedDefinition kDef_MS100{
    .zigbee_models=kModels_MS100, .zigbee_models_count=sizeof(kModels_MS100)/sizeof(kModels_MS100[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MS100", .vendor="Tplink",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MS100, .from_zigbee_count=sizeof(kFz_MS100)/sizeof(kFz_MS100[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::tplink
