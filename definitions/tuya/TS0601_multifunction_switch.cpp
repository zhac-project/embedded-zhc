// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 multifunction 1-gang touch panel switch _TZE284_7e6v8u9f (z2m v26.76.0 parity).
// OMITTED: DP19 "inching" (z2m inchingSwitch2 — packed converter, no dp:: factory),
// DP103 "on_color" (backlight colour enum, z2m colour set incomplete in source view).
// z2m-source: tuya.ts #TS0601_multifunction_switch
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPob[]={{0,"off"},{1,"on"},{2,"previous"}};
constexpr ::zhc::tuya::TuyaEnumEntry kInd[]={{0,"none"},{1,"relay"},{2,"pos"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"state"),                          // DP1 onOff
    ::zhc::tuya::dp::numeric(7,"countdown",1),                   // DP7 countdown (s)
    ::zhc::tuya::dp::enum_lookup(14,"power_on_behavior",kPob,3), // DP14 powerOnBehaviorEnum
    ::zhc::tuya::dp::enum_lookup(15,"indicator_mode",kInd,3),    // DP15 none/relay/pos
    ::zhc::tuya::dp::binary(16,"backlight_mode"),                // DP16 onOff
    ::zhc::tuya::dp::binary(101,"child_lock"),                   // DP101 onOff
    ::zhc::tuya::dp::numeric(102,"backlight_brightness",1)};     // DP102 raw
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,7}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE284_7e6v8u9f"};
constexpr const char* kPobOpts[]={"off","on","previous"};
constexpr const char* kIndOpts[]={"none","relay","pos"};
constexpr Expose kExp[]={
    {"state",                ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,  0},
    {"countdown",            ExposeType::Numeric, Access::StateSet, "s",     nullptr, nullptr,  0},
    {"power_on_behavior",    ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kPobOpts, 3},
    {"indicator_mode",       ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kIndOpts, 3},
    {"backlight_mode",       ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,  0},
    {"child_lock",           ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,  0},
    {"backlight_brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr,  0}};
}
extern const PreparedDefinition kDef_TS0601_multifunction_switch{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TS0601_multifunction_switch",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
