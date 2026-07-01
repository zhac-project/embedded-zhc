// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HOBEIAN ZG-301Z-MOTO curtain motor controller (z2m v26.76.0 parity).
// Matched by zigbeeModel "ZG-301Z-MOTO" (no manufacturerName fingerprint in z2m).
// z2m-source: tuya.ts #ZG-301Z-MOTO
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSt[]={{0,"open"},{1,"stop"},{2,"close"}};
constexpr ::zhc::tuya::TuyaEnumEntry kCal[]={{0,"start"},{1,"end"}};
constexpr ::zhc::tuya::TuyaEnumEntry kBack[]={{0,"forward"},{1,"back"}};
constexpr ::zhc::tuya::TuyaEnumEntry kInd[]={{0,"relay"},{1,"pos"},{2,"none"}};
constexpr ::zhc::tuya::TuyaEnumEntry kSw[]={{0,"flip_switch"},{1,"sync_switch"},{2,"button_switch"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kSt,3),
    ::zhc::tuya::dp::numeric(2,"position",1),
    ::zhc::tuya::dp::enum_lookup(3,"cur_calibration",kCal,2),
    ::zhc::tuya::dp::enum_lookup(8,"control_back",kBack,2),
    ::zhc::tuya::dp::numeric(10,"tr_timecon",1),
    ::zhc::tuya::dp::enum_lookup(14,"indicator_mode",kInd,3),
    ::zhc::tuya::dp::enum_lookup(101,"switch_type",kSw,3)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,7}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"ZG-301Z-MOTO"};
constexpr const char* kStOpts[]={"open","stop","close"};
constexpr const char* kCalOpts[]={"start","end"};
constexpr const char* kBackOpts[]={"forward","back"};
constexpr const char* kIndOpts[]={"relay","pos","none"};
constexpr const char* kSwOpts[]={"flip_switch","sync_switch","button_switch"};
constexpr Expose kExp[]={
    {"state",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStOpts,   3},
    {"position",        ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,   0},
    {"cur_calibration", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kCalOpts,  2},
    {"control_back",    ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kBackOpts, 2},
    {"tr_timecon",      ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr,   0},
    {"indicator_mode",  ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kIndOpts,  3},
    {"switch_type",     ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kSwOpts,   3}};
}
extern const PreparedDefinition kDef_ZG_301Z_MOTO{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr,.manufacturer_names_count=0,.model="ZG-301Z-MOTO",
    .vendor="HOBEIAN",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
