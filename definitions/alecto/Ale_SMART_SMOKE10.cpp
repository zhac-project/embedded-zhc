// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Alecto SMART-SMOKE10 — Tuya-MCU (0xEF00 manuSpecificTuya) smoke
// detector (TS0601 / _TZE200_qtbrwrfv + _TYST11_qtbrwrfv).
//
// Bug fixed: the auto extract ported this as a generic IAS-zone (ssIasZone
// 0x0500) sensor emitting bare alarm/tamper/battery_low. The device speaks
// NONE of that — z2m decodes it entirely through legacy.fromZigbee.tuya_alecto_smoke,
// a 0xEF00 DP stream. The IAS-vs-DP misroute left every real channel dead.
// Re-wired to fz_tuya_datapoints + tz_tuya_datapoints with the full DP map:
//   DP 1  smoke_state      enum  {0: alarm, 1: normal}
//   DP 2  smoke_value      numeric (raw, no scale)
//   DP 8  self_checking    bool  (STATE_SET — trigger a self test)
//   DP 9  checking_result  enum  {0: checking, 1: check_success,
//                                 2: check_failure, 3: others}
//   DP 11 smoke_test       numeric (decoded by z2m but not exposed there)
//   DP 12 lifecycle        bool  (binary)
//   DP 14 battery_state    enum  {0: low, 1: middle, 2: high}
//   DP 15 battery          numeric % (raw, no scale)
//   DP 16 silence          bool  (STATE_SET — silence the alarm)
//
// SUSPECT (wrong scale on smoke_value / battery) confirmed FALSE: z2m's
// legacy converter passes both straight through with NO divisor. The
// self_checking / silence write DPs are the alarm-silence / self-test
// controls the IAS stub entirely dropped.
//
// z2m-source: alecto.ts #SMART-SMOKE10 + lib/legacy.ts fz/tz.tuya_alecto_smoke.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::alecto {
namespace {

// z2m: smoke_state = {0: "alarm", 1: "normal"}[value]
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_SMOKE10_smoke_state_dp1[] = {
    { 0, "alarm" },
    { 1, "normal" },
};
// z2m: checking_result = {0: "checking", 1: "check_success",
//                         2: "check_failure", 3: "others"}[value]
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_SMOKE10_checking_result_dp9[] = {
    { 0, "checking" },
    { 1, "check_success" },
    { 2, "check_failure" },
    { 3, "others" },
};
// z2m: battery_state = {0: "low", 1: "middle", 2: "high"}[value]
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_SMOKE10_battery_state_dp14[] = {
    { 0, "low" },
    { 1, "middle" },
    { 2, "high" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_SMOKE10[] = {
    { 1,  "smoke_state",     ::zhc::TuyaDpType::Enum,    1, kEnum_SMOKE10_smoke_state_dp1,     sizeof(kEnum_SMOKE10_smoke_state_dp1)/sizeof(kEnum_SMOKE10_smoke_state_dp1[0]),         0 },
    { 2,  "smoke_value",     ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 8,  "self_checking",   ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 9,  "checking_result", ::zhc::TuyaDpType::Enum,    1, kEnum_SMOKE10_checking_result_dp9, sizeof(kEnum_SMOKE10_checking_result_dp9)/sizeof(kEnum_SMOKE10_checking_result_dp9[0]), 0 },
    { 11, "smoke_test",      ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 12, "lifecycle",       ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 14, "battery_state",   ::zhc::TuyaDpType::Enum,    1, kEnum_SMOKE10_battery_state_dp14,  sizeof(kEnum_SMOKE10_battery_state_dp14)/sizeof(kEnum_SMOKE10_battery_state_dp14[0]),   0 },
    { 15, "battery",         ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 16, "silence",         ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_SMOKE10{
    kEntries_SMOKE10, sizeof(kEntries_SMOKE10)/sizeof(kEntries_SMOKE10[0]) };

constexpr FzConverter kFzDp_SMOKE10{
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
    .user_config       = &kMap_SMOKE10,
};
const FzConverter* const kFz_SMART_SMOKE10[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_SMOKE10,
};
constexpr TzConverter kTzDp_SMOKE10{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_SMOKE10,
};
const TzConverter* const kTz_SMART_SMOKE10[] = { &kTzDp_SMOKE10 };

constexpr const char* kModels_SMART_SMOKE10[] = { "tbrwrfv", "TS0601" };
constexpr const char* kManus_SMART_SMOKE10[] = { "_TYST11_qtbrwrfv", "_TZE200_qtbrwrfv" };

// z2m exposes (#SMART-SMOKE10). self_checking + silence are STATE_SET writes.
constexpr Expose kExp_SMART_SMOKE10[] = {
    { "smoke_state",     ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "battery_state",   ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "checking_result", ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "smoke_value",     ExposeType::Numeric, ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "battery",         ExposeType::Numeric, ::zhc::Access::State,    "%",     nullptr, nullptr, 0 },
    { "lifecycle",       ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "self_checking",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "silence",         ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_SMART_SMOKE10[] = {
    { 1, 0xEF00 },
};
}  // namespace

extern const PreparedDefinition kDef_SMART_SMOKE10{
    .zigbee_models=kModels_SMART_SMOKE10, .zigbee_models_count=sizeof(kModels_SMART_SMOKE10)/sizeof(kModels_SMART_SMOKE10[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SMART_SMOKE10, .manufacturer_names_count=sizeof(kManus_SMART_SMOKE10)/sizeof(kManus_SMART_SMOKE10[0]),
    .model="SMART-SMOKE10", .vendor="Alecto",
    .meta=nullptr, .exposes=kExp_SMART_SMOKE10, .exposes_count=sizeof(kExp_SMART_SMOKE10)/sizeof(kExp_SMART_SMOKE10[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SMART_SMOKE10, .from_zigbee_count=sizeof(kFz_SMART_SMOKE10)/sizeof(kFz_SMART_SMOKE10[0]),
    .to_zigbee=kTz_SMART_SMOKE10, .to_zigbee_count=sizeof(kTz_SMART_SMOKE10)/sizeof(kTz_SMART_SMOKE10[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBind_SMART_SMOKE10, .bindings_count=sizeof(kBind_SMART_SMOKE10)/sizeof(kBind_SMART_SMOKE10[0]),
};

}  // namespace zhc::devices::alecto
