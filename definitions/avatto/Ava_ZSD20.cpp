// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: AVATTO ZSD20 smart smoke alarm (z2m v26.114.0, #13187).
// DP11 `fault`: bit names joined with ", ", "No faults" when clear, as upstream.
// z2m-source: avatto.ts #ZSD20.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::avatto {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSmokeSensorState1[] = { {0,"alarm"}, {1,"normal"} };
constexpr const char* kFaults[] = { "serious fault", "sensor fault", "probe fault", "power fault" };
constexpr ::zhc::tuya::TuyaFaultTable kFaultTable{ kFaults, 4, ", ", "No faults" };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "smoke_sensor_state", kSmokeSensorState1, 2),
        ::zhc::tuya::dp::binary(8, "self_checking"),
        ::zhc::tuya::dp::fault_bitmap(11, "fault", &kFaultTable),
        ::zhc::tuya::dp::numeric(15, "battery", 1),
        ::zhc::tuya::dp::binary(16, "muffling"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts0[] = { "alarm", "normal" };
constexpr Expose kExp[] = {
    {"smoke_sensor_state", ExposeType::Enum, Access::State, nullptr, "Smoke sensor state", kOpts0, 2},
    {"self_checking", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"fault", ExposeType::String, Access::State, nullptr, "Fault status", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"muffling", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_uqzwwjas", "_TZE284_zeeqkb0p" };
}  // namespace

extern const PreparedDefinition kDef_ZSD20{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZSD20",.vendor="AVATTO",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::avatto
