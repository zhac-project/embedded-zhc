// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: _TZE200_vzekyi4c smoke sensor (z2m TS0601_smoke_4).
//
// z2m has listed this fingerprint as TS0601_smoke_4 since 2022-12-11
// (zigbee2mqtt#15483). The first port here called it a PIR and published the
// smoke alarm as `occupancy`, so a fire raised presence and no smoke alert.
// Same datapoints as its generated siblings (Gen__TZE200_t5p1vj8r & co):
// DP1 smoke (z2m trueFalse0: 0 = alarm), DP14 battery_state, DP15 battery.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_vzekyi4c_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_vzekyi4c[] = {
    { 1, "smoke", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 14, "battery_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_vzekyi4c_battery_state_dp14, sizeof(kEnum__TZE200_vzekyi4c_battery_state_dp14)/sizeof(kEnum__TZE200_vzekyi4c_battery_state_dp14[0]) },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_vzekyi4c{ kEntries__TZE200_vzekyi4c, 3 };
constexpr FzConverter kFzDp__TZE200_vzekyi4c{
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
    .user_config       = &kMap__TZE200_vzekyi4c,
};
const FzConverter* const kFz__TZE200_vzekyi4c[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_vzekyi4c,
};
constexpr TzConverter kTzDp__TZE200_vzekyi4c{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_vzekyi4c,
};
const TzConverter* const kTz__TZE200_vzekyi4c[] = { &kTzDp__TZE200_vzekyi4c };
constexpr const char* kM__TZE200_vzekyi4c[] = { "TS0601" };
constexpr const char* kN__TZE200_vzekyi4c[] = { "_TZE200_vzekyi4c" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE200_vzekyi4c_2[] = {"low", "medium", "high"};
constexpr Expose kAutoExposes__TZE200_vzekyi4c[] = {
    {"smoke", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"battery_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts__TZE200_vzekyi4c_2, 3, ExposeCategory::Diagnostic},
};
// --- end auto-generated exposes ---
}  // namespace
extern const PreparedDefinition kDefTZE200_vzekyi4c{
    .zigbee_models=kM__TZE200_vzekyi4c,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_vzekyi4c,.manufacturer_names_count=1,
    .model="TS0601_smoke_4",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE200_vzekyi4c,.exposes_count=sizeof(kAutoExposes__TZE200_vzekyi4c)/sizeof(kAutoExposes__TZE200_vzekyi4c[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_vzekyi4c,
    .from_zigbee_count=sizeof(kFz__TZE200_vzekyi4c)/sizeof(kFz__TZE200_vzekyi4c[0]),
    .to_zigbee=kTz__TZE200_vzekyi4c,
    .to_zigbee_count=sizeof(kTz__TZE200_vzekyi4c)/sizeof(kTz__TZE200_vzekyi4c[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
