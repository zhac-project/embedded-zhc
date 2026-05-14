// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_nhgdf6qr).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_nhgdf6qr_temperature_unit_dp9[] = {
    { 0, "celsius" },
    { 1, "fahrenheit" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_nhgdf6qr_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_nhgdf6qr[] = {
    { 3, "soil_moisture", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 5, "temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 9, "temperature_unit", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_nhgdf6qr_temperature_unit_dp9, sizeof(kEnum__TZE284_nhgdf6qr_temperature_unit_dp9)/sizeof(kEnum__TZE284_nhgdf6qr_temperature_unit_dp9[0]) },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_nhgdf6qr_battery_state_dp14, sizeof(kEnum__TZE284_nhgdf6qr_battery_state_dp14)/sizeof(kEnum__TZE284_nhgdf6qr_battery_state_dp14[0]) },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_nhgdf6qr{ kEntries__TZE284_nhgdf6qr, 5 };
constexpr FzConverter kFzDp__TZE284_nhgdf6qr{
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
    .user_config       = &kMap__TZE284_nhgdf6qr,
};
const FzConverter* const kFz__TZE284_nhgdf6qr[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_nhgdf6qr,
};
constexpr TzConverter kTzDp__TZE284_nhgdf6qr{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_nhgdf6qr,
};
const TzConverter* const kTz__TZE284_nhgdf6qr[] = { &kTzDp__TZE284_nhgdf6qr };
constexpr const char* kM__TZE284_nhgdf6qr[] = { "TS0601" };
constexpr const char* kN__TZE284_nhgdf6qr[] = { "_TZE284_nhgdf6qr" };
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE284_nhgdf6qr[] = {
    {"GIEX","GX04"},
    {"GIEX","GX06"},
};
extern const PreparedDefinition kDefGen__TZE284_nhgdf6qr{
    .zigbee_models=kM__TZE284_nhgdf6qr,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_nhgdf6qr,.manufacturer_names_count=1,
    .model="TS0601__TZE284_nhgdf6qr",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_Gen_TZE284_nhgdf6qr, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE284_nhgdf6qr)/sizeof(kWhiteLabels_Gen_TZE284_nhgdf6qr[0]),
    .from_zigbee=kFz__TZE284_nhgdf6qr,
    .from_zigbee_count=sizeof(kFz__TZE284_nhgdf6qr)/sizeof(kFz__TZE284_nhgdf6qr[0]),
    .to_zigbee=kTz__TZE284_nhgdf6qr,
    .to_zigbee_count=sizeof(kTz__TZE284_nhgdf6qr)/sizeof(kTz__TZE284_nhgdf6qr[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
