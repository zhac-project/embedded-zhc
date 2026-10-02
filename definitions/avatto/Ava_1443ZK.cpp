// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: CBE 1443ZK thermostatic radiator valve (z2m v26.108.0, #13147).
// DP3 running_state is a plain-number lookup {idle: 1, heat: 0}. Upstream drops
// a DP4 setpoint of -1 / 0xFFFFFFFF; this port publishes it as -0.1.
// z2m-source: avatto.ts #1443ZK.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::avatto {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPreset2[] = { {0,"manual"}, {1,"schedule"}, {2,"eco"}, {3,"comfort"}, {4,"antifrost"}, {5,"off"} };
constexpr ::zhc::tuya::TuyaEnumEntry kRunningState3[] = { {1,"idle"}, {0,"heat"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(2, "preset", kPreset2, 6),
        { 3, "running_state", ::zhc::TuyaDpType::Numeric, 1, kRunningState3, 2, ::zhc::tuya::kTuyaDpFlagNumericLookup },
        ::zhc::tuya::dp::numeric(4, "current_heating_setpoint", 10),
        ::zhc::tuya::dp::numeric(5, "local_temperature", 10),
        ::zhc::tuya::dp::numeric(6, "battery", 1),
        ::zhc::tuya::dp::binary(7, "child_lock"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts2[] = { "manual", "schedule", "eco", "comfort", "antifrost", "off" };
constexpr const char* kOpts5[] = { "idle", "heat" };
constexpr Expose kExp[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, "Enables/disables physical input on the device", nullptr, 0},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, "Mode of this device", kOpts2, 6},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", "Current temperature measured on the device", nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", "Temperature setpoint", nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"running_state", ExposeType::Enum, Access::State, nullptr, "The current running state", kOpts5, 2},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_16m4bgsv" };
}  // namespace

extern const PreparedDefinition kDef_1443ZK{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="1443ZK",.vendor="CBE",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr,
    .tuya_time_start=2 };

}  // namespace zhc::devices::avatto
