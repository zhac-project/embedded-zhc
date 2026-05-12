// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_mvtclclq).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE204_mvtclclq_relay_status_dp14[] = {
    { 0, "memory" },
    { 1, "on" },
    { 2, "off" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_mvtclclq[] = {
    { 1, "state_usb_a", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 2, "state_usb_c", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 3, "state_plug_1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 4, "state_plug_2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 7, "countdown_usb_a", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8, "countdown_usb_c", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 9, "countdown_plug_1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 10, "countdown_plug_2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 14, "relay_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE204_mvtclclq_relay_status_dp14, sizeof(kEnum__TZE204_mvtclclq_relay_status_dp14)/sizeof(kEnum__TZE204_mvtclclq_relay_status_dp14[0]) },
    { 16, "switch_backlight", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 21, "current", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 22, "power", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 23, "voltage", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 105, "produced_energy", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 106, "child_lock", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_mvtclclq{ kEntries__TZE204_mvtclclq, 15 };
constexpr FzConverter kFzDp__TZE204_mvtclclq{
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
    .user_config       = &kMap__TZE204_mvtclclq,
};
const FzConverter* const kFz__TZE204_mvtclclq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_mvtclclq,
};
constexpr TzConverter kTzDp__TZE204_mvtclclq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_mvtclclq,
};
const TzConverter* const kTz__TZE204_mvtclclq[] = { &kTzDp__TZE204_mvtclclq };
constexpr const char* kM__TZE204_mvtclclq[] = { "TS0601" };
constexpr const char* kN__TZE204_mvtclclq[] = { "_TZE204_mvtclclq" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_mvtclclq{
    .zigbee_models=kM__TZE204_mvtclclq,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_mvtclclq,.manufacturer_names_count=1,
    .model="TS0601__TZE204_mvtclclq",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_mvtclclq,
    .from_zigbee_count=sizeof(kFz__TZE204_mvtclclq)/sizeof(kFz__TZE204_mvtclclq[0]),
    .to_zigbee=kTz__TZE204_mvtclclq,
    .to_zigbee_count=sizeof(kTz__TZE204_mvtclclq)/sizeof(kTz__TZE204_mvtclclq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
