// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Moes ZC-HM carbon monoxide alarm, graduated from the generated stub
// (which read a Tuya datapoint device through the IAS zone converter).
// z2m v26.115.1 adds manufacturer `JM720ES-EF-3.0`. Datapoints: 1
// carbon_monoxide (trueFalse0: 0 = alarm), 2 co (ppm), 9 self-test result,
// 15 battery, 16 silence.
// z2m-source: moes.ts #ZC-HM.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::moes {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSelfTestResult9[] = { {0,"checking"}, {1,"success"}, {2,"failure"}, {3,"others"} };
constexpr ::zhc::tuya::TuyaEnumEntry kAlarm0[] = { {0, "alarm"} };   // trueFalse0
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        { 1, "carbon_monoxide", ::zhc::TuyaDpType::Enum, 1, kAlarm0, 1, ::zhc::tuya::kTuyaDpFlagEnumBool },
        ::zhc::tuya::dp::numeric(2, "co", 1),
        ::zhc::tuya::dp::enum_lookup(9, "self_test_result", kSelfTestResult9, 4),
        ::zhc::tuya::dp::numeric(15, "battery", 1),
        ::zhc::tuya::dp::binary(16, "silence"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts2[] = { "checking", "success", "failure", "others" };
constexpr Expose kExp[] = {
    {"carbon_monoxide", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"co", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
    {"self_test_result", ExposeType::Enum, Access::State, nullptr, "Result of the self-test", kOpts2, 4},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"silence", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE200_hr0tdd47", "_TZE200_rjxqso4a", "_TZE284_rjxqso4a", "JM720ES-EF-3.0" };
constexpr WhiteLabel kWL[] = { {"Heiman", "HS-720ES"} };
}  // namespace

extern const PreparedDefinition kDef_ZC_HM_dp{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZC-HM",.vendor="Moes",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::moes
