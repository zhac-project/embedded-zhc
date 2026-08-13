// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya fan-coil unit (FCU) thermostat (z2m v26.95.0 parity, added v26.95.0).
// z2m-source: tuya.ts #TS0601_thermostat_fancoil  (TS0601 / _TZE200_xixlazkg)
//
// Divisors from each triple: local_temperature divideBy10, setpoint raw,
// everything else raw. child_lock is z2m `lockUnlock`, a lookup over booleans,
// so it is a Bool DP (the emitter falls back to a numeric pass-through).
//
// DEFERRED -- DP44 local_temperature_calibration. z2m's
// valueConverter.localTemperatureCalibration is a TWELVE-BIT signed wrap,
// `from: v => v > 4000 ? v - 4096 : v`, not a divisor and not the 32-bit
// two's complement the Tuya decoder already handles. Wiring it as a plain
// numeric would report 4090 instead of -6. Needs a wrap-aware DP factory.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kFanMode_fancoil[] = {
    {0, "low"}, {1, "medium"}, {2, "high"}, {3, "auto"} };

struct cfg_fancoil {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "preset"),
        ::zhc::tuya::dp::numeric(16, "current_heating_setpoint", 1),
        ::zhc::tuya::dp::numeric(24, "local_temperature", 10),
        ::zhc::tuya::dp::enum_lookup(28, "fan_mode", kFanMode_fancoil, 4),
        ::zhc::tuya::dp::binary_inv(35, "battery_low"),
        ::zhc::tuya::dp::binary(40, "child_lock"),
        // DP44 local_temperature_calibration DEFERRED -- 12-bit wrap, see header.
        ::zhc::tuya::dp::numeric(45, "error", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_fancoil>;
constexpr const char* kFanOpts[] = { "low", "medium", "high", "auto" };
constexpr Expose kExp[] = {
    {"preset",                   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Thermostat on/off",       nullptr,    0},
    {"local_temperature",        ExposeType::Numeric, ::zhc::Access::State,    "\u00b0C",    "Measured temperature",    nullptr,    0},
    {"current_heating_setpoint", ExposeType::Numeric, ::zhc::Access::StateSet, "\u00b0C",    "Target temperature",      nullptr,    0},
    {"fan_mode",                 ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Fan mode",                kFanOpts,   4},
    {"child_lock",               ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Child lock",              nullptr,    0},
    {"battery_low",              ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Battery low",             nullptr,    0},
    {"error",                    ExposeType::Numeric, ::zhc::Access::State,    nullptr, "Error code",              nullptr,    0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE200_xixlazkg" };
}  // namespace
extern const PreparedDefinition kDef_TS0601_thermostat_fancoil{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0601_thermostat_fancoil",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
