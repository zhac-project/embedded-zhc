// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: AOYAN AY-204Z PIR motion sensor (z2m v26.92.0 parity, added v26.77.0).
// z2m-source: tuya.ts #AY-204Z
//
// Hybrid device: motion/tamper/battery_low ride standard IAS Zone
// (z2m `m.iasZoneAlarm({zoneType: "occupancy", zoneAttributes: ["alarm_1",
// "tamper", "battery_low"]})`), battery rides genPowerCfg (`m.battery()`),
// and only the two config knobs are Tuya DPs. So the fz list is built by hand
// rather than via factory::TuyaRw, which would cover the DP half alone.
// `kFzIasMotionAlarm` emits exactly the three zone bits z2m asks for.
//
// NOTE the fingerprint: upstream lists the SAME modelID under three
// manufacturer names that differ only by trailing spaces -- "AOYAN",
// "AOYAN ", "AOYAN  ". That is a real device quirk (padded Basic string),
// not a typo; all three must be matched verbatim or units drop off.
//
// Not to be confused with AY-204ZX, a white-label of HOBEIAN ZG-204ZK already
// covered by ZG_204ZK.cpp -- different device, different fingerprint.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

// z2m lookups: sensitivity {low:0, medium:1, high:2},
//              keep_time  {"10":0, "30":1, "60":2, "120":3} (seconds, as labels)
constexpr ::zhc::tuya::TuyaEnumEntry kSensitivity[] = { {0, "low"}, {1, "medium"}, {2, "high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kKeepTime[]    = { {0, "10"}, {1, "30"}, {2, "60"}, {3, "120"} };

constexpr ::zhc::tuya::TuyaDpMapEntry kDpMap[] = {
    ::zhc::tuya::dp::enum_lookup(9,  "sensitivity", kSensitivity, 3),
    ::zhc::tuya::dp::enum_lookup(10, "keep_time",   kKeepTime,    4),
};
constexpr ::zhc::tuya::TuyaDatapointMap kDpMapWrap{ kDpMap, sizeof(kDpMap) / sizeof(kDpMap[0]) };

constexpr FzConverter kFzDp{
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
    .user_config       = &kDpMapWrap,
};
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzIasMotionAlarm,   // occupancy + tamper + battery_low
    &::zhc::generic::kFzBattery,
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp,
};
constexpr TzConverter kTzDp{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kDpMapWrap,
};
const TzConverter* const kTz[] = { &kTzDp };

constexpr const char* kSensitivityOpts[] = { "low", "medium", "high" };
constexpr const char* kKeepTimeOpts[]    = { "10", "30", "60", "120" };

constexpr Expose kExp[] = {
    { "occupancy",   ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Motion detected",     nullptr,           0 },
    { "tamper",      ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Tamper detected",     nullptr,           0 },
    { "battery_low", ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Battery low",         nullptr,           0 },
    { "battery",     ExposeType::Numeric, ::zhc::Access::State,    "%",     "Battery percentage",  nullptr,           0 },
    { "sensitivity", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "PIR sensitivity (refresh and update only while active)", kSensitivityOpts, 3 },
    { "keep_time",   ExposeType::Enum,    ::zhc::Access::StateSet, "s",     "PIR keep time (refresh and update only while active)",   kKeepTimeOpts,    4 },
};

constexpr const char* kM[] = { "AY-204Z" };
constexpr const char* kN[] = { "AOYAN", "AOYAN ", "AOYAN  " };
}  // namespace

extern const PreparedDefinition kDef_AY_204Z{
    .zigbee_models=kM,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="AY-204Z",.vendor="AOYAN",
    .meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
