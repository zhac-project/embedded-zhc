// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Siterwell GS361A-H04 — Tuya-MCU (0xEF00) radiator-valve thermostat.
//
// Graduated out of generated/ to fix a MISROUTED decoder. The auto-port
// wired the generic genThermostat (ZCL 0x0201) fz/tz pair (kFzThermostat /
// kTzThermostat) and bound 0x0201, but this device is a Tuya-MCU TRV: z2m
// decodes it via `legacy.fz.tuya_thermostat` over the manuSpecificTuya
// (0xEF00) DP stream — it never speaks genThermostat, so every reading
// (setpoint, local_temperature, system_mode, child_lock, battery, …) was
// dead. Replaced with the Tuya DP map matching z2m's per-DP semantics.
//
// z2m-source: siterwell.ts #GS361A-H04 (legacy.fz.tuya_thermostat) — DP ids
// from lib/legacy.ts `dataPoints` + `thermostatSystemModes4`:
//   DP 2  heatingSetpoint  → current_heating_setpoint  (value/10)
//   DP 3  localTemp        → local_temperature         (value/10)
//   DP 4  mode             → system_mode enum {0:off,1:auto,2:heat}
//   DP 7  childLock        → child_lock                (bool)
//   DP 18 siterwellWindowDetection → window_detection  (bool)
//   DP 20 valveDetection   → valve_detection           (bool)
//   DP 21 battery          → battery                   (raw %)
//   DP 109 valvePos        → position                  (raw valve %)
// (z2m also derives running_state from DP109's value, but that requires a
//  numeric→enum derivation no generic converter performs; the literal
//  position value is the faithful decodable output — see notes.)
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::siterwell {
namespace {

// thermostatSystemModes4 (lib/legacy.ts): 0=off, 1=auto, 2=heat.
constexpr ::zhc::tuya::TuyaEnumEntry kSystemModes_GS361A_H04[] = {
    {0, "off"}, {1, "auto"}, {2, "heat"},
};

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry entries[] = {
        ::zhc::tuya::dp::numeric(2, "current_heating_setpoint", 10),
        ::zhc::tuya::dp::numeric(3, "local_temperature", 10),
        ::zhc::tuya::dp::enum_lookup(4, "system_mode", kSystemModes_GS361A_H04, 3),
        ::zhc::tuya::dp::binary(7, "child_lock"),
        ::zhc::tuya::dp::binary(18, "window_detection"),
        ::zhc::tuya::dp::binary(20, "valve_detection"),
        ::zhc::tuya::dp::numeric(21, "battery"),
        ::zhc::tuya::dp::numeric(109, "position"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{entries, 8};
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kModels_GS361A_H04[] = { "ivfvd7h", "eaxp72v", "kfvq6avy", "fvq6avy", "zk78ptr", "4yfvweb", "TS0601" };
constexpr const char* kManus_GS361A_H04[] = { "_TZE200_zivfvd7h", "_TZE200_kfvq6avy", "_TZE200_hhrtiq0x", "_TZE200_ps5v5jor", "_TZE200_jeaxp72v", "_TZE200_owwdxjbx", "_TZE200_2cs6g9i7", "_TZE200_04yfvweb", "_TZE200_lrznf59v", "_TZE204_woww89ip" };
}  // namespace


// --- exposes (z2m siterwell.ts #GS361A-H04) — flat thermostat surface ---
constexpr const char* kSystemModeOpts_GS361A_H04[] = { "off", "auto", "heat" };
constexpr Expose kAutoExposes[] = {
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"valve_detection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModeOpts_GS361A_H04, 3},
    {"position", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};


constexpr WhiteLabel kWhiteLabels_GS361A_H04[] = {
    {"Brennenstuhl","HT CZ 01"},
};
extern const PreparedDefinition kDef_GS361A_H04{
    .zigbee_models=kModels_GS361A_H04, .zigbee_models_count=sizeof(kModels_GS361A_H04)/sizeof(kModels_GS361A_H04[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_GS361A_H04, .manufacturer_names_count=sizeof(kManus_GS361A_H04)/sizeof(kManus_GS361A_H04[0]),
    .model="GS361A-H04", .vendor="Siterwell",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_GS361A_H04, .white_labels_count=sizeof(kWhiteLabels_GS361A_H04)/sizeof(kWhiteLabels_GS361A_H04[0]),
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::siterwell
