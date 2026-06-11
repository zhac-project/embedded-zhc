// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Shyugj S901D-ZG — IAS dual-alarm fix.
// Door sensor.
// z2m-source: shyugj.ts #S901D-ZG — m.iasZoneAlarm({zoneType:"generic",
//   zoneAttributes:["alarm_1","alarm_2","tamper","battery_low"]}) + m.battery().
// z2m's generic zoneType keeps bare zone keys (no semantic rename) and, with
// both alarm_1 + alarm_2 requested, exposes BOTH alarms: alarm_1 = zoneStatus
// bit0, alarm_2 = bit1, tamper = bit2, battery_low = bit3. The generated def
// wired kFzIasZone, which emits a single `alarm` (bit0) + tamper + battery_low
// — wrong key (`alarm` vs `alarm_1`) AND dropping alarm_2 (bit1) entirely.
// kFzIasZoneStatusChange emits exactly alarm_1/alarm_2/tamper/battery_low.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::shyugj {
namespace {
const FzConverter* const kFz_S901D_ZG[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZoneStatusChange,
};

constexpr const char* kModels_S901D_ZG[] = { "DoorSensor-ZB3.0" };

}  // namespace


// z2m generic zoneType with alarm_1 + alarm_2 exposes both bare alarm keys.
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"alarm_1", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"alarm_2", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_S901D_ZG{
    .zigbee_models=kModels_S901D_ZG, .zigbee_models_count=sizeof(kModels_S901D_ZG)/sizeof(kModels_S901D_ZG[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S901D-ZG", .vendor="Shyugj",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_S901D_ZG, .from_zigbee_count=sizeof(kFz_S901D_ZG)/sizeof(kFz_S901D_ZG[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::shyugj
