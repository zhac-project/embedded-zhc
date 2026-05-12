// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0601_temperature_humidity_co2_sensor — auto-generated.
// CO2 sensor
// z2m-source: tuya.ts #TS0601_temperature_humidity_co2_sensor.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0601_temperature_humidity_co2_sensor[] = { "TS0601" };
constexpr const char* kManus_TS0601_temperature_humidity_co2_sensor[] = { "_TZE200_ogkdpgy2", "_TZE200_3ejwxpmu", "_TZE204_3ejwxpmu" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TS0601_temperature_humidity_co2_sensor_Tuy_TS0601_temperatur[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0601_temperature_humidity_co2_sensor_Tuy_TS0601_temperatur[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0601_temperature_humidity_co2_sensor_Tuy_TS0601_temperatur[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0601_temperature_humidity_co2_sensor{
    .zigbee_models=kModels_TS0601_temperature_humidity_co2_sensor, .zigbee_models_count=sizeof(kModels_TS0601_temperature_humidity_co2_sensor)/sizeof(kModels_TS0601_temperature_humidity_co2_sensor[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0601_temperature_humidity_co2_sensor, .manufacturer_names_count=sizeof(kManus_TS0601_temperature_humidity_co2_sensor)/sizeof(kManus_TS0601_temperature_humidity_co2_sensor[0]),
    .model="TS0601_temperature_humidity_co2_sensor", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0601_temperature_humidity_co2_sensor_Tuy_TS0601_temperatur, .exposes_count=sizeof(kExp_min_TS0601_temperature_humidity_co2_sensor_Tuy_TS0601_temperatur)/sizeof(kExp_min_TS0601_temperature_humidity_co2_sensor_Tuy_TS0601_temperatur[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0601_temperature_humidity_co2_sensor_Tuy_TS0601_temperatur, .from_zigbee_count=sizeof(kFz_min_TS0601_temperature_humidity_co2_sensor_Tuy_TS0601_temperatur)/sizeof(kFz_min_TS0601_temperature_humidity_co2_sensor_Tuy_TS0601_temperatur[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
