// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya THAH202001 thermostatic radiator valve (z2m v26.95.0 parity).
// z2m-source: tuya.ts #THAH202001 (added v26.95.0)
// Datapoint map emitted from the upstream tuyaDatapoints triples; each
// divisor is the one that device's own valueConverter uses. Where upstream
// maps ONE datapoint to several keys with different lookups, the emitter
// names both tables after the DP and they collide -- those are suffixed _vN.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_m5r5nlxc_2_v1[] = { {0,"manual"},{1,"eco"},{2,"away"},{3,"holiday"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_m5r5nlxc_2_v2[] = { {0,"heat"},{4,"off"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_m5r5nlxc_3[] = { {0,"heat"},{1,"idle"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_m5r5nlxc_15[] = { {0,"OPEN"},{1,"CLOSE"} };

struct cfg_THAH202001 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
    ::zhc::tuya::dp::enum_lookup(2,"preset",kEnum_m5r5nlxc_2_v2,4),
    ::zhc::tuya::dp::enum_lookup(2,"system_mode",kEnum_m5r5nlxc_2_v2,2),
    ::zhc::tuya::dp::enum_lookup(3,"running_state",kEnum_m5r5nlxc_3,2),
    ::zhc::tuya::dp::numeric(4,"current_heating_setpoint",10),
    ::zhc::tuya::dp::numeric(5,"local_temperature",10),
    ::zhc::tuya::dp::numeric(6,"battery",1),
    ::zhc::tuya::dp::binary(14,"window_detection"),
    ::zhc::tuya::dp::enum_lookup(15,"window_open",kEnum_m5r5nlxc_15,2),
    ::zhc::tuya::dp::numeric(21,"holiday_temperature",10),
    ::zhc::tuya::dp::numeric(24,"eco_temperature",10),
    ::zhc::tuya::dp::numeric(25,"away_temperature",10),
    ::zhc::tuya::dp::binary(36,"frost_protection"),
    ::zhc::tuya::dp::binary(39,"scale_protection"),
    ::zhc::tuya::dp::numeric(47,"local_temperature_calibration",10),
    ::zhc::tuya::dp::numeric(101,"operating_time",10),
    ::zhc::tuya::dp::numeric(102,"scale_protection_remaining_time",10)
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_THAH202001>;

constexpr const char* kOpts_THAH202001_preset[] = { "heat", "off" };
constexpr const char* kOpts_THAH202001_system_mode[] = { "heat", "off" };
constexpr const char* kOpts_THAH202001_running_state[] = { "heat", "idle" };
constexpr const char* kOpts_THAH202001_window_open[] = { "OPEN", "CLOSE" };

constexpr Expose kExp_THAH202001[] = {
    {"preset", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_THAH202001_preset, 2},
    {"system_mode", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_THAH202001_system_mode, 2},
    {"running_state", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_THAH202001_running_state, 2},
    {"current_heating_setpoint", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, ::zhc::Access::StateSet, "%", nullptr, nullptr, 0},
    {"window_detection", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_open", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_THAH202001_window_open, 2},
    {"holiday_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"eco_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"away_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"frost_protection", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scale_protection", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"operating_time", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"scale_protection_remaining_time", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr const char* kM_THAH202001[] = { "TS0601" };
constexpr const char* kN_THAH202001[] = { "_TZE204_m5r5nlxc" };
}  // namespace

extern const PreparedDefinition kDef_THAH202001{
    .zigbee_models=kM_THAH202001,.zigbee_models_count=sizeof(kM_THAH202001)/sizeof(kM_THAH202001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_THAH202001,.manufacturer_names_count=sizeof(kN_THAH202001)/sizeof(kN_THAH202001[0]),
    .model="THAH202001",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp_THAH202001,.exposes_count=sizeof(kExp_THAH202001)/sizeof(kExp_THAH202001[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
