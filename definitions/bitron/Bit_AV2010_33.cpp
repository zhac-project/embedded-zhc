// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bitron AV2010/33 — graduated 2026-06-10 (IAS bit-selection fix).
// Vibration sensor.
// z2m-source: bitron.ts #AV2010/33 (fz.ias_occupancy_alarm_2).
//
// z2m bundle: fz.ias_occupancy_alarm_2
//   exposes [occupancy, battery_low].
//
// Mapped:
//   kFzIasVibrationAlarm2 — ssIasZone status → "vibration" + "tamper" + "battery_low".
//                           z2m's ias_occupancy_alarm_2 reads zoneStatus bit 1
//                           (alarm_2), NOT bit 0. The previously wired
//                           kFzIasVibrationAlarm reads bit 0, so this sensor's
//                           vibration event (signalled on alarm_2) never fired.
//                           kFzIasVibrationAlarm2 is the bit-1 variant emitting
//                           the same semantic "vibration" key. The other four
//                           vibration sensors (develco WISZB-137, heiman
//                           HS1VS-EF/N, third_reality 3RVS01031Z) use
//                           fz.ias_vibration_alarm_1 (bit 0) and keep
//                           kFzIasVibrationAlarm unchanged.
// NOTE: z2m exposes "occupancy" here; we expose semantic "vibration" (zoneType
//       0x002D). This diverges from z2m's label but matches the underlying
//       hardware kind.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bitron {
namespace {
const FzConverter* const kFz_AV2010_33[] = {
    &::zhc::generic::kFzIasVibrationAlarm2,
};

constexpr const char* kModels_AV2010_33[] = { "AV2010/33", "902010/33" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"vibration", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};

extern const PreparedDefinition kDef_AV2010_33{
    .zigbee_models=kModels_AV2010_33, .zigbee_models_count=sizeof(kModels_AV2010_33)/sizeof(kModels_AV2010_33[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AV2010/33", .vendor="Bitron",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AV2010_33, .from_zigbee_count=sizeof(kFz_AV2010_33)/sizeof(kFz_AV2010_33[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bitron
