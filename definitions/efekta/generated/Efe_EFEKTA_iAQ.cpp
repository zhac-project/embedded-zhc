// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Efekta EFEKTA_iAQ — auto-generated.
// CO2 Monitor with IPS TFT Display, outdoor temperature and humidity, date and time
// z2m-source: efekta.ts #EFEKTA_iAQ.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::efekta {
namespace {
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_iAQ_reading_interval{ "reading_interval", 0x0201, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_iAQ_reading_interval{
    .key="reading_interval", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_iAQ_reading_interval };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_iAQ_night_on_backlight{ "night_on_backlight", 0x0405, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_iAQ_night_on_backlight{
    .key="night_on_backlight", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_iAQ_night_on_backlight };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_iAQ_night_off_backlight{ "night_off_backlight", 0x0406, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_iAQ_night_off_backlight{
    .key="night_off_backlight", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_iAQ_night_off_backlight };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_iAQ_set_altitude{ "set_altitude", 0x0205, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_iAQ_set_altitude{
    .key="set_altitude", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_iAQ_set_altitude };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_iAQ_temperature_offset{ "temperature_offset", 0x0210, 0x29, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_iAQ_temperature_offset{
    .key="temperature_offset", .cluster="msTemperatureMeasurement",
    .cluster_id=0x0402, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_iAQ_temperature_offset };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_iAQ_humidity_offset{ "humidity_offset", 0x0210, 0x29, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_iAQ_humidity_offset{
    .key="humidity_offset", .cluster="msRelativeHumidity",
    .cluster_id=0x0405, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_iAQ_humidity_offset };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_iAQ_manual_forced_recalibration{ "manual_forced_recalibration", 0x0207, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_iAQ_manual_forced_recalibration{
    .key="manual_forced_recalibration", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_iAQ_manual_forced_recalibration };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_iAQ_high_gas{ "high_gas", 0x0221, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_iAQ_high_gas{
    .key="high_gas", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_iAQ_high_gas };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_iAQ_low_gas{ "low_gas", 0x0222, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_iAQ_low_gas{
    .key="low_gas", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_iAQ_low_gas };

const TzConverter* const kTz_EFEKTA_iAQ[] = {
    &kTzWrite_EFEKTA_iAQ_reading_interval,
    &kTzWrite_EFEKTA_iAQ_night_on_backlight,
    &kTzWrite_EFEKTA_iAQ_night_off_backlight,
    &kTzWrite_EFEKTA_iAQ_set_altitude,
    &kTzWrite_EFEKTA_iAQ_temperature_offset,
    &kTzWrite_EFEKTA_iAQ_humidity_offset,
    &kTzWrite_EFEKTA_iAQ_manual_forced_recalibration,
    &kTzWrite_EFEKTA_iAQ_high_gas,
    &kTzWrite_EFEKTA_iAQ_low_gas,
};
constexpr const char* kModels_EFEKTA_iAQ[] = { "EFEKTA_iAQ" };

constexpr ::zhc::EndpointLabel kEndpoints_EFEKTA_iAQ[] = { {"1", 1}, {"2", 2} };

}  // namespace


namespace {
// TODO(parity-efekta): no shared FzConverter for: co2
const FzConverter* const kFz_min_EFEKTA_iAQ[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzTemperature,
};
constexpr Expose kExp_min_EFEKTA_iAQ[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
    {"co2", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
};
constexpr BindingSpec kBind_min_EFEKTA_iAQ[] = {
    { 1, 0x0001 },
    { 1, 0x0006 },
    { 1, 0x0400 },
    { 1, 0x0402 },
    { 1, 0x0405 },
    { 1, 0x040D },
};
}
extern const PreparedDefinition kDef_EFEKTA_iAQ{
    .zigbee_models=kModels_EFEKTA_iAQ, .zigbee_models_count=sizeof(kModels_EFEKTA_iAQ)/sizeof(kModels_EFEKTA_iAQ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EFEKTA_iAQ", .vendor="Efekta",
    .meta=nullptr, .exposes=kExp_min_EFEKTA_iAQ, .exposes_count=sizeof(kExp_min_EFEKTA_iAQ)/sizeof(kExp_min_EFEKTA_iAQ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_EFEKTA_iAQ, .from_zigbee_count=sizeof(kFz_min_EFEKTA_iAQ)/sizeof(kFz_min_EFEKTA_iAQ[0]),
    .to_zigbee=kTz_EFEKTA_iAQ, .to_zigbee_count=sizeof(kTz_EFEKTA_iAQ)/sizeof(kTz_EFEKTA_iAQ[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_EFEKTA_iAQ,
    .bindings_count           = sizeof(kBind_min_EFEKTA_iAQ)/sizeof(kBind_min_EFEKTA_iAQ[0]),
    .endpoint_map       = kEndpoints_EFEKTA_iAQ,
    .endpoint_map_count = sizeof(kEndpoints_EFEKTA_iAQ)/sizeof(kEndpoints_EFEKTA_iAQ[0]),
};

}  // namespace zhc::devices::efekta
