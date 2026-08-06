// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya ZF24Pro 24GHz presence sensor with temperature & humidity
// (z2m v26.92.0 parity, added v26.88.0).
// z2m-source: tuya.ts #ZF24Pro
//
// Divisors read off each triple's valueConverter:
//   DP4  detection_distance_max  divideBy100   (metres)
//   DP9  distance                divideBy100   (metres)
//   DP22 temperature             divideBy10
//   DP110 temperature_correction divideBy10
//   everything else              raw (÷1)
// DP104/105-109 are the scene toggles z2m exposes as plain switches
// (state / living_room / bedroom / bathroom / sleep / radar_switch).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {

struct cfg_ZF24Pro {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "presence"),
        ::zhc::tuya::dp::numeric(2, "move_sensitivity", 1),
        ::zhc::tuya::dp::numeric(4, "detection_distance_max", 100),
        ::zhc::tuya::dp::numeric(9, "distance", 100),
        ::zhc::tuya::dp::temperature(22),
        ::zhc::tuya::dp::numeric(23, "humidity", 1),
        ::zhc::tuya::dp::numeric(101, "presence_timeout", 1),
        ::zhc::tuya::dp::numeric(102, "illuminance", 1),
        ::zhc::tuya::dp::numeric(103, "presence_sensitivity", 1),
        ::zhc::tuya::dp::binary(104, "state"),
        ::zhc::tuya::dp::binary(105, "living_room"),
        ::zhc::tuya::dp::binary(106, "bedroom"),
        ::zhc::tuya::dp::binary(107, "bathroom"),
        ::zhc::tuya::dp::binary(108, "sleep"),
        ::zhc::tuya::dp::binary(109, "radar_switch"),
        ::zhc::tuya::dp::numeric(110, "temperature_correction", 10),
        ::zhc::tuya::dp::numeric(111, "humidity_correction", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_ZF24Pro>;

constexpr Expose kExp[] = {
    { "presence",               ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Presence state",                nullptr, 0 },
    { "distance",               ExposeType::Numeric, ::zhc::Access::State,    "m",     "Target distance",               nullptr, 0 },
    { "temperature",            ExposeType::Numeric, ::zhc::Access::State,    "°C",    "Measured temperature",          nullptr, 0 },
    { "humidity",               ExposeType::Numeric, ::zhc::Access::State,    "%",     "Measured relative humidity",    nullptr, 0 },
    { "illuminance",            ExposeType::Numeric, ::zhc::Access::State,    "lx",    "Measured illuminance",          nullptr, 0 },
    { "move_sensitivity",       ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Motion detection sensitivity",  nullptr, 0 },
    { "presence_sensitivity",   ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Presence detection sensitivity", nullptr, 0 },
    { "detection_distance_max", ExposeType::Numeric, ::zhc::Access::StateSet, "m",     "Maximum detection distance",    nullptr, 0 },
    { "presence_timeout",       ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Presence keep time",            nullptr, 0 },
    { "temperature_correction", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Temperature calibration offset", nullptr, 0 },
    { "humidity_correction",    ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Humidity calibration offset",   nullptr, 0 },
    { "state",                  ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Sensor enable",                 nullptr, 0 },
    { "radar_switch",           ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Radar enable",                  nullptr, 0 },
    { "living_room",            ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Living-room scene preset",      nullptr, 0 },
    { "bedroom",                ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Bedroom scene preset",          nullptr, 0 },
    { "bathroom",               ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Bathroom scene preset",         nullptr, 0 },
    { "sleep",                  ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Sleep scene preset",            nullptr, 0 },
};

constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE28C1000000_vosmoqsg", "_TZE28C1000000_ewn672ef" };
}  // namespace

extern const PreparedDefinition kDef_ZF24Pro{
    .zigbee_models=kM,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZF24Pro",.vendor="Tuya",
    .meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
