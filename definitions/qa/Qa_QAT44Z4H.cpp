// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Qa QAT44Z4H — hand-rewritten (was wrong-bundle: wired generic
// genOnOff (0x0006) + a single bare `state` expose for a 4-channel wall
// switch that never speaks plain genOnOff).
//
// This is a TS0601 / _TZE204_kyzjsjo3 Tuya-MCU device. z2m decodes it with
// `legacy.fz.tuya_switch` / `legacy.tz.tuya_switch_state` on cluster
// manuSpecificTuya (0xEF00): multiEndpoint maps datapoints 1..4 to gangs
// l1..l4 (bool ON/OFF) — every gang multiplexed through the single physical
// endpoint 1 (z2m `endpoint: {l1:1, l2:1, l3:1, l4:1}`). The previous bundle
// lowered kFzOnOff/kTzOnOff on 0x0006, so a paired switch surfaced one bare
// `state` and decoded/controlled none of its four gangs.
//
// Rewritten to the Tuya-DP infra (fz_tuya_datapoints / tz_tuya_datapoints)
// exactly like the sibling DP ports (Qa__TZE284_ms97nkyy etc.); the DP map
// emits per-gang state_l1..l4 keys directly (no endpoint_map needed, all on
// EP1). Registry references kDef_QAT44Z4H by symbol — name preserved.
// z2m-source: qa.ts #QAT44Z4H; legacy.ts fz.tuya_switch / tz.tuya_switch_state.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::qa {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_QAT44Z4H[] = {
    { 1, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_l3", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_l4", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_QAT44Z4H{ kEntries_QAT44Z4H, 4 };
constexpr FzConverter kFzDp_QAT44Z4H{
    .family            = FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
    .user_config       = &kMap_QAT44Z4H,
};
const FzConverter* const kFz_QAT44Z4H[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_QAT44Z4H,
};
constexpr TzConverter kTzDp_QAT44Z4H{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_QAT44Z4H,
};
const TzConverter* const kTz_QAT44Z4H[] = { &kTzDp_QAT44Z4H };
constexpr const char* kModels_QAT44Z4H[] = { "TS0601" };
constexpr const char* kManus_QAT44Z4H[] = { "_TZE204_kyzjsjo3" };

constexpr Expose kExp_QAT44Z4H[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
constexpr BindingSpec kBind_QAT44Z4H[] = {
    {1, 0xEF00},
};
}  // namespace

extern const PreparedDefinition kDef_QAT44Z4H{
    .zigbee_models=kModels_QAT44Z4H, .zigbee_models_count=sizeof(kModels_QAT44Z4H)/sizeof(kModels_QAT44Z4H[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_QAT44Z4H, .manufacturer_names_count=sizeof(kManus_QAT44Z4H)/sizeof(kManus_QAT44Z4H[0]),
    .model="QAT44Z4H", .vendor="Qa",
    .meta=nullptr, .exposes=kExp_QAT44Z4H, .exposes_count=sizeof(kExp_QAT44Z4H)/sizeof(kExp_QAT44Z4H[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_QAT44Z4H, .from_zigbee_count=sizeof(kFz_QAT44Z4H)/sizeof(kFz_QAT44Z4H[0]),
    .to_zigbee=kTz_QAT44Z4H, .to_zigbee_count=sizeof(kTz_QAT44Z4H)/sizeof(kTz_QAT44Z4H[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBind_QAT44Z4H, .bindings_count=sizeof(kBind_QAT44Z4H)/sizeof(kBind_QAT44Z4H[0]),
};

}  // namespace zhc::devices::qa
