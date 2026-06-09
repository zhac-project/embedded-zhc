// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bacchus Flower_Sensor_v2 — generic converters + soil moisture.
// Soil moisture / temperature plant sensor.
// z2m-source: bacchus.ts #Flower_Sensor_v2 (extend: [m.soilMoisture(),
// m.temperature(), ..., m.battery()]).
//
// Coverage shape:
//   - kFzSoilMoisture → msSoilMoisture (0x0408) measuredValue, u16 /100 → %.
//   - kFzTemperature  → msTemperatureMeasurement (0x0402) — °C.
//   - kFzBattery      → genPowerCfg (0x0001) — battery + voltage.
//
// Bug fixed (parity pass): `soil_moisture` is the headline measurement
// (z2m `m.soilMoisture()` on msSoilMoisture 0x0408), but the generated
// Tier-1 def only wired kFzTemperature + kFzBattery, so every soil report
// silently dropped. kFzSoilMoisture now exists in _generic/_shared (added
// for the sibling custom_devices_diy/diyruz/efekta flower sensors) — wired
// here. Graduated generated/ → parent to do so.
//
// Still not decoded (manu attrs, not a parity gap): report_delay /
// threshold are msSoilMoisture manu attrs 0x0202/0x0203 (write-side,
// need a vendor TZ helper); listed in exposes for discoverability.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bacchus {
namespace {
const FzConverter* const kFz_Flower_Sensor_v2[] = {
    &::zhc::generic::kFzSoilMoisture,
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
