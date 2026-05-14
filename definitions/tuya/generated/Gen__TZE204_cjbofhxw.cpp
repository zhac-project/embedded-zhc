// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_cjbofhxw).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_cjbofhxw[] = {
    { 18, "current", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 19, "power", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 20, "voltage", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 101, "energy", ::zhc::TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_cjbofhxw{ kEntries__TZE204_cjbofhxw, 4 };
constexpr FzConverter kFzDp__TZE204_cjbofhxw{
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
    .user_config       = &kMap__TZE204_cjbofhxw,
};
const FzConverter* const kFz__TZE204_cjbofhxw[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_cjbofhxw,
};
constexpr TzConverter kTzDp__TZE204_cjbofhxw{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_cjbofhxw,
};
const TzConverter* const kTz__TZE204_cjbofhxw[] = { &kTzDp__TZE204_cjbofhxw };
constexpr const char* kM__TZE204_cjbofhxw[] = { "TS0601" };
constexpr const char* kN__TZE204_cjbofhxw[] = { "_TZE204_cjbofhxw" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE204_cjbofhxw[] = {
    {"Tuya","PJ-1203-W"},
};
extern const PreparedDefinition kDefGen__TZE204_cjbofhxw{
    .zigbee_models=kM__TZE204_cjbofhxw,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_cjbofhxw,.manufacturer_names_count=1,
    .model="TS0601__TZE204_cjbofhxw",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE204_cjbofhxw, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE204_cjbofhxw)/sizeof(kWhiteLabels_Gen_TZE204_cjbofhxw[0]),
    .from_zigbee=kFz__TZE204_cjbofhxw,
    .from_zigbee_count=sizeof(kFz__TZE204_cjbofhxw)/sizeof(kFz__TZE204_cjbofhxw[0]),
    .to_zigbee=kTz__TZE204_cjbofhxw,
    .to_zigbee_count=sizeof(kTz__TZE204_cjbofhxw)/sizeof(kTz__TZE204_cjbofhxw[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
