// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_gwkapsoq).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_gwkapsoq[] = {
    { 6, "countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 12, "countdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_gwkapsoq{ kEntries__TZE200_gwkapsoq, 2 };
constexpr FzConverter kFzDp__TZE200_gwkapsoq{
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
    .user_config       = &kMap__TZE200_gwkapsoq,
};
const FzConverter* const kFz__TZE200_gwkapsoq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_gwkapsoq,
};
constexpr TzConverter kTzDp__TZE200_gwkapsoq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_gwkapsoq,
};
const TzConverter* const kTz__TZE200_gwkapsoq[] = { &kTzDp__TZE200_gwkapsoq };
constexpr const char* kM__TZE200_gwkapsoq[] = { "TS0601" };
constexpr const char* kN__TZE200_gwkapsoq[] = { "_TZE200_gwkapsoq" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_gwkapsoq[] = {
    {"Moes","ZS-SR-EUD-2"},
    {"Moes","ZS-EUD_2gang"},
    {"Moes","MS-105B"},
};
extern const PreparedDefinition kDefGen__TZE200_gwkapsoq{
    .zigbee_models=kM__TZE200_gwkapsoq,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_gwkapsoq,.manufacturer_names_count=1,
    .model="TS0601__TZE200_gwkapsoq",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_gwkapsoq, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_gwkapsoq)/sizeof(kWhiteLabels_Gen_TZE200_gwkapsoq[0]),
    .from_zigbee=kFz__TZE200_gwkapsoq,
    .from_zigbee_count=sizeof(kFz__TZE200_gwkapsoq)/sizeof(kFz__TZE200_gwkapsoq[0]),
    .to_zigbee=kTz__TZE200_gwkapsoq,
    .to_zigbee_count=sizeof(kTz__TZE200_gwkapsoq)/sizeof(kTz__TZE200_gwkapsoq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
