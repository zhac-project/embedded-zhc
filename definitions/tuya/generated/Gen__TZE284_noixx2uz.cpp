// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_noixx2uz).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_noixx2uz_preset_dp2[] = {
    { 0, "auto" },
    { 1, "manual" },
    { 2, "leave" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_noixx2uz_running_state_dp3[] = {
    { 0, "idle" },
    { 1, "heat" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_noixx2uz[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_noixx2uz_preset_dp2, sizeof(kEnum__TZE284_noixx2uz_preset_dp2)/sizeof(kEnum__TZE284_noixx2uz_preset_dp2[0]) },
    { 3, "running_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_noixx2uz_running_state_dp3, sizeof(kEnum__TZE284_noixx2uz_running_state_dp3)/sizeof(kEnum__TZE284_noixx2uz_running_state_dp3[0]) },
    { 6, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_noixx2uz{ kEntries__TZE284_noixx2uz, 3 };
constexpr FzConverter kFzDp__TZE284_noixx2uz{
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
    .user_config       = &kMap__TZE284_noixx2uz,
};
const FzConverter* const kFz__TZE284_noixx2uz[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_noixx2uz,
};
constexpr TzConverter kTzDp__TZE284_noixx2uz{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_noixx2uz,
};
const TzConverter* const kTz__TZE284_noixx2uz[] = { &kTzDp__TZE284_noixx2uz };
constexpr const char* kM__TZE284_noixx2uz[] = { "TS0601" };
constexpr const char* kN__TZE284_noixx2uz[] = { "_TZE284_noixx2uz" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_noixx2uz{
    .zigbee_models=kM__TZE284_noixx2uz,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_noixx2uz,.manufacturer_names_count=1,
    .model="TS0601__TZE284_noixx2uz",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_noixx2uz,
    .from_zigbee_count=sizeof(kFz__TZE284_noixx2uz)/sizeof(kFz__TZE284_noixx2uz[0]),
    .to_zigbee=kTz__TZE284_noixx2uz,
    .to_zigbee_count=sizeof(kTz__TZE284_noixx2uz)/sizeof(kTz__TZE284_noixx2uz[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
