// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bacchus Flower_Sensor_v4 — generic converters only.
// Soil moisture / temperature / illuminance plant sensor.
// z2m-source: bacchus.ts #Flower_Sensor_v4.
//
// Coverage shape:
//   - kFzTemperature → msTemperatureMeasurement (0x0402) — °C.
//   - kFzIlluminance → msIlluminanceMeasurement (0x0400) — lx.
//   - kFzBattery     → genPowerCfg (0x0001) — battery + voltage.
//
// Gap: msSoilMoisture (0x0408) — same as Flower_Sensor_v2; not in Tier-1
// generic. The v4 firmware adds manu battery attrs 0x0204/0x0205 on
// genPowerCfg which the generic kFzBattery does not surface either.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bacchus {
namespace {
const FzConverter* const kFz_Flower_Sensor_v4[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_Flower_Sensor_v4[] = { "Flower_Sensor_v4" };

constexpr Expose kExp_Flower_Sensor_v4[] = {
    { "soil_moisture", ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0 },
    { "temperature",   ExposeType::Numeric, ::zhc::Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "illuminance",   ExposeType::Numeric, ::zhc::Access::State,    "lx",  nullptr, nullptr, 0 },
    { "report_delay",  ExposeType::Numeric, ::zhc::Access::StateSet, "min", "Reporting interval",  nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "threshold",     ExposeType::Numeric, ::zhc::Access::StateSet, "%",   "Reporting threshold", nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "battery",       ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",       ExposeType::Numeric, ::zhc::Access::State,    "mV",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
};

constexpr BindingSpec kBind_Flower_Sensor_v4[] = {
    { 1, 0x0001 },
    { 1, 0x0400 },
    { 1, 0x0402 },
    { 1, 0x0408 },
};

}  // namespace

extern const PreparedDefinition kDef_Flower_Sensor_v4{
    .zigbee_models=kModels_Flower_Sensor_v4, .zigbee_models_count=sizeof(kModels_Flower_Sensor_v4)/sizeof(kModels_Flower_Sensor_v4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Flower_Sensor_v4", .vendor="Bacchus",
    .meta=nullptr, .exposes=kExp_Flower_Sensor_v4, .exposes_count=sizeof(kExp_Flower_Sensor_v4)/sizeof(kExp_Flower_Sensor_v4[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Flower_Sensor_v4, .from_zigbee_count=sizeof(kFz_Flower_Sensor_v4)/sizeof(kFz_Flower_Sensor_v4[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_Flower_Sensor_v4, .bindings_count=sizeof(kBind_Flower_Sensor_v4)/sizeof(kBind_Flower_Sensor_v4[0]),
};

}  // namespace zhc::devices::bacchus
