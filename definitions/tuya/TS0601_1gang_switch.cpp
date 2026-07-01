// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 1-gang DP switch _TZE204_gm8h14wy (z2m v26.76.0 parity).
// z2m-source: tuya.ts #TS0601_1gang_switch
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kPob[]={{0,"off"},{1,"on"},{2,"previous"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"state"),                          // DP1 onOff
    ::zhc::tuya::dp::enum_lookup(14,"power_on_behavior",kPob,3), // DP14 powerOnBehaviorEnum
    ::zhc::tuya::dp::binary(16,"backlight_mode"),                // DP16 onOff
    ::zhc::tuya::dp::binary(101,"child_lock")};                  // DP101 onOff
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,4}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE204_gm8h14wy"};
constexpr const char* kPobOpts[]={"off","on","previous"};
constexpr Expose kExp[]={
    {"state",             ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr,   0},
    {"power_on_behavior", ExposeType::Enum,   Access::StateSet, nullptr, nullptr, kPobOpts,  3},
    {"backlight_mode",    ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr,   0},
    {"child_lock",        ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr,   0}};
}
extern const PreparedDefinition kDef_TS0601_1gang_switch{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TS0601_1gang_switch",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
