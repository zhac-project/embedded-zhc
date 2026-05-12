// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0001 / _TZ3210_7vgttna6).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZ3210_7vgttna6_mode_dp101[] = {
    { 0, "click" },
    { 1, "switch" },
    { 2, "program" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZ3210_7vgttna6_reverse_dp104[] = {
    { 1, "ON" },
    { 0, "OFF" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZ3210_7vgttna6[] = {
    { 101, "mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZ3210_7vgttna6_mode_dp101, sizeof(kEnum__TZ3210_7vgttna6_mode_dp101)/sizeof(kEnum__TZ3210_7vgttna6_mode_dp101[0]) },
    { 102, "lower", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "delay", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "reverse", ::zhc::TuyaDpType::Enum, 1, kEnum__TZ3210_7vgttna6_reverse_dp104, sizeof(kEnum__TZ3210_7vgttna6_reverse_dp104)/sizeof(kEnum__TZ3210_7vgttna6_reverse_dp104[0]) },
    { 105, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 106, "upper", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "program", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZ3210_7vgttna6{ kEntries__TZ3210_7vgttna6, 7 };
constexpr FzConverter kFzDp__TZ3210_7vgttna6{
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
    .user_config       = &kMap__TZ3210_7vgttna6,
};
const FzConverter* const kFz__TZ3210_7vgttna6[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZ3210_7vgttna6,
};
constexpr TzConverter kTzDp__TZ3210_7vgttna6{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZ3210_7vgttna6,
};
const TzConverter* const kTz__TZ3210_7vgttna6[] = { &kTzDp__TZ3210_7vgttna6 };
constexpr const char* kM__TZ3210_7vgttna6[] = { "TS0001" };
constexpr const char* kN__TZ3210_7vgttna6[] = { "_TZ3210_7vgttna6" };
}  // namespace
extern const PreparedDefinition kDefGen__TZ3210_7vgttna6{
    .zigbee_models=kM__TZ3210_7vgttna6,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZ3210_7vgttna6,.manufacturer_names_count=1,
    .model="TS0001__TZ3210_7vgttna6",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZ3210_7vgttna6,
    .from_zigbee_count=sizeof(kFz__TZ3210_7vgttna6)/sizeof(kFz__TZ3210_7vgttna6[0]),
    .to_zigbee=kTz__TZ3210_7vgttna6,
    .to_zigbee_count=sizeof(kTz__TZ3210_7vgttna6)/sizeof(kTz__TZ3210_7vgttna6[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
