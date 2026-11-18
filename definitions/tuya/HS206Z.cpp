// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HYSYIOT HS206Z PIR motion / presence sensor (z2m v26.133.0).
// Upstream matches TS0202 + `_TZD200_rdbronta`, or the zigbeeModel HS206Z alone;
// the bare-model fingerprint is the second definition, kDef_HS206Z_model.
// ZCL: fz.ias_occupancy_alarm_1 (+ _report, attribute path not expressible,
// see tuya/TS0202.cpp) and fz.battery. DPs: 1 occupancy (trueFalse0),
// 3 battery_state, 4 battery, 9 pir_sensitivity, 12 pir_delay.
// z2m-source: tuya.ts #HS206Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kLowMiddleHigh[] = { {0,"low"}, {1,"middle"}, {2,"high"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary_inv(1, "occupancy"),
        ::zhc::tuya::dp::enum_lookup(3, "battery_state", kLowMiddleHigh, 3),
        ::zhc::tuya::dp::numeric(4, "battery", 1),
        ::zhc::tuya::dp::enum_lookup(9, "pir_sensitivity", kLowMiddleHigh, 3),
        ::zhc::tuya::dp::numeric(12, "pir_delay", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
const FzConverter* const kFz[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::tuya::factory::detail::BoundFz<cfg>::converter,
    &::zhc::generic::kFzIasMotionAlarm,   // occupancy + tamper + battery_low
    &::zhc::generic::kFzBattery,
};
constexpr const char* kOptsLMH[] = { "low", "middle", "high" };
constexpr Expose kExp[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, "Indicates whether the device detected occupancy", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", "Remaining battery in %", nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, "Indicates if the battery is low", nullptr, 0, ExposeCategory::Diagnostic},
    {"battery_state", ExposeType::Enum, Access::State, nullptr, "Battery level state: low, middle, or high", kOptsLMH, 3},
    {"pir_sensitivity", ExposeType::Enum, Access::StateSet, nullptr, "PIR detection sensitivity", kOptsLMH, 3},
    {"pir_delay", ExposeType::Numeric, Access::StateSet, "s", "PIR occupancy detection delay / keep time", nullptr, 0, ExposeCategory::State, 30, 120, 1},
};
constexpr const char* kM[] = { "TS0202" };
constexpr const char* kN[] = { "_TZD200_rdbronta" };
constexpr const char* kM_model[] = { "HS206Z" };
}  // namespace

extern const PreparedDefinition kDef_HS206Z{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="HS206Z",.vendor="HYSYIOT",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

// Upstream's second fingerprint: zigbeeModel "HS206Z" with any manufacturer.
extern const PreparedDefinition kDef_HS206Z_model{
    .zigbee_models=kM_model,.zigbee_models_count=sizeof(kM_model)/sizeof(kM_model[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="HS206Z",.vendor="HYSYIOT",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
