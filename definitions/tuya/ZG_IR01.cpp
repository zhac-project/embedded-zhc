// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya ZG-IR01 smart IR remote switch (z2m v26.95.0 parity).
// z2m-source: tuya.ts #ZG-IR01 (added v26.95.0)
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

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_111[] = { {0,"celsius"},{1,"fahrenheit"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_120[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_121[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_122[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_123[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_124[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_125[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_126[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_127[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_128[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_129[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_130[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZG_IR01_131[] = { {0,"study"},{1,"registered"},{2,"unregistered"} };

struct cfg_ZG_IR01 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
    ::zhc::tuya::dp::binary(1,"switch1"),
    ::zhc::tuya::dp::binary(2,"switch2"),
    ::zhc::tuya::dp::binary(3,"switch3"),
    ::zhc::tuya::dp::binary(4,"switch4"),
    ::zhc::tuya::dp::binary(5,"switch5"),
    ::zhc::tuya::dp::binary(6,"switch6"),
    ::zhc::tuya::dp::temperature(109),
    ::zhc::tuya::dp::numeric(110,"humidity",1),
    ::zhc::tuya::dp::numeric(112,"battery",1),
    ::zhc::tuya::dp::enum_lookup(111,"temperature_unit",kEnum_ZG_IR01_111,2),
    ::zhc::tuya::dp::numeric(107,"temperature_calibration",10),
    ::zhc::tuya::dp::numeric(108,"humidity_calibration",1),
    ::zhc::tuya::dp::enum_lookup(120,"switch1_on",kEnum_ZG_IR01_120,3),
    ::zhc::tuya::dp::enum_lookup(121,"switch1_off",kEnum_ZG_IR01_121,3),
    ::zhc::tuya::dp::enum_lookup(122,"switch2_on",kEnum_ZG_IR01_122,3),
    ::zhc::tuya::dp::enum_lookup(123,"switch2_off",kEnum_ZG_IR01_123,3),
    ::zhc::tuya::dp::enum_lookup(124,"switch3_on",kEnum_ZG_IR01_124,3),
    ::zhc::tuya::dp::enum_lookup(125,"switch3_off",kEnum_ZG_IR01_125,3),
    ::zhc::tuya::dp::enum_lookup(126,"switch4_on",kEnum_ZG_IR01_126,3),
    ::zhc::tuya::dp::enum_lookup(127,"switch4_off",kEnum_ZG_IR01_127,3),
    ::zhc::tuya::dp::enum_lookup(128,"switch5_on",kEnum_ZG_IR01_128,3),
    ::zhc::tuya::dp::enum_lookup(129,"switch5_off",kEnum_ZG_IR01_129,3),
    ::zhc::tuya::dp::enum_lookup(130,"switch6_on",kEnum_ZG_IR01_130,3),
    ::zhc::tuya::dp::enum_lookup(131,"switch6_off",kEnum_ZG_IR01_131,3)
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_ZG_IR01>;

constexpr const char* kOpts_ZG_IR01_temperature_unit[] = { "celsius", "fahrenheit" };
constexpr const char* kOpts_ZG_IR01_switch1_on[] = { "study", "registered", "unregistered" };
constexpr const char* kOpts_ZG_IR01_switch1_off[] = { "study", "registered", "unregistered" };
constexpr const char* kOpts_ZG_IR01_switch2_on[] = { "study", "registered", "unregistered" };
constexpr const char* kOpts_ZG_IR01_switch2_off[] = { "study", "registered", "unregistered" };
constexpr const char* kOpts_ZG_IR01_switch3_on[] = { "study", "registered", "unregistered" };
constexpr const char* kOpts_ZG_IR01_switch3_off[] = { "study", "registered", "unregistered" };
constexpr const char* kOpts_ZG_IR01_switch4_on[] = { "study", "registered", "unregistered" };
constexpr const char* kOpts_ZG_IR01_switch4_off[] = { "study", "registered", "unregistered" };
constexpr const char* kOpts_ZG_IR01_switch5_on[] = { "study", "registered", "unregistered" };
constexpr const char* kOpts_ZG_IR01_switch5_off[] = { "study", "registered", "unregistered" };
constexpr const char* kOpts_ZG_IR01_switch6_on[] = { "study", "registered", "unregistered" };
constexpr const char* kOpts_ZG_IR01_switch6_off[] = { "study", "registered", "unregistered" };

constexpr Expose kExp_ZG_IR01[] = {
    {"switch1", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch2", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch3", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch4", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch5", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch6", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, ::zhc::Access::StateSet, "%", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, ::zhc::Access::StateSet, "%", nullptr, nullptr, 0},
    {"temperature_unit", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_temperature_unit, 2},
    {"temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch1_on", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch1_on, 3},
    {"switch1_off", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch1_off, 3},
    {"switch2_on", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch2_on, 3},
    {"switch2_off", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch2_off, 3},
    {"switch3_on", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch3_on, 3},
    {"switch3_off", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch3_off, 3},
    {"switch4_on", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch4_on, 3},
    {"switch4_off", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch4_off, 3},
    {"switch5_on", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch5_on, 3},
    {"switch5_off", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch5_off, 3},
    {"switch6_on", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch6_on, 3},
    {"switch6_off", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_IR01_switch6_off, 3},
};

constexpr const char* kM_ZG_IR01[] = { "ZG-IR01" };
}  // namespace

extern const PreparedDefinition kDef_ZG_IR01{
    .zigbee_models=kM_ZG_IR01,.zigbee_models_count=sizeof(kM_ZG_IR01)/sizeof(kM_ZG_IR01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="ZG-IR01",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp_ZG_IR01,.exposes_count=sizeof(kExp_ZG_IR01)/sizeof(kExp_ZG_IR01[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
