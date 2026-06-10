// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Somgoms ZSTY-SM-1CTZG-US-W — curtain switch.
// z2m-source: somgoms.ts #ZSTY-SM-1CTZG-US-W (legacy.fz.tuya_cover /
//   legacy.tz.tuya_cover_control + legacy.tz.tuya_cover_options).
//
// PARITY FIX: the auto-port wired generic kFzCoverPosition/kTzCoverPosition +
// bound windowCovering (0x0102). This is a Tuya-MCU curtain motor that speaks
// the 0xEF00 (manuSpecificTuya) DP stream — windowCovering is never used, so
// the port was DEAD (no position ever decoded). z2m tuya_cover decodes DP2
// (coverPosition) and DP3 (coverArrived) -> position 0-100; tuya_cover_control
// writes DP2. Re-wired onto the Tuya-DP read+write infra (DP2 + DP3 position).
// z2m does NOT invert this manufacturer (not in isCoverInverted list), so the
// position is non-inverted. DP1 state is intentionally ignored by z2m ("not
// reliable between covers"); DP105 coverSpeed is a tz-only config option, not a
// core read — deferred.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::somgoms {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::numeric(2,"position",1),    // DP2 coverPosition 0-100
    ::zhc::tuya::dp::numeric(3,"position",1)};   // DP3 coverArrived (position echo)
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,2}; };
using FX=::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kModels_ZSTY_SM_1CTZG_US_W[] = { "bordckq" };

constexpr Expose kAutoExposes[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
};
}  // namespace

extern const PreparedDefinition kDef_ZSTY_SM_1CTZG_US_W{
    .zigbee_models=kModels_ZSTY_SM_1CTZG_US_W, .zigbee_models_count=sizeof(kModels_ZSTY_SM_1CTZG_US_W)/sizeof(kModels_ZSTY_SM_1CTZG_US_W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZSTY-SM-1CTZG-US-W", .vendor="Somgoms",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
};

}  // namespace zhc::devices::somgoms
