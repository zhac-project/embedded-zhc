// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Onenuo 288WZ smoke detector (TS0601 / _TZE284_n4ttsck2).
// z2m-source: onenuo.ts #288WZ — completed the Tuya-DP map: DP1 fans to
// smoke (bool, alarm state) + smoke_state (enum), DP101 self_test_result
// (bool→failure/success). Was generator-incomplete (only DP15/16/102) and
// carried phantom state/action binary exposes.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::onenuo {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_n4ttsck2_sensitivity_dp102[] = {
    { 0, "low" },
    { 1, "medium" },
    { 2, "high" },
};

// DP1 state enum (z2m lookup): 0=alarm 1=normal 2=detecting 3=unknown.
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_n4ttsck2_smoke_state_dp1[] = {
    { 0, "alarm" },
    { 1, "normal" },
    { 2, "detecting" },
    { 3, "unknown" },
};
// `smoke` bool is true only in the alarm state (z2m: state === "alarm").
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_n4ttsck2_smoke_dp1[] = {
    { 0, "alarm" },
};
// DP101 self_test_result (z2m lookup {failure:false, success:true}) over a
// boolean DP → string via kTuyaDpFlagBoolEnum (0=failure, 1=success).
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_n4ttsck2_self_test_dp101[] = {
    { 0, "failure" },
    { 1, "success" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_n4ttsck2[] = {
    // DP1 fans to two keys (both fire on decode): the string state + the bool.
    { 1, "smoke_state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_n4ttsck2_smoke_state_dp1, sizeof(kEnum__TZE284_n4ttsck2_smoke_state_dp1)/sizeof(kEnum__TZE284_n4ttsck2_smoke_state_dp1[0]) },
    { 1, "smoke", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_n4ttsck2_smoke_dp1, sizeof(kEnum__TZE284_n4ttsck2_smoke_dp1)/sizeof(kEnum__TZE284_n4ttsck2_smoke_dp1[0]), ::zhc::tuya::kTuyaDpFlagEnumBool },
    { 15, "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "silence", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "self_test_result", ::zhc::TuyaDpType::Bool, 1, kEnum__TZE284_n4ttsck2_self_test_dp101, sizeof(kEnum__TZE284_n4ttsck2_self_test_dp101)/sizeof(kEnum__TZE284_n4ttsck2_self_test_dp101[0]), ::zhc::tuya::kTuyaDpFlagBoolEnum },
    { 102, "sensitivity", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_n4ttsck2_sensitivity_dp102, sizeof(kEnum__TZE284_n4ttsck2_sensitivity_dp102)/sizeof(kEnum__TZE284_n4ttsck2_sensitivity_dp102[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_n4ttsck2{ kEntries__TZE284_n4ttsck2, sizeof(kEntries__TZE284_n4ttsck2)/sizeof(kEntries__TZE284_n4ttsck2[0]) };
constexpr FzConverter kFzDp__TZE284_n4ttsck2{
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
    .user_config       = &kMap__TZE284_n4ttsck2,
};
const FzConverter* const kFz__TZE284_n4ttsck2[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_n4ttsck2,
};
constexpr TzConverter kTzDp__TZE284_n4ttsck2{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_n4ttsck2,
};
const TzConverter* const kTz__TZE284_n4ttsck2[] = { &kTzDp__TZE284_n4ttsck2 };
constexpr const char* kM__TZE284_n4ttsck2[] = { "TS0601" };
constexpr const char* kN__TZE284_n4ttsck2[] = { "_TZE284_n4ttsck2" };
}  // namespace

// Expose enum value lists (mirror z2m onenuo.ts #288WZ exposes).
constexpr const char* kSmokeStateValues__TZE284_n4ttsck2[] = { "alarm", "normal", "detecting", "unknown" };
constexpr const char* kSelfTestValues__TZE284_n4ttsck2[]   = { "checking", "success", "failure", "others" };
constexpr const char* kSensitivityValues__TZE284_n4ttsck2[] = { "low", "medium", "high" };

constexpr Expose kAutoExposes[] = {
    {"smoke", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"silence", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"self_test_result", ExposeType::Enum, Access::State, nullptr, nullptr, kSelfTestValues__TZE284_n4ttsck2, sizeof(kSelfTestValues__TZE284_n4ttsck2)/sizeof(kSelfTestValues__TZE284_n4ttsck2[0])},
    {"smoke_state", ExposeType::Enum, Access::State, nullptr, nullptr, kSmokeStateValues__TZE284_n4ttsck2, sizeof(kSmokeStateValues__TZE284_n4ttsck2)/sizeof(kSmokeStateValues__TZE284_n4ttsck2[0])},
    {"sensitivity", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSensitivityValues__TZE284_n4ttsck2, sizeof(kSensitivityValues__TZE284_n4ttsck2)/sizeof(kSensitivityValues__TZE284_n4ttsck2[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDefOne__TZE284_n4ttsck2{
    .zigbee_models=kM__TZE284_n4ttsck2,.zigbee_models_count=sizeof(kM__TZE284_n4ttsck2)/sizeof(kM__TZE284_n4ttsck2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_n4ttsck2,.manufacturer_names_count=1,
    .model="TS0601__TZE284_n4ttsck2",.vendor="Onenuo",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE284_n4ttsck2,
    .from_zigbee_count=sizeof(kFz__TZE284_n4ttsck2)/sizeof(kFz__TZE284_n4ttsck2[0]),
    .to_zigbee=kTz__TZE284_n4ttsck2,
    .to_zigbee_count=sizeof(kTz__TZE284_n4ttsck2)/sizeof(kTz__TZE284_n4ttsck2[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::onenuo
