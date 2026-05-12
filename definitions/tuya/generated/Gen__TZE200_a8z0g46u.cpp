// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_a8z0g46u).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_a8z0g46u_state_dp2[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_a8z0g46u[] = {
    { 2, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_a8z0g46u_state_dp2, sizeof(kEnum__TZE200_a8z0g46u_state_dp2)/sizeof(kEnum__TZE200_a8z0g46u_state_dp2[0]) },
    { 7, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_a8z0g46u{ kEntries__TZE200_a8z0g46u, 3 };
constexpr FzConverter kFzDp__TZE200_a8z0g46u{
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
    .user_config       = &kMap__TZE200_a8z0g46u,
};
const FzConverter* const kFz__TZE200_a8z0g46u[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_a8z0g46u,
};
constexpr TzConverter kTzDp__TZE200_a8z0g46u{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_a8z0g46u,
};
const TzConverter* const kTz__TZE200_a8z0g46u[] = { &kTzDp__TZE200_a8z0g46u };
constexpr const char* kM__TZE200_a8z0g46u[] = { "TS0601" };
constexpr const char* kN__TZE200_a8z0g46u[] = { "_TZE200_a8z0g46u" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_a8z0g46u{
    .zigbee_models=kM__TZE200_a8z0g46u,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_a8z0g46u,.manufacturer_names_count=1,
    .model="TS0601__TZE200_a8z0g46u",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_a8z0g46u,
    .from_zigbee_count=sizeof(kFz__TZE200_a8z0g46u)/sizeof(kFz__TZE200_a8z0g46u[0]),
    .to_zigbee=kTz__TZE200_a8z0g46u,
    .to_zigbee_count=sizeof(kTz__TZE200_a8z0g46u)/sizeof(kTz__TZE200_a8z0g46u[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
