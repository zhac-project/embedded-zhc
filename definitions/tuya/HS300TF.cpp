// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HYSYIOT HS300TF solar soil moisture / temperature / humidity /
// illuminance / fertility sensor (z2m v26.133.0). Upstream fingerprints TS0601
// with `{_TZD200_,_TZE200_}7sa1ogeny` and `{_TZD200_,_TZE200_}sa1ogeny`; the bare
// zigbeeModel HS300TF is the second definition, kDef_HS300TF_model.
// DPs: 3 soil_moisture, 5 temperature /10, 9 temperature_unit, 15 battery,
// 101 humidity, 102 illuminance, 103 report_period, 104 soil_calibration,
// 105 humidity_calibration, 106 illuminance_calibration,
// 107 temperature_calibration /10, 110 soil_warning, 111 water_warning,
// 112 soil_fertility, 113 soil_fertility_calibration, 114/115
// soil_fertility_set_v0/v1, 116 soil_fertility_warning.
// z2m-source: tuya.ts #HS300TF.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kTemperatureUnit[] = { {0,"celsius"}, {1,"fahrenheit"} };
constexpr ::zhc::tuya::TuyaEnumEntry kWaterWarning[] = { {0,"none"}, {1,"alarm"} };
constexpr ::zhc::tuya::TuyaEnumEntry kFertilityWarning[] = { {0,"none"}, {1,"low"}, {2,"high"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(3, "soil_moisture", 1),
        ::zhc::tuya::dp::numeric(5, "temperature", 10),
        ::zhc::tuya::dp::enum_lookup(9, "temperature_unit", kTemperatureUnit, 2),
        ::zhc::tuya::dp::numeric(15, "battery", 1),
        ::zhc::tuya::dp::numeric(101, "humidity", 1),
        ::zhc::tuya::dp::numeric(102, "illuminance", 1),
        ::zhc::tuya::dp::numeric(103, "report_period", 1),
        ::zhc::tuya::dp::numeric(104, "soil_calibration", 1),
        ::zhc::tuya::dp::numeric(105, "humidity_calibration", 1),
        ::zhc::tuya::dp::numeric(106, "illuminance_calibration", 1),
        ::zhc::tuya::dp::numeric(107, "temperature_calibration", 10),
        ::zhc::tuya::dp::numeric(110, "soil_warning", 1),
        ::zhc::tuya::dp::enum_lookup(111, "water_warning", kWaterWarning, 2),
        ::zhc::tuya::dp::numeric(112, "soil_fertility", 1),
        ::zhc::tuya::dp::numeric(113, "soil_fertility_calibration", 1),
        ::zhc::tuya::dp::numeric(114, "soil_fertility_set_v0", 1),
        ::zhc::tuya::dp::numeric(115, "soil_fertility_set_v1", 1),
        ::zhc::tuya::dp::enum_lookup(116, "soil_fertility_warning", kFertilityWarning, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOptsWater[] = { "none", "alarm" };
constexpr const char* kOptsFertility[] = { "none", "low", "high" };
constexpr const char* kOptsUnit[] = { "celsius", "fahrenheit" };
constexpr Expose kExp[] = {
    {"water_warning", ExposeType::Enum, Access::State, nullptr, "Water shortage warning", kOptsWater, 2},
    {"soil_fertility_warning", ExposeType::Enum, Access::State, nullptr, "Soil fertility warning", kOptsFertility, 3},
    {"battery", ExposeType::Numeric, Access::State, "%", "Remaining battery in %", nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"soil_moisture", ExposeType::Numeric, Access::State, "%", "Measured soil moisture value", nullptr, 0},
    {"soil_fertility", ExposeType::Numeric, Access::State, "μS/cm", "Soil fertility value", nullptr, 0, ExposeCategory::State, 0, 5000, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", "Measured temperature value", nullptr, 0},
    {"temperature_unit", ExposeType::Enum, Access::StateSet, nullptr, "Temperature unit", kOptsUnit, 2},
    {"humidity", ExposeType::Numeric, Access::State, "%", "Measured relative humidity", nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", "Measured illuminance", nullptr, 0},
    {"report_period", ExposeType::Numeric, Access::StateSet, "s", "Sensor reporting period", nullptr, 0, ExposeCategory::State, 5, 1200, 5},
    {"soil_calibration", ExposeType::Numeric, Access::StateSet, "%", "Offset for measured soil moisture", nullptr, 0, ExposeCategory::State, -30, 30, 1},
    {"humidity_calibration", ExposeType::Numeric, Access::StateSet, "%", "Humidity calibration", nullptr, 0, ExposeCategory::State, -30, 30, 1},
    {"illuminance_calibration", ExposeType::Numeric, Access::StateSet, "lux", "Illuminance calibration", nullptr, 0, ExposeCategory::State, -15, 1000, 1},
    // Upstream step is 0.1 °C; Expose bounds are integers, so the step is left unset.
    {"temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", "Temperature calibration", nullptr, 0, ExposeCategory::State, -2, 2, 0},
    {"soil_warning", ExposeType::Numeric, Access::StateSet, "%", "Soil water shortage humidity value", nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"soil_fertility_calibration", ExposeType::Numeric, Access::StateSet, "μS/cm", "Soil fertility calibration", nullptr, 0, ExposeCategory::State, -1000, 1000, 5},
    {"soil_fertility_set_v0", ExposeType::Numeric, Access::StateSet, "μS/cm", "Soil fertility low warning threshold", nullptr, 0, ExposeCategory::State, 0, 5000, 1},
    {"soil_fertility_set_v1", ExposeType::Numeric, Access::StateSet, "μS/cm", "Soil fertility high warning threshold", nullptr, 0, ExposeCategory::State, 0, 5000, 1},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZD200_7sa1ogeny", "_TZE200_7sa1ogeny", "_TZD200_sa1ogeny", "_TZE200_sa1ogeny" };
constexpr const char* kM_model[] = { "HS300TF" };
}  // namespace

extern const PreparedDefinition kDef_HS300TF{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="HS300TF",.vendor="HYSYIOT",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

// Upstream's zigbeeModel fingerprint: "HS300TF" with any manufacturer.
extern const PreparedDefinition kDef_HS300TF_model{
    .zigbee_models=kM_model,.zigbee_models_count=sizeof(kM_model)/sizeof(kM_model[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="HS300TF",.vendor="HYSYIOT",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
