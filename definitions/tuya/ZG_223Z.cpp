// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HOBEIAN ZG-223Z rainwater sensor, graduated from the generated copy
// (which exposed only a stray `action`). z2m v26.115.1 moves it to hobeian.ts
// and adds `_TZE200_gt1gge3x` and the HYSYIOT HS118Z (zigbeeModel HS118Z,
// manufacturer HYSYIOT). Datapoints: 1 rainwater, 2 sensitivity,
// 101 illuminance_sampling, 102 illuminance, 104 battery. The zigbeeModel
// fingerprints are the second definition, kDef_ZG_223Z_model.
// z2m-source: hobeian.ts #ZG-223Z.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kRainwater1[] = { {0,"none"}, {1,"raining"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "rainwater", kRainwater1, 2),
        ::zhc::tuya::dp::numeric(102, "illuminance", 1),
        ::zhc::tuya::dp::numeric(104, "battery", 1),
        ::zhc::tuya::dp::numeric(2, "sensitivity", 1),
        ::zhc::tuya::dp::numeric(101, "illuminance_sampling", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts0[] = { "none", "raining" };
constexpr Expose kExp[] = {
    {"rainwater", ExposeType::Enum, Access::State, nullptr, "Sensor rainwater status", kOpts0, 2},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, "The larger the value, the more sensitive it is", nullptr, 0, ExposeCategory::State, 0, 9, 1},
    {"illuminance_sampling", ExposeType::Numeric, Access::StateSet, "min", "Brightness acquisition interval", nullptr, 0, ExposeCategory::State, 1, 480, 1},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE200_jsaqgakf", "_TZE200_u6x1zyv2", "_TZE200_2pddnnrk", "_TZE200_gt1gge3x" };
constexpr WhiteLabel kWL[] = { {"HYSYIOT", "HS118Z"} };
}  // namespace

extern const PreparedDefinition kDef_ZG_223Z{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZG-223Z",.vendor="HOBEIAN",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

namespace {
constexpr const char* kM_model[] = { "ZG-223Z", "HS118Z" };
}  // namespace

// Upstream's zigbeeModel fingerprints match whatever the manufacturer name.
extern const PreparedDefinition kDef_ZG_223Z_model{
    .zigbee_models=kM_model,.zigbee_models_count=sizeof(kM_model)/sizeof(kM_model[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="ZG-223Z",.vendor="HOBEIAN",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
