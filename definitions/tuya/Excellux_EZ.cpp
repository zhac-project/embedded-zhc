// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Excellux environment sensors (z2m v26.92.0 parity)
//   EZ-104UPT  AIRPRS1  UV / illuminance / pressure station  (added v26.88.0)
//   EZ-500FL   EZ500FL  soil probe: temp / humidity / fertility / moisture
//                                                            (added v26.91.0)
// z2m-source: tuya.ts #EZ-104UPT / #EZ-500FL
//
// FINGERPRINT NOTE: both report the SAME modelID "Excellux" and are told apart
// only by manufacturerName. Keep the manufacturer_names lists disjoint or the
// two defs will collide on the first match.
//
// Divisors read off each triple's valueConverter. The two families use
// different scales for the same-sounding keys, so they are NOT interchangeable:
//   EZ-104UPT: temperature ÷100, temperature_calibration/v0/v1 ÷100,
//              pressure + its calibration/v0/v1 ÷100, uv_level + uv_calibration
//              ÷10, illuminance + uv + battery raw.
//   EZ-500FL:  temperature ÷100 but probe_temperature + its calibration/v0/v1
//              ÷10, humidity + its calibration/v0/v1 ÷100, fertility + moisture
//              + their v0/v1 + moisture_calibration raw.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kWarnNoneLowHigh[] = { {0, "none"}, {1, "low"}, {2, "high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kUvWarning[]       = { {0, "Low"}, {1, "Moderate"}, {2, "High"},
                                                            {3, "Very High"}, {4, "Extreme"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPressureTend[]    = { {0, "normal"}, {1, "rise"}, {2, "fall"} };

// ---- EZ-104UPT ----------------------------------------------------------
struct cfg_104 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(4, "battery", 1),
        ::zhc::tuya::dp::temperature(5, 100),
        ::zhc::tuya::dp::numeric(20, "illuminance", 1),
        ::zhc::tuya::dp::numeric(101, "sampling_interval", 1),
        ::zhc::tuya::dp::numeric(104, "illuminance_v0", 1),
        ::zhc::tuya::dp::numeric(105, "illuminance_v1", 1),
        ::zhc::tuya::dp::numeric(106, "illuminance_calibration", 1),
        ::zhc::tuya::dp::enum_lookup(107, "illuminance_warning", kWarnNoneLowHigh, 3),
        ::zhc::tuya::dp::numeric(108, "uv", 1),
        ::zhc::tuya::dp::numeric(109, "uv_level", 10),
        ::zhc::tuya::dp::numeric(110, "uv_calibration", 10),
        ::zhc::tuya::dp::enum_lookup(111, "uv_warning", kUvWarning, 5),
        ::zhc::tuya::dp::numeric(114, "temperature_calibration", 100),
        ::zhc::tuya::dp::numeric(115, "temperature_v0_set", 100),
        ::zhc::tuya::dp::numeric(116, "temperature_v1_set", 100),
        ::zhc::tuya::dp::enum_lookup(117, "temperature_warning", kWarnNoneLowHigh, 3),
        ::zhc::tuya::dp::numeric(124, "pressure", 100),
        ::zhc::tuya::dp::numeric(125, "pressure_calibration", 100),
        ::zhc::tuya::dp::numeric(126, "pressure_v0_set", 100),
        ::zhc::tuya::dp::numeric(127, "pressure_v1_set", 100),
        ::zhc::tuya::dp::enum_lookup(128, "pressure_warn", kWarnNoneLowHigh, 3),
        ::zhc::tuya::dp::enum_lookup(129, "pressure_tend", kPressureTend, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX104 = ::zhc::tuya::factory::TuyaRw<cfg_104>;

// ---- EZ-500FL -----------------------------------------------------------
struct cfg_500 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "probe_temperature", 10),
        ::zhc::tuya::dp::numeric(3, "moisture", 1),
        ::zhc::tuya::dp::numeric(4, "battery", 1),
        ::zhc::tuya::dp::temperature(5, 100),
        ::zhc::tuya::dp::numeric(101, "sampling_interval", 1),
        ::zhc::tuya::dp::numeric(108, "probe_temperature_calibration", 10),
        ::zhc::tuya::dp::numeric(109, "probe_temperature_v0_set", 10),
        ::zhc::tuya::dp::numeric(110, "probe_temperature_v1_set", 10),
        ::zhc::tuya::dp::enum_lookup(112, "probe_temperature_warning", kWarnNoneLowHigh, 3),
        ::zhc::tuya::dp::numeric(114, "temperature_calibration", 100),
        ::zhc::tuya::dp::numeric(115, "temperature_v0_set", 100),
        ::zhc::tuya::dp::numeric(116, "temperature_v1_set", 100),
        ::zhc::tuya::dp::enum_lookup(117, "temperature_warning", kWarnNoneLowHigh, 3),
        ::zhc::tuya::dp::numeric(118, "humidity", 100),
        ::zhc::tuya::dp::numeric(119, "humidity_calibration", 100),
        ::zhc::tuya::dp::numeric(120, "humidity_v0_set", 100),
        ::zhc::tuya::dp::numeric(121, "humidity_v1_set", 100),
        ::zhc::tuya::dp::enum_lookup(122, "humidity_warning", kWarnNoneLowHigh, 3),
        ::zhc::tuya::dp::numeric(124, "fertility", 1),
        ::zhc::tuya::dp::numeric(125, "fertility_v0_set", 1),
        ::zhc::tuya::dp::numeric(126, "fertility_v1_set", 1),
        ::zhc::tuya::dp::enum_lookup(127, "fertility_warning", kWarnNoneLowHigh, 3),
        ::zhc::tuya::dp::numeric(129, "moisture_v0_set", 1),
        ::zhc::tuya::dp::numeric(130, "moisture_v1_set", 1),
        ::zhc::tuya::dp::numeric(131, "moisture_calibration", 1),
        ::zhc::tuya::dp::enum_lookup(132, "moisture_warning", kWarnNoneLowHigh, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX500 = ::zhc::tuya::factory::TuyaRw<cfg_500>;

constexpr const char* kWarnOpts[]         = { "none", "low", "high" };
constexpr const char* kUvWarningOpts[]    = { "Low", "Moderate", "High", "Very High", "Extreme" };
constexpr const char* kPressureTendOpts[] = { "normal", "rise", "fall" };

constexpr Expose kExp104[] = {
    { "temperature",             ExposeType::Numeric, ::zhc::Access::State,    "°C",    "Measured temperature",        nullptr,            0 },
    { "illuminance",             ExposeType::Numeric, ::zhc::Access::State,    "lx",    "Measured illuminance",        nullptr,            0 },
    { "uv",                      ExposeType::Numeric, ::zhc::Access::State,    nullptr, "Measured UV",                 nullptr,            0 },
    { "uv_level",                ExposeType::Numeric, ::zhc::Access::State,    nullptr, "UV index",                    nullptr,            0 },
    { "pressure",                ExposeType::Numeric, ::zhc::Access::State,    "hPa",   "Measured pressure",           nullptr,            0 },
    { "battery",                 ExposeType::Numeric, ::zhc::Access::State,    "%",     "Battery percentage",          nullptr,            0 },
    { "pressure_tend",           ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Pressure tendency",           kPressureTendOpts,  3 },
    { "illuminance_warning",     ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Illuminance warning",         kWarnOpts,          3 },
    { "uv_warning",              ExposeType::Enum,    ::zhc::Access::State,    nullptr, "UV warning",                  kUvWarningOpts,     5 },
    { "temperature_warning",     ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Temperature warning",         kWarnOpts,          3 },
    { "pressure_warn",           ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Pressure warning",            kWarnOpts,          3 },
    { "sampling_interval",       ExposeType::Numeric, ::zhc::Access::StateSet, "min",   "Sampling interval",           nullptr,            0 },
    { "illuminance_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Illuminance calibration",     nullptr,            0 },
    { "illuminance_v0",          ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Illuminance low threshold",   nullptr,            0 },
    { "illuminance_v1",          ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Illuminance high threshold",  nullptr,            0 },
    { "uv_calibration",          ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "UV calibration",              nullptr,            0 },
    { "temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Temperature calibration",     nullptr,            0 },
    { "temperature_v0_set",      ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Temperature low threshold",   nullptr,            0 },
    { "temperature_v1_set",      ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Temperature high threshold",  nullptr,            0 },
    { "pressure_calibration",    ExposeType::Numeric, ::zhc::Access::StateSet, "hPa",   "Pressure calibration",        nullptr,            0 },
    { "pressure_v0_set",         ExposeType::Numeric, ::zhc::Access::StateSet, "hPa",   "Pressure low threshold",      nullptr,            0 },
    { "pressure_v1_set",         ExposeType::Numeric, ::zhc::Access::StateSet, "hPa",   "Pressure high threshold",     nullptr,            0 },
};

constexpr Expose kExp500[] = {
    { "temperature",                   ExposeType::Numeric, ::zhc::Access::State,    "°C",    "Measured air temperature",     nullptr,   0 },
    { "probe_temperature",             ExposeType::Numeric, ::zhc::Access::State,    "°C",    "Measured probe temperature",   nullptr,   0 },
    { "humidity",                      ExposeType::Numeric, ::zhc::Access::State,    "%",     "Measured relative humidity",   nullptr,   0 },
    { "moisture",                      ExposeType::Numeric, ::zhc::Access::State,    "%",     "Soil moisture",                nullptr,   0 },
    { "fertility",                     ExposeType::Numeric, ::zhc::Access::State,    "us/cm", "Soil fertility",               nullptr,   0 },
    { "battery",                       ExposeType::Numeric, ::zhc::Access::State,    "%",     "Battery percentage",           nullptr,   0 },
    { "temperature_warning",           ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Temperature warning",          kWarnOpts, 3 },
    { "probe_temperature_warning",     ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Probe temperature warning",    kWarnOpts, 3 },
    { "humidity_warning",              ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Humidity warning",             kWarnOpts, 3 },
    { "fertility_warning",             ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Fertility warning",            kWarnOpts, 3 },
    { "moisture_warning",              ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Moisture warning",             kWarnOpts, 3 },
    { "sampling_interval",             ExposeType::Numeric, ::zhc::Access::StateSet, "min",   "Sampling interval",            nullptr,   0 },
    { "temperature_calibration",       ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Temperature calibration",      nullptr,   0 },
    { "temperature_v0_set",            ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Temperature low threshold",    nullptr,   0 },
    { "temperature_v1_set",            ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Temperature high threshold",   nullptr,   0 },
    { "probe_temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Probe temp. calibration",      nullptr,   0 },
    { "probe_temperature_v0_set",      ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Probe temp. low threshold",    nullptr,   0 },
    { "probe_temperature_v1_set",      ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Probe temp. high threshold",   nullptr,   0 },
    { "humidity_calibration",          ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Humidity calibration",         nullptr,   0 },
    { "humidity_v0_set",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Humidity low threshold",       nullptr,   0 },
    { "humidity_v1_set",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Humidity high threshold",      nullptr,   0 },
    { "fertility_v0_set",              ExposeType::Numeric, ::zhc::Access::StateSet, "us/cm", "Fertility low threshold",      nullptr,   0 },
    { "fertility_v1_set",              ExposeType::Numeric, ::zhc::Access::StateSet, "us/cm", "Fertility high threshold",     nullptr,   0 },
    { "moisture_calibration",          ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Moisture calibration",         nullptr,   0 },
    { "moisture_v0_set",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Moisture low threshold",       nullptr,   0 },
    { "moisture_v1_set",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Moisture high threshold",      nullptr,   0 },
};

constexpr const char* kM[]    = { "Excellux" };
constexpr const char* kN104[] = { "AIRPRS1" };
constexpr const char* kN500[] = { "EZ500FL" };
}  // namespace

extern const PreparedDefinition kDef_EZ_104UPT{
    .zigbee_models=kM,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN104,.manufacturer_names_count=1,
    .model="EZ-104UPT",.vendor="Excellux",
    .meta=nullptr,
    .exposes=kExp104,.exposes_count=sizeof(kExp104)/sizeof(kExp104[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX104::fz_list,.from_zigbee_count=FX104::fz_count,
    .to_zigbee=FX104::tz_list,.to_zigbee_count=FX104::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

extern const PreparedDefinition kDef_EZ_500FL{
    .zigbee_models=kM,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN500,.manufacturer_names_count=1,
    .model="EZ-500FL",.vendor="Excellux",
    .meta=nullptr,
    .exposes=kExp500,.exposes_count=sizeof(kExp500)/sizeof(kExp500[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX500::fz_list,.from_zigbee_count=FX500::fz_count,
    .to_zigbee=FX500::tz_list,.to_zigbee_count=FX500::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
