// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HOBEIAN ZG-308Z water valve (z2m v26.115.0, #13340). The lookups are over
// plain numbers upstream (no tuya.enum) — kTuyaDpFlagNumericLookup.
// z2m-source: hobeian.ts #ZG-308Z.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kWeatherDelay10[] = { {0,"cancel"}, {1,"hour_12h"}, {2,"hour_24h"}, {3,"hour_48h"}, {4,"hour_72h"} };
constexpr ::zhc::tuya::TuyaEnumEntry kValveStatus12[] = { {0,"auto"}, {1,"manual"}, {2,"idle"} };
constexpr ::zhc::tuya::TuyaEnumEntry kCurrentWeather13[] = { {0,"sunny"}, {1,"clear"}, {2,"cloud"}, {3,"cloudy"}, {4,"rainy"}, {5,"snow"}, {6,"fog"} };
constexpr ::zhc::tuya::TuyaEnumEntry kWeatherStatus101[] = { {0,"sunny"}, {1,"cloudy"}, {2,"rainy"}, {3,"snow"}, {4,"null"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "switch"),
        ::zhc::tuya::dp::numeric(7, "battery", 1),
        ::zhc::tuya::dp::numeric(9, "total_irrigation_duration", 1),
        { 10, "weather_delay", ::zhc::TuyaDpType::Numeric, 1, kWeatherDelay10, 5, ::zhc::tuya::kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::numeric(11, "countdown", 1),
        { 12, "valve_status", ::zhc::TuyaDpType::Numeric, 1, kValveStatus12, 3, ::zhc::tuya::kTuyaDpFlagNumericLookup },
        { 13, "current_weather", ::zhc::TuyaDpType::Numeric, 1, kCurrentWeather13, 7, ::zhc::tuya::kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::binary(14, "weather_onoff"),
        ::zhc::tuya::dp::numeric(15, "valve_duration", 1),
        { 101, "weather_status", ::zhc::TuyaDpType::Numeric, 1, kWeatherStatus101, 5, ::zhc::tuya::kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::binary(102, "get_weather"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts1[] = { "auto", "manual", "idle" };
constexpr const char* kOpts5[] = { "cancel", "hour_12h", "hour_24h", "hour_48h", "hour_72h" };
constexpr const char* kOpts6[] = { "sunny", "clear", "cloud", "cloudy", "rainy", "snow", "fog" };
constexpr const char* kOpts9[] = { "sunny", "cloudy", "rainy", "snow", "null" };
constexpr Expose kExp[] = {
    {"switch", ExposeType::Binary, Access::StateSet, nullptr, "Valve on/off", nullptr, 0},
    {"valve_status", ExposeType::Enum, Access::State, nullptr, "Valve 1 status (manual, auto, idle)", kOpts1, 3},
    {"countdown", ExposeType::Numeric, Access::StateSet, "s", "Valve countdown in seconds", nullptr, 0, ExposeCategory::State, 0, 86400, 0},
    {"valve_duration", ExposeType::Numeric, Access::State, "s", "Valve irrigation last duration in seconds", nullptr, 0},
    {"total_irrigation_duration", ExposeType::Numeric, Access::State, "s", nullptr, nullptr, 0},
    {"weather_delay", ExposeType::Enum, Access::StateSet, nullptr, "Weather delay: No operation when raining", kOpts5, 5},
    {"current_weather", ExposeType::Enum, Access::StateSet, nullptr, "Weather status needs to be sent to the device", kOpts6, 7},
    {"weather_onoff", ExposeType::Binary, Access::StateSet, nullptr, "smart weather_onoff on/off", nullptr, 0},
    {"get_weather", ExposeType::Binary, Access::State, nullptr, "The device requests weather data from the gateway", nullptr, 0},
    {"weather_status", ExposeType::Enum, Access::State, nullptr, "Weather information feedback received", kOpts9, 5},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "ZG-308Z" };
}  // namespace

extern const PreparedDefinition kDef_ZG_308Z{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="ZG-308Z",.vendor="HOBEIAN",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
