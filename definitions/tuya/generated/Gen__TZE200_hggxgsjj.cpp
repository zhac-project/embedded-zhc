// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_hggxgsjj).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_hggxgsjj_sensitivity_dp104[] = {
    { 0, "low" },
    { 1, "middle" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_hggxgsjj[] = {
    { 1, "vibration", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 7, "tilt", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 101, "x", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "y", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "z", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "sensitivity", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_hggxgsjj_sensitivity_dp104, sizeof(kEnum__TZE200_hggxgsjj_sensitivity_dp104)/sizeof(kEnum__TZE200_hggxgsjj_sensitivity_dp104[0]) },
    { 105, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_hggxgsjj{ kEntries__TZE200_hggxgsjj, 7 };
constexpr FzConverter kFzDp__TZE200_hggxgsjj{
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
    .user_config       = &kMap__TZE200_hggxgsjj,
};
const FzConverter* const kFz__TZE200_hggxgsjj[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_hggxgsjj,
};
constexpr TzConverter kTzDp__TZE200_hggxgsjj{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_hggxgsjj,
};
const TzConverter* const kTz__TZE200_hggxgsjj[] = { &kTzDp__TZE200_hggxgsjj };
constexpr const char* kM__TZE200_hggxgsjj[] = { "TS0601" };
constexpr const char* kN__TZE200_hggxgsjj[] = { "_TZE200_hggxgsjj" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_hggxgsjj{
    .zigbee_models=kM__TZE200_hggxgsjj,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_hggxgsjj,.manufacturer_names_count=1,
    .model="TS0601__TZE200_hggxgsjj",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_hggxgsjj,
    .from_zigbee_count=sizeof(kFz__TZE200_hggxgsjj)/sizeof(kFz__TZE200_hggxgsjj[0]),
    .to_zigbee=kTz__TZE200_hggxgsjj,
    .to_zigbee_count=sizeof(kTz__TZE200_hggxgsjj)/sizeof(kTz__TZE200_hggxgsjj[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
