// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 CO2 / temperature / humidity sensor
// (`_TZE204_pkpfn9hc`) — z2m v26.99.0 parity (new upstream).
//
// Three datapoints, no config surface. Note dp19 humidity is `raw` (whole
// percent) while dp18 temperature is `divideBy10` — the pairing differs from
// most Tuya TH sensors, which scale both, so the divisors are taken from
// upstream rather than from the family default.
//
// z2m-source: tuya.ts #TS0601_co2_temperature_humidity_sensor.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

struct cfg_co2_th {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(2, "co2", 1),
        ::zhc::tuya::dp::numeric(18, "temperature", 10),
        ::zhc::tuya::dp::numeric(19, "humidity", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXCo2Th = ::zhc::tuya::factory::TuyaRw<cfg_co2_th>;

constexpr Expose kExp_Co2Th[] = {
    { "co2",         ExposeType::Numeric, ::zhc::Access::State, "ppm", "Measured CO2 concentration", nullptr, 0 },
    { "temperature", ExposeType::Numeric, ::zhc::Access::State, "°C",  "Measured temperature",       nullptr, 0 },
    { "humidity",    ExposeType::Numeric, ::zhc::Access::State, "%",   "Measured humidity",          nullptr, 0 },
};

constexpr const char* kM_Co2Th[] = { "TS0601" };
constexpr const char* kN_Co2Th[] = { "_TZE204_pkpfn9hc" };

}  // namespace

extern const PreparedDefinition kDef_TS0601_co2_th{
    .zigbee_models=kM_Co2Th, .zigbee_models_count=sizeof(kM_Co2Th)/sizeof(kM_Co2Th[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_Co2Th, .manufacturer_names_count=sizeof(kN_Co2Th)/sizeof(kN_Co2Th[0]),
    .model="TS0601_co2_temperature_humidity_sensor", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExp_Co2Th, .exposes_count=sizeof(kExp_Co2Th)/sizeof(kExp_Co2Th[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXCo2Th::fz_list, .from_zigbee_count=FXCo2Th::fz_count,
    .to_zigbee=FXCo2Th::tz_list, .to_zigbee_count=FXCo2Th::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
