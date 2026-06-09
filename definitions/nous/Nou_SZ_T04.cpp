// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Nous SZ-T04 — graduated from generated/ to wire Tuya-DP decode.
// Temperature & humidity sensor with clock (TS0601, multiple TZE200 manufacturers).
// Streams every channel over manuSpecificTuya (0xEF00) DP, not the standard
// genPowerCfg/msTemperature clusters. The generated def carried kFzBattery
// (genPowerCfg) only, so temperature/humidity/battery never decoded. Wire the
// generic Tuya-DP decoder with the z2m legacy DP map.
// White-label: Tuya TH01Z (_TZE200_locansqn).
// z2m-source: nous.ts #SZ-T04 + lib/legacy.ts fz.nous_lcd_temperature_humidity_sensor.
//   DP 1=temperature/10, 2=humidity, 4=battery, 9=temperature_unit_convert,
//   10=max_temperature/10, 11=min_temperature/10, 12=max_humidity,
//   13=min_humidity, 14=temperature_alarm, 15=humidity_alarm,
//   17=temperature_report_interval, 18=humidity_report_interval,
//   19=temperature_sensitivity/10, 20=humidity_sensitivity.
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::nous {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kTempUnit_SZ_T04[] = {
    { 0, "celsius" }, { 1, "fahrenheit" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kAlarm_SZ_T04[] = {
    { 0, "lower_alarm" }, { 1, "upper_alarm" }, { 2, "canceled" },
};
constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_SZ_T04[] = {
    { 1,  "temperature",                 ::zhc::TuyaDpType::Numeric, 10, nullptr,          0, 0 },
    { 2,  "humidity",                    ::zhc::TuyaDpType::Numeric, 1,  nullptr,          0, 0 },
    { 4,  "battery",                     ::zhc::TuyaDpType::Numeric, 1,  nullptr,          0, 0 },
    { 9,  "temperature_unit_convert",    ::zhc::TuyaDpType::Enum,    0,  kTempUnit_SZ_T04, 2, 0 },
    { 10, "max_temperature",             ::zhc::TuyaDpType::Numeric, 10, nullptr,          0, 0 },
    { 11, "min_temperature",             ::zhc::TuyaDpType::Numeric, 10, nullptr,          0, 0 },
    { 12, "max_humidity",                ::zhc::TuyaDpType::Numeric, 1,  nullptr,          0, 0 },
    { 13, "min_humidity",                ::zhc::TuyaDpType::Numeric, 1,  nullptr,          0, 0 },
    { 14, "temperature_alarm",           ::zhc::TuyaDpType::Enum,    0,  kAlarm_SZ_T04,    3, 0 },
    { 15, "humidity_alarm",              ::zhc::TuyaDpType::Enum,    0,  kAlarm_SZ_T04,    3, 0 },
    { 17, "temperature_report_interval", ::zhc::TuyaDpType::Numeric, 1,  nullptr,          0, 0 },
    { 18, "humidity_report_interval",    ::zhc::TuyaDpType::Numeric, 1,  nullptr,          0, 0 },
    { 19, "temperature_sensitivity",     ::zhc::TuyaDpType::Numeric, 10, nullptr,          0, 0 },
    { 20, "humidity_sensitivity",        ::zhc::TuyaDpType::Numeric, 1,  nullptr,          0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_SZ_T04{
    kEntries_SZ_T04, sizeof(kEntries_SZ_T04) / sizeof(kEntries_SZ_T04[0]) };
constexpr FzConverter kFzDp_SZ_T04{
    .family            = FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
    .user_config       = &kMap_SZ_T04,
};
const FzConverter* const kFz_SZ_T04[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_SZ_T04,
};

constexpr const char* kModels_SZ_T04[] = { "TS0601" };
constexpr const char* kManus_SZ_T04[] = {
    "_TZE200_lve3dvpy", "_TZE200_c7emyjom", "_TZE200_locansqn", "_TZE200_qrztc3ev",
    "_TZE200_snloy4rw", "_TZE200_eanjj2pa", "_TZE200_ydrdfkim", "_TZE284_locansqn",
};

constexpr Expose kExp_SZ_T04[] = {
    { "temperature",                ExposeType::Numeric, ::zhc::Access::State,    "C",   nullptr, nullptr, 0 },
    { "humidity",                   ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0 },
    { "battery",                    ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0 },
    { "temperature_report_interval",ExposeType::Numeric, ::zhc::Access::StateSet, "min", nullptr, nullptr, 0 },
    { "humidity_report_interval",   ExposeType::Numeric, ::zhc::Access::StateSet, "min", nullptr, nullptr, 0 },
    { "max_temperature",            ExposeType::Numeric, ::zhc::Access::StateSet, "C",   nullptr, nullptr, 0 },
    { "min_temperature",            ExposeType::Numeric, ::zhc::Access::StateSet, "C",   nullptr, nullptr, 0 },
    { "max_humidity",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",   nullptr, nullptr, 0 },
    { "min_humidity",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",   nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_SZ_T04[] = {
    { 1, 0xEF00 },  // manuSpecificTuya
    { 1, 0x0000 },  // genBasic (z2m configure binds it)
};
constexpr TzConverter kTzDp_SZ_T04{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_SZ_T04,
};
const TzConverter* const kTz_SZ_T04[] = { &kTzDp_SZ_T04 };
}  // namespace


constexpr WhiteLabel kWhiteLabels_SZ_T04[] = {
    {"Tuya","TH01Z"},
};
extern const PreparedDefinition kDef_SZ_T04{
    .zigbee_models=kModels_SZ_T04, .zigbee_models_count=sizeof(kModels_SZ_T04)/sizeof(kModels_SZ_T04[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SZ_T04, .manufacturer_names_count=sizeof(kManus_SZ_T04)/sizeof(kManus_SZ_T04[0]),
    .model="SZ-T04", .vendor="Nous",
    .meta=nullptr, .exposes=kExp_SZ_T04, .exposes_count=sizeof(kExp_SZ_T04)/sizeof(kExp_SZ_T04[0]),
    .white_labels=kWhiteLabels_SZ_T04, .white_labels_count=sizeof(kWhiteLabels_SZ_T04)/sizeof(kWhiteLabels_SZ_T04[0]),
    .from_zigbee=kFz_SZ_T04, .from_zigbee_count=sizeof(kFz_SZ_T04)/sizeof(kFz_SZ_T04[0]),
    .to_zigbee=kTz_SZ_T04, .to_zigbee_count=sizeof(kTz_SZ_T04)/sizeof(kTz_SZ_T04[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_SZ_T04, .bindings_count=sizeof(kBind_SZ_T04)/sizeof(kBind_SZ_T04[0]),
};

}  // namespace zhc::devices::nous
