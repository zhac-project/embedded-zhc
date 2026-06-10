// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Somgoms ZSTY-SM-11ZG-US-W — 1 gang switch.
// z2m-source: somgoms.ts #ZSTY-SM-11ZG-US-W (legacy.fz.tuya_switch /
//   legacy.tz.tuya_switch_state).
//
// PARITY FIX: the auto-port wired generic kFzOnOff/kTzOnOff + bound genOnOff
// (0x0006). This device is a Tuya-MCU TS0601-class switch that speaks the
// 0xEF00 (manuSpecificTuya) DP stream — genOnOff is never used, so the port
// was DEAD (no state ever decoded, writes went nowhere). z2m tuya_switch
// decodes DP1 (state) bool -> {state: ON/OFF}; tuya_switch_state writes DP1.
// Re-wired onto the Tuya-DP read+write infra (DP1 state).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::somgoms {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"state")};   // DP1 on/off
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,1}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kModels_ZSTY_SM_11ZG_US_W[] = { "tdtqgwv" };

constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
}  // namespace

extern const PreparedDefinition kDef_ZSTY_SM_11ZG_US_W{
    .zigbee_models=kModels_ZSTY_SM_11ZG_US_W, .zigbee_models_count=sizeof(kModels_ZSTY_SM_11ZG_US_W)/sizeof(kModels_ZSTY_SM_11ZG_US_W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZSTY-SM-11ZG-US-W", .vendor="Somgoms",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
};

}  // namespace zhc::devices::somgoms
