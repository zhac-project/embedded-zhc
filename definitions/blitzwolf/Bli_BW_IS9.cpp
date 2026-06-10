// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Blitzwolf BW-IS9 — IAS water-leak sensor (bothAlarms).
// ZigBee Water Leak Sensor.
//
// Bug fixed (IAS dead-key): z2m wires
//   m.iasZoneAlarm({zoneType:"water_leak",
//                   zoneAttributes:["alarm_1","alarm_2","tamper","battery_low"]})
// which (bothAlarms) exposes `water_leak_alarm_1` (zoneStatus bit 0) and
// `water_leak_alarm_2` (bit 1), NOT a bare `alarm`. The auto-port wired the
// generic kFzIasZone (emits bare alarm/alarm_1) against an `alarm` expose, so
// the declared semantic keys never populated. Repointed to the typed
// kFzIasWaterLeakAlarm1 (bit 0) + kFzIasWaterLeakAlarm2 (bit 1) converters and
// renamed the exposes to match z2m. Battery half (m.battery) unchanged.
//
// z2m-source: blitzwolf.ts #BW-IS9 + lib/modernExtend.ts iasZoneAlarm.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::blitzwolf {
namespace {
const FzConverter* const kFz_BW_IS9[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasWaterLeakAlarm1,
    &::zhc::generic::kFzIasWaterLeakAlarm2,
};

constexpr const char* kModels_BW_IS9[] = { "TS0207" };
constexpr const char* kManus_BW_IS9[] = { "_TZ3000_eit7p838 " };
}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"water_leak_alarm_1", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"water_leak_alarm_2", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_BW_IS9{
    .zigbee_models=kModels_BW_IS9, .zigbee_models_count=sizeof(kModels_BW_IS9)/sizeof(kModels_BW_IS9[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_BW_IS9, .manufacturer_names_count=sizeof(kManus_BW_IS9)/sizeof(kManus_BW_IS9[0]),
    .model="BW-IS9", .vendor="Blitzwolf",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_BW_IS9, .from_zigbee_count=sizeof(kFz_BW_IS9)/sizeof(kFz_BW_IS9[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::blitzwolf
