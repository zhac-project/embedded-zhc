// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya T3E_mua6ucdj touch panel, 3 relays + scene buttons (`_TZE200_mua6ucdj`).
// DP24-26 state_l1..l3; DP130-141 scene_switch_<row>_<col> (bool, writable)
// each also firing action `<name>_on` / `<name>_off`; DP5-10 and DP104-127
// (no 113/114) fire a fixed action when they report 0 (z2m t3eAction).
// z2m-source: tuya.ts #T3E_mua6ucdj.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

using ::zhc::tuya::TuyaDpMapEntry;
using Out = FixedPayload<ZHC_FIXED_PAYLOAD_CAP>;

// z2m t3eAction: `value === 0 ? {action} : {}` — numeric or enum wire.
bool t3e_action(const TuyaDpMapEntry& e, const Value& raw, RuntimeContext&, Out& out) {
    if ((raw.type == ValueType::Uint && raw.u == 0) || (raw.type == ValueType::Int && raw.i == 0)) {
        Value v{}; v.type = ValueType::StringRef; v.str = static_cast<const char*>(e.expand_cfg);
        out.put("action", v);
    }
    return true;
}

struct SceneActs { const char* on; const char* off; };

// z2m scene_switch receive: boolean only -> {<name>: ON/OFF, action: <name>_on/_off}.
bool t3e_scene(const TuyaDpMapEntry& e, const Value& raw, RuntimeContext&, Out& out) {
    if (raw.type != ValueType::Bool) return false;
    const auto* a = static_cast<const SceneActs*>(e.expand_cfg);
    out.put(e.out_key, raw);
    Value v{}; v.type = ValueType::StringRef; v.str = raw.b ? a->on : a->off;
    out.put("action", v);
    return true;
}

constexpr TuyaDpMapEntry act(std::uint8_t dp, const char* name) {
    return { dp, nullptr, TuyaDpType::Raw, 1, nullptr, 0, 0, 0.0f, &t3e_action, name };
}

constexpr SceneActs kScene[12] = {
    {"scene_switch_1_1_on", "scene_switch_1_1_off"}, {"scene_switch_1_2_on", "scene_switch_1_2_off"},
    {"scene_switch_1_3_on", "scene_switch_1_3_off"}, {"scene_switch_1_4_on", "scene_switch_1_4_off"},
    {"scene_switch_2_1_on", "scene_switch_2_1_off"}, {"scene_switch_2_2_on", "scene_switch_2_2_off"},
    {"scene_switch_2_3_on", "scene_switch_2_3_off"}, {"scene_switch_2_4_on", "scene_switch_2_4_off"},
    {"scene_switch_3_1_on", "scene_switch_3_1_off"}, {"scene_switch_3_2_on", "scene_switch_3_2_off"},
    {"scene_switch_3_3_on", "scene_switch_3_3_off"}, {"scene_switch_3_4_on", "scene_switch_3_4_off"},
};

// Bool row: decode via t3e_scene, writes ON/OFF/bool through the plain Bool encode.
constexpr TuyaDpMapEntry scene(std::uint8_t i, const char* key) {
    return { static_cast<std::uint8_t>(130 + i), key, TuyaDpType::Bool, 1, nullptr, 0, 0, 0.0f,
             &t3e_scene, &kScene[i] };
}

