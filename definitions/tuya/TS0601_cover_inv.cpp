// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601 curtain motors whose position is INVERTED in z2m
// (manufacturerName in coverPositionInvert): _TZE200_nogaemzt, _TZE200_xuzcvlku,
// _TZE200_r0jdjrvi.
//   DP1 state {open,stop,close} — open/stop/close control,
//   DP2 position 0-100 — inverted via dp::position_inv (decode 100-raw,
//        encode 100-value), so read + set both align with z2m.
// READ + WRITE via factory::TuyaRw. On-device control UNVERIFIED (no hardware).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kCovStInv[]={ {0,"open"},{1,"stop"},{2,"close"} };
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::enum_lookup(1,"state",kCovStInv,3),  // DP1 open/stop/close
    ::zhc::tuya::dp::position_inv(2,"position")};         // DP2 0-100 inverted
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,2}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_nogaemzt","_TZE200_xuzcvlku","_TZE200_r0jdjrvi"};
constexpr const char* kStateOpts[]={"open","stop","close"};
constexpr Expose kExp[]={
    {"state",    ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStateOpts, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,    0}};
}
extern const PreparedDefinition kDefTS0601_cover_inv{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=3,.model="TS0601_cover",
    .vendor="Tuya",.meta=nullptr,.exposes=kExp,.exposes_count=2,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
