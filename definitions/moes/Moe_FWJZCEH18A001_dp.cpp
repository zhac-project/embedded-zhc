// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Moes FWJZCEH18A001 roller blind motor, graduated from the generated
// stub (which drove this Tuya datapoint motor through closuresWindowCovering).
// z2m v26.115.1 adds `_TZE2841000000_u68q868h`. Datapoints: 1 state, 8/9
// position, 11 motor_direction, 16 cover_limit. Not ported: battery (dp 13
// arrives as a base64 string, z2m readUInt32BE of the decoded bytes).
// z2m-source: moes.ts #FWJZCEH18A001.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::moes {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kState1[] = { {0,"OPEN"}, {1,"STOP"}, {2,"CLOSE"}, {3,"CONTINUE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kMotorDirection11[] = { {0,"normal"}, {1,"reversed"} };
constexpr ::zhc::tuya::TuyaEnumEntry kCoverLimit16[] = { {0,"set_up"}, {1,"set_down"}, {2,"delete_up"}, {3,"delete_down"}, {4,"delete_both"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kState1, 4),
        ::zhc::tuya::dp::numeric(9, "position", 1),
        ::zhc::tuya::dp::numeric(8, "position", 1),
        ::zhc::tuya::dp::enum_lookup(11, "motor_direction", kMotorDirection11, 2),
        ::zhc::tuya::dp::enum_lookup(16, "cover_limit", kCoverLimit16, 5),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts0[] = { "OPEN", "CLOSE", "STOP" };
constexpr const char* kOpts2[] = { "normal", "reversed" };
constexpr const char* kOpts3[] = { "set_up", "set_down", "delete_up", "delete_down", "delete_both" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts0, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"motor_direction", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts2, 2},
    {"cover_limit", ExposeType::Enum, Access::StateSet, nullptr, "Set current position as the limit position", kOpts3, 5, ExposeCategory::Config},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_qoi1aqxg", "_TZE2841000000_u68q868h" };
}  // namespace

extern const PreparedDefinition kDef_FWJZCEH18A001_dp{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="FWJZCEH18A001",.vendor="Moes",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::moes
