// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 curtain motor RM28-LE _TZE200_fodv6bkr (z2m v26.76.0 parity).
// v26.35->v26.76 delta: reworked to full cover model (DP6/8/9/10/11/19 + mode/motor_direction/etc).
// DP2 & DP3 both carry position; border/motor_fault/click_control decode-only (not exposed, mirrors z2m).
// z2m-source: tuya.ts #RM28-LE
// z2m v26.95.0 renamed the motor_direction labels: the inline lookup
// {forward:0, back:1} became valueConverter.tubularMotorDirection,
// {normal:0, reversed:1}. Wire values are unchanged; only the published
// strings differ. Renamed here for z2m parity (product decision).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kState[]={{0,"open"},{1,"stop"},{2,"close"}};
constexpr ::zhc::tuya::TuyaEnumEntry kMode[]={{0,"morning"},{1,"night"}};
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]={{0,"normal"},{1,"reversed"}};
constexpr ::zhc::tuya::TuyaEnumEntry kWork[]={{0,"opening"},{1,"closing"}};
constexpr ::zhc::tuya::TuyaEnumEntry kCd[]={{0,"cancel"},{1,"1h"},{2,"2h"},{3,"3h"},{4,"4h"}};
constexpr ::zhc::tuya::TuyaEnumEntry kSit[]={{0,"fully_open"},{1,"fully_close"}};
constexpr ::zhc::tuya::TuyaEnumEntry kBorder[]={{0,"up"},{1,"down"},{2,"up_delete"},{3,"down_delete"},{4,"remove_top_bottom"}};
constexpr ::zhc::tuya::TuyaEnumEntry kClick[]={{0,"up"},{1,"down"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kState,3),
    ::zhc::tuya::dp::numeric(2,"position",1),
    ::zhc::tuya::dp::numeric(3,"position",1),
    ::zhc::tuya::dp::enum_lookup(4,"mode",kMode,2),
    ::zhc::tuya::dp::enum_lookup(5,"motor_direction",kDir,2),
    ::zhc::tuya::dp::binary(6,"auto_power"),
    ::zhc::tuya::dp::enum_lookup(7,"work_state",kWork,2),
    ::zhc::tuya::dp::enum_lookup(8,"countdown",kCd,5),
    ::zhc::tuya::dp::numeric(9,"countdown_left",1),
    ::zhc::tuya::dp::numeric(10,"time_total",1),
    ::zhc::tuya::dp::enum_lookup(11,"situation_set",kSit,2),
    ::zhc::tuya::dp::numeric(12,"motor_fault",1),
    ::zhc::tuya::dp::numeric(13,"battery",1),
    ::zhc::tuya::dp::enum_lookup(16,"border",kBorder,5),
    ::zhc::tuya::dp::numeric(19,"position_best",1),
    ::zhc::tuya::dp::enum_lookup(20,"click_control",kClick,2)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,16}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_fodv6bkr"};
constexpr const char* kStateOpts[]={"open","stop","close"};
constexpr const char* kModeOpts[]={"morning","night"};
constexpr const char* kDirOpts[]={"normal","reversed"};
constexpr const char* kWorkOpts[]={"opening","closing"};
constexpr const char* kCdOpts[]={"cancel","1h","2h","3h","4h"};
constexpr const char* kSitOpts[]={"fully_open","fully_close"};
constexpr Expose kExp[]={
    {"state",          ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStateOpts, 3},
    {"position",       ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,     0},
    {"mode",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kModeOpts,   2},
    {"motor_direction",ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kDirOpts,    2},
    {"auto_power",     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,     0},
    {"work_state",     ExposeType::Enum,    Access::State,    nullptr, nullptr, kWorkOpts,   2},
    {"countdown",      ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kCdOpts,     5},
    {"countdown_left", ExposeType::Numeric, Access::State,    "s",     nullptr, nullptr,     0},
    {"time_total",     ExposeType::Numeric, Access::State,    "ms",    nullptr, nullptr,     0},
    {"situation_set",  ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kSitOpts,    2},
    {"position_best",  ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr,     0},
    {"battery",        ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr,     0}};
}
extern const PreparedDefinition kDef_RM28_LE{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="RM28-LE",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
