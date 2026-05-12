// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Efekta EFEKTA_AQ_Smart_Monitor_Gen2 — auto-generated.
// CO2, VOC Index, illuminance Smart Monitor, rgb indicator, binding, control the relays
// z2m-source: efekta.ts #EFEKTA_AQ_Smart_Monitor_Gen2.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::efekta {
namespace {
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_night_on_backlight{ "night_on_backlight", 0x0405, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_night_on_backlight{
    .key="night_on_backlight", .cluster="msIlluminanceMeasurement",
    .cluster_id=0x0400, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_night_on_backlight };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_night_off_backlight{ "night_off_backlight", 0x0406, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_night_off_backlight{
    .key="night_off_backlight", .cluster="msIlluminanceMeasurement",
    .cluster_id=0x0400, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_night_off_backlight };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_reading_interval{ "reading_interval", 0x0201, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_reading_interval{
    .key="reading_interval", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_reading_interval };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_set_altitude{ "set_altitude", 0x0205, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_set_altitude{
    .key="set_altitude", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_set_altitude };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_temperature_offset{ "temperature_offset", 0x0210, 0x29, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_temperature_offset{
    .key="temperature_offset", .cluster="msTemperatureMeasurement",
    .cluster_id=0x0402, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_temperature_offset };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_humidity_offset{ "humidity_offset", 0x0210, 0x29, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_humidity_offset{
    .key="humidity_offset", .cluster="msRelativeHumidity",
    .cluster_id=0x0405, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_humidity_offset };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_manual_forced_recalibration{ "manual_forced_recalibration", 0x0207, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_manual_forced_recalibration{
    .key="manual_forced_recalibration", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_manual_forced_recalibration };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_high_co2_gas{ "high_co2_gas", 0x0221, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_high_co2_gas{
    .key="high_co2_gas", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_high_co2_gas };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_low_co2_gas{ "low_co2_gas", 0x0222, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_low_co2_gas{
    .key="low_co2_gas", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_low_co2_gas };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_high_voc{ "high_voc", 0x0221, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_high_voc{
    .key="high_voc", .cluster="genAnalogInput",
    .cluster_id=0x000C, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_high_voc };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_low_voc{ "low_voc", 0x0222, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_low_voc{
    .key="low_voc", .cluster="genAnalogInput",
    .cluster_id=0x000C, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_AQ_Smart_Monitor_Gen2_low_voc };
// TODO(parity-efekta): no shared FzConverter for: co2
const FzConverter* const kFz_EFEKTA_AQ_Smart_Monitor_Gen2[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzTemperature,
};
const TzConverter* const kTz_EFEKTA_AQ_Smart_Monitor_Gen2[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_night_on_backlight,
    &kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_night_off_backlight,
    &kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_reading_interval,
    &kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_set_altitude,
    &kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_temperature_offset,
    &kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_humidity_offset,
    &kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_manual_forced_recalibration,
    &kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_high_co2_gas,
    &kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_low_co2_gas,
    &kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_high_voc,
    &kTzWrite_EFEKTA_AQ_Smart_Monitor_Gen2_low_voc,
};
constexpr const char* kModels_EFEKTA_AQ_Smart_Monitor_Gen2[] = { "EFEKTA_AQ_Smart_Monitor_Gen2" };

}  // namespace


// --- auto-generated by emit_expose_bindings.py ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"co2", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0400},
    {1, 0x0402},
    {1, 0x0405},
    {1, 0x040D},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_EFEKTA_AQ_Smart_Monitor_Gen2{
    .zigbee_models=kModels_EFEKTA_AQ_Smart_Monitor_Gen2, .zigbee_models_count=sizeof(kModels_EFEKTA_AQ_Smart_Monitor_Gen2)/sizeof(kModels_EFEKTA_AQ_Smart_Monitor_Gen2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EFEKTA_AQ_Smart_Monitor_Gen2", .vendor="Efekta",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EFEKTA_AQ_Smart_Monitor_Gen2, .from_zigbee_count=sizeof(kFz_EFEKTA_AQ_Smart_Monitor_Gen2)/sizeof(kFz_EFEKTA_AQ_Smart_Monitor_Gen2[0]),
    .to_zigbee=kTz_EFEKTA_AQ_Smart_Monitor_Gen2, .to_zigbee_count=sizeof(kTz_EFEKTA_AQ_Smart_Monitor_Gen2)/sizeof(kTz_EFEKTA_AQ_Smart_Monitor_Gen2[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::efekta
