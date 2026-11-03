// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Appartme APRM-04-005 thermostatic radiator valve, TS0601
// _TZE284_fziifcxj (z2m v26.125.0 window). DP2 carries both preset and
// system_mode: two rows fan out on decode (0 schedule/heat, 3 manual/heat,
// 5 antifrost/off); a write is matched by key, and "heat" encodes as 3
// (manual), the first heat entry. Upstream's fallback for unknown DP2 values
// (heat/manual) is not reproduced: an unknown value publishes nothing.
// DPs 4/5/47 /10, 6 battery, 7 child_lock, 14 window_detection, 15
// window_open (trueFalseEnum1), 48 valve_testing.
// z2m-source: tuya.ts #APRM-04-005.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPreset[] = { {0, "schedule"}, {3, "manual"}, {5, "antifrost"} };
constexpr ::zhc::tuya::TuyaEnumEntry kSystemMode[] = { {3, "heat"}, {0, "heat"}, {5, "off"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum1[] = { {1, "true"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(2, "preset", kPreset, 3),
        ::zhc::tuya::dp::enum_lookup(2, "system_mode", kSystemMode, 3),
        ::zhc::tuya::dp::numeric(4, "current_heating_setpoint", 10),
        ::zhc::tuya::dp::numeric(5, "local_temperature", 10),
        ::zhc::tuya::dp::numeric(6, "battery", 1),
        ::zhc::tuya::dp::binary(7, "child_lock"),
        ::zhc::tuya::dp::binary(14, "window_detection"),
        { 15, "window_open", ::zhc::TuyaDpType::Enum, 1, kEnum1, 1, ::zhc::tuya::kTuyaDpFlagEnumBool },
        ::zhc::tuya::dp::numeric(47, "local_temperature_calibration", 10),
        ::zhc::tuya::dp::binary(48, "valve_testing"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kSystemModeOpts[] = { "heat", "off" };
constexpr const char* kPresetOpts[] = { "schedule", "manual", "antifrost" };
constexpr Expose kExp[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 30, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::Config, -5, 5, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, "heat = manual mode, off = anti-freeze mode", kSystemModeOpts, 2},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kPresetOpts, 3},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_open", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"valve_testing", ExposeType::Binary, Access::StateSet, nullptr, "Valve test, the valve opens and closes while F1 is displayed", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_fziifcxj" };
}  // namespace

extern const PreparedDefinition kDef_APRM_04_005{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="APRM-04-005",.vendor="Appartme",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
