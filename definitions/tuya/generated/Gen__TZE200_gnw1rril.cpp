// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_gnw1rril).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_gnw1rril_state_dp1[] = {
    { 2, "OPEN" },
    { 1, "STOP" },
    { 0, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_gnw1rril[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_gnw1rril_state_dp1, sizeof(kEnum__TZE200_gnw1rril_state_dp1)/sizeof(kEnum__TZE200_gnw1rril_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_gnw1rril{ kEntries__TZE200_gnw1rril, 3 };
constexpr FzConverter kFzDp__TZE200_gnw1rril{
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
    .user_config       = &kMap__TZE200_gnw1rril,
};
const FzConverter* const kFz__TZE200_gnw1rril[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_gnw1rril,
};
constexpr TzConverter kTzDp__TZE200_gnw1rril{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_gnw1rril,
};
const TzConverter* const kTz__TZE200_gnw1rril[] = { &kTzDp__TZE200_gnw1rril };
constexpr const char* kM__TZE200_gnw1rril[] = { "TS0601" };
constexpr const char* kN__TZE200_gnw1rril[] = { "_TZE200_gnw1rril" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_gnw1rril{
    .zigbee_models=kM__TZE200_gnw1rril,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_gnw1rril,.manufacturer_names_count=1,
    .model="TS0601__TZE200_gnw1rril",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_gnw1rril,
    .from_zigbee_count=sizeof(kFz__TZE200_gnw1rril)/sizeof(kFz__TZE200_gnw1rril[0]),
    .to_zigbee=kTz__TZE200_gnw1rril,
    .to_zigbee_count=sizeof(kTz__TZE200_gnw1rril)/sizeof(kTz__TZE200_gnw1rril[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
