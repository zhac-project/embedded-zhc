// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bseed BSEED_TS0601_cover — Tuya-DP curtain switch (hand-fixed).
//
// Bug fixed (Tuya-DP MISROUTE): the auto-generated Tier-1 file wired this
// TS0601 _TZE200/_TZE204 device to the generic ZCL windowCovering cluster
// (kFzCoverPosition / kTzCoverPosition on 0x0102) with a 0x0102 binding.
// A TS0601 Tuya curtain switch never speaks ZCL 0x0102 — it reports and is
// driven entirely over the manuSpecificTuya (0xEF00) DP stream, so the
// generic cover converter matched nothing and `position` was a dead key.
//
// z2m decodes this via legacy.fz.tuya_cover / legacy.tz.tuya_cover_control
// (cluster manuSpecificTuya). DP map (lib/legacy.ts dataPoints):
//   dp 2  coverPosition  → position (running, started moving)
//   dp 3  coverArrived   → position (arrived at target)
//   dp 105 coverSpeed    → motor_speed
//   dp 1  state / dp 5 config / dp 7 coverChange → ignored by z2m.
// None of the three bseed manufacturerNames are in z2m's coverPositionInvert
// list, so no position inversion (kTuyaDpFlagInvertPosition NOT set);
// z2m masks `value & 0xff`, matching the Numeric DP pass-through here.
//
// z2m-source: bseed.ts #BSEED_TS0601_cover.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::bseed {
namespace {

// DP map: position (dp 2 + dp 3 both target the `position` key), motor speed.
constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_BSEED_TS0601_cover[] = {
    {   2, "position",    ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    {   3, "position",    ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "motor_speed", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_BSEED_TS0601_cover{
    kEntries_BSEED_TS0601_cover,
    sizeof(kEntries_BSEED_TS0601_cover) / sizeof(kEntries_BSEED_TS0601_cover[0]),
};

constexpr FzConverter kFzDp_BSEED_TS0601_cover{
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
    .user_config       = &kMap_BSEED_TS0601_cover,
};
const FzConverter* const kFz_BSEED_TS0601_cover[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_BSEED_TS0601_cover,
};

constexpr TzConverter kTzDp_BSEED_TS0601_cover{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_BSEED_TS0601_cover,
};
const TzConverter* const kTz_BSEED_TS0601_cover[] = {
    &kTzDp_BSEED_TS0601_cover,
};

constexpr const char* kModels_BSEED_TS0601_cover[] = { "TS0601" };
constexpr const char* kManus_BSEED_TS0601_cover[] = { "_TZE200_yenbr4om", "_TZE204_bdblidq3", "_TZE200_bdblidq3" };
}  // namespace


constexpr Expose kAutoExposes[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"motor_speed", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};

extern const PreparedDefinition kDef_BSEED_TS0601_cover{
    .zigbee_models=kModels_BSEED_TS0601_cover, .zigbee_models_count=sizeof(kModels_BSEED_TS0601_cover)/sizeof(kModels_BSEED_TS0601_cover[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_BSEED_TS0601_cover, .manufacturer_names_count=sizeof(kManus_BSEED_TS0601_cover)/sizeof(kManus_BSEED_TS0601_cover[0]),
    .model="BSEED_TS0601_cover", .vendor="Bseed",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_BSEED_TS0601_cover, .from_zigbee_count=sizeof(kFz_BSEED_TS0601_cover)/sizeof(kFz_BSEED_TS0601_cover[0]),
    .to_zigbee=kTz_BSEED_TS0601_cover, .to_zigbee_count=sizeof(kTz_BSEED_TS0601_cover)/sizeof(kTz_BSEED_TS0601_cover[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bseed
