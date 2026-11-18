// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HYSYIOT HS209Z PIR + mmWave radar presence sensor (z2m v26.133.0).
// Upstream fingerprints TS0601 and TS0202, each with both `_TZD200_bgjlfsuy` and
// `_TZE200_bgjlfsuy`, so the manu x model cross product is exact; the bare
// zigbeeModel HS209Z is the second definition, kDef_HS209Z_model.
// ZCL like HS208Z: iasZoneAlarm occupancy {alarm_1, battery_low} + m.battery
// (genPowerCfg bind, percentage reporting 1 h / max / 10). dp* extends:
// 3 battery_state (dpBatteryState, enum low/medium/high), 12 pir_delay,
// 103 detection_distance, 104 static_sensitivity, 107 indicator (ON/OFF),
// 123 detection_sensitivity.
// z2m-source: tuya.ts #HS209Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kBatteryState[] = { {0,"low"}, {1,"medium"}, {2,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kOnOff[] = { {0,"OFF"}, {1,"ON"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(3, "battery_state", kBatteryState, 3),
        ::zhc::tuya::dp::numeric(12, "pir_delay", 1),
        ::zhc::tuya::dp::numeric(103, "detection_distance", 1),
        ::zhc::tuya::dp::numeric(104, "static_sensitivity", 1),
        { 107, "indicator", ::zhc::TuyaDpType::Bool, 1, kOnOff, 2, ::zhc::tuya::kTuyaDpFlagBoolEnum },
        ::zhc::tuya::dp::numeric(123, "detection_sensitivity", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
const FzConverter* const kFz[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::tuya::factory::detail::BoundFz<cfg>::converter,
    &::zhc::generic::kFzIasMotionAlarm,   // occupancy + battery_low (+ tamper, unexposed)
    &::zhc::generic::kFzBattery,
};
constexpr BindingSpec kBindings[] = { {1, 0x0001} };
constexpr ReportingSpec kReports[] = {
    {1, 0x0001, 0x0021, 0x20, 3600, 65000, 10, 0},   // batteryPercentageRemaining
};
constexpr const char* kOptsBattery[] = { "low", "medium", "high" };
constexpr Expose kExp[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, "Indicates whether the device detected occupancy", nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, "Indicates whether the battery of the device is almost empty", nullptr, 0, ExposeCategory::Diagnostic},
    {"battery", ExposeType::Numeric, Access::State, "%", "Remaining battery in %", nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"battery_state", ExposeType::Enum, Access::State, nullptr, "State of the battery", kOptsBattery, 3, ExposeCategory::Diagnostic},
    {"pir_delay", ExposeType::Numeric, Access::StateSet, "s", "PIR occupancy keep time", nullptr, 0, ExposeCategory::State, 30, 28800, 30},
    {"detection_distance", ExposeType::Numeric, Access::StateSet, "cm", "Radar detection distance", nullptr, 0, ExposeCategory::State, 1, 500, 0},
    {"static_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, "Radar static detection sensitivity", nullptr, 0, ExposeCategory::State, 1, 10, 0},
    {"detection_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, "Radar motion detection sensitivity", nullptr, 0, ExposeCategory::State, 1, 10, 0},
    {"indicator", ExposeType::Binary, Access::StateSet, nullptr, "LED indicator", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601", "TS0202" };
constexpr const char* kN[] = { "_TZD200_bgjlfsuy", "_TZE200_bgjlfsuy" };
constexpr const char* kM_model[] = { "HS209Z" };
}  // namespace

extern const PreparedDefinition kDef_HS209Z{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="HS209Z",.vendor="HYSYIOT",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr,
    .bindings=kBindings,.bindings_count=1,
    .reports=kReports,.reports_count=1 };

// Upstream's zigbeeModel fingerprint: "HS209Z" with any manufacturer.
extern const PreparedDefinition kDef_HS209Z_model{
    .zigbee_models=kM_model,.zigbee_models_count=sizeof(kM_model)/sizeof(kM_model[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="HS209Z",.vendor="HYSYIOT",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr,
    .bindings=kBindings,.bindings_count=1,
    .reports=kReports,.reports_count=1 };

}  // namespace zhc::devices::tuya
