// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0601_temperature_humidity_sensor_2 — auto-generated.
// Temperature and humidity sensor
// z2m-source: tuya.ts #TS0601_temperature_humidity_sensor_2.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0601_temperature_humidity_sensor_2[] = { "TS0601" };
constexpr const char* kManus_TS0601_temperature_humidity_sensor_2[] = { "_TZE200_yjjdcqsq", "_TZE284_9yapgbuv", "_TZE200_9yapgbuv", "_TZE200_utkemkbs", "_TZE204_utkemkbs", "_TZE284_utkemkbs", "_TZE204_9yapgbuv", "_TZE204_upagmta9", "_TZE200_cirvgep4", "_TZE204_d7lpruvi", "_TZE200_upagmta9", "_TZE204_yjjdcqsq", "_TZE204_jygvp6fk", "_TZE204_cirvgep4", "_TZE284_yjjdcqsq", "_TZE200_d7lpruvi", "_TZE284_hdyjyqjm", "_TZE284_d7lpruvi", "_TZE284_upagmta9", "_TZE204_ksz749x8", "_TZE204_1wnh8bqp", "_TZE284_1wnh8bqp" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TS0601_temperature_humidity_sensor_2_Tuy_TS0601_temperature_[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0601_temperature_humidity_sensor_2_Tuy_TS0601_temperature_[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0601_temperature_humidity_sensor_2_Tuy_TS0601_temperature_[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0601_temperature_humidity_sensor_2{
    .zigbee_models=kModels_TS0601_temperature_humidity_sensor_2, .zigbee_models_count=sizeof(kModels_TS0601_temperature_humidity_sensor_2)/sizeof(kModels_TS0601_temperature_humidity_sensor_2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0601_temperature_humidity_sensor_2, .manufacturer_names_count=sizeof(kManus_TS0601_temperature_humidity_sensor_2)/sizeof(kManus_TS0601_temperature_humidity_sensor_2[0]),
    .model="TS0601_temperature_humidity_sensor_2", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0601_temperature_humidity_sensor_2_Tuy_TS0601_temperature_, .exposes_count=sizeof(kExp_min_TS0601_temperature_humidity_sensor_2_Tuy_TS0601_temperature_)/sizeof(kExp_min_TS0601_temperature_humidity_sensor_2_Tuy_TS0601_temperature_[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0601_temperature_humidity_sensor_2_Tuy_TS0601_temperature_, .from_zigbee_count=sizeof(kFz_min_TS0601_temperature_humidity_sensor_2_Tuy_TS0601_temperature_)/sizeof(kFz_min_TS0601_temperature_humidity_sensor_2_Tuy_TS0601_temperature_[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
