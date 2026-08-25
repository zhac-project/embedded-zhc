// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Excellux EZ-500FS soil probe — z2m v26.97.0 parity (new upstream).
//
// The EZ-500FL's sibling: same split soil probe, same datapoint numbering,
// minus the fertility/conductivity half (no dp124-128 fertility group). Kept
// in its own file rather than folded into Excellux_EZ.cpp so the two probes'
// datapoint tables stay independently readable — they drift apart upstream.
//
// Divisors are copied from upstream one by one rather than assumed from the
// FL: dp3 moisture is `raw` here, dp131 moisture_calibration is `divideBy100`
// (the FL carried `raw` for dp131 until v26.99.0 corrected it).
//
// z2m-source: tuya.ts #EZ-500FS.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kWarnFS[] = { {0, "none"}, {1, "low"}, {2, "high"} };

struct cfg_500fs {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "probe_temperature", 10),
        ::zhc::tuya::dp::numeric(3, "moisture", 1),
        ::zhc::tuya::dp::numeric(4, "battery", 1),
        ::zhc::tuya::dp::temperature(5, 100),
        ::zhc::tuya::dp::numeric(101, "sampling_interval", 1),
        ::zhc::tuya::dp::numeric(108, "probe_temperature_calibration", 10),
        ::zhc::tuya::dp::numeric(109, "probe_temperature_v0_set", 10),
        ::zhc::tuya::dp::numeric(110, "probe_temperature_v1_set", 10),
        ::zhc::tuya::dp::enum_lookup(112, "probe_temperature_warning", kWarnFS, 3),
        ::zhc::tuya::dp::numeric(114, "temperature_calibration", 100),
        ::zhc::tuya::dp::numeric(115, "temperature_v0_set", 100),
        ::zhc::tuya::dp::numeric(116, "temperature_v1_set", 100),
        ::zhc::tuya::dp::enum_lookup(117, "temperature_warning", kWarnFS, 3),
        ::zhc::tuya::dp::numeric(118, "humidity", 100),
        ::zhc::tuya::dp::numeric(119, "humidity_calibration", 100),
        ::zhc::tuya::dp::numeric(120, "humidity_v0_set", 100),
        ::zhc::tuya::dp::numeric(121, "humidity_v1_set", 100),
        ::zhc::tuya::dp::enum_lookup(122, "humidity_warning", kWarnFS, 3),
        ::zhc::tuya::dp::numeric(129, "moisture_v0_set", 1),
        ::zhc::tuya::dp::numeric(130, "moisture_v1_set", 1),
        ::zhc::tuya::dp::numeric(131, "moisture_calibration", 100),
        ::zhc::tuya::dp::enum_lookup(132, "moisture_warning", kWarnFS, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX500FS = ::zhc::tuya::factory::TuyaRw<cfg_500fs>;

constexpr const char* kWarnOptsFS[] = { "none", "low", "high" };

constexpr Expose kExp500FS[] = {
    { "temperature",                   ExposeType::Numeric, ::zhc::Access::State,    "°C", "Measured air temperature",    nullptr,     0 },
    { "humidity",                      ExposeType::Numeric, ::zhc::Access::State,    "%",  "Measured air humidity",       nullptr,     0 },
    { "moisture",                      ExposeType::Numeric, ::zhc::Access::State,    "%",  "Soil moisture",               nullptr,     0,
      ExposeCategory::State, 0, 100, 1 },
    { "probe_temperature",             ExposeType::Numeric, ::zhc::Access::State,    "°C", "Probe temperature sensor",    nullptr,     0,
      ExposeCategory::State, -40, 120, 1 },
    { "battery",                       ExposeType::Numeric, ::zhc::Access::State,    "%",  "Battery level",               nullptr,     0,
      ExposeCategory::Diagnostic },
    { "probe_temperature_warning",     ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Probe temperature warning", kWarnOptsFS, 3 },
    { "temperature_warning",           ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Temperature warning",       kWarnOptsFS, 3 },
    { "humidity_warning",              ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Humidity warning",          kWarnOptsFS, 3 },
    { "moisture_warning",              ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Moisture warning",          kWarnOptsFS, 3 },
    { "sampling_interval",             ExposeType::Numeric, ::zhc::Access::StateSet, "s",  "Sampling interval",           nullptr,     0,
      ExposeCategory::Config, 5, 1200, 5 },
    { "probe_temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, "°C", "Probe temperature calibration", nullptr,   0,
      ExposeCategory::Config },
    { "probe_temperature_v0_set",      ExposeType::Numeric, ::zhc::Access::StateSet, "°C", "Probe temperature low threshold",  nullptr, 0,
      ExposeCategory::Config },
    { "probe_temperature_v1_set",      ExposeType::Numeric, ::zhc::Access::StateSet, "°C", "Probe temperature high threshold", nullptr, 0,
      ExposeCategory::Config },
    { "temperature_calibration",       ExposeType::Numeric, ::zhc::Access::StateSet, "°C", "Temperature calibration",     nullptr,     0,
      ExposeCategory::Config },
    { "temperature_v0_set",            ExposeType::Numeric, ::zhc::Access::StateSet, "°C", "Temperature low threshold",   nullptr,     0,
      ExposeCategory::Config },
    { "temperature_v1_set",            ExposeType::Numeric, ::zhc::Access::StateSet, "°C", "Temperature high threshold",  nullptr,     0,
      ExposeCategory::Config },
    { "humidity_calibration",          ExposeType::Numeric, ::zhc::Access::StateSet, "%",  "Humidity calibration",        nullptr,     0,
      ExposeCategory::Config },
    { "humidity_v0_set",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",  "Humidity low threshold",      nullptr,     0,
      ExposeCategory::Config },
    { "humidity_v1_set",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",  "Humidity high threshold",     nullptr,     0,
      ExposeCategory::Config },
    { "moisture_calibration",          ExposeType::Numeric, ::zhc::Access::StateSet, "%",  "Moisture calibration",        nullptr,     0,
      ExposeCategory::Config },
    { "moisture_v0_set",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",  "Moisture low threshold",      nullptr,     0,
      ExposeCategory::Config },
    { "moisture_v1_set",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",  "Moisture high threshold",     nullptr,     0,
      ExposeCategory::Config },
};

constexpr const char* kM_FS[] = { "Excellux" };
constexpr const char* kN_FS[] = { "EZ500FS" };

}  // namespace

extern const PreparedDefinition kDef_EZ_500FS{
    .zigbee_models=kM_FS, .zigbee_models_count=sizeof(kM_FS)/sizeof(kM_FS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_FS, .manufacturer_names_count=sizeof(kN_FS)/sizeof(kN_FS[0]),
    .model="EZ-500FS", .vendor="Excellux",
    .meta=nullptr,
    .exposes=kExp500FS, .exposes_count=sizeof(kExp500FS)/sizeof(kExp500FS[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX500FS::fz_list, .from_zigbee_count=FX500FS::fz_count,
    .to_zigbee=FX500FS::tz_list, .to_zigbee_count=FX500FS::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
