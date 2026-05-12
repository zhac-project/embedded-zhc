// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Woox R7049 — smart smoke alarm (TS0601 Tuya MCU).
// z2m-source: woox.ts #R7049 (legacy.fromZigbee.R7049_status DP-stream).
//   DP 1   bool — smoke (z2m emits Boolean(!value); we use invert-bool)
//   DP 8   bool — test_alarm
//   DP 9   enum — test_alarm_result {0:checking,1:check_success,
//                  2:check_failure,3:others}
//   DP 11  bool — fault_alarm
//   DP 14  enum — battery_level {0:low,1:middle,2:high} + battery_low (low)
//   DP 16  bool — silence_siren
//   DP 20  enum — alarm 0→true, 1→false (inverted-enum, see below)
// tz: silence_siren → DP 16 bool, test_alarm → DP 8 bool,
//     alarm → DP 20 enum (value? 0 : 1 — inverted in z2m).
//
// Notes:
//   - "alarm" DP 20 doesn't fit the standard bool-invert flag because
//     z2m maps {0:true, 1:false} as a lookup, not a Bool. Encode as
//     2-entry enum lookup so reads honour the same mapping.
//   - This port matches z2m as closely as ZHC's runtime allows; full
//     parity (battery_level enum string, test_alarm_result, fault_alarm
//     write-back) needs a per-attribute test on hardware before any
//     production claim.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::woox {
namespace {

// --- Enum lookup tables ----------------------------------------------------
constexpr ::zhc::tuya::TuyaEnumEntry kBatteryLevel_R7049[] = {
    { 0, "low" },
    { 1, "middle" },
    { 2, "high" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kTestAlarmResult_R7049[] = {
    { 0, "checking" },
    { 1, "check_success" },
    { 2, "check_failure" },
    { 3, "others" },
};
// alarm: z2m maps {0:true, 1:false}. We model as enum for symmetric
// rd/wr — encode true as label "true" (value 0), false as "false"
// (value 1). The shadow tier coerces these to bool via the StringRef
// emit path; on the tz side z2m does `value ? 0 : 1` which we mirror
// with the lookup by label.
constexpr ::zhc::tuya::TuyaEnumEntry kAlarm_R7049[] = {
    { 0, "true" },
    { 1, "false" },
};

// --- DP map ----------------------------------------------------------------
constexpr ::zhc::tuya::TuyaDpMapEntry kDps_R7049[] = {
    // smoke: z2m does `Boolean(!value)`. We invert via flag.
    { 1,  "smoke",             ::zhc::TuyaDpType::Bool,    1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 8,  "test_alarm",        ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 9,  "test_alarm_result", ::zhc::TuyaDpType::Enum,    1,
        kTestAlarmResult_R7049, sizeof(kTestAlarmResult_R7049)/sizeof(kTestAlarmResult_R7049[0]), 0 },
    { 11, "fault_alarm",       ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 14, "battery_level",     ::zhc::TuyaDpType::Enum,    1,
        kBatteryLevel_R7049, sizeof(kBatteryLevel_R7049)/sizeof(kBatteryLevel_R7049[0]), 0 },
    { 16, "silence_siren",     ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 20, "alarm",             ::zhc::TuyaDpType::Enum,    1,
        kAlarm_R7049, sizeof(kAlarm_R7049)/sizeof(kAlarm_R7049[0]), 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kDpMap_R7049{
    kDps_R7049, sizeof(kDps_R7049)/sizeof(kDps_R7049[0])
};

constexpr FzConverter kFzDp_R7049{
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
    .user_config       = &kDpMap_R7049,
};
constexpr TzConverter kTzDp_R7049{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kDpMap_R7049,
};

const FzConverter* const kFz_R7049[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::tuya::kFzIgnoreTuyaSetTime,
    &kFzDp_R7049,
};
const TzConverter* const kTz_R7049[] = { &kTzDp_R7049 };

constexpr const char* kModels_R7049[] = { "TS0601" };
constexpr const char* kManus_R7049[] = { "_TZE200_aycxwiau", "_TZE200_bxdyeaa9", "_TZE200_ft523twt" };

constexpr Expose kExp_R7049[] = {
    {"smoke",             ExposeType::Binary, Access::State,    nullptr, nullptr, nullptr, 0},
    {"test_alarm",        ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"test_alarm_result", ExposeType::Binary, Access::State,    nullptr, nullptr, nullptr, 0},
    {"fault_alarm",       ExposeType::Binary, Access::State,    nullptr, nullptr, nullptr, 0},
    {"battery_level",     ExposeType::Binary, Access::State,    nullptr, nullptr, nullptr, 0},
    {"silence_siren",     ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alarm",             ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_R7049[] = {
    {1, 0xEF00},  // manuSpecificTuya
};

}  // namespace

extern const PreparedDefinition kDef_R7049{
    .zigbee_models=kModels_R7049, .zigbee_models_count=sizeof(kModels_R7049)/sizeof(kModels_R7049[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_R7049, .manufacturer_names_count=sizeof(kManus_R7049)/sizeof(kManus_R7049[0]),
    .model="R7049", .vendor="Woox",
    .meta=nullptr, .exposes=kExp_R7049, .exposes_count=sizeof(kExp_R7049)/sizeof(kExp_R7049[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_R7049, .from_zigbee_count=sizeof(kFz_R7049)/sizeof(kFz_R7049[0]),
    .to_zigbee=kTz_R7049,   .to_zigbee_count=sizeof(kTz_R7049)/sizeof(kTz_R7049[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_R7049,
    .bindings_count=sizeof(kBind_R7049)/sizeof(kBind_R7049[0]),
};

}  // namespace zhc::devices::woox
