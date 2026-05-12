// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sylvania 74388 — hand-port (extends auto-generated baseline).
// Smart+ contact and temperature sensor.
// z2m-source: sylvania.ts #74388.
//
// z2m wiring: fz.ias_contact_alarm_1 + fz.temperature + fz.battery;
// configure binds msTemperatureMeasurement + genPowerCfg and reports
// temperature + batteryVoltage. The auto-generator missed the
// temperature axis — patched in here. `alarm` (key emitted by the
// generic IAS Zone Fz) maps to z2m's `contact` semantically; renaming
// is a generator-side concern (see PARITY_SUMMARY heiman row).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sylvania {
namespace {
const FzConverter* const kFz_D74388[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZone,
    &::zhc::generic::kFzTemperature,
};

constexpr const char* kModels_D74388[] = { "Contact Sensor-A" };

constexpr Expose kExposes_D74388[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"alarm",       ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D74388[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0500},
};

}  // namespace

extern const PreparedDefinition kDef_D74388{
    .zigbee_models=kModels_D74388, .zigbee_models_count=sizeof(kModels_D74388)/sizeof(kModels_D74388[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="74388", .vendor="Sylvania",
    .meta=nullptr,
    .exposes=kExposes_D74388, .exposes_count=sizeof(kExposes_D74388)/sizeof(kExposes_D74388[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D74388, .from_zigbee_count=sizeof(kFz_D74388)/sizeof(kFz_D74388[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D74388,
    .bindings_count=sizeof(kBindings_D74388)/sizeof(kBindings_D74388[0]),
};

}  // namespace zhc::devices::sylvania
