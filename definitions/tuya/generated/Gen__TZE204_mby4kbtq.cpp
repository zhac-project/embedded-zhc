// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE204_mby4kbtq).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE204_mby4kbtq[] = {
    { 1, "gas", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 2, "gas_value", ::zhc::TuyaDpType::Numeric, 10, nullptr, 0, 0 },
    { 10, "preheat", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11, "fault_alarm", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 13, "alarm_switch", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "silence", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE204_mby4kbtq{ kEntries__TZE204_mby4kbtq, 6 };
constexpr FzConverter kFzDp__TZE204_mby4kbtq{
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
    .user_config       = &kMap__TZE204_mby4kbtq,
};
const FzConverter* const kFz__TZE204_mby4kbtq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE204_mby4kbtq,
};
constexpr TzConverter kTzDp__TZE204_mby4kbtq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE204_mby4kbtq,
};
const TzConverter* const kTz__TZE204_mby4kbtq[] = { &kTzDp__TZE204_mby4kbtq };
constexpr const char* kM__TZE204_mby4kbtq[] = { "TS0601" };
constexpr const char* kN__TZE204_mby4kbtq[] = { "_TZE204_mby4kbtq" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr Expose kAutoExposes__TZE204_mby4kbtq[] = {
    {"gas", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"gas_value", ExposeType::Numeric, Access::State, "LEL", nullptr, nullptr, 0},
    {"preheat", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"fault_alarm", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"alarm_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"silence", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefGen__TZE204_mby4kbtq{
    .zigbee_models=kM__TZE204_mby4kbtq,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE204_mby4kbtq,.manufacturer_names_count=1,
    .model="TS0601__TZE204_mby4kbtq",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE204_mby4kbtq,.exposes_count=sizeof(kAutoExposes__TZE204_mby4kbtq)/sizeof(kAutoExposes__TZE204_mby4kbtq[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE204_mby4kbtq,
    .from_zigbee_count=sizeof(kFz__TZE204_mby4kbtq)/sizeof(kFz__TZE204_mby4kbtq[0]),
    .to_zigbee=kTz__TZE204_mby4kbtq,
    .to_zigbee_count=sizeof(kTz__TZE204_mby4kbtq)/sizeof(kTz__TZE204_mby4kbtq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
