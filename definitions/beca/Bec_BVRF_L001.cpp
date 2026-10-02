// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Beca BVRF-L001 VRF/VRV central air-conditioning thermostat
// (z2m v26.114.0, #13325; new vendor). Upstream refuses non-integer setpoints
// outside 16..32 on write; the bounds are on the expose here.
// z2m-source: beca.ts #BVRF-L001.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::beca {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSystemMode2[] = { {0,"cool"}, {1,"heat"}, {2,"fan_only"}, {3,"dry"} };
constexpr ::zhc::tuya::TuyaEnumEntry kFanMode49[] = { {0,"auto"}, {1,"low"}, {2,"medium"}, {3,"high"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        ::zhc::tuya::dp::enum_lookup(2, "system_mode", kSystemMode2, 4),
        ::zhc::tuya::dp::numeric(16, "current_heating_setpoint", 10),
        ::zhc::tuya::dp::numeric(24, "local_temperature", 10),
        ::zhc::tuya::dp::binary(40, "child_lock"),
        ::zhc::tuya::dp::enum_lookup(49, "fan_mode", kFanMode49, 4),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts1[] = { "cool", "heat", "fan_only", "dry" };
constexpr const char* kOpts2[] = { "auto", "low", "medium", "high" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, "Turn the thermostat on or off independently of the operating mode", nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, "Mode of this device", kOpts1, 4},
    {"fan_mode", ExposeType::Enum, Access::StateSet, nullptr, "Mode of the fan", kOpts2, 4},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", "Temperature setpoint", nullptr, 0, ExposeCategory::State, 16, 32, 1},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", "Current temperature measured on the device", nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, "Enables/disables physical input on the device", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_6ewjlefg" };
}  // namespace

extern const PreparedDefinition kDef_BVRF_L001{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="BVRF-L001",.vendor="Beca",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::beca
