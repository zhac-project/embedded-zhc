// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Novato ZSN-03P temperature & humidity sensor (z2m v26.92.0 parity).
// z2m-source: tuya.ts #ZSN-03P (added v26.77.0)
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

// Enum tables mirror z2m's named lookups:
//   DP3  tuya.valueConverter.batteryState        -> {0 low, 1 medium, 2 high}
//   DP9  tuya.valueConverter.temperatureUnitEnum -> {0 celsius, 1 fahrenheit}
constexpr ::zhc::tuya::TuyaEnumEntry kEnumBatteryState_ZSN_03P[] = {
    {0, "low"}, {1, "medium"}, {2, "high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnumTempUnit_ZSN_03P[] = {
    {0, "celsius"}, {1, "fahrenheit"} };

// DP map mirrors z2m v26.92.0 meta.tuyaDatapoints for ZSN-03P. Divisors taken
// from the valueConverters on each triple: temperature + temperature_calibration
// = divideBy10, humidity + humidity_calibration = raw (÷1).
constexpr ::zhc::tuya::TuyaDpMapEntry kDpMap_ZSN_03P[] = {
    ::zhc::tuya::dp::temperature(1),
    ::zhc::tuya::dp::numeric(2, "humidity", 1),
    ::zhc::tuya::dp::enum_lookup(3, "battery_state", kEnumBatteryState_ZSN_03P,
                                 sizeof(kEnumBatteryState_ZSN_03P) / sizeof(kEnumBatteryState_ZSN_03P[0])),
    ::zhc::tuya::dp::enum_lookup(9, "temperature_unit", kEnumTempUnit_ZSN_03P,
                                 sizeof(kEnumTempUnit_ZSN_03P) / sizeof(kEnumTempUnit_ZSN_03P[0])),
    ::zhc::tuya::dp::numeric(23, "temperature_calibration", 10),
    ::zhc::tuya::dp::numeric(24, "humidity_calibration", 1),
};
constexpr ::zhc::tuya::TuyaDatapointMap kDpMapWrap_ZSN_03P{
    kDpMap_ZSN_03P, sizeof(kDpMap_ZSN_03P) / sizeof(kDpMap_ZSN_03P[0]) };

constexpr FzConverter kFzDp_ZSN_03P{
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
    .user_config       = &kDpMapWrap_ZSN_03P,
};
const FzConverter* const kFz_ZSN_03P[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_ZSN_03P,
};
constexpr TzConverter kTzDp_ZSN_03P{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kDpMapWrap_ZSN_03P,
};
const TzConverter* const kTz_ZSN_03P[] = { &kTzDp_ZSN_03P };

constexpr const char* kBatteryStateOpts_ZSN_03P[] = { "low", "medium", "high" };
constexpr const char* kTempUnitOpts_ZSN_03P[]     = { "celsius", "fahrenheit" };

constexpr Expose kExp_ZSN_03P[] = {
    { "temperature",             ExposeType::Numeric, ::zhc::Access::State,    "°C",    "Measured temperature",         nullptr, 0 },
    { "humidity",                ExposeType::Numeric, ::zhc::Access::State,    "%",     "Measured relative humidity",   nullptr, 0 },
    { "battery_state",           ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Battery state",                kBatteryStateOpts_ZSN_03P, 3 },
    { "temperature_unit",        ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Temperature unit",             kTempUnitOpts_ZSN_03P,     2 },
    { "temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Temperature calibration offset", nullptr, 0 },
    { "humidity_calibration",    ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Humidity calibration offset",  nullptr, 0 },
};

constexpr const char* kM_ZSN_03P[] = { "TS0601" };
constexpr const char* kN_ZSN_03P[] = { "_TZE284_rjjsib2d" };
}  // namespace

extern const PreparedDefinition kDef_ZSN_03P{
    .zigbee_models=kM_ZSN_03P,.zigbee_models_count=sizeof(kM_ZSN_03P)/sizeof(kM_ZSN_03P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_ZSN_03P,.manufacturer_names_count=sizeof(kN_ZSN_03P)/sizeof(kN_ZSN_03P[0]),
    .model="ZSN-03P",.vendor="Novato",
    .meta=nullptr,
    .exposes=kExp_ZSN_03P,.exposes_count=sizeof(kExp_ZSN_03P)/sizeof(kExp_ZSN_03P[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz_ZSN_03P,.from_zigbee_count=sizeof(kFz_ZSN_03P)/sizeof(kFz_ZSN_03P[0]),
    .to_zigbee=kTz_ZSN_03P,.to_zigbee_count=sizeof(kTz_ZSN_03P)/sizeof(kTz_ZSN_03P[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
