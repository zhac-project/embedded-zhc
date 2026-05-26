// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya Matsee garage-door opener (_TZE200_wfxuhoea, _TZE204_wfxuhoea,
// legacy matsee_garage_door_opener): DP3 garage_door_contact (INVERTED:
// Boolean(!value) — closed when wire 0), DP1 trigger is a momentary action in
// z2m (write-only command, not a steady DP). READ-ONLY here — contact status
// only; trigger control deferred (momentary-action encoding + security review).
// Best-effort, not hardware-verified.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary_inv(3,"garage_door_contact")};  // DP3 !value
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,1}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_wfxuhoea","_TZE204_wfxuhoea"};
}
extern const PreparedDefinition kDefTS0601_garage_matsee{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=2,.model="TS0601_garage_door_opener_2",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
