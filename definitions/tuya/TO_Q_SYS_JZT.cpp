// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tongou TO-Q-SYS-JZT DIN-rail meter / breaker (`_TZE284_6ocnqlhn`),
// graduated from the generated copy, which decoded this Tuya datapoint device
// with ZCL metering converters. Datapoints as z2m v26.115.1 (#13170 adds the
// LCD and recloser settings and the ac_frequency rule: values above 100 are
// hundredths of a hertz). Labels follow z2m's lookups where its exposes
// disagree (event `timing_switch_on`, lcd_rotation `RWD`).
// z2m-source: tuya.ts #TO-Q-SYS-JZT.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kOverVoltageSetting102[] = { {0,"Ignore"}, {1,"Alarm"}, {2,"Trip"} };
constexpr ::zhc::tuya::TuyaEnumEntry kUnderVoltageSetting103[] = { {0,"Ignore"}, {1,"Alarm"}, {2,"Trip"} };
constexpr ::zhc::tuya::TuyaEnumEntry kOverCurrentSetting104[] = { {0,"Ignore"}, {1,"Alarm"}, {2,"Trip"} };
constexpr ::zhc::tuya::TuyaEnumEntry kOverPowerSetting105[] = { {0,"Ignore"}, {1,"Alarm"}, {2,"Trip"} };
constexpr ::zhc::tuya::TuyaEnumEntry kTemperatureSetting107[] = { {0,"Ignore"}, {1,"Alarm"}, {2,"Trip"} };
constexpr ::zhc::tuya::TuyaEnumEntry kControlMode108[] = { {0,"local_lock"}, {1,"local_mode"}, {2,"remote_mode"}, {3,"full_control"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEvent110[] = { {0,"normal"}, {1,"over_current_trip"}, {2,"over_power_trip"}, {3,"high_temp_trip"}, {4,"over_voltage_trip"}, {5,"under_voltage_trip"}, {6,"over_current_alarm"}, {7,"over_power_alarm"}, {8,"high_temp_alarm"}, {9,"over_voltage_alarm"}, {10,"under_voltage_alarm"}, {11,"remote_on"}, {12,"remote_off"}, {13,"manual_on"}, {14,"manual_off"}, {15,"leakage_trip"}, {16,"leakage_alarm"}, {17,"restore_default"}, {18,"automatic_closing"}, {19,"electricity_shortage"}, {20,"electricity_shortage_alarm"}, {21,"timing_switch_on"}, {22,"timing_switch_off"} };
constexpr ::zhc::tuya::TuyaEnumEntry kLcdRotation143[] = { {0,"FWD"}, {1,"RWD"} };
// Tze2846ocnqlhnfrequencyConverter: v > 100 ? v / 100 : v.
bool ac_frequency(const ::zhc::tuya::TuyaDpMapEntry& e, const Value& raw, RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (raw.type != ValueType::Uint && raw.type != ValueType::Int) return false;
    const double v = raw.type == ValueType::Uint ? static_cast<double>(raw.u) : static_cast<double>(raw.i);
    Value o{}; o.type = ValueType::Float; o.f = static_cast<float>(v > 100 ? v / 100.0 : v);
    out.put(e.out_key, o);
    return true;
}
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "energy", 100),
        ::zhc::tuya::dp::phase_variant2(6, &::zhc::tuya::kTuyaPhaseKeysPlain),
        ::zhc::tuya::dp::numeric(15, "leakage_current", 1),
        ::zhc::tuya::dp::binary(16, "state"),
        { 32, "ac_frequency", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, 0.0f, &ac_frequency, nullptr },
        ::zhc::tuya::dp::numeric(50, "power_factor", 1),
        ::zhc::tuya::dp::enum_lookup(102, "over_voltage_setting", kOverVoltageSetting102, 3),
        ::zhc::tuya::dp::enum_lookup(103, "under_voltage_setting", kUnderVoltageSetting103, 3),
        ::zhc::tuya::dp::enum_lookup(104, "over_current_setting", kOverCurrentSetting104, 3),
        ::zhc::tuya::dp::enum_lookup(105, "over_power_setting", kOverPowerSetting105, 3),
        ::zhc::tuya::dp::enum_lookup(107, "temperature_setting", kTemperatureSetting107, 3),
        ::zhc::tuya::dp::enum_lookup(108, "control_mode", kControlMode108, 4),
        ::zhc::tuya::dp::enum_lookup(110, "event", kEvent110, 23),
        ::zhc::tuya::dp::numeric(114, "over_current_threshold", 1),
        ::zhc::tuya::dp::numeric(115, "over_voltage_threshold", 1),
        ::zhc::tuya::dp::numeric(116, "under_voltage_threshold", 1),
        ::zhc::tuya::dp::numeric(118, "temperature_threshold", 10),
        ::zhc::tuya::dp::numeric(119, "over_power_threshold", 1),
        ::zhc::tuya::dp::numeric(131, "temperature", 10),
        { 140, "lcd_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, 0.05f },
        ::zhc::tuya::dp::binary(141, "lcd_backlight_off"),
        ::zhc::tuya::dp::enum_lookup(143, "lcd_rotation", kLcdRotation143, 2),
        ::zhc::tuya::dp::binary(144, "current_recloser"),
        ::zhc::tuya::dp::binary(145, "power_recloser"),
        ::zhc::tuya::dp::binary(146, "voltage_recloser"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts9[] = { "normal", "over_current_trip", "over_power_trip", "high_temp_trip", "over_voltage_trip", "under_voltage_trip", "over_current_alarm", "over_power_alarm", "high_temp_alarm", "over_voltage_alarm", "under_voltage_alarm", "remote_on", "remote_off", "manual_on", "manual_off", "leakage_trip", "leakage_alarm", "restore_default", "automatic_closing", "electricity_shortage", "electricity_shortage_alarm", "timing_switch_on", "timing_switch_off" };
constexpr const char* kOpts10[] = { "local_lock", "local_mode", "remote_mode", "full_control" };
constexpr const char* kOpts11[] = { "Ignore", "Alarm", "Trip" };
constexpr const char* kOpts13[] = { "Ignore", "Alarm", "Trip" };
constexpr const char* kOpts15[] = { "Ignore", "Alarm", "Trip" };
constexpr const char* kOpts17[] = { "Ignore", "Alarm", "Trip" };
constexpr const char* kOpts19[] = { "Ignore", "Alarm", "Trip" };
constexpr const char* kOpts26[] = { "FWD", "RWD" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"ac_frequency", ExposeType::Numeric, Access::State, "Hz", nullptr, nullptr, 0},
    {"power_factor", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"leakage_current", ExposeType::Numeric, Access::State, "mA", nullptr, nullptr, 0},
    {"event", ExposeType::Enum, Access::State, nullptr, "Last event of the device", kOpts9, 23},
    {"control_mode", ExposeType::Enum, Access::StateSet, nullptr, "Device control mode", kOpts10, 4},
    {"over_current_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts11, 3},
    {"over_current_threshold", ExposeType::Numeric, Access::StateSet, "A", nullptr, nullptr, 0, ExposeCategory::State, 1, 50, 1},
    {"over_voltage_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts13, 3},
    {"over_voltage_threshold", ExposeType::Numeric, Access::StateSet, "V", nullptr, nullptr, 0, ExposeCategory::State, 240, 295, 1},
    {"under_voltage_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts15, 3},
    {"under_voltage_threshold", ExposeType::Numeric, Access::StateSet, "V", nullptr, nullptr, 0, ExposeCategory::State, 90, 220, 1},
    {"temperature_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts17, 3},
    {"temperature_threshold", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -25, 80, 1},
    {"over_power_setting", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts19, 3},
    {"over_power_threshold", ExposeType::Numeric, Access::StateSet, "W", nullptr, nullptr, 0, ExposeCategory::State, 1000, 26000, 1},
    {"current_recloser", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_recloser", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"voltage_recloser", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"lcd_backlight_off", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"lcd_brightness", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 20},
    {"lcd_rotation", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts26, 2},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_6ocnqlhn" };
}  // namespace

extern const PreparedDefinition kDef_TO_Q_SYS_JZT_dp{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TO-Q-SYS-JZT",.vendor="Tongou",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
