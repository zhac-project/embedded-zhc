// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 curtain/roller motor cover_13 _TZE204_tgl8i2np (z2m v26.76.0 parity).
// DP2 & DP3 both carry position (control + arrived echo); both decode, writes hit DP2.
// DP6 auto_power is z2m raw -> mapped Numeric (converter is not onOff; type unconfirmed).
// z2m-source: tuya.ts #TS0601_cover_13
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSt[]={{0,"open"},{1,"stop"},{2,"close"}};
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]={{0,"forward"},{1,"back"}};
constexpr ::zhc::tuya::TuyaEnumEntry kWork[]={{0,"opening"},{1,"closing"}};
constexpr ::zhc::tuya::TuyaEnumEntry kSit[]={{0,"fully_close"},{1,"fully_open"}};
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kSt,3),
    ::zhc::tuya::dp::numeric(2,"position",1),
    ::zhc::tuya::dp::numeric(3,"position",1),
    ::zhc::tuya::dp::enum_lookup(5,"motor_direction",kDir,2),
    ::zhc::tuya::dp::numeric(6,"auto_power",1),
    ::zhc::tuya::dp::enum_lookup(7,"work_state",kWork,2),
    ::zhc::tuya::dp::numeric(10,"total_time",1),
    ::zhc::tuya::dp::enum_lookup(11,"situation_set",kSit,2),
    ::zhc::tuya::dp::numeric(12,"fault",1)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,9}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE204_tgl8i2np"};
constexpr const char* kStOpts[]={"open","stop","close"};
constexpr const char* kDirOpts[]={"forward","back"};
constexpr const char* kWorkOpts[]={"opening","closing"};
constexpr const char* kSitOpts[]={"fully_close","fully_open"};
constexpr Expose kExp[]={
    {"state",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStOpts,   3},
    {"position",        ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,   0},
    {"motor_direction", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kDirOpts,  2},
    {"auto_power",      ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr,   0},
    {"work_state",      ExposeType::Enum,    Access::State,    nullptr, nullptr, kWorkOpts, 2},
    {"total_time",      ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr,   0},
    {"situation_set",   ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kSitOpts,  2},
    {"fault",           ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr,   0}};
}
extern const PreparedDefinition kDef_TS0601_cover_13{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TS0601_cover_13",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
