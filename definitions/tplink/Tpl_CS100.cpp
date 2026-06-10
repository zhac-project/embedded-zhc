// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: TP-Link CS100 — contact sensor (parity fix).
// z2m-source: tplink.ts #CS100.
//
// z2m wires `fz.ias_contact_alarm_1` (z2m-INVERTED: `contact = !(zoneStatus
// bit0)`, bit3 -> `battery_low`) plus `fz.battery`. The Tier-1 auto-port
// lowered the generic `kFzIasZone`, which emits the bare `alarm` key — the
// `contact` expose was therefore dead. Fixed by lowering the typed
// `kFzIasContactAlarm`, whose label inverts bit0 onto `contact` exactly as
// z2m does. Exposes now match z2m: contact/battery_low/battery (CS100 has
// no tamper expose).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tplink {
namespace {
const FzConverter* const kFz_CS100[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,   // contact = !bit0 (z2m-inverted), battery_low (bit3)
};

constexpr const char* kModels_CS100[] = { "CS100" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_CS100{
    .zigbee_models=kModels_CS100, .zigbee_models_count=sizeof(kModels_CS100)/sizeof(kModels_CS100[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CS100", .vendor="Tplink",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CS100, .from_zigbee_count=sizeof(kFz_CS100)/sizeof(kFz_CS100[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::tplink
