// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Efekta EFEKTA_Smart_AQ_Box_R3 — auto-generated.
// EFEKTA Smart Air Quality Box, can control the relay, binding on some other devices
// z2m-source: efekta.ts #EFEKTA_Smart_AQ_Box_R3.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::efekta {
namespace {
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Smart_AQ_Box_R3_reading_delay{ "reading_delay", 0x0201, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Smart_AQ_Box_R3_reading_delay{
    .key="reading_delay", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Smart_AQ_Box_R3_reading_delay };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Smart_AQ_Box_R3_light_indicator_level{ "light_indicator_level", 0x0209, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Smart_AQ_Box_R3_light_indicator_level{
    .key="light_indicator_level", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Smart_AQ_Box_R3_light_indicator_level };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Smart_AQ_Box_R3_temperature_offset{ "temperature_offset", 0x0410, 0x29, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Smart_AQ_Box_R3_temperature_offset{
    .key="temperature_offset", .cluster="msTemperatureMeasurement",
    .cluster_id=0x0402, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Smart_AQ_Box_R3_temperature_offset };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Smart_AQ_Box_R3_humidity_offset{ "humidity_offset", 0x0210, 0x29, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Smart_AQ_Box_R3_humidity_offset{
    .key="humidity_offset", .cluster="msRelativeHumidity",
    .cluster_id=0x0405, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Smart_AQ_Box_R3_humidity_offset };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Smart_AQ_Box_R3_manual_forced_recalibration{ "manual_forced_recalibration", 0x0207, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Smart_AQ_Box_R3_manual_forced_recalibration{
    .key="manual_forced_recalibration", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Smart_AQ_Box_R3_manual_forced_recalibration };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Smart_AQ_Box_R3_high_co2_gas{ "high_co2_gas", 0x0221, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Smart_AQ_Box_R3_high_co2_gas{
    .key="high_co2_gas", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Smart_AQ_Box_R3_high_co2_gas };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Smart_AQ_Box_R3_low_co2_gas{ "low_co2_gas", 0x0222, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Smart_AQ_Box_R3_low_co2_gas{
    .key="low_co2_gas", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Smart_AQ_Box_R3_low_co2_gas };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Smart_AQ_Box_R3_high_voc{ "high_voc", 0x0221, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Smart_AQ_Box_R3_high_voc{
    .key="high_voc", .cluster="genAnalogInput",
    .cluster_id=0x000C, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Smart_AQ_Box_R3_high_voc };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Smart_AQ_Box_R3_low_voc{ "low_voc", 0x0222, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Smart_AQ_Box_R3_low_voc{
    .key="low_voc", .cluster="genAnalogInput",
    .cluster_id=0x000C, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Smart_AQ_Box_R3_low_voc };

const TzConverter* const kTz_EFEKTA_Smart_AQ_Box_R3[] = {
    &kTzWrite_EFEKTA_Smart_AQ_Box_R3_reading_delay,
    &kTzWrite_EFEKTA_Smart_AQ_Box_R3_light_indicator_level,
    &kTzWrite_EFEKTA_Smart_AQ_Box_R3_temperature_offset,
    &kTzWrite_EFEKTA_Smart_AQ_Box_R3_humidity_offset,
    &kTzWrite_EFEKTA_Smart_AQ_Box_R3_manual_forced_recalibration,
    &kTzWrite_EFEKTA_Smart_AQ_Box_R3_high_co2_gas,
    &kTzWrite_EFEKTA_Smart_AQ_Box_R3_low_co2_gas,
    &kTzWrite_EFEKTA_Smart_AQ_Box_R3_high_voc,
    &kTzWrite_EFEKTA_Smart_AQ_Box_R3_low_voc,
};
constexpr const char* kModels_EFEKTA_Smart_AQ_Box_R3[] = { "EFEKTA_Smart_AQ_Box_R3" };

}  // namespace


namespace {
// TODO(parity-efekta): no shared FzConverter for: co2,pm25
const FzConverter* const kFz_min_EFEKTA_Smart_AQ_Box_R3[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzPressure,
    &::zhc::generic::kFzTemperature,
};
constexpr Expose kExp_min_EFEKTA_Smart_AQ_Box_R3[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
    {"co2", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"pm25", ExposeType::Numeric, Access::State, "ug/m3", nullptr, nullptr, 0},
    {"pressure", ExposeType::Numeric, Access::State, "hPa", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
};
constexpr BindingSpec kBind_min_EFEKTA_Smart_AQ_Box_R3[] = {
    { 1, 0x0001 },
    { 1, 0x0006 },
    { 1, 0x0402 },
    { 1, 0x0403 },
    { 1, 0x0405 },
    { 1, 0x040D },
    { 1, 0x042A },
};
}
extern const PreparedDefinition kDef_EFEKTA_Smart_AQ_Box_R3{
    .zigbee_models=kModels_EFEKTA_Smart_AQ_Box_R3, .zigbee_models_count=sizeof(kModels_EFEKTA_Smart_AQ_Box_R3)/sizeof(kModels_EFEKTA_Smart_AQ_Box_R3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EFEKTA_Smart_AQ_Box_R3", .vendor="Efekta",
    .meta=nullptr, .exposes=kExp_min_EFEKTA_Smart_AQ_Box_R3, .exposes_count=sizeof(kExp_min_EFEKTA_Smart_AQ_Box_R3)/sizeof(kExp_min_EFEKTA_Smart_AQ_Box_R3[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_EFEKTA_Smart_AQ_Box_R3, .from_zigbee_count=sizeof(kFz_min_EFEKTA_Smart_AQ_Box_R3)/sizeof(kFz_min_EFEKTA_Smart_AQ_Box_R3[0]),
    .to_zigbee=kTz_EFEKTA_Smart_AQ_Box_R3, .to_zigbee_count=sizeof(kTz_EFEKTA_Smart_AQ_Box_R3)/sizeof(kTz_EFEKTA_Smart_AQ_Box_R3[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_EFEKTA_Smart_AQ_Box_R3,
    .bindings_count           = sizeof(kBind_min_EFEKTA_Smart_AQ_Box_R3)/sizeof(kBind_min_EFEKTA_Smart_AQ_Box_R3[0]),
};

}  // namespace zhc::devices::efekta
