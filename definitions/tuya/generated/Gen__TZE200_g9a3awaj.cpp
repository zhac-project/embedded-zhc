// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_g9a3awaj).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_g9a3awaj_preset_dp2[] = {
    { 1, "manual" },
    { 0, "program" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_g9a3awaj[] = {
    { 2, "preset", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_g9a3awaj_preset_dp2, sizeof(kEnum__TZE200_g9a3awaj_preset_dp2)/sizeof(kEnum__TZE200_g9a3awaj_preset_dp2[0]) },
    { 16, "current_heating_setpoint", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 24, "local_temperature", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 10, "frost", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_g9a3awaj{ kEntries__TZE200_g9a3awaj, 4 };
constexpr FzConverter kFzDp__TZE200_g9a3awaj{
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
    .user_config       = &kMap__TZE200_g9a3awaj,
};
const FzConverter* const kFz__TZE200_g9a3awaj[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_g9a3awaj,
};
constexpr TzConverter kTzDp__TZE200_g9a3awaj{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_g9a3awaj,
};
const TzConverter* const kTz__TZE200_g9a3awaj[] = { &kTzDp__TZE200_g9a3awaj };
constexpr const char* kM__TZE200_g9a3awaj[] = { "TS0601" };
constexpr const char* kN__TZE200_g9a3awaj[] = { "_TZE200_g9a3awaj" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_g9a3awaj{
    .zigbee_models=kM__TZE200_g9a3awaj,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_g9a3awaj,.manufacturer_names_count=1,
    .model="TS0601__TZE200_g9a3awaj",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_g9a3awaj,
    .from_zigbee_count=sizeof(kFz__TZE200_g9a3awaj)/sizeof(kFz__TZE200_g9a3awaj[0]),
    .to_zigbee=kTz__TZE200_g9a3awaj,
    .to_zigbee_count=sizeof(kTz__TZE200_g9a3awaj)/sizeof(kTz__TZE200_g9a3awaj[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
