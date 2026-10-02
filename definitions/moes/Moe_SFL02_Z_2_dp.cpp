// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Moes SFL02-Z-2 Star feather 2-gang switch, graduated from the generated
// stub (which decoded this Tuya datapoint device with genOnOff converters).
// z2m v26.115.1 adds `_TZE284_uenof8jd`. Datapoints as upstream: 24/25
// state_l1/l2, 30/31 countdown, 105/106 momentary timers, 18/19 switch or
// scene mode, 1/2 scene actions (static), 36 backlight, 37 indicator,
// 38 power-on behaviour, 103 induction and 104 vibration mode.
// z2m-source: moes.ts #SFL02-Z-2.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::moes {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kModeL118[] = { {0,"switch_1"}, {1,"scene_1"} };
constexpr ::zhc::tuya::TuyaEnumEntry kModeL219[] = { {0,"switch_2"}, {1,"scene_2"} };
constexpr ::zhc::tuya::TuyaEnumEntry kIndicatorStatus37[] = { {0,"off"}, {1,"relay"}, {2,"invert"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehavior38[] = { {0,"off"}, {1,"on"}, {2,"previous"} };
constexpr ::zhc::tuya::TuyaEnumEntry kInductionMode103[] = { {0,"OFF"}, {1,"ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kVibrationMode104[] = { {0,"Gear 0"}, {1,"Gear 1"}, {2,"Gear 2"}, {3,"Gear 3"} };
// z2m tuya.valueConverter.static: the datapoint only signals; publish the
// label carried in expand_cfg whatever its value.
bool static_label(const ::zhc::tuya::TuyaDpMapEntry& e, const Value&, RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value v{}; v.type = ValueType::StringRef; v.str = static_cast<const char*>(e.expand_cfg);
    out.put(e.out_key, v);
    return true;
}
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        { 1, "action", ::zhc::TuyaDpType::Enum, 1, nullptr, 0, 0, 0.0f, &static_label, "scene_1" },
        { 2, "action", ::zhc::TuyaDpType::Enum, 1, nullptr, 0, 0, 0.0f, &static_label, "scene_2" },
        ::zhc::tuya::dp::enum_lookup(18, "mode_l1", kModeL118, 2),
        ::zhc::tuya::dp::enum_lookup(19, "mode_l2", kModeL219, 2),
        ::zhc::tuya::dp::binary(24, "state_l1"),
        ::zhc::tuya::dp::binary(25, "state_l2"),
        ::zhc::tuya::dp::numeric(30, "countdown_l1", 1),
        ::zhc::tuya::dp::numeric(31, "countdown_l2", 1),
        ::zhc::tuya::dp::binary(36, "backlight_mode"),
        ::zhc::tuya::dp::enum_lookup(37, "indicator_status", kIndicatorStatus37, 3),
        ::zhc::tuya::dp::enum_lookup(38, "power_on_behavior", kPowerOnBehavior38, 3),
        { 103, "induction_mode", ::zhc::TuyaDpType::Bool, 1, kInductionMode103, 2, ::zhc::tuya::kTuyaDpFlagBoolEnum },
        ::zhc::tuya::dp::enum_lookup(104, "vibration_mode", kVibrationMode104, 4),
        ::zhc::tuya::dp::numeric(105, "momentary_1", 1),
        ::zhc::tuya::dp::numeric(106, "momentary_2", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts7[] = { "off", "on", "previous" };
constexpr const char* kOpts8[] = { "switch_1", "scene_1" };
constexpr const char* kOpts9[] = { "switch_2", "scene_2" };
constexpr const char* kOpts10[] = { "scene_1", "scene_2" };
constexpr const char* kOpts11[] = { "off", "relay", "invert" };
constexpr const char* kOpts12[] = { "ON", "OFF" };
constexpr const char* kOpts13[] = { "Gear 0", "Gear 1", "Gear 2", "Gear 3" };
constexpr Expose kExp[] = {
    {"backlight_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"countdown_l1", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0},
    {"countdown_l2", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0},
    {"momentary_1", ExposeType::Numeric, Access::StateSet, "s", "Momentary switch timer (0=disable)", nullptr, 0, ExposeCategory::State, 0, 3600, 1},
    {"momentary_2", ExposeType::Numeric, Access::StateSet, "s", "Momentary switch timer (0=disable)", nullptr, 0, ExposeCategory::State, 0, 3600, 1},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts7, 3},
    {"mode_l1", ExposeType::Enum, Access::StateSet, nullptr, "Switch1 mode", kOpts8, 2},
    {"mode_l2", ExposeType::Enum, Access::StateSet, nullptr, "Switch2 mode", kOpts9, 2},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kOpts10, 2},
    {"indicator_status", ExposeType::Enum, Access::StateSet, nullptr, "Indicator status", kOpts11, 3},
    {"induction_mode", ExposeType::Enum, Access::StateSet, nullptr, "Induction mode", kOpts12, 2},
    {"vibration_mode", ExposeType::Enum, Access::StateSet, nullptr, "Vibration", kOpts13, 4},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE200_uenof8jd", "_TZE284_uenof8jd", "_TZE200_tzyy0rtq", "_TZE200_hktk6hze" };
constexpr WhiteLabel kWL[] = { {"Nova Digital", "TPZ-2"} };
}  // namespace

extern const PreparedDefinition kDef_SFL02_Z_2_dp{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="SFL02-Z-2",.vendor="Moes",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL,.white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr,
    .tuya_time_start=1 };

}  // namespace zhc::devices::moes
