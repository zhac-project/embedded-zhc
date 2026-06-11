// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: iLightsIn HSSA18-Z-MID (Zhaga module) — restored multi-sensor.
//
// z2m extend = [m.light(), m.illuminance(), m.occupancy(),
//   m.iasZoneAlarm({zoneType:"occupancy",
//                   zoneAttributes:["alarm_1","alarm_2","tamper","battery_low"]})].
//
// Bugs fixed (auto-port dropped three whole channels + used a dead IAS key):
//   * m.illuminance() (msIlluminanceMeasurement 0x0400) was DROPPED — no
//     illuminance decoder/expose/binding. Wired kFzIlluminance + 0x0400.
//   * m.occupancy()   (msOccupancySensing 0x0406) was DROPPED — no occupancy
//     decoder/expose/binding. Wired kFzOccupancy + 0x0406.
//   * The IAS-zone alarm was wired to the generic kFzIasZone, which emits the
//     bare key `alarm` (bit 0 only) and drops bit 1 entirely. z2m's
//     iasZoneAlarm with both alarm_1 + alarm_2 (zoneType "occupancy") publishes
//     the SPLIT keys `occupancy_alarm_1` (zoneStatus bit 0) and
//     `occupancy_alarm_2` (bit 1), plus `tamper` (bit 2) and `battery_low`
//     (bit 3). Wired the typed kFzIasOccupancyAlarm1 / kFzIasOccupancyAlarm2.
//
// z2m-source: ilightsin.ts #HSSA18-Z-MID.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ilightsin {
namespace {
const FzConverter* const kFz_HSSA18_Z_MID[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIasOccupancyAlarm1,
    &::zhc::generic::kFzIasOccupancyAlarm2,
};
const TzConverter* const kTz_HSSA18_Z_MID[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_HSSA18_Z_MID[] = { "13D" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_alarm_1", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_alarm_2", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0400},  // msIlluminanceMeasurement
    {1, 0x0406},  // msOccupancySensing
    {1, 0x0500},  // ssIasZone
};

extern const PreparedDefinition kDef_HSSA18_Z_MID{
    .zigbee_models=kModels_HSSA18_Z_MID, .zigbee_models_count=sizeof(kModels_HSSA18_Z_MID)/sizeof(kModels_HSSA18_Z_MID[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HSSA18-Z-MID", .vendor="Ilightsin",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HSSA18_Z_MID, .from_zigbee_count=sizeof(kFz_HSSA18_Z_MID)/sizeof(kFz_HSSA18_Z_MID[0]),
    .to_zigbee=kTz_HSSA18_Z_MID, .to_zigbee_count=sizeof(kTz_HSSA18_Z_MID)/sizeof(kTz_HSSA18_Z_MID[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ilightsin
