// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya 07P4 smart panel — 4-gang switch with scene, dimmer and
// curtain control (`_TZE284_0or4eirj`), new upstream in z2m v26.133.0.
// F3-Pro family with shifted datapoints (relays 24-27); reports arrive with
// EF00 command 0x0A, which the decoder already treats as a DP report.
//
//   dp1-8     static scene actions scene_1..scene_8 (value ignored)
//   dp24-27   state_l1..l4            dp39-42   power_on_behavior_l1..l4
//   dp119-122 cover_state_l1..l4      dp123-126 led_switch_l1..l4
//   dp127-130 led_bright_l1..l4       dp131-134 led_warm_l1..l4
//   dp135-138 cover_position_l1..l4 (raw, not inverted)
// tuyaBase timeStart "1970" -> tuya_time_start 1.
//
// NOT PORTED: the weather-forecast extend (temperature_0 / humidity_0 /
// condition_0 and its request-sequence replies), the UTF-8 name datapoints
// 103-118 (string DPs; no string DP codec here) and forceTimeUpdates.
//
// z2m-source: tuya.ts #07P4.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

// z2m tuya.valueConverter.static: the datapoint only signals; publish the
// label carried in expand_cfg whatever its value.
bool static_label(const ::zhc::tuya::TuyaDpMapEntry& e, const Value&, RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value v{}; v.type = ValueType::StringRef; v.str = static_cast<const char*>(e.expand_cfg);
    out.put(e.out_key, v);
    return true;
}

constexpr ::zhc::tuya::TuyaEnumEntry kPob[]   = { {0, "off"}, {1, "on"}, {2, "previous"} };
constexpr ::zhc::tuya::TuyaEnumEntry kCover[] = { {0, "open"}, {1, "stop"}, {2, "close"} };

#define ZHC_07P4_SCENE(n) \
    { n, "action", ::zhc::TuyaDpType::Enum, 1, nullptr, 0, 0, 0.0f, &static_label, "scene_" #n }

struct cfg_07p4 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ZHC_07P4_SCENE(1), ZHC_07P4_SCENE(2), ZHC_07P4_SCENE(3), ZHC_07P4_SCENE(4),
        ZHC_07P4_SCENE(5), ZHC_07P4_SCENE(6), ZHC_07P4_SCENE(7), ZHC_07P4_SCENE(8),
        ::zhc::tuya::dp::binary(24, "state_l1"),
        ::zhc::tuya::dp::binary(25, "state_l2"),
        ::zhc::tuya::dp::binary(26, "state_l3"),
        ::zhc::tuya::dp::binary(27, "state_l4"),
        ::zhc::tuya::dp::enum_lookup(39, "power_on_behavior_l1", kPob, 3),
        ::zhc::tuya::dp::enum_lookup(40, "power_on_behavior_l2", kPob, 3),
        ::zhc::tuya::dp::enum_lookup(41, "power_on_behavior_l3", kPob, 3),
        ::zhc::tuya::dp::enum_lookup(42, "power_on_behavior_l4", kPob, 3),
        ::zhc::tuya::dp::enum_lookup(119, "cover_state_l1", kCover, 3),
        ::zhc::tuya::dp::enum_lookup(120, "cover_state_l2", kCover, 3),
        ::zhc::tuya::dp::enum_lookup(121, "cover_state_l3", kCover, 3),
        ::zhc::tuya::dp::enum_lookup(122, "cover_state_l4", kCover, 3),
        ::zhc::tuya::dp::binary(123, "led_switch_l1"),
        ::zhc::tuya::dp::binary(124, "led_switch_l2"),
        ::zhc::tuya::dp::binary(125, "led_switch_l3"),
        ::zhc::tuya::dp::binary(126, "led_switch_l4"),
        ::zhc::tuya::dp::numeric(127, "led_bright_l1", 1),
        ::zhc::tuya::dp::numeric(128, "led_bright_l2", 1),
        ::zhc::tuya::dp::numeric(129, "led_bright_l3", 1),
        ::zhc::tuya::dp::numeric(130, "led_bright_l4", 1),
        ::zhc::tuya::dp::numeric(131, "led_warm_l1", 1),
        ::zhc::tuya::dp::numeric(132, "led_warm_l2", 1),
        ::zhc::tuya::dp::numeric(133, "led_warm_l3", 1),
        ::zhc::tuya::dp::numeric(134, "led_warm_l4", 1),
        ::zhc::tuya::dp::numeric(135, "cover_position_l1", 1),
        ::zhc::tuya::dp::numeric(136, "cover_position_l2", 1),
        ::zhc::tuya::dp::numeric(137, "cover_position_l3", 1),
        ::zhc::tuya::dp::numeric(138, "cover_position_l4", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
#undef ZHC_07P4_SCENE
using FX = ::zhc::tuya::factory::TuyaRw<cfg_07p4>;

constexpr const char* kPobOpts[]    = { "off", "on", "previous" };
constexpr const char* kCoverOpts[]  = { "open", "stop", "close" };
constexpr const char* kActionOpts[] = {
    "scene_1", "scene_2", "scene_3", "scene_4", "scene_5", "scene_6", "scene_7", "scene_8",
};

#define ZHC_07P4_GANG(n)                                                                          \
    { "state_l" #n, ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },  \
    { "power_on_behavior_l" #n, ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr,      \
      kPobOpts, 3 },                                                                              \
    { "led_switch_l" #n, ExposeType::Binary, ::zhc::Access::StateSet, nullptr,                    \
      "Switch of LED " #n, nullptr, 0 },                                                          \
    { "led_warm_l" #n, ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,                     \
      "Color temperature of LED " #n, nullptr, 0, ExposeCategory::State, 1, 100, 0 },             \
    { "led_bright_l" #n, ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,                   \
      "Brightness of LED " #n, nullptr, 0, ExposeCategory::State, 1, 100, 0 },                    \
    { "cover_state_l" #n, ExposeType::Enum, ::zhc::Access::StateSet, nullptr,                     \
      "State of Cover " #n, kCoverOpts, 3 },                                                      \
    { "cover_position_l" #n, ExposeType::Numeric, ::zhc::Access::StateSet, "%",                   \
      "Position of Cover " #n, nullptr, 0, ExposeCategory::State, 0, 100, 0 }

constexpr Expose kExposes[] = {
    ZHC_07P4_GANG(1), ZHC_07P4_GANG(2), ZHC_07P4_GANG(3), ZHC_07P4_GANG(4),
    { "action", ExposeType::Enum, ::zhc::Access::State, nullptr, nullptr,
      kActionOpts, sizeof(kActionOpts)/sizeof(kActionOpts[0]) },
};
#undef ZHC_07P4_GANG

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE284_0or4eirj" };

}  // namespace

extern const PreparedDefinition kDef_07P4{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="07P4", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .tuya_time_start=1 };

}  // namespace zhc::devices::tuya
