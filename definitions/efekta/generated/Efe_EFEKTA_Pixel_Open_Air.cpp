// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Efekta EFEKTA_Pixel_Open_Air — auto-generated.
// CO2 Monitor with retro LCD Display, V1
// z2m-source: efekta.ts #EFEKTA_Pixel_Open_Air.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::efekta {
namespace {
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Pixel_Open_Air_night_on_backlight{ "night_on_backlight", 0x0405, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Pixel_Open_Air_night_on_backlight{
    .key="night_on_backlight", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Pixel_Open_Air_night_on_backlight };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Pixel_Open_Air_night_off_backlight{ "night_off_backlight", 0x0406, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Pixel_Open_Air_night_off_backlight{
    .key="night_off_backlight", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Pixel_Open_Air_night_off_backlight };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Pixel_Open_Air_contrast{ "contrast", 0x0445, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Pixel_Open_Air_contrast{
    .key="contrast", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Pixel_Open_Air_contrast };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Pixel_Open_Air_temperature_offset{ "temperature_offset", 0x0210, 0x29, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Pixel_Open_Air_temperature_offset{
    .key="temperature_offset", .cluster="msTemperatureMeasurement",
    .cluster_id=0x0402, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Pixel_Open_Air_temperature_offset };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Pixel_Open_Air_humidity_offset{ "humidity_offset", 0x0210, 0x29, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Pixel_Open_Air_humidity_offset{
    .key="humidity_offset", .cluster="msRelativeHumidity",
    .cluster_id=0x0405, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Pixel_Open_Air_humidity_offset };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Pixel_Open_Air_high_gas{ "high_gas", 0x0221, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Pixel_Open_Air_high_gas{
    .key="high_gas", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Pixel_Open_Air_high_gas };
constexpr ::zhc::generic::ZclWriteSpec kWriteSpec_EFEKTA_Pixel_Open_Air_low_gas{ "low_gas", 0x0222, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzWrite_EFEKTA_Pixel_Open_Air_low_gas{
    .key="low_gas", .cluster="msCO2",
    .cluster_id=0x040D, .command_id=0x02,
    .fn=::zhc::generic::tz_zcl_write_attr,
    .user_config=&kWriteSpec_EFEKTA_Pixel_Open_Air_low_gas };

const TzConverter* const kTz_EFEKTA_Pixel_Open_Air[] = {
    &kTzWrite_EFEKTA_Pixel_Open_Air_night_on_backlight,
    &kTzWrite_EFEKTA_Pixel_Open_Air_night_off_backlight,
    &kTzWrite_EFEKTA_Pixel_Open_Air_contrast,
    &kTzWrite_EFEKTA_Pixel_Open_Air_temperature_offset,
    &kTzWrite_EFEKTA_Pixel_Open_Air_humidity_offset,
    &kTzWrite_EFEKTA_Pixel_Open_Air_high_gas,
    &kTzWrite_EFEKTA_Pixel_Open_Air_low_gas,
};
constexpr const char* kModels_EFEKTA_Pixel_Open_Air[] = { "EFEKTA_Pixel_Open_Air" };

}  // namespace


namespace {
// TODO(parity-efekta): no shared FzConverter for: co2
const FzConverter* const kFz_min_EFEKTA_Pixel_Open_Air[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzTemperature,
};
constexpr Expose kExp_min_EFEKTA_Pixel_Open_Air[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
    {"co2", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
};
constexpr BindingSpec kBind_min_EFEKTA_Pixel_Open_Air[] = {
    { 1, 0x0001 },
    { 1, 0x0006 },
    { 1, 0x0402 },
    { 1, 0x0405 },
    { 1, 0x040D },
};
}
extern const PreparedDefinition kDef_EFEKTA_Pixel_Open_Air{
    .zigbee_models=kModels_EFEKTA_Pixel_Open_Air, .zigbee_models_count=sizeof(kModels_EFEKTA_Pixel_Open_Air)/sizeof(kModels_EFEKTA_Pixel_Open_Air[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EFEKTA_Pixel_Open_Air", .vendor="Efekta",
    .meta=nullptr, .exposes=kExp_min_EFEKTA_Pixel_Open_Air, .exposes_count=sizeof(kExp_min_EFEKTA_Pixel_Open_Air)/sizeof(kExp_min_EFEKTA_Pixel_Open_Air[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_EFEKTA_Pixel_Open_Air, .from_zigbee_count=sizeof(kFz_min_EFEKTA_Pixel_Open_Air)/sizeof(kFz_min_EFEKTA_Pixel_Open_Air[0]),
    .to_zigbee=kTz_EFEKTA_Pixel_Open_Air, .to_zigbee_count=sizeof(kTz_EFEKTA_Pixel_Open_Air)/sizeof(kTz_EFEKTA_Pixel_Open_Air[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_EFEKTA_Pixel_Open_Air,
    .bindings_count           = sizeof(kBind_min_EFEKTA_Pixel_Open_Air)/sizeof(kBind_min_EFEKTA_Pixel_Open_Air[0]),
};

}  // namespace zhc::devices::efekta
