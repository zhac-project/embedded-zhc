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
    "_TZE200_nkoabg8w","_TZE200_pk0sfzvr","_TZE204_xu4a5rhj","_TZE600_ogyg1y6b"};
constexpr const char* kStateOpts[]={"open","stop","close"};
constexpr Expose kExp[]={
    {"state",    ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStateOpts, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,    0}};
}
extern const PreparedDefinition kDefTS0601_cover{
    .zigbee_models=kM,.zigbee_models_count=2,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=8,.model="TS0601_cover",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=2,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
