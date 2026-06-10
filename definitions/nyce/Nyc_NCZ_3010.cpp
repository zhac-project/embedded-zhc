// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Nyce NCZ-3010 — door hinge sensor (graduated from generated/).
// Door hinge sensor
// z2m-source: nyce.ts #NCZ-3010 — fromZigbee:[fz.ias_contact_alarm_1, fz.battery],
//             exposes:[e.contact(), e.battery_low(), e.battery()].
//
// Fix: the generated def lowered the generic kFzIasZone (emits a bare `alarm`
// key) and exposed alarm/tamper/voltage — none matching z2m. z2m wires
// fz.ias_contact_alarm_1 (zoneStatus bit 0 -> `contact`). Re-wired to typed
// kFzIasContactAlarm and corrected exposes to contact/battery_low/battery.
// No tamper on this model (z2m omits e.tamper()).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nyce {
namespace {
const FzConverter* const kFz_NCZ_3010[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_NCZ_3010[] = { "3010" };

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

extern const PreparedDefinition kDef_NCZ_3010{
    .zigbee_models=kModels_NCZ_3010, .zigbee_models_count=sizeof(kModels_NCZ_3010)/sizeof(kModels_NCZ_3010[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NCZ-3010", .vendor="Nyce",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_NCZ_3010, .from_zigbee_count=sizeof(kFz_NCZ_3010)/sizeof(kFz_NCZ_3010[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nyce
