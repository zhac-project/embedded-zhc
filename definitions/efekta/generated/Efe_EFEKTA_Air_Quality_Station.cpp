// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Efekta EFEKTA_Air_Quality_Station — auto-generated.
// Air quality station
// z2m-source: efekta.ts #EFEKTA_Air_Quality_Station.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::efekta {
namespace {
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Air_Quality_Station_night_on_backlight{ "night_on_backlight", 0x0405, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Air_Quality_Station_night_on_backlight{
    .key="night_on_backlight", .cluster="msIlluminanceMeasurement",
    .cluster_id=0x0400, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Air_Quality_Station_night_on_backlight };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Air_Quality_Station_night_off_backlight{ "night_off_backlight", 0x0406, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Air_Quality_Station_night_off_backlight{
    .key="night_off_backlight", .cluster="msIlluminanceMeasurement",
    .cluster_id=0x0400, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Air_Quality_Station_night_off_backlight };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Air_Quality_Station_temperature_offset{ "temperature_offset", 0x0210, 0x29, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Air_Quality_Station_temperature_offset{
    .key="temperature_offset", .cluster="msTemperatureMeasurement",
    .cluster_id=0x0402, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Air_Quality_Station_temperature_offset };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Air_Quality_Station_humidity_offset{ "humidity_offset", 0x0210, 0x29, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Air_Quality_Station_humidity_offset{
    .key="humidity_offset", .cluster="msRelativeHumidity",
    .cluster_id=0x0405, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Air_Quality_Station_humidity_offset };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Air_Quality_Station_set_altitude{ "set_altitude", 0x0205, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Air_Quality_Station_set_altitude{
    .key="set_altitude", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Air_Quality_Station_set_altitude };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Air_Quality_Station_manual_forced_recalibration{ "manual_forced_recalibration", 0x0207, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Air_Quality_Station_manual_forced_recalibration{
    .key="manual_forced_recalibration", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Air_Quality_Station_manual_forced_recalibration };

const TzConverter* const kTz_EFEKTA_Air_Quality_Station[] = {
    &kTzWrite_EFEKTA_Air_Quality_Station_night_on_backlight,
    &kTzWrite_EFEKTA_Air_Quality_Station_night_off_backlight,
    &kTzWrite_EFEKTA_Air_Quality_Station_temperature_offset,
    &kTzWrite_EFEKTA_Air_Quality_Station_humidity_offset,
    &kTzWrite_EFEKTA_Air_Quality_Station_set_altitude,
    &kTzWrite_EFEKTA_Air_Quality_Station_manual_forced_recalibration,
};
constexpr const char* kModels_EFEKTA_Air_Quality_Station[] = { "EFEKTA_Air_Quality_Station" };

}  // namespace


namespace {
// TODO(parity-efekta): no shared FzConverter for: co2,pm25
const FzConverter* const kFz_min_EFEKTA_Air_Quality_Station[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzTemperature,
};
constexpr Expose kExp_min_EFEKTA_Air_Quality_Station[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
    {"co2", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"pm25", ExposeType::Numeric, Access::State, "ug/m3", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
};
constexpr BindingSpec kBind_min_EFEKTA_Air_Quality_Station[] = {
    { 1, 0x0001 },
    { 1, 0x0006 },
    { 1, 0x0400 },
    { 1, 0x0402 },
    { 1, 0x0405 },
    { 1, 0x040D },
    { 1, 0x042A },
};
}
extern const PreparedDefinition kDef_EFEKTA_Air_Quality_Station{
    .zigbee_models=kModels_EFEKTA_Air_Quality_Station, .zigbee_models_count=sizeof(kModels_EFEKTA_Air_Quality_Station)/sizeof(kModels_EFEKTA_Air_Quality_Station[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EFEKTA_Air_Quality_Station", .vendor="Efekta",
    .meta=nullptr, .exposes=kExp_min_EFEKTA_Air_Quality_Station, .exposes_count=sizeof(kExp_min_EFEKTA_Air_Quality_Station)/sizeof(kExp_min_EFEKTA_Air_Quality_Station[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_EFEKTA_Air_Quality_Station, .from_zigbee_count=sizeof(kFz_min_EFEKTA_Air_Quality_Station)/sizeof(kFz_min_EFEKTA_Air_Quality_Station[0]),
    .to_zigbee=kTz_EFEKTA_Air_Quality_Station, .to_zigbee_count=sizeof(kTz_EFEKTA_Air_Quality_Station)/sizeof(kTz_EFEKTA_Air_Quality_Station[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_EFEKTA_Air_Quality_Station,
    .bindings_count           = sizeof(kBind_min_EFEKTA_Air_Quality_Station)/sizeof(kBind_min_EFEKTA_Air_Quality_Station[0]),
};

}  // namespace zhc::devices::efekta
