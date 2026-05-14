// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_abatw3kj).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_abatw3kj[] = {
    { 16, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 1, "energy", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 15, "leakage", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 103, "temperature", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_abatw3kj{ kEntries__TZE200_abatw3kj, 4 };
constexpr FzConverter kFzDp__TZE200_abatw3kj{
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
    .user_config       = &kMap__TZE200_abatw3kj,
};
const FzConverter* const kFz__TZE200_abatw3kj[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_abatw3kj,
};
constexpr TzConverter kTzDp__TZE200_abatw3kj{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_abatw3kj,
};
const TzConverter* const kTz__TZE200_abatw3kj[] = { &kTzDp__TZE200_abatw3kj };
constexpr const char* kM__TZE200_abatw3kj[] = { "TS0601" };
constexpr const char* kN__TZE200_abatw3kj[] = { "_TZE200_abatw3kj" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_abatw3kj[] = {
    {"RTX","TS0601_RTX_DIN"},
};
extern const PreparedDefinition kDefGen__TZE200_abatw3kj{
    .zigbee_models=kM__TZE200_abatw3kj,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_abatw3kj,.manufacturer_names_count=1,
    .model="TS0601__TZE200_abatw3kj",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE200_abatw3kj, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_abatw3kj)/sizeof(kWhiteLabels_Gen_TZE200_abatw3kj[0]),
    .from_zigbee=kFz__TZE200_abatw3kj,
    .from_zigbee_count=sizeof(kFz__TZE200_abatw3kj)/sizeof(kFz__TZE200_abatw3kj[0]),
    .to_zigbee=kTz__TZE200_abatw3kj,
    .to_zigbee_count=sizeof(kTz__TZE200_abatw3kj)/sizeof(kTz__TZE200_abatw3kj[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