struct cfg {
    static constexpr TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(24, "state_l1"),
        ::zhc::tuya::dp::binary(25, "state_l2"),
        ::zhc::tuya::dp::binary(26, "state_l3"),
        scene(0, "scene_switch_1_1"), scene(1, "scene_switch_1_2"), scene(2, "scene_switch_1_3"),
        scene(3, "scene_switch_1_4"), scene(4, "scene_switch_2_1"), scene(5, "scene_switch_2_2"),
        scene(6, "scene_switch_2_3"), scene(7, "scene_switch_2_4"), scene(8, "scene_switch_3_1"),
        scene(9, "scene_switch_3_2"), scene(10, "scene_switch_3_3"), scene(11, "scene_switch_3_4"),
        act(5, "scene_1"), act(6, "scene_2"), act(7, "scene_3"),
        act(8, "scene_4"), act(9, "scene_5"), act(10, "scene_6"),
        act(104, "smart_curtain_open"), act(105, "smart_curtain_stop"), act(106, "smart_curtain_close"),
        act(107, "roller_shutter_down"), act(108, "roller_shutter_stop"), act(109, "roller_shutter_up"),
        act(110, "blinds_open"), act(111, "blinds_stop"), act(112, "blinds_close"),
        act(115, "screen_down"), act(116, "screen_stop"), act(117, "screen_up"),
        act(118, "gauze_curtain_open"), act(119, "gauze_curtain_stop"), act(120, "gauze_curtain_close"),
        act(121, "smart_window_open"), act(122, "smart_window_stop"), act(123, "smart_window_close"),
        act(124, "air_conditioner_on"), act(125, "air_conditioner_off"),
        act(126, "air_conditioner_cooling"), act(127, "air_conditioner_heating"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kActions[] = {
    "scene_1", "scene_2", "scene_3", "scene_4", "scene_5", "scene_6",
    "smart_curtain_open", "smart_curtain_stop", "smart_curtain_close",
    "roller_shutter_down", "roller_shutter_stop", "roller_shutter_up",
    "blinds_open", "blinds_stop", "blinds_close",
    "screen_down", "screen_stop", "screen_up",
    "gauze_curtain_open", "gauze_curtain_stop", "gauze_curtain_close",
    "smart_window_open", "smart_window_stop", "smart_window_close",
    "air_conditioner_on", "air_conditioner_off", "air_conditioner_cooling", "air_conditioner_heating",
    "scene_switch_1_1_on", "scene_switch_1_1_off", "scene_switch_1_2_on", "scene_switch_1_2_off",
    "scene_switch_1_3_on", "scene_switch_1_3_off", "scene_switch_1_4_on", "scene_switch_1_4_off",
    "scene_switch_2_1_on", "scene_switch_2_1_off", "scene_switch_2_2_on", "scene_switch_2_2_off",
    "scene_switch_2_3_on", "scene_switch_2_3_off", "scene_switch_2_4_on", "scene_switch_2_4_off",
    "scene_switch_3_1_on", "scene_switch_3_1_off", "scene_switch_3_2_on", "scene_switch_3_2_off",
    "scene_switch_3_3_on", "scene_switch_3_3_off", "scene_switch_3_4_on", "scene_switch_3_4_off",
};

#define ZHC_T3E_SCENE(n) {n, ExposeType::Binary, Access::StateSet, nullptr, "Panel scene switch", nullptr, 0}
constexpr Expose kExposes[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    ZHC_T3E_SCENE("scene_switch_1_1"), ZHC_T3E_SCENE("scene_switch_1_2"),
    ZHC_T3E_SCENE("scene_switch_1_3"), ZHC_T3E_SCENE("scene_switch_1_4"),
    ZHC_T3E_SCENE("scene_switch_2_1"), ZHC_T3E_SCENE("scene_switch_2_2"),
    ZHC_T3E_SCENE("scene_switch_2_3"), ZHC_T3E_SCENE("scene_switch_2_4"),
    ZHC_T3E_SCENE("scene_switch_3_1"), ZHC_T3E_SCENE("scene_switch_3_2"),
    ZHC_T3E_SCENE("scene_switch_3_3"), ZHC_T3E_SCENE("scene_switch_3_4"),
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions,
     static_cast<std::uint8_t>(sizeof(kActions) / sizeof(kActions[0]))},
};
#undef ZHC_T3E_SCENE

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE200_mua6ucdj" };

}  // namespace

extern const PreparedDefinition kDef_T3E_mua6ucdj{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="T3E_mua6ucdj", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
