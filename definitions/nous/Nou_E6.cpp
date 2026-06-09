// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Nous E6 — graduated from generated/ to wire Tuya-DP decode.
// Temperature & humidity LCD sensor (TS0601, multiple TZE manufacturers).
// This device streams every channel over manuSpecificTuya (0xEF00) DP, not
// the standard genPowerCfg/msTemperature clusters. The generated def carried
// kFzBattery (genPowerCfg) only, so temperature/humidity/battery never
// decoded. Wire the generic Tuya-DP decoder with the z2m legacy DP map.
// z2m-source: nous.ts #E6 + lib/legacy.ts fz.nous_lcd_temperature_humidity_sensor.
//   DP 1=temperature/10, 2=humidity, 4=battery, 9=temperature_unit_convert,
//   10=max_temperature/10, 11=min_temperature/10, 12=max_humidity,
//   13=min_humidity, 14=temperature_alarm, 15=humidity_alarm.
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::nous {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kTempUnit_E6[] = {
    { 0, "celsius" }, { 1, "fahrenheit" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kAlarm_E6[] = {
    { 0, "lower_alarm" }, { 1, "upper_alarm" }, { 2, "canceled" },
};
constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_E6[] = {
    { 1,  "temperature",              ::zhc::TuyaDpType::Numeric, 10, nullptr,      0, 0 },
    { 2,  "humidity",                 ::zhc::TuyaDpType::Numeric, 1,  nullptr,      0, 0 },
    { 4,  "battery",                  ::zhc::TuyaDpType::Numeric, 1,  nullptr,      0, 0 },
    { 9,  "temperature_unit_convert", ::zhc::TuyaDpType::Enum,    0,  kTempUnit_E6, 2, 0 },
    { 10, "max_temperature",          ::zhc::TuyaDpType::Numeric, 10, nullptr,      0, 0 },
    { 11, "min_temperature",          ::zhc::TuyaDpType::Numeric, 10, nullptr,      0, 0 },
    { 12, "max_humidity",             ::zhc::TuyaDpType::Numeric, 1,  nullptr,      0, 0 },
    { 13, "min_humidity",             ::zhc::TuyaDpType::Numeric, 1,  nullptr,      0, 0 },
    { 14, "temperature_alarm",        ::zhc::TuyaDpType::Enum,    0,  kAlarm_E6,    3, 0 },
    { 15, "humidity_alarm",           ::zhc::TuyaDpType::Enum,    0,  kAlarm_E6,    3, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_E6{
    kEntries_E6, sizeof(kEntries_E6) / sizeof(kEntries_E6[0]) };
constexpr FzConverter kFzDp_E6{
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
    .user_config       = &kMap_E6,
};
const FzConverter* const kFz_E6[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_E6,
};

constexpr const char* kModels_E6[] = { "TS0601" };
constexpr const char* kManus_E6[] = {
    "_TZE284_wtikaxzs", "_TZE200_nnrfa68v", "_TZE200_zppcgbdj", "_TZE200_wtikaxzs",
};

constexpr Expose kExp_E6[] = {
    { "temperature",     ExposeType::Numeric, ::zhc::Access::State,    "C", nullptr, nullptr, 0 },
    { "humidity",        ExposeType::Numeric, ::zhc::Access::State,    "%", nullptr, nullptr, 0 },
    { "battery",         ExposeType::Numeric, ::zhc::Access::State,    "%", nullptr, nullptr, 0 },
    { "battery_low",     ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "max_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "min_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "max_humidity",    ExposeType::Numeric, ::zhc::Access::StateSet, "%", nullptr, nullptr, 0 },
    { "min_humidity",    ExposeType::Numeric, ::zhc::Access::StateSet, "%", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_E6[] = {
    { 1, 0xEF00 },  // manuSpecificTuya
};
constexpr TzConverter kTzDp_E6{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_E6,
};
const TzConverter* const kTz_E6[] = { &kTzDp_E6 };
}  // namespace

extern const PreparedDefinition kDef_E6{
    .zigbee_models=kModels_E6, .zigbee_models_count=sizeof(kModels_E6)/sizeof(kModels_E6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_E6, .manufacturer_names_count=sizeof(kManus_E6)/sizeof(kManus_E6[0]),
    .model="E6", .vendor="Nous",
    .meta=nullptr, .exposes=kExp_E6, .exposes_count=sizeof(kExp_E6)/sizeof(kExp_E6[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E6, .from_zigbee_count=sizeof(kFz_E6)/sizeof(kFz_E6[0]),
    .to_zigbee=kTz_E6, .to_zigbee_count=sizeof(kTz_E6)/sizeof(kTz_E6[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_E6, .bindings_count=sizeof(kBind_E6)/sizeof(kBind_E6[0]),
};

}  // namespace zhc::devices::nous
