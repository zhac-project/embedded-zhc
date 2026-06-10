// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: FrankEver FK_V02 — Tuya-DP water valve (graduated from generated/).
//
// FrankEver "Zigbee smart water valve" (TS0601 / _TZE200_*). The generator
// misrouted this Tuya-MCU device to bare genOnOff (cluster 0x0006) and dropped
// the threshold + timer channels — it only knew the `state` expose. z2m decodes
// it entirely over the 0xEF00 DP stream via `legacy.fz.frankever_valve`:
//
//   DP 1   (state)             -> state     Bool     ON/OFF
//   DP 101 (frankEverTreshold) -> threshold Numeric  raw (0..100, multiple of 10)
//   DP 9   (frankEverTimer)    -> timer     Numeric  value / 60  (seconds -> minutes)
//
// Write path mirrors legacy.tz.tuya_switch_state / frankever_threshold /
// frankever_timer — all three go out as DP setData on 0xEF00. z2m's tz encoders
// clamp/quantise the input, but the wire value is the same raw int (threshold)
// / value*60 (timer) the generic tz_tuya_datapoints emits with divisor 1 / 60.
//
// z2m-source: frankever.ts #FK_V02 + lib/legacy.ts fz.frankever_valve /
//             tz.tuya_switch_state / tz.frankever_threshold / tz.frankever_timer.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::frankever {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_FK_V02[] = {
    // DP 1   -> state     (Bool)            z2m: value ? "ON" : "OFF"
    { 1,   "state",     ::zhc::TuyaDpType::Bool,    1,  nullptr, 0, 0 },
    // DP 101 -> threshold (Numeric, raw)    z2m: {threshold: value}
    { 101, "threshold", ::zhc::TuyaDpType::Numeric, 1,  nullptr, 0, 0 },
    // DP 9   -> timer     (Numeric, /60)    z2m: {timer: value / 60} (s -> min)
    { 9,   "timer",     ::zhc::TuyaDpType::Numeric, 60, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_FK_V02{
    kEntries_FK_V02, sizeof(kEntries_FK_V02) / sizeof(kEntries_FK_V02[0]) };

constexpr FzConverter kFzDp_FK_V02{
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
    .user_config       = &kMap_FK_V02,
};
const FzConverter* const kFz_FK_V02[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_FK_V02,
};

constexpr TzConverter kTzDp_FK_V02{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_FK_V02,
};
const TzConverter* const kTz_FK_V02[] = { &kTzDp_FK_V02 };

constexpr const char* kModels_FK_V02[] = { "TS0601" };
constexpr const char* kManus_FK_V02[] = { "_TZE200_wt9agwf3", "_TZE200_5uodvhgc", "_TZE200_1n2zev06" };
}  // namespace


// --- exposes (state + threshold + timer, all StateSet) ---
// Field order: {name, type, access, unit, description, enum_values, enum_count}.
constexpr Expose kAutoExposes[] = {
    {"state",     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"threshold", ExposeType::Numeric, Access::StateSet, "%",     "Valve open percentage (multiple of 10)", nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"timer",     ExposeType::Numeric, Access::StateSet, "min",   "Countdown timer in minutes",             nullptr, 0, ExposeCategory::State, 0, 600, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end ---

extern const PreparedDefinition kDef_FK_V02{
    .zigbee_models=kModels_FK_V02, .zigbee_models_count=sizeof(kModels_FK_V02)/sizeof(kModels_FK_V02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_FK_V02, .manufacturer_names_count=sizeof(kManus_FK_V02)/sizeof(kManus_FK_V02[0]),
    .model="FK_V02", .vendor="FrankEver",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_FK_V02, .from_zigbee_count=sizeof(kFz_FK_V02)/sizeof(kFz_FK_V02[0]),
    .to_zigbee=kTz_FK_V02, .to_zigbee_count=sizeof(kTz_FK_V02)/sizeof(kTz_FK_V02[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::frankever
