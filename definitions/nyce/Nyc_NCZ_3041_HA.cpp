// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Nyce NCZ-3041-HA — wall motion sensor (graduated from generated/).
// Wall motion sensor
// z2m-source: nyce.ts #NCZ-3041-HA —
//   fromZigbee:[fz.occupancy, fz.humidity, fz.temperature, fz.battery,
//               fz.ias_occupancy_alarm_2],
//   exposes:[e.occupancy(), e.humidity(), e.temperature(), e.battery(),
//            e.battery_low(), e.tamper()].
//
// Fix: identical to NCZ-3043-HA — the generated def dropped occupancy/
// temperature/humidity and read the wrong IAS bit. Re-wired msOccupancySensing
// 0x0406 (fz.occupancy) + ssIasZone alarm_2/bit 1 (kFzIasMotionAlarm2) +
// temperature 0x0402 + humidity 0x0405 + battery, and corrected exposes.
//
// NOTE: z2m sets meta {battery:{dontDividePercentage:true}} for this model
// (device reports 0-100 directly, not the Zigbee 0-200 scale). The shared
// kFzBattery always halves batteryPercentageRemaining, so the reported %
// reads 2x low here. This is a vendor-wide modeling gap (kFzBattery has no
// per-device no-divide path; see vesternet which also leaves it unmodelled)
// — deferred, not fixed in this pass.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nyce {
namespace {
const FzConverter* const kFz_NCZ_3041_HA[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIasMotionAlarm2,
};

constexpr const char* kModels_NCZ_3041_HA[] = { "3041" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0405},
    {1, 0x0406},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_NCZ_3041_HA{
    .zigbee_models=kModels_NCZ_3041_HA, .zigbee_models_count=sizeof(kModels_NCZ_3041_HA)/sizeof(kModels_NCZ_3041_HA[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NCZ-3041-HA", .vendor="Nyce",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_NCZ_3041_HA, .from_zigbee_count=sizeof(kFz_NCZ_3041_HA)/sizeof(kFz_NCZ_3041_HA[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nyce
