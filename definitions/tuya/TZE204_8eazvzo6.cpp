// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Semicom touch panel: 6 switches (z2m v26.95.0 parity).
// z2m-source: tuya.ts #_TZE204_8eazvzo6 (added v26.95.0)
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

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_8eazvzo6_1[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_8eazvzo6_2[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_8eazvzo6_3[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_8eazvzo6_4[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_8eazvzo6_5[] = { {0,"OFF"},{1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_8eazvzo6_6[] = { {0,"OFF"},{1,"ON"} };

struct cfg_TZE204_8eazvzo6 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
    ::zhc::tuya::dp::enum_lookup(1,"state_s1",kEnum_8eazvzo6_1,2),
    ::zhc::tuya::dp::enum_lookup(2,"state_s2",kEnum_8eazvzo6_2,2),
    ::zhc::tuya::dp::enum_lookup(3,"state_s3",kEnum_8eazvzo6_3,2),
    ::zhc::tuya::dp::enum_lookup(4,"state_s4",kEnum_8eazvzo6_4,2),
    ::zhc::tuya::dp::enum_lookup(5,"state_s5",kEnum_8eazvzo6_5,2),
    ::zhc::tuya::dp::enum_lookup(6,"state_s6",kEnum_8eazvzo6_6,2)
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_TZE204_8eazvzo6>;

constexpr const char* kOpts_TZE204_8eazvzo6_state_s1[] = { "OFF", "ON" };
constexpr const char* kOpts_TZE204_8eazvzo6_state_s2[] = { "OFF", "ON" };
constexpr const char* kOpts_TZE204_8eazvzo6_state_s3[] = { "OFF", "ON" };
constexpr const char* kOpts_TZE204_8eazvzo6_state_s4[] = { "OFF", "ON" };
constexpr const char* kOpts_TZE204_8eazvzo6_state_s5[] = { "OFF", "ON" };
constexpr const char* kOpts_TZE204_8eazvzo6_state_s6[] = { "OFF", "ON" };

constexpr Expose kExp_TZE204_8eazvzo6[] = {
    {"state_s1", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TZE204_8eazvzo6_state_s1, 2},
    {"state_s2", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TZE204_8eazvzo6_state_s2, 2},
    {"state_s3", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TZE204_8eazvzo6_state_s3, 2},
    {"state_s4", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TZE204_8eazvzo6_state_s4, 2},
    {"state_s5", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TZE204_8eazvzo6_state_s5, 2},
    {"state_s6", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_TZE204_8eazvzo6_state_s6, 2},
};

constexpr const char* kM_TZE204_8eazvzo6[] = { "TS0601" };
constexpr const char* kN_TZE204_8eazvzo6[] = { "_TZE204_8eazvzo6" };
}  // namespace

extern const PreparedDefinition kDef_TZE204_8eazvzo6{
    .zigbee_models=kM_TZE204_8eazvzo6,.zigbee_models_count=sizeof(kM_TZE204_8eazvzo6)/sizeof(kM_TZE204_8eazvzo6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_TZE204_8eazvzo6,.manufacturer_names_count=sizeof(kN_TZE204_8eazvzo6)/sizeof(kN_TZE204_8eazvzo6[0]),
    .model="_TZE204_8eazvzo6",.vendor="Semicom",.meta=nullptr,
    .exposes=kExp_TZE204_8eazvzo6,.exposes_count=sizeof(kExp_TZE204_8eazvzo6)/sizeof(kExp_TZE204_8eazvzo6[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
