// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya BAC-001 heating/cooling thermostat with fan control
// (z2m v26.92.0 parity, added v26.89.0).
// z2m-source: tuya.ts #BAC-001  (fingerprint TS0601 / _TZE204_hpkusvom)
//
// PARTIAL PORT — system_mode is DEFERRED, deliberately:
//   Upstream models system_mode as a COMPOSITE of two datapoints. DP1 is a
//   bare bool with a null key whose only job is to stash `power_state_device`
//   in converter state; DP2 stashes `system_mode_device`; the published
//   system_mode is `bac001SystemMode(meta)` computed from both. The write path
//   is equally cross-DP: setting "off" sends DP1=false, anything else sends
//   DP1=true AND then DP2=<enum>.
//   A TuyaDpMapEntry is a 1:1 DP<->key mapping with no cross-DP state and no
//   multi-DP write, so wiring DP2 alone would publish a mode that contradicts
//   the device whenever it is powered off, and writing it would leave the unit
//   off. Reporting nothing is better than reporting a lie. Needs a composite-DP
//   hook before this can be honoured.
//
// Ported and correct: setpoint, measured temperature, calibration, fan mode.
//
// DP27 local_temperature_calibration: z2m's inline converter is the signed
// 32-bit two's-complement pass-through (`v > 0x7fffffff ? v - 0x100000000 : v`)
// with divisor 1 — this is the same family whose named helpers upstream
// deleted in this window. Decode sign handling comes from the s32 fix in
// 4ace25e; see the Task 12 calibration ticket.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kFanMode[] = {
    {0, "low"}, {1, "medium"}, {2, "high"}, {3, "auto"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        // DP1 + DP2 (system_mode composite) DEFERRED — see file header.
        ::zhc::tuya::dp::numeric(16, "current_heating_setpoint", 10),
        ::zhc::tuya::dp::numeric(24, "local_temperature", 10),
        ::zhc::tuya::dp::numeric(27, "local_temperature_calibration", 1),
        ::zhc::tuya::dp::enum_lookup(28, "fan_mode", kFanMode, 4),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kFanModeOpts[] = { "low", "medium", "high", "auto" };

constexpr Expose kExp[] = {
    { "local_temperature",             ExposeType::Numeric, ::zhc::Access::State,    "°C",    "Measured room temperature",      nullptr,       0 },
    { "current_heating_setpoint",      ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Target temperature",             nullptr,       0 },
    { "local_temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",    "Temperature calibration offset", nullptr,       0 },
    { "fan_mode",                      ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Fan mode",                       kFanModeOpts,  4 },
};

constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_hpkusvom" };
}  // namespace

extern const PreparedDefinition kDef_BAC_001{
    .zigbee_models=kM,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,
    .model="BAC-001",.vendor="Tuya",
    .meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
