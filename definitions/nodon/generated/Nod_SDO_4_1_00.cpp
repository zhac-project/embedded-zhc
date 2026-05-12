// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nodon SDO-4-1-00 — hand-rewrite (was wrong-bundle).
// Door & window opening sensor.
//
// z2m: `fromZigbee:[fz.battery, fz.ias_contact_alarm_1]` + `e.contact()`.
// Original auto-generated port wired the generic `kFzIasZone` (emits
// alarm/tamper/battery_low) instead of `kFzIasContactAlarm` (emits
// the typed `contact` key z2m exposes). Switched to the typed converter
// and aligned exposes to z2m.
//
// z2m-source: nodon.ts #SDO-4-1-00.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_SDO_4_1_00[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,   // emits "contact" + tamper + battery_low
};

constexpr const char* kModels_SDO_4_1_00[] = { "SDO-4-1-00" };

}  // namespace


// Hand-aligned to z2m exposes (was: `alarm`/`tamper`/`battery_low` only).
constexpr Expose kAutoExposes[] = {
    {"contact",     ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_SDO_4_1_00{
    .zigbee_models=kModels_SDO_4_1_00, .zigbee_models_count=sizeof(kModels_SDO_4_1_00)/sizeof(kModels_SDO_4_1_00[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SDO-4-1-00", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SDO_4_1_00, .from_zigbee_count=sizeof(kFz_SDO_4_1_00)/sizeof(kFz_SDO_4_1_00[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
