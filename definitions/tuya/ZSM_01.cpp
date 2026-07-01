// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 curtain motor ZSM-01 _TZE284_zofmmt9s (z2m v26.76.0 parity).
// DP9 = set position, DP8 = position report; both decode, writes hit DP9 (listed first).
// z2m-source: tuya.ts #ZSM-01
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSt[]={{0,"open"},{1,"stop"},{2,"close"}};
constexpr ::zhc::tuya::TuyaEnumEntry kBack[]={{0,"forward"},{1,"back"}};
constexpr ::zhc::tuya::TuyaEnumEntry kClick[]={{0,"up"},{1,"down"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kSt,3),
    ::zhc::tuya::dp::numeric(9,"position",1),
    ::zhc::tuya::dp::numeric(8,"position",1),
    ::zhc::tuya::dp::enum_lookup(11,"control_back_mode",kBack,2),
    ::zhc::tuya::dp::numeric(19,"position_best",1),
    ::zhc::tuya::dp::enum_lookup(20,"click_control",kClick,2)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,6}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE284_zofmmt9s"};
constexpr const char* kStOpts[]={"open","stop","close"};
constexpr const char* kBackOpts[]={"forward","back"};
constexpr const char* kClickOpts[]={"up","down"};
constexpr Expose kExp[]={
    {"state",             ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStOpts,    3},
    {"position",          ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,    0},
    {"control_back_mode", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kBackOpts,  2},
    {"position_best",     ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr,    0},
    {"click_control",     ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kClickOpts, 2}};
}
extern const PreparedDefinition kDef_ZSM_01{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="ZSM-01",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
