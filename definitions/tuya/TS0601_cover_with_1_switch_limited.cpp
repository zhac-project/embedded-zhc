// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya cover switch without position control + 1 gang (z2m v26.113.0, #13294).
// The gang is a datapoint on endpoint 1, so `state_l1` is a datapoint key.
// z2m-source: tuya.ts #TS0601_cover_with_1_switch_limited.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kState1[] = { {0,"OPEN"}, {1,"STOP"}, {2,"CLOSE"}, {3,"CONTINUE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMotorDirection8[] = { {0,"normal"}, {1,"reversed"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kState1, 4),
        ::zhc::tuya::dp::binary(7, "state_l1"),
        ::zhc::tuya::dp::enum_lookup(8, "motor_direction", kMotorDirection8, 2),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts0[] = { "OPEN", "CLOSE", "STOP" };
constexpr const char* kOpts2[] = { "normal", "reversed" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts0, 3},
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"motor_direction", ExposeType::Enum, Access::StateSet, nullptr, "Motor direction", kOpts2, 2},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_pxbjch8m" };
}  // namespace

extern const PreparedDefinition kDef_TS0601_cover_with_1_switch_limited{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0601_cover_with_1_switch_limited",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
