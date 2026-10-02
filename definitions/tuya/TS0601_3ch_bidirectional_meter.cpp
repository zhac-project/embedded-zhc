// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya 3-channel bidirectional energy meter with relay (z2m v26.110.0, #13194).
// Upstream lists only the `_TZE20C1000000_` spelling; it is listed as is.
// z2m-source: tuya.ts #TS0601_3ch_bidirectional_meter.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(16, "state"),
        ::zhc::tuya::dp::numeric(1, "energy", 100),
        ::zhc::tuya::dp::numeric(2, "produced_energy", 100),
        ::zhc::tuya::dp::numeric(9, "power", 10),
        ::zhc::tuya::dp::numeric(101, "voltage", 10),
        ::zhc::tuya::dp::numeric(131, "temperature", 10),
        ::zhc::tuya::dp::numeric(102, "current_a", 1000),
        ::zhc::tuya::dp::numeric(105, "power_a", 10),
        ::zhc::tuya::dp::numeric(108, "power_factor_a", 100),
        ::zhc::tuya::dp::numeric(125, "energy_a", 100),
        ::zhc::tuya::dp::numeric(126, "reverse_energy_a", 100),
        ::zhc::tuya::dp::numeric(111, "power_setting_a", 1),
        ::zhc::tuya::dp::binary(114, "power_alarm_a"),
        ::zhc::tuya::dp::numeric(133, "current_a_calibration", 1),
        ::zhc::tuya::dp::numeric(136, "power_a_calibration", 1),
        ::zhc::tuya::dp::numeric(103, "current_b", 1000),
        ::zhc::tuya::dp::numeric(106, "power_b", 10),
        ::zhc::tuya::dp::numeric(109, "power_factor_b", 100),
        ::zhc::tuya::dp::numeric(127, "energy_b", 100),
        ::zhc::tuya::dp::numeric(128, "reverse_energy_b", 100),
        ::zhc::tuya::dp::numeric(112, "power_setting_b", 1),
        ::zhc::tuya::dp::binary(115, "power_alarm_b"),
        ::zhc::tuya::dp::numeric(134, "current_b_calibration", 1),
        ::zhc::tuya::dp::numeric(137, "power_b_calibration", 1),
        ::zhc::tuya::dp::numeric(104, "current_c", 1000),
        ::zhc::tuya::dp::numeric(107, "power_c", 10),
        ::zhc::tuya::dp::numeric(110, "power_factor_c", 100),
        ::zhc::tuya::dp::numeric(129, "energy_c", 100),
        ::zhc::tuya::dp::numeric(130, "reverse_energy_c", 100),
        ::zhc::tuya::dp::numeric(113, "power_setting_c", 1),
        ::zhc::tuya::dp::binary(116, "power_alarm_c"),
        ::zhc::tuya::dp::numeric(135, "current_c_calibration", 1),
        ::zhc::tuya::dp::numeric(138, "power_c_calibration", 1),
        ::zhc::tuya::dp::numeric(132, "voltage_calibration", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_a", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power_a", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_factor_a", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"energy_a", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"reverse_energy_a", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power_setting_a", ExposeType::Numeric, Access::StateSet, "W", nullptr, nullptr, 0, ExposeCategory::State, 0, 3680, 0},
    {"power_alarm_a", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"current_a_calibration", ExposeType::Numeric, Access::StateSet, "mA", nullptr, nullptr, 0, ExposeCategory::State, 0, 100000, 0},
    {"power_a_calibration", ExposeType::Numeric, Access::StateSet, "W", nullptr, nullptr, 0, ExposeCategory::State, 0, 3680, 0},
    {"current_b", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power_b", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_factor_b", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"energy_b", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"reverse_energy_b", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power_setting_b", ExposeType::Numeric, Access::StateSet, "W", nullptr, nullptr, 0, ExposeCategory::State, 0, 3680, 0},
    {"power_alarm_b", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"current_b_calibration", ExposeType::Numeric, Access::StateSet, "mA", nullptr, nullptr, 0, ExposeCategory::State, 0, 100000, 0},
    {"power_b_calibration", ExposeType::Numeric, Access::StateSet, "W", nullptr, nullptr, 0, ExposeCategory::State, 0, 3680, 0},
    {"current_c", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power_c", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_factor_c", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"energy_c", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"reverse_energy_c", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power_setting_c", ExposeType::Numeric, Access::StateSet, "W", nullptr, nullptr, 0, ExposeCategory::State, 0, 3680, 0},
    {"power_alarm_c", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"current_c_calibration", ExposeType::Numeric, Access::StateSet, "mA", nullptr, nullptr, 0, ExposeCategory::State, 0, 100000, 0},
    {"power_c_calibration", ExposeType::Numeric, Access::StateSet, "W", nullptr, nullptr, 0, ExposeCategory::State, 0, 3680, 0},
    {"voltage_calibration", ExposeType::Numeric, Access::StateSet, "V", nullptr, nullptr, 0, ExposeCategory::State, 0, 260, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE20C1000000_p3g8xiug" };
}  // namespace

extern const PreparedDefinition kDef_TS0601_3ch_bidirectional_meter{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0601_3ch_bidirectional_meter",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
