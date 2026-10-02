// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_cover_12 curtain motor — the `_TZE204_tgl8i2np`
// (TS0601_cover_13) and `_TZE284_a0hirjnh` (TS0601_cover_14) variants.
//
// z2m v26.113.0 folded TS0601_cover_13 into TS0601_cover_12 as a white label
// and added `_TZE284_a0hirjnh` as TS0601_cover_14 (#13295). Upstream branches
// the exposes on the manufacturer name — battery only on `_TZE200_mlglxwp3`
// (TZE200_mlglxwp3.cpp), DP6 auto_power only on tgl8i2np, DP19
// favorite_position only on a0hirjnh — so each is its own definition here.
// Parity fixes on the way: DP7 is z2m's `motor_state` (valueConverter.motorState
// {opening, closing, stopped}), it was `work_state` without `stopped`; DP1 is
// coverAction {OPEN, STOP, CLOSE, CONTINUE}, it was lower-case; DP6
// auto_power is a boolean (z2m e.binary over valueConverter.raw).
// DP2 and DP3 both carry position; both decode, writes hit DP2.
// z2m-source: tuya.ts #TS0601_cover_12.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSt[]={{0,"OPEN"},{1,"STOP"},{2,"CLOSE"},{3,"CONTINUE"}};
constexpr ::zhc::tuya::TuyaEnumEntry kDir[]={{0,"normal"},{1,"reversed"}};
constexpr ::zhc::tuya::TuyaEnumEntry kMotor[]={{0,"opening"},{1,"closing"},{2,"stopped"}};
constexpr ::zhc::tuya::TuyaEnumEntry kSit[]={{0,"fully_close"},{1,"fully_open"}};
struct cfg13 { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kSt,4),
    ::zhc::tuya::dp::numeric(2,"position",1),
    ::zhc::tuya::dp::numeric(3,"position",1),
    ::zhc::tuya::dp::enum_lookup(5,"motor_direction",kDir,2),
    ::zhc::tuya::dp::binary(6,"auto_power"),
    ::zhc::tuya::dp::enum_lookup(7,"motor_state",kMotor,3),
    ::zhc::tuya::dp::numeric(10,"total_time",1),
    ::zhc::tuya::dp::enum_lookup(11,"situation_set",kSit,2),
    ::zhc::tuya::dp::numeric(12,"fault",1)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,sizeof(e)/sizeof(e[0])}; };
struct cfg14 { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kSt,4),
    ::zhc::tuya::dp::numeric(2,"position",1),
    ::zhc::tuya::dp::numeric(3,"position",1),
    ::zhc::tuya::dp::enum_lookup(5,"motor_direction",kDir,2),
    ::zhc::tuya::dp::enum_lookup(7,"motor_state",kMotor,3),
    ::zhc::tuya::dp::numeric(10,"total_time",1),
    ::zhc::tuya::dp::enum_lookup(11,"situation_set",kSit,2),
    ::zhc::tuya::dp::numeric(12,"fault",1),
    ::zhc::tuya::dp::numeric(19,"favorite_position",1)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,sizeof(e)/sizeof(e[0])}; };
using FX13=::zhc::tuya::factory::TuyaRw<cfg13>;
using FX14=::zhc::tuya::factory::TuyaRw<cfg14>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN13[]={"_TZE204_tgl8i2np"};
constexpr const char* kN14[]={"_TZE284_a0hirjnh"};
constexpr const char* kStOpts[]={"OPEN","CLOSE","STOP"};
constexpr const char* kDirOpts[]={"normal","reversed"};
constexpr const char* kMotorOpts[]={"opening","closing","stopped"};
constexpr const char* kSitOpts[]={"fully_close","fully_open"};
#define ZHC_COVER12_EXPOSES                                                                         \
    {"state",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStOpts,    3},     \
    {"position",        ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,    0},     \
    {"motor_direction", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kDirOpts,   2},     \
    {"motor_state",     ExposeType::Enum,    Access::State,    nullptr, nullptr, kMotorOpts, 3},     \
    {"total_time",      ExposeType::Numeric, Access::State,    "ms",    "Total running time in milliseconds", nullptr, 0}, \
    {"situation_set",   ExposeType::Enum,    Access::StateSet, nullptr, "Set fully open or fully close position", kSitOpts, 2}, \
    {"fault",           ExposeType::Numeric, Access::State,    nullptr, "Fault details", nullptr, 0}
constexpr Expose kExp13[]={
    ZHC_COVER12_EXPOSES,
    {"auto_power",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0}};
constexpr Expose kExp14[]={
    ZHC_COVER12_EXPOSES,
    {"favorite_position", ExposeType::Numeric, Access::StateSet, "%", "Favorite position of this cover", nullptr, 0,
     ExposeCategory::State, 0, 100, 0}};
#undef ZHC_COVER12_EXPOSES
}
extern const PreparedDefinition kDef_TS0601_cover_13{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN13,.manufacturer_names_count=sizeof(kN13)/sizeof(kN13[0]),.model="TS0601_cover_13",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp13,.exposes_count=sizeof(kExp13)/sizeof(kExp13[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX13::fz_list,.from_zigbee_count=FX13::fz_count,
    .to_zigbee=FX13::tz_list,.to_zigbee_count=FX13::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
extern const PreparedDefinition kDef_TS0601_cover_14{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN14,.manufacturer_names_count=sizeof(kN14)/sizeof(kN14[0]),.model="TS0601_cover_14",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp14,.exposes_count=sizeof(kExp14)/sizeof(kExp14[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX14::fz_list,.from_zigbee_count=FX14::fz_count,
    .to_zigbee=FX14::tz_list,.to_zigbee_count=FX14::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
