// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: ONENUO TH05Z (Tuya ZTH05Z family) temperature + humidity sensor,
// manufacturer `_TZE2841000000_qf5mzewi` — z2m v26.99.0 parity.
//
// Upstream appended this manufacturer to the shared ZTH05Z fingerprint, but
// the entry then BRANCHES on it at runtime: z2m's `exposes` callback and DP
// table both special-case this firmware batch. ZHC resolves variants at match
// time rather than decode time, so the batch gets its own definition instead
// of a conditional inside the shared one.
//
// What differs from the other four ZTH05Z manufacturers (which stay on their
// generated defs, e.g. Gen__TZE200_vvmbj46n.cpp):
//   - DP18 `humidity_periodic_report` is NEVER reported by this batch
//     (upstream note: "tested empirically"), so it is omitted entirely.
//   - DP23 `temperature_calibration` (÷10) and DP24 `humidity_calibration`
//     (raw) exist ONLY here.
//   - Sensitivity ranges are wider: temperature 0.6..2.0 °C (vs 0.3..1.0),
//     humidity 6..20 % (vs 3..10).
//
// z2m-source: tuya.ts #ZTH05Z (the `_TZE2841000000_qf5mzewi` branch).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnumBatteryState[] = {
    { 0, "low" }, { 1, "medium" }, { 2, "high" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnumTemperatureUnit[] = {
    { 0, "celsius" }, { 1, "fahrenheit" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnumAlarm[] = {
    { 0, "lower_alarm" }, { 1, "upper_alarm" }, { 2, "cancel" },
};

struct cfg_onenuo {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "temperature", 10),
        ::zhc::tuya::dp::numeric(2, "humidity", 1),
        ::zhc::tuya::dp::enum_lookup(3, "battery_state", kEnumBatteryState, 3),
        ::zhc::tuya::dp::numeric(4, "battery", 1),
        ::zhc::tuya::dp::enum_lookup(9, "temperature_unit", kEnumTemperatureUnit, 2),
        ::zhc::tuya::dp::numeric(10, "max_temperature_alarm", 10),
        ::zhc::tuya::dp::numeric(11, "min_temperature_alarm", 10),
        ::zhc::tuya::dp::numeric(12, "max_humidity_alarm", 1),
        ::zhc::tuya::dp::numeric(13, "min_humidity_alarm", 1),
        ::zhc::tuya::dp::enum_lookup(14, "temperature_alarm", kEnumAlarm, 3),
        ::zhc::tuya::dp::enum_lookup(15, "humidity_alarm", kEnumAlarm, 3),
        ::zhc::tuya::dp::numeric(17, "temperature_periodic_report", 1),
        // DP18 (humidity_periodic_report) deliberately absent — see header.
        ::zhc::tuya::dp::numeric(19, "temperature_sensitivity", 10),
        ::zhc::tuya::dp::numeric(20, "humidity_sensitivity", 1),
        // DP23/24 exist only on this batch.
        ::zhc::tuya::dp::numeric(23, "temperature_calibration", 10),
        ::zhc::tuya::dp::numeric(24, "humidity_calibration", 1),
    };
    // sizeof-derived, never a literal: a literal count here silently ignores
    // appended rows (and reads past the end when it over-counts).
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXOnenuo = ::zhc::tuya::factory::TuyaRw<cfg_onenuo>;

constexpr const char* kUnitOpts[]  = { "celsius", "fahrenheit" };
constexpr const char* kAlarmOpts[] = { "lower_alarm", "upper_alarm", "cancel" };
constexpr const char* kBattOpts[]  = { "low", "medium", "high" };

constexpr Expose kExposes_Onenuo[] = {
    { "temperature",                 ExposeType::Numeric, ::zhc::Access::State,    "C",   "Temperature",                 nullptr, 0 },
    { "humidity",                    ExposeType::Numeric, ::zhc::Access::State,    "%",   "Humidity",                    nullptr, 0 },
    { "battery",                     ExposeType::Numeric, ::zhc::Access::State,    "%",   "Battery",                     nullptr, 0,
      ExposeCategory::Diagnostic },
    { "battery_state",               ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Battery state",             kBattOpts, 3,
      ExposeCategory::Diagnostic },
    { "temperature_unit",            ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Temperature unit",          kUnitOpts, 2,
      ExposeCategory::Config },
    { "max_temperature_alarm",       ExposeType::Numeric, ::zhc::Access::StateSet, "C",   "Alarm temperature max",       nullptr, 0,
      ExposeCategory::Config, -20, 60, 1 },
    { "min_temperature_alarm",       ExposeType::Numeric, ::zhc::Access::StateSet, "C",   "Alarm temperature min",       nullptr, 0,
      ExposeCategory::Config, -20, 60, 1 },
    { "max_humidity_alarm",          ExposeType::Numeric, ::zhc::Access::StateSet, "%",   "Alarm humidity max",          nullptr, 0,
      ExposeCategory::Config, 0, 100, 1 },
    { "min_humidity_alarm",          ExposeType::Numeric, ::zhc::Access::StateSet, "%",   "Alarm humidity min",          nullptr, 0,
      ExposeCategory::Config, 0, 100, 1 },
    { "temperature_alarm",           ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Temperature alarm",         kAlarmOpts, 3 },
    { "humidity_alarm",              ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Humidity alarm",            kAlarmOpts, 3 },
    { "temperature_periodic_report", ExposeType::Numeric, ::zhc::Access::StateSet, "min", "Temp periodic report",        nullptr, 0,
      ExposeCategory::Config, 1, 120, 1 },
    // Wider than the other ZTH05Z batches: 0.6..2.0 C and 6..20 %. Bounds are
    // stored pre-scaled the way the DP divisor delivers them (÷10 -> 6..20).
    { "temperature_sensitivity",     ExposeType::Numeric, ::zhc::Access::StateSet, "C",   "Sensitivity of temperature",  nullptr, 0,
      ExposeCategory::Config, 6, 20, 1 },
    { "humidity_sensitivity",        ExposeType::Numeric, ::zhc::Access::StateSet, "%",   "Sensitivity of humidity",     nullptr, 0,
      ExposeCategory::Config, 6, 20, 1 },
    { "temperature_calibration",     ExposeType::Numeric, ::zhc::Access::StateSet, "C",   "Temperature calibration",     nullptr, 0,
      ExposeCategory::Config },
    { "humidity_calibration",        ExposeType::Numeric, ::zhc::Access::StateSet, "%",   "Humidity calibration",        nullptr, 0,
      ExposeCategory::Config },
};

constexpr const char* kModels_Onenuo[] = { "TS0601" };
constexpr const char* kManus_Onenuo[]  = { "_TZE2841000000_qf5mzewi" };

constexpr WhiteLabel kWhiteLabels_Onenuo[] = {
    { "ONENUO", "TH05Z" },
};

}  // namespace

extern const PreparedDefinition kDef_ZTH05Z_onenuo{
    .zigbee_models=kModels_Onenuo,
    .zigbee_models_count=sizeof(kModels_Onenuo)/sizeof(kModels_Onenuo[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_Onenuo,
    .manufacturer_names_count=sizeof(kManus_Onenuo)/sizeof(kManus_Onenuo[0]),
    .model="ZTH05Z", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes_Onenuo,
    .exposes_count=sizeof(kExposes_Onenuo)/sizeof(kExposes_Onenuo[0]),
    .white_labels=kWhiteLabels_Onenuo,
    .white_labels_count=sizeof(kWhiteLabels_Onenuo)/sizeof(kWhiteLabels_Onenuo[0]),
    .from_zigbee=FXOnenuo::fz_list, .from_zigbee_count=FXOnenuo::fz_count,
    .to_zigbee=FXOnenuo::tz_list, .to_zigbee_count=FXOnenuo::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
