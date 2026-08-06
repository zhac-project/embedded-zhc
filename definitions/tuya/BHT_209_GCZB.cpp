// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya BHT-209-GCZB floor-heating thermostat (z2m v26.92.0 parity,
// added v26.90.0).
// z2m-source: tuya.ts #BHT-209-GCZB  (fingerprint TS0601 / _TZE284_4cgmagba)
//
// Divisors read off each triple's valueConverter:
//   DP16 current_heating_setpoint      divideBy10
//   DP24 local_temperature             divideBy10
//   DP18 deadzone_temperature          raw
//   DP34 max_temperature_limit         raw
//   DP27 local_temperature_calibration raw, SIGNED 32-bit two's complement
//
// DP27 note: upstream deleted valueConverter.localTempCalibration1..5 in this
// window and folded the sign handling into convertBufferToNumber(signed) /
// convertDecimalValueTo4ByteHexArray. This device's triple is plain `raw`, so
// the divisor is 1 and the value is signed — the decode side is covered by the
// s32 sign-extension fix in 4ace25e. See the Task 12 calibration ticket.
//
// Bool DPs that z2m publishes as strings (house convention: ZHC surfaces the
// Bool, the adapter formats it):
//   DP40  child_lock   z2m lockUnlock  {LOCK: true,  UNLOCK: false}
//   DP104 heating_mode z2m lookup      {ON:   true,  OFF:    false}
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {

// z2m: valueConverterBasic.lookup({idle: 1, heat: 0}) -> 0 = heat, 1 = idle.
constexpr ::zhc::tuya::TuyaEnumEntry kRunningState[] = { {0, "heat"}, {1, "idle"} };

struct cfg_BHT_209 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        ::zhc::tuya::dp::numeric(16, "current_heating_setpoint", 10),
        ::zhc::tuya::dp::numeric(18, "deadzone_temperature", 1),
        ::zhc::tuya::dp::numeric(24, "local_temperature", 10),
        ::zhc::tuya::dp::numeric(27, "local_temperature_calibration", 1),
        ::zhc::tuya::dp::numeric(34, "max_temperature_limit", 1),
        ::zhc::tuya::dp::enum_lookup(36, "running_state", kRunningState, 2),
        ::zhc::tuya::dp::binary(40, "child_lock"),
        ::zhc::tuya::dp::binary(104, "heating_mode"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_BHT_209>;

constexpr const char* kRunningStateOpts[] = { "heat", "idle" };

constexpr Expose kExp[] = {
    { "state",                         ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Thermostat on/off",              nullptr,            0 },
    { "local_temperature",             ExposeType::Numeric, ::zhc::Access::State,    "°C",    "Measured room temperature",      nullptr,            0 },
    { "current_heating_setpoint",      ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Target temperature",             nullptr,            0 },
    { "local_temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Temperature calibration offset", nullptr,            0 },
    { "deadzone_temperature",          ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Hysteresis / dead zone",         nullptr,            0 },
    { "max_temperature_limit",         ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Maximum temperature limit",      nullptr,            0 },
    { "running_state",                 ExposeType::Enum,    ::zhc::Access::State,    nullptr, "Heating running state",          kRunningStateOpts,  2 },
    { "child_lock",                    ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Child lock",                     nullptr,            0 },
    { "heating_mode",                  ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Heating mode",                   nullptr,            0 },
};

constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_4cgmagba" };
}  // namespace

extern const PreparedDefinition kDef_BHT_209_GCZB{
    .zigbee_models=kM,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,
    .model="BHT-209-GCZB",.vendor="Tuya",
    .meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
