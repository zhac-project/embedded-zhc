// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE200_vuwtqx0t).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_vuwtqx0t_report_period_dp4[] = {
    { 0, "1h" },
    { 1, "2h" },
    { 2, "3h" },
    { 3, "4h" },
    { 4, "6h" },
    { 5, "8h" },
    { 6, "12h" },
    { 7, "24h" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_vuwtqx0t[] = {
    { 1, "water_consumed", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 4, "report_period", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_vuwtqx0t_report_period_dp4, sizeof(kEnum__TZE200_vuwtqx0t_report_period_dp4)/sizeof(kEnum__TZE200_vuwtqx0t_report_period_dp4[0]) },
    { 13, "state", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 14, "auto_clean", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 16, "meter_id", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 22, "temperature", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
    { 26, "battery_voltage", ::zhc::TuyaDpType::Numeric, 100, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_vuwtqx0t{ kEntries__TZE200_vuwtqx0t, 7 };
constexpr FzConverter kFzDp__TZE200_vuwtqx0t{
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
    .user_config       = &kMap__TZE200_vuwtqx0t,
};
const FzConverter* const kFz__TZE200_vuwtqx0t[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_vuwtqx0t,
};
constexpr TzConverter kTzDp__TZE200_vuwtqx0t{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_vuwtqx0t,
};
const TzConverter* const kTz__TZE200_vuwtqx0t[] = { &kTzDp__TZE200_vuwtqx0t };
constexpr const char* kM__TZE200_vuwtqx0t[] = { "TS0601" };
constexpr const char* kN__TZE200_vuwtqx0t[] = { "_TZE200_vuwtqx0t" };
}  // namespace
extern const PreparedDefinition kDefGen__TZE200_vuwtqx0t{
    .zigbee_models=kM__TZE200_vuwtqx0t,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_vuwtqx0t,.manufacturer_names_count=1,
    .model="TS0601__TZE200_vuwtqx0t",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_vuwtqx0t,
    .from_zigbee_count=sizeof(kFz__TZE200_vuwtqx0t)/sizeof(kFz__TZE200_vuwtqx0t[0]),
    .to_zigbee=kTz__TZE200_vuwtqx0t,
    .to_zigbee_count=sizeof(kTz__TZE200_vuwtqx0t)/sizeof(kTz__TZE200_vuwtqx0t[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
