// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Osram AC01353010G — hand-tuned IAS motion + temperature sensor.
// SMART+ Motion Sensor (zigbeeModel "Motion Sensor-A").
//
// Parity fix (two gaps):
//   1. Temperature was dead. z2m wires fz.temperature and binds
//      msTemperatureMeasurement (0x0402), exposing `temperature`. The
//      generated port wired neither the decoder nor the expose nor the
//      0x0402 binding — the msTemperatureMeasurement reports never reached
//      the shadow. Added kFzTemperature + a `temperature` expose + 0x0402
//      binding.
//   2. Occupancy key mismatch. z2m wires fz.ias_occupancy_only_alarm_2,
//      which publishes the `occupancy` key from zoneStatus bit 1. The
//      generated port lowered the generic kFzIasZone, which emits a bare
//      `alarm` key (declared expose was `alarm`) — never the `occupancy`
//      key z2m advertises. Re-pointed at the typed kFzIasMotionAlarm2
//      (occupancy / tamper / battery_low), same fix shape as the heiman /
//      vsmart IAS sensors.
//
// z2m-source: osram.ts #AC01353010G.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::osram {
namespace {
const FzConverter* const kFz_AC01353010G[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasMotionAlarm2,
};

constexpr const char* kModels_AC01353010G[] = { "Motion Sensor-A" };

constexpr Expose kExposes_AC01353010G[] = {
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_AC01353010G[] = {
    {1, 0x0001}, {1, 0x0402}, {1, 0x0500},
};

}  // namespace

extern const PreparedDefinition kDef_AC01353010G{
    .zigbee_models=kModels_AC01353010G, .zigbee_models_count=sizeof(kModels_AC01353010G)/sizeof(kModels_AC01353010G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AC01353010G", .vendor="Osram",
    .meta=nullptr,
    .exposes=kExposes_AC01353010G,
    .exposes_count=sizeof(kExposes_AC01353010G)/sizeof(kExposes_AC01353010G[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AC01353010G, .from_zigbee_count=sizeof(kFz_AC01353010G)/sizeof(kFz_AC01353010G[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_AC01353010G,
    .bindings_count=sizeof(kBindings_AC01353010G)/sizeof(kBindings_AC01353010G[0]),
};

}  // namespace zhc::devices::osram
