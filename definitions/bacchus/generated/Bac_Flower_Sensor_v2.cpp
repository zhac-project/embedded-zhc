// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bacchus Flower_Sensor_v2 — generic converters only.
// Soil moisture / temperature plant sensor.
// z2m-source: bacchus.ts #Flower_Sensor_v2.
//
// Coverage shape:
//   - kFzTemperature → msTemperatureMeasurement (0x0402) — °C.
//   - kFzBattery     → genPowerCfg (0x0001) — battery + voltage.
//
// Gap: Soil moisture (msSoilMoisture, cluster 0x0408) — there's no
// `kFzSoilMoisture` in Tier-1 generic. Listed in exposes for parity but
// not decoded today. Adding it to `_generic/_shared.{hpp,cpp}` is the
// natural next step (single-attr u16 % shape, mirrors kFzHumidity).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bacchus {
namespace {
const FzConverter* const kFz_Flower_Sensor_v2[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_Flower_Sensor_v2[] = { "Flower_Sensor_v2" };

constexpr Expose kExp_Flower_Sensor_v2[] = {
    { "soil_moisture", ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0 },
    { "temperature",   ExposeType::Numeric, ::zhc::Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "report_delay",  ExposeType::Numeric, ::zhc::Access::StateSet, "min", "Reporting interval", nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "threshold",     ExposeType::Numeric, ::zhc::Access::StateSet, "%",   "Reporting threshold", nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "battery",       ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",       ExposeType::Numeric, ::zhc::Access::State,    "mV",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
};

constexpr BindingSpec kBind_Flower_Sensor_v2[] = {
    { 1, 0x0001 }, // genPowerCfg
    { 1, 0x0402 }, // msTemperatureMeasurement
    { 1, 0x0408 }, // msSoilMoisture (declared for the join-time bind even though
                   //  Tier-1 has no decoder)
};

}  // namespace

extern const PreparedDefinition kDef_Flower_Sensor_v2{
    .zigbee_models=kModels_Flower_Sensor_v2, .zigbee_models_count=sizeof(kModels_Flower_Sensor_v2)/sizeof(kModels_Flower_Sensor_v2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Flower_Sensor_v2", .vendor="Bacchus",
    .meta=nullptr, .exposes=kExp_Flower_Sensor_v2, .exposes_count=sizeof(kExp_Flower_Sensor_v2)/sizeof(kExp_Flower_Sensor_v2[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Flower_Sensor_v2, .from_zigbee_count=sizeof(kFz_Flower_Sensor_v2)/sizeof(kFz_Flower_Sensor_v2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_Flower_Sensor_v2, .bindings_count=sizeof(kBind_Flower_Sensor_v2)/sizeof(kBind_Flower_Sensor_v2[0]),
};

}  // namespace zhc::devices::bacchus
