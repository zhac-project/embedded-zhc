// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Zemismart ZM25TQ tubular motor (TS0601 / _TZE200_fzo2pocs).
//
// Graduated from the generated generic-cover stub. ZM25TQ is a pure Tuya
// datapoint device (z2m `tuya.modernExtend.tuyaBase({dp:true})`) — it speaks
// ONLY manuSpecificTuya (0xEF00), never the standard closuresWindowCovering
// (0x0102) cluster the stub bound. The stub wired generic::kFzCoverPosition /
// kTzCoverPosition + bound 0x0102, so `position` could never decode and the
// device surfaced nothing on the real wire.
//
// DP map: z2m `legacy.fz.tuya_cover` (dataPoints.coverPosition=2 set,
// coverArrived=3 arrived → both `position`; state=1 control) plus the
// per-model `tuyaDatapoints` meta (103/104/105 upper/middle/lower stroke
// limits, lookup {SET:true, RESET:false}). Mirrors the ZM25RX-08/30 / ZMP1
// sibling DP defs (DP 2 + DP 3 → "position").
//
// z2m-source: zemismart.ts #ZM25TQ (legacy.fz.tuya_cover + meta.tuyaDatapoints
// 103/104/105).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::zemismart {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZM25TQ_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ZM25TQ_stroke_limit[] = {
    { 0, "RESET" },
    { 1, "SET" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_ZM25TQ[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum_ZM25TQ_state_dp1, sizeof(kEnum_ZM25TQ_state_dp1)/sizeof(kEnum_ZM25TQ_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "upper_stroke_limit", ::zhc::TuyaDpType::Enum, 1, kEnum_ZM25TQ_stroke_limit, sizeof(kEnum_ZM25TQ_stroke_limit)/sizeof(kEnum_ZM25TQ_stroke_limit[0]) },
    { 104, "middle_stroke_limit", ::zhc::TuyaDpType::Enum, 1, kEnum_ZM25TQ_stroke_limit, sizeof(kEnum_ZM25TQ_stroke_limit)/sizeof(kEnum_ZM25TQ_stroke_limit[0]) },
    { 105, "lower_stroke_limit", ::zhc::TuyaDpType::Enum, 1, kEnum_ZM25TQ_stroke_limit, sizeof(kEnum_ZM25TQ_stroke_limit)/sizeof(kEnum_ZM25TQ_stroke_limit[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_ZM25TQ{ kEntries_ZM25TQ, sizeof(kEntries_ZM25TQ)/sizeof(kEntries_ZM25TQ[0]) };

constexpr FzConverter kFzDp_ZM25TQ{
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
    .user_config       = &kMap_ZM25TQ,
};
const FzConverter* const kFz_ZM25TQ[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_ZM25TQ,
};
constexpr TzConverter kTzDp_ZM25TQ{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_ZM25TQ,
};
const TzConverter* const kTz_ZM25TQ[] = { &kTzDp_ZM25TQ };
constexpr const char* kModels_ZM25TQ[] = { "TS0601" };
constexpr const char* kManus_ZM25TQ[] = { "_TZE200_fzo2pocs" };

constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"upper_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"middle_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"lower_stroke_limit", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
}  // namespace

extern const PreparedDefinition kDef_ZM25TQ{
    .zigbee_models=kModels_ZM25TQ, .zigbee_models_count=sizeof(kModels_ZM25TQ)/sizeof(kModels_ZM25TQ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZM25TQ, .manufacturer_names_count=sizeof(kManus_ZM25TQ)/sizeof(kManus_ZM25TQ[0]),
    .model="ZM25TQ", .vendor="Zemismart",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZM25TQ, .from_zigbee_count=sizeof(kFz_ZM25TQ)/sizeof(kFz_ZM25TQ[0]),
    .to_zigbee=kTz_ZM25TQ, .to_zigbee_count=sizeof(kTz_ZM25TQ)/sizeof(kTz_ZM25TQ[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::zemismart
