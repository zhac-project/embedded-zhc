// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Datek HSE2927E — hand-upgraded for full z2m parity.
// Eva motion sensor — battery + temperature + IAS motion + mfg-spec
//                     led_on_motion (ssIasZone 0x4000) + standard
//                     occupancy_timeout (msOccupancySensing 0x0010).
// z2m-source: datek.ts #HSE2927E.
// Mfg-spec writes route through tz_zcl_write_attr with mfg=0x1337.
#include "definitions/_generic/_shared.hpp"
#include "definitions/datek/_shared.hpp"

namespace zhc::devices::datek {
namespace {
const FzConverter* const kFz_HSE2927E[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasMotionAlarm,
    &::zhc::datek::kFzLedOnMotion,
};
const TzConverter* const kTz_HSE2927E[] = {
    &::zhc::datek::kTzOccupancyTimeout,
    &::zhc::datek::kTzLedOnMotion,
};

constexpr const char* kModels_HSE2927E[] = { "Motion Sensor" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"led_on_motion", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"occupancy_timeout", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0406},  // msOccupancySensing
    {1, 0x0500},  // ssIasZone
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_HSE2927E{
    .zigbee_models=kModels_HSE2927E, .zigbee_models_count=sizeof(kModels_HSE2927E)/sizeof(kModels_HSE2927E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HSE2927E", .vendor="Datek",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HSE2927E, .from_zigbee_count=sizeof(kFz_HSE2927E)/sizeof(kFz_HSE2927E[0]),
    .to_zigbee=kTz_HSE2927E, .to_zigbee_count=sizeof(kTz_HSE2927E)/sizeof(kTz_HSE2927E[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::datek
