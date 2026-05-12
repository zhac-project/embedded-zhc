// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_zm8zpwas).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_zm8zpwas_valve_state_dp8[] = {
    { 0, "unknown" },
    { 1, "open" },
    { 2, "closed" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_zm8zpwas[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 8, "valve_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_zm8zpwas_valve_state_dp8, sizeof(kEnum__TZE284_zm8zpwas_valve_state_dp8)/sizeof(kEnum__TZE284_zm8zpwas_valve_state_dp8[0]) },
    { 101, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_zm8zpwas{ kEntries__TZE284_zm8zpwas, 3 };
constexpr FzConverter kFzDp__TZE284_zm8zpwas{
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
    .user_config       = &kMap__TZE284_zm8zpwas,
};
const FzConverter* const kFz__TZE284_zm8zpwas[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_zm8zpwas,
};
constexpr TzConverter kTzDp__TZE284_zm8zpwas{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_zm8zpwas,
};
const TzConverter* const kTz__TZE284_zm8zpwas[] = { &kTzDp__TZE284_zm8zpwas };
constexpr const char* kM__TZE284_zm8zpwas[] = { "TS0601" };
constexpr const char* kN__TZE284_zm8zpwas[] = { "_TZE284_zm8zpwas" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE284_zm8zpwas{
    .zigbee_models=kM__TZE284_zm8zpwas,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_zm8zpwas,.manufacturer_names_count=1,
    .model="TS0601__TZE284_zm8zpwas",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_zm8zpwas,
    .from_zigbee_count=sizeof(kFz__TZE284_zm8zpwas)/sizeof(kFz__TZE284_zm8zpwas[0]),
    .to_zigbee=kTz__TZE284_zm8zpwas,
    .to_zigbee_count=sizeof(kTz__TZE284_zm8zpwas)/sizeof(kTz__TZE284_zm8zpwas[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
