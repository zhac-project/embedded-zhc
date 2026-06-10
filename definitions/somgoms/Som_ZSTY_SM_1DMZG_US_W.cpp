// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Somgoms ZSTY-SM-1DMZG-US-W — dimmer switch.
// z2m-source: somgoms.ts #ZSTY-SM-1DMZG-US-W (legacy.fz.tuya_dimmer /
//   legacy.tz.tuya_dimmer_state + legacy.tz.tuya_dimmer_level).
//
// PARITY FIX: the auto-port wired generic kFzOnOff/kFzBrightness + bound
// genOnOff (0x0006) / genLevelCtrl (0x0008). This is a Tuya-MCU TS0601-class
// dimmer that speaks the 0xEF00 (manuSpecificTuya) DP stream — the standard
// clusters are never used, so the port was DEAD. z2m tuya_dimmer decodes the
// default branch: DP1 (state) bool -> {state}, DP3 (dimmerLevel) -> brightness.
// Re-wired onto the Tuya-DP read+write infra.
//
// SCALE NOTE: z2m maps the DP3 raw 10..1000 onto brightness 0..254 via
// mapNumberRange (an affine remap, NOT a divisor). The Tuya-DP map only
// supports divisor/divisor_f scaling, so — matching every other ported Tuya
// dimmer in this tree (e.g. TZE200_dimmer) — brightness is passed through with
// divisor=1 (raw 0..1000). The affine 0..254 remap is a documented INFRA gap.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::somgoms {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary(1,"state"),          // DP1 on/off
    ::zhc::tuya::dp::numeric(3,"brightness",1)};  // DP3 dimmerLevel (raw 0..1000)
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,2}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kModels_ZSTY_SM_1DMZG_US_W[] = { "hpb9yts" };

constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
}  // namespace

extern const PreparedDefinition kDef_ZSTY_SM_1DMZG_US_W{
    .zigbee_models=kModels_ZSTY_SM_1DMZG_US_W, .zigbee_models_count=sizeof(kModels_ZSTY_SM_1DMZG_US_W)/sizeof(kModels_ZSTY_SM_1DMZG_US_W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZSTY-SM-1DMZG-US-W", .vendor="Somgoms",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
};

}  // namespace zhc::devices::somgoms
