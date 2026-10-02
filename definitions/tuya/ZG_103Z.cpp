// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya ZG-103Z vibration sensor, graduated from the generated copy.
// z2m v26.115.1 moves `_TZE200_yjryxpot` from here to ZG-102ZM. Datapoints:
// 1 vibration / 7 tilt (trueFalseEnum1), 101-103 x / y / z, 104 sensitivity,
// 105 battery. zigbeeModel ZG-103Z is kDef_ZG_103Z_model.
// z2m-source: hobeian.ts #ZG-103Z.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSensitivity104[] = { {0,"low"}, {1,"middle"}, {2,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum1[] = { {1, "true"} };   // trueFalseEnum1
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        { 1, "vibration", ::zhc::TuyaDpType::Enum, 1, kEnum1, 1, ::zhc::tuya::kTuyaDpFlagEnumBool },
        { 7, "tilt", ::zhc::TuyaDpType::Enum, 1, kEnum1, 1, ::zhc::tuya::kTuyaDpFlagEnumBool },
        ::zhc::tuya::dp::numeric(101, "x", 1),
        ::zhc::tuya::dp::numeric(102, "y", 1),
        ::zhc::tuya::dp::numeric(103, "z", 1),
        ::zhc::tuya::dp::enum_lookup(104, "sensitivity", kSensitivity104, 3),
        ::zhc::tuya::dp::numeric(105, "battery", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts6[] = { "low", "middle", "high" };
constexpr Expose kExp[] = {
    {"vibration", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tilt", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"x", ExposeType::Numeric, Access::State, nullptr, "X coordinate", nullptr, 0, ExposeCategory::State, 0, 256, 1},
    {"y", ExposeType::Numeric, Access::State, nullptr, "Y coordinate", nullptr, 0, ExposeCategory::State, 0, 256, 1},
    {"z", ExposeType::Numeric, Access::State, nullptr, "Z coordinate", nullptr, 0, ExposeCategory::State, 0, 256, 1},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"sensitivity", ExposeType::Enum, Access::StateSet, nullptr, "Vibration detection sensitivity", kOpts6, 3},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE200_iba1ckek", "_TZE200_hggxgsjj", "_TZE200_afycb3cg" };
constexpr WhiteLabel kWL[] = { {"HOBEIAN", "ZG-103Z"} };
}  // namespace

extern const PreparedDefinition kDef_ZG_103Z{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZG-103Z",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

namespace {
constexpr const char* kM_model[] = { "ZG-103Z" };
}  // namespace

// Upstream's zigbeeModel fingerprints match whatever the manufacturer name.
extern const PreparedDefinition kDef_ZG_103Z_model{
    .zigbee_models=kM_model,.zigbee_models_count=sizeof(kM_model)/sizeof(kM_model[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="ZG-103Z",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
