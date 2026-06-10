// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Javis JS-MC-SENSOR-ZB — microwave/presence sensor (Tuya TS0601 DP).
// Microwave sensor.
//
// Parity fix (z2m<->embedded-zhc): the auto-port mis-wired this Tuya TS0601
// DP-stream sensor to the IAS-zone converter (kFzIasZone) with phantom IAS
// exposes (alarm/tamper/battery_low) and a 0x0500 bind. It is NOT an IAS
// device: z2m decodes it via `legacy.fz.javis_microwave_sensor` over the
// 0xEF00 (manuSpecificTuya) datapoint stream. Ported the real DP->key map.
//
// z2m DPs (legacy.fz.javis_microwave_sensor):
//   DP1   presence state enum -> `presence_state` + `occupancy` (true for 1..4)
//   DP2   `sensitivity`
//   DP101 `illuminance`
//   DP102 `keep_time`               (this variant)
//   DP103 `led_enable` (bool)       (this variant)
//   DP104 `illuminance`
//   DP105 `illuminance_calibration` (this variant)
// The `_TZE200_kagkgk0i` variant remaps DP102/106/107 — split into its own
// def (Jav_JS_MC_SENSOR_ZB_kagkgk0i.cpp) so each manufacturer gets z2m's
// exact DP wiring.
//
// z2m-source: javis.ts #JS-MC-SENSOR-ZB; lib/legacy.ts fz/tz javis_microwave_sensor.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::javis {
namespace {

// DP1 presence-state enum -> string. z2m lookup over values 0..6.
constexpr ::zhc::tuya::TuyaEnumEntry kPresence[] = {
    {0, "no_motion"},
    {1, "big_motion"},
    {2, "minor_motion"},
    {3, "breathing"},
    {4, "abnormal_state"},
    {5, "initializing"},
    {6, "initialization_completed"},
};
// DP1 occupancy fan-out: occupancy = (0 < value < 5) -> values 1..4 are
// "occupied". EnumBool flag matches raw against this table -> Bool true.
constexpr ::zhc::tuya::TuyaEnumEntry kOccupied[] = {
    {1, "x"}, {2, "x"}, {3, "x"}, {4, "x"},
};

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        // DP1 fans out to both the presence_state string and the occupancy bool.
        ::zhc::tuya::dp::enum_lookup(1, "presence_state", kPresence,
                                     sizeof(kPresence) / sizeof(kPresence[0])),
        {1, "occupancy", ::zhc::TuyaDpType::Enum, 1, kOccupied,
         sizeof(kOccupied) / sizeof(kOccupied[0]), ::zhc::tuya::kTuyaDpFlagEnumBool},
        ::zhc::tuya::dp::numeric(2, "sensitivity"),
        ::zhc::tuya::dp::numeric(101, "illuminance"),
        ::zhc::tuya::dp::numeric(102, "keep_time"),
        ::zhc::tuya::dp::binary(103, "led_enable"),
        ::zhc::tuya::dp::numeric(104, "illuminance"),
        ::zhc::tuya::dp::numeric(105, "illuminance_calibration"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{
        e, sizeof(e) / sizeof(e[0])};
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kModels_JS_MC_SENSOR_ZB[] = { "JAVISSENSOR", "TS0601" };
constexpr const char* kManus_JS_MC_SENSOR_ZB[] = { "_TZE200_lgstepha", "_TZE200_i0b1dbqu" };
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"presence_state", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"led_enable", ExposeType::Binary, Access::StateSet, nullptr, "Enabled LED", nullptr, 0},
    {"keep_time", ExposeType::Enum, Access::StateSet, nullptr, "PIR keep time", nullptr, 0},
    {"sensitivity", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"illuminance_calibration", ExposeType::Numeric, Access::StateSet, nullptr, "Illuminance calibration", nullptr, 0},
};

extern const PreparedDefinition kDef_JS_MC_SENSOR_ZB{
    .zigbee_models=kModels_JS_MC_SENSOR_ZB, .zigbee_models_count=sizeof(kModels_JS_MC_SENSOR_ZB)/sizeof(kModels_JS_MC_SENSOR_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_JS_MC_SENSOR_ZB, .manufacturer_names_count=sizeof(kManus_JS_MC_SENSOR_ZB)/sizeof(kManus_JS_MC_SENSOR_ZB[0]),
    .model="JS-MC-SENSOR-ZB", .vendor="Javis",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::javis
