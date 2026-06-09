// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Adeo 83633205 — hand-maintained parity override.
// Smart 4 in 1 sensor (ZB-SMART-PIRTH-V3)
//
// Graduated from generated/Ade_D83633205.cpp: the generated def lowered only
// kFzBattery + kFzIasZone and exposed battery/voltage/alarm/tamper/battery_low
// — dropping the three measurement channels the "4-in-1" device exists to
// report and leaving the motion flag on a dead `alarm` key. z2m builds it from
// m.battery() + m.illuminance() + m.temperature() + m.humidity() +
// m.iasZoneAlarm({zoneType: "occupancy"}), exposing battery, illuminance,
// temperature, humidity, occupancy (+ tamper, battery_low). Wired the generic
// kFzIlluminance / kFzTemperature / kFzHumidity decoders and swapped kFzIasZone
// for the typed kFzIasMotionAlarm (ssIasZone bit 0 -> `occupancy`). Note the
// occupancy here rides ssIasZone (0x0500), NOT msOccupancySensing — mirrors
// z2m's iasZoneAlarm path. Generic temp/humidity are /100 and illuminance is
// raw, matching z2m's standard cluster converters.
// z2m-source: adeo.ts #83633205.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adeo {
namespace {
const FzConverter* const kFz_D83633205[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_D83633205[] = { "ZB-SMART-PIRTH-V3" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0400},  // msIlluminanceMeasurement
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0500},  // ssIasZone (occupancy)
};

extern const PreparedDefinition kDef_D83633205{
    .zigbee_models=kModels_D83633205, .zigbee_models_count=sizeof(kModels_D83633205)/sizeof(kModels_D83633205[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="83633205", .vendor="Adeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D83633205, .from_zigbee_count=sizeof(kFz_D83633205)/sizeof(kFz_D83633205[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::adeo
