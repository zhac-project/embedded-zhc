// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0201 / _TZE200_iq4ygaai).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_iq4ygaai[] = {
    { 1, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 4, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_iq4ygaai{ kEntries__TZE200_iq4ygaai, 2 };
constexpr FzConverter kFzDp__TZE200_iq4ygaai{
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
    .user_config       = &kMap__TZE200_iq4ygaai,
};
const FzConverter* const kFz__TZE200_iq4ygaai[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_iq4ygaai,
};
constexpr TzConverter kTzDp__TZE200_iq4ygaai{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_iq4ygaai,
};
const TzConverter* const kTz__TZE200_iq4ygaai[] = { &kTzDp__TZE200_iq4ygaai };
constexpr const char* kM__TZE200_iq4ygaai[] = { "TS0201" };
constexpr const char* kN__TZE200_iq4ygaai[] = { "_TZE200_iq4ygaai" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_iq4ygaai[] = {
    {"OWON","THS317-ET-EY"},
};
extern const PreparedDefinition kDefGen__TZE200_iq4ygaai{
    .zigbee_models=kM__TZE200_iq4ygaai,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_iq4ygaai,.manufacturer_names_count=1,
    .model="TS0201__TZE200_iq4ygaai",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_iq4ygaai, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_iq4ygaai)/sizeof(kWhiteLabels_Gen_TZE200_iq4ygaai[0]),
    .from_zigbee=kFz__TZE200_iq4ygaai,
    .from_zigbee_count=sizeof(kFz__TZE200_iq4ygaai)/sizeof(kFz__TZE200_iq4ygaai[0]),
    .to_zigbee=kTz__TZE200_iq4ygaai,
    .to_zigbee_count=sizeof(kTz__TZE200_iq4ygaai)/sizeof(kTz__TZE200_iq4ygaai[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
