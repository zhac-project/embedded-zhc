// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Moes ZHT-002 thermostat (TS0601 / _TZE204_xalsoe3m), z2m v26.115.1
// parity. Graduated from tuya/generated/Gen__TZE204_xalsoe3m.cpp.
//
// z2m v26.106.0 (#13106, "heat only") and v26.110.0 (#13200):
//   * DP1 now also drives the climate `system_mode` {off, heat} next to the
//     on/off `state` (one boolean, two rows; ZHC fires both);
//   * DP47 is `running_state` {idle, heat}, it was `valve_state`.
// Upstream's table still decodes DP2 (manual / auto schedule) under
// `system_mode` while the expose list calls it `schedule_mode`; the decode is
// mirrored as upstream has it and `schedule_mode` is not exposed, since no
// datapoint carries that key (a write to it has nowhere to go upstream either).
// Calibration (DP19) is upstream's 12-bit `localTemperatureCalibration`
// (v > 4000 ? v - 4096 : v); this port has no 12-bit wrap yet, so negative
// offsets read as 4095.. — the copy divided by 10, which was wrong for every
// value. The DP68 48-byte schedule composite is not ported (no composite value).
// z2m-source: tuya.ts #ZHT-002.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSysMode[]  = { {0,"off"}, {1,"heat"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSchedule[] = { {0,"auto"}, {1,"manual"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSensor[]   = { {0,"IN"}, {2,"AL"}, {1,"OU"} };
constexpr ::zhc::tuya::TuyaEnumEntry kRunning[]  = { {0,"idle"}, {1,"heat"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        { 1, "system_mode", ::zhc::TuyaDpType::Bool, 1, kSysMode, 2, ::zhc::tuya::kTuyaDpFlagBoolEnum },
        ::zhc::tuya::dp::enum_lookup(2, "system_mode", kSchedule, 2),
        ::zhc::tuya::dp::numeric(16, "local_temperature", 10),
        ::zhc::tuya::dp::numeric(18, "min_temperature", 1),
        ::zhc::tuya::dp::numeric(19, "local_temperature_calibration", 1),
        ::zhc::tuya::dp::enum_lookup(32, "sensor", kSensor, 3),
        ::zhc::tuya::dp::numeric(34, "max_temperature", 1),
        ::zhc::tuya::dp::binary(39, "child_lock"),
        ::zhc::tuya::dp::binary(40, "eco_mode"),
        ::zhc::tuya::dp::enum_lookup(47, "running_state", kRunning, 2),
        ::zhc::tuya::dp::numeric(50, "current_heating_setpoint", 1),
        ::zhc::tuya::dp::numeric(101, "max_temperature_limit", 1),
        ::zhc::tuya::dp::numeric(102, "deadzone_temperature", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kSysModeOpts[] = { "off", "heat" };
constexpr const char* kSensorOpts[]  = { "IN", "AL", "OU" };
constexpr const char* kRunningOpts[] = { "idle", "heat" };
constexpr Expose kExp[] = {
    {"state",                         ExposeType::Binary,  Access::StateSet, nullptr, "Turn the thermostat ON/OFF", nullptr, 0},
    {"child_lock",                    ExposeType::Binary,  Access::StateSet, nullptr, "Enables/disables physical input on the device", nullptr, 0},
    {"eco_mode",                      ExposeType::Binary,  Access::StateSet, nullptr, "ECO mode (energy saving mode)", nullptr, 0},
    {"sensor",                        ExposeType::Enum,    Access::StateSet, nullptr, "Choose which sensor to use. Default: AL", kSensorOpts, 3},
    {"min_temperature",               ExposeType::Numeric, Access::StateSet, "°C",    "Minimum temperature", nullptr, 0, ExposeCategory::State, 0, 20, 0},
    {"max_temperature",               ExposeType::Numeric, Access::StateSet, "°C",    "Maximum temperature", nullptr, 0, ExposeCategory::State, 20, 50, 0},
    {"local_temperature",             ExposeType::Numeric, Access::State,    "°C",    "Current temperature measured on the device", nullptr, 0},
    {"current_heating_setpoint",      ExposeType::Numeric, Access::StateSet, "°C",    "Temperature setpoint", nullptr, 0, ExposeCategory::State, 0, 50, 1},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C",    "Offset to add/subtract to the local temperature", nullptr, 0, ExposeCategory::State, -9, 9, 1},
    {"system_mode",                   ExposeType::Enum,    Access::StateSet, nullptr, "Mode of this device", kSysModeOpts, 2},
    {"running_state",                 ExposeType::Enum,    Access::State,    nullptr, "The current running state", kRunningOpts, 2},
    {"max_temperature_limit",         ExposeType::Numeric, Access::StateSet, nullptr, "Max temperature limit", nullptr, 0, ExposeCategory::State, 25, 70, 1},
    {"deadzone_temperature",          ExposeType::Numeric, Access::StateSet, nullptr, "The difference between local temp and set temp that triggers heating", nullptr, 0, ExposeCategory::State, 1, 5, 1},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_xalsoe3m" };
}  // namespace

extern const PreparedDefinition kDef_ZHT_002{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="ZHT-002",
    .vendor="Moes",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr,
    .tuya_time_start=2 };

}  // namespace zhc::devices::tuya
