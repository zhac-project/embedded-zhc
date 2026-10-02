// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_heat_meter ultrasonic heat meter (_TZE200_jt50ea5d),
// z2m v26.115.1 parity. Graduated from tuya/generated/Gen__TZE200_jt50ea5d.cpp.
//
// z2m v26.110.0 (#13184) corrected two datapoint ids: the heat-metering switch
// `prepayment_switch` is DP7 (was DP6) and `cumulative_heat` is DP8 (was DP7),
// so the generated copy published the switch as heat and lost the heat value.
// Completed on the way to upstream's table:
//   * DP2/DP3 monthly/daily water: valueConverter.waterConsumption, a Raw
//     payload whose bytes 4..7 are a BE u32 / 1000 (the copy read them as a
//     number, which a Raw payload never is);
//   * DP5 `fault`: bitmap names joined with ", ", "OK" when clear;
//   * DP19 `instantaneous_flow_rate`: Raw BE u32 at 0 / 1000;
//   * DP24 is `voltage` (×10, e.battery_voltage()), it was `battery_voltage`.
// z2m-source: tuya.ts #TS0601_heat_meter.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kPeriod[] = {
    {0,"1h"}, {1,"2h"}, {2,"3h"}, {3,"4h"}, {4,"6h"}, {5,"8h"}, {6,"12h"}, {7,"24h"}, {8,"48h"}, {9,"72h"} };
constexpr const char* kFaults[] = {
    "battery_alarm", "magnetism_alarm", "cover_alarm", "credit_alarm", "switch_gaps_alarm",
    "meter_body_alarm", "abnormal_water_alarm", "arrearage_alarm", "overflow_alarm",
    "revflow_alarm", "over_pre_alarm", "empty_pip_alarm", "transduce_alarm" };
constexpr ::zhc::tuya::TuyaFaultTable kFaultTable{ kFaults, sizeof(kFaults)/sizeof(kFaults[0]), ", ", "OK" };
constexpr ::zhc::tuya::TuyaRawU32Spec kWater{ 4, 8, 1000 };
constexpr ::zhc::tuya::TuyaRawU32Spec kFlow{ 0, 4, 1000 };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "water_consumed", 1000),
        ::zhc::tuya::dp::raw_u32(2, "monthly_water_consumption", &kWater),
        ::zhc::tuya::dp::raw_u32(3, "daily_water_consumption", &kWater),
        ::zhc::tuya::dp::enum_lookup(4, "report_period", kPeriod, 10),
        ::zhc::tuya::dp::fault_bitmap(5, "fault", &kFaultTable),
        ::zhc::tuya::dp::binary(7, "prepayment_switch"),
        ::zhc::tuya::dp::numeric(8, "cumulative_heat", 100),
        ::zhc::tuya::dp::numeric(16, "meter_id", 1),
        ::zhc::tuya::dp::raw_u32(19, "instantaneous_flow_rate", &kFlow),
        ::zhc::tuya::dp::numeric(21, "inlet_water_temperature", 100),
        ::zhc::tuya::dp::numeric(22, "outlet_water_temperature", 100),
        ::zhc::tuya::dp::numeric(24, "voltage", -10),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kPeriodOpts[] = { "1h", "2h", "3h", "4h", "6h", "8h", "12h", "24h", "48h", "72h" };
constexpr Expose kExp[] = {
    {"water_consumed",            ExposeType::Numeric, Access::State,    "m³",   "Total water consumption", nullptr, 0},
    {"monthly_water_consumption", ExposeType::Numeric, Access::State,    "m³",   "Monthly water consumption", nullptr, 0},
    {"daily_water_consumption",   ExposeType::Numeric, Access::State,    "m³",   "Daily water consumption", nullptr, 0},
    {"report_period",             ExposeType::Enum,    Access::StateSet, nullptr, "Report period", kPeriodOpts, 10},
    {"fault",                     ExposeType::String,  Access::State,    nullptr, "Alarm event status", nullptr, 0},
    {"prepayment_switch",         ExposeType::Binary,  Access::StateSet, nullptr, "Cumulative metering switch", nullptr, 0},
    {"cumulative_heat",           ExposeType::Numeric, Access::State,    "kWh",  "Cumulative heat", nullptr, 0},
    {"meter_id",                  ExposeType::Numeric, Access::State,    nullptr, "Meter identification number", nullptr, 0},
    {"instantaneous_flow_rate",   ExposeType::Numeric, Access::State,    "m³/h", "Instantaneous flow rate", nullptr, 0},
    {"inlet_water_temperature",   ExposeType::Numeric, Access::State,    "°C",   "Inlet water temperature", nullptr, 0},
    {"outlet_water_temperature",  ExposeType::Numeric, Access::State,    "°C",   "Outlet water temperature", nullptr, 0},
    {"voltage",                   ExposeType::Numeric, Access::State,    "mV",   "Battery voltage", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE200_jt50ea5d" };
}  // namespace

extern const PreparedDefinition kDef_TS0601_heat_meter{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="TS0601_heat_meter",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
