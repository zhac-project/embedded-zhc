// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601/TS0105 curtain / roller-blind motor (legacy tuya_cover),
// NON-inverted-position variants:
//   DP1 state {open,stop,close} — open/stop/close control (+ best-effort read;
//        z2m treats DP1 state-read as less reliable than position),
//   DP2 position 0-100 — read + set.
// READ + WRITE via factory::TuyaRw. Inverted-position variants live in
// TS0601_cover_inv.cpp. DP3 (coverArrived) position echo not mapped (DP2 is the
// primary). On-device control UNVERIFIED (no hardware).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kCovSt[]={ {0,"open"},{1,"stop"},{2,"close"} };
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kCovSt,3),  // DP1 open/stop/close
    ::zhc::tuya::dp::numeric(2,"position",1)};         // DP2 0-100
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,2}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601","TS0105"};
constexpr const char* kN[]={
    "_TZE200_4vobcgd3","_TZE284_4vobcgd3","_TZE200_5zbp6j0u","_TZE200_eegnwoyw",
    "_TZE200_nkoabg8w","_TZE204_xu4a5rhj","_TZE600_ogyg1y6b",
    // z2m v26.111.0 TS0601_cover_2 (#13208).
    "_TZE200_fu14oapz",
    // Zemismart ZMS1-TYZ, upstream TS0601_cover_1; v26.111.0 added the _TZE284_
    // spelling (#13222). The generated per-manufacturer copy for _TZE204_ was a
    // genOnOff placeholder that decoded nothing for this motor — retired.
    "_TZE204_zuq5xxib","_TZE284_zuq5xxib",
    // 1fuxihti: upstream TS0601_cover_1 (z2m v26.103.0 added the _TZE28C1000000_
    // batch). Until this window three generated per-manufacturer stubs claimed
    // these names and decoded the motor as a genOnOff switch — retired.
    "_TZE200_1fuxihti","_TZE204_1fuxihti","_TZE284_1fuxihti","_TZE28C1000000_1fuxihti"};
constexpr const char* kStateOpts[]={"open","stop","close"};
constexpr Expose kExp[]={
    {"state",    ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStateOpts, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,    0}};
}
extern const PreparedDefinition kDefTS0601_cover{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),.model="TS0601_cover",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=2,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
// RINNconnect WSER40 roller controller (`_TZE200_pk0sfzvr`). z2m v26.113.0
// (#13302, "fix not controllable") moved it off TS0601_cover_1's legacy
// tuya_cover onto its own datapoints: DP1 {OPEN: 0, CLOSE: 1, STOP: 2} and the
// position on DP102 (set) / DP103 (report). It was listed above with the
// legacy DP1/DP2 layout, which this firmware does not use.
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kWser40St[]={ {0,"OPEN"},{1,"CLOSE"},{2,"STOP"} };
struct cfg_wser40 { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kWser40St,3),
    ::zhc::tuya::dp::numeric(102,"position",1),
    ::zhc::tuya::dp::numeric(103,"position",1)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,sizeof(e)/sizeof(e[0])}; };
using FXWser40=::zhc::tuya::factory::TuyaRw<cfg_wser40>;
constexpr const char* kWser40N[]={"_TZE200_pk0sfzvr"};
constexpr const char* kWser40StOpts[]={"OPEN","CLOSE","STOP"};
constexpr Expose kWser40Exp[]={
    {"state",    ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kWser40StOpts, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,       0}};
constexpr const char* kWser40M[]={"TS0601"};
}
extern const PreparedDefinition kDef_WSER40{
    .zigbee_models=kWser40M,.zigbee_models_count=sizeof(kWser40M)/sizeof(kWser40M[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kWser40N,.manufacturer_names_count=sizeof(kWser40N)/sizeof(kWser40N[0]),.model="WSER40",
    .vendor="RINNconnect",.meta=nullptr,.exposes=kWser40Exp,.exposes_count=sizeof(kWser40Exp)/sizeof(kWser40Exp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FXWser40::fz_list,.from_zigbee_count=FXWser40::fz_count,
    .to_zigbee=FXWser40::tz_list,.to_zigbee_count=FXWser40::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
// `_TZE204_wzre8hu2` (upstream TS0601_cover_1): the legacy DP1 / DP2 cover plus,
// from z2m v26.115.0, slat tilt on DP21 — an angle 0..180 published as `tilt`
// (0..100 %) and `flip_angle` (degrees); both write DP21. Its generated
// per-manufacturer copy decoded the motor as a battery + genOnOff device.
namespace {
struct cfg_tilt { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kCovSt,3),
    ::zhc::tuya::dp::numeric(2,"position",1),
    { 21, "tilt", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, 1.8f },
    ::zhc::tuya::dp::numeric(21,"flip_angle",1)};
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,sizeof(e)/sizeof(e[0])}; };
using FXTilt=::zhc::tuya::factory::TuyaRw<cfg_tilt>;
constexpr const char* kTiltN[]={"_TZE204_wzre8hu2"};
constexpr Expose kTiltExp[]={
    {"state",      ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStateOpts, 3},
    {"position",   ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,    0},
    {"tilt",       ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,    0},
    {"flip_angle", ExposeType::Numeric, Access::StateSet, "\xC2\xB0", "Slat angle in degrees, same as tilt on a 0-180 scale",
     nullptr, 0, ExposeCategory::State, 0, 180, 1}};
}
extern const PreparedDefinition kDef_TS0601_cover_1_tilt{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kTiltN,.manufacturer_names_count=sizeof(kTiltN)/sizeof(kTiltN[0]),.model="TS0601_cover_1",
    .vendor="Tuya",.meta=nullptr,.exposes=kTiltExp,.exposes_count=sizeof(kTiltExp)/sizeof(kTiltExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FXTilt::fz_list,.from_zigbee_count=FXTilt::fz_count,
    .to_zigbee=FXTilt::tz_list,.to_zigbee_count=FXTilt::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
