// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Jikaida JKD-839Z gas sensor, TS0601 `_TZE204_mqij5jas` (z2m v26.133.0).
// DPs: 1 gas (trueFalse0), 2 gas_value, 9 self_test_result, 16 silence.
// tuyaBase queryOnConfigure / queryOnDeviceAnnounce (dataQuery) not ported.
// z2m-source: tuya.ts #JKD-839Z.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSelfTestResult[] = { {0,"checking"}, {1,"success"}, {2,"failure"}, {3,"others"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary_inv(1, "gas"),
        ::zhc::tuya::dp::numeric(2, "gas_value", 1),
        ::zhc::tuya::dp::enum_lookup(9, "self_test_result", kSelfTestResult, 4),
        ::zhc::tuya::dp::binary(16, "silence"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOptsSelfTest[] = { "checking", "success", "failure", "others" };
constexpr Expose kExp[] = {
    {"gas", ExposeType::Binary, Access::State, nullptr, "Indicates whether the device detected gas", nullptr, 0},
    {"gas_value", ExposeType::Numeric, Access::State, nullptr, "Measured gas concentration", nullptr, 0},
    {"self_test_result", ExposeType::Enum, Access::State, nullptr, "Result of the self-test", kOptsSelfTest, 4},
    {"silence", ExposeType::Binary, Access::StateSet, nullptr, "Silence the alarm", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_mqij5jas" };
}  // namespace

extern const PreparedDefinition kDef_JKD_839Z{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="JKD-839Z",.vendor="Jikaida",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
