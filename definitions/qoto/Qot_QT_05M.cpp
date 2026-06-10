// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Qoto QT-05M — solar powered garden watering timer
// (TS0601 / _TZE200_arge1ptm, _TZE200_anv5ujhv, _TZE200_xlppj4f5).
//
// The auto extract ported this Tuya-MCU (0xEF00 manuSpecificTuya) watering
// timer as a bare kFzBattery + kFzOnOff stub, wiring ZCL clusters
// 0x0001/0x0006 the device never speaks and dropping every one of z2m's
// watering datapoints — leaving only a dead state/battery/voltage surface.
// z2m decodes everything through legacy.fromZigbee.watering_timer, a 0xEF00
// DP stream (dataPoints.wateringTimer):
//   DP 2   valve_state_auto_shutdown   raw % (divisor 1)
//   DP 3   water_flow                  raw % (divisor 1)
//   DP 11  shutdown_timer              raw s (divisor 1)
//   DP 101 remaining_watering_time     raw s (divisor 1)
//   DP 102 valve_state                 raw % (divisor 1)
//   DP 107 last_watering_duration      raw s (divisor 1)
//   DP 110 battery                     raw % (divisor 1)
// SUSPECT (valve/battery/duration/countdown float divisor or per-DP scale)
// confirmed FALSE (giex precedent): z2m's legacy watering_timer converter
// passes every value DP straight through with NO scaling — each
// `getDataValue` result is assigned to the output key verbatim. So every DP
// decodes raw (Int), divisor 1. There is no water_consumed DP on this model.
//
// z2m wires legacy.tz.valve_state (DP 102), legacy.tz.shutdown_timer (DP 11)
// and legacy.tz.valve_state_auto_shutdown (DP 2) as toZigbee — surfaced here
// via tz_tuya_datapoints over the same DP map (host-tested encode; on-device
// write behaviour unverified without hardware). In z2m the DP 2 decode also
// mirrors its value onto valve_state; that mirror is cosmetic — the firmware
// shadow holds valve_state and valve_state_auto_shutdown independently, so we
// keep one DP -> one key (DP 102 -> valve_state, DP 2 ->
// valve_state_auto_shutdown).
//
// z2m-source: devices/qoto.ts #QT-05M + lib/legacy.ts watering_timer.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::qoto {
namespace {

// Every watering datapoint is a plain Numeric pass-through (divisor 1).
constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_QT_05M[] = {
    { 2,   "valve_state_auto_shutdown", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3,   "water_flow",                ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 11,  "shutdown_timer",            ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 101, "remaining_watering_time",   ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 102, "valve_state",               ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 107, "last_watering_duration",    ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 110, "battery",                   ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_QT_05M{
    kEntries_QT_05M, sizeof(kEntries_QT_05M)/sizeof(kEntries_QT_05M[0]) };

constexpr FzConverter kFzDp_QT_05M{
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
    .user_config       = &kMap_QT_05M,
};
const FzConverter* const kFz_QT_05M[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_QT_05M,
};
constexpr TzConverter kTzDp_QT_05M{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_QT_05M,
};
const TzConverter* const kTz_QT_05M[] = { &kTzDp_QT_05M };

constexpr const char* kModels_QT_05M[] = { "TS0601" };
constexpr const char* kManus_QT_05M[]  = {
    "_TZE200_arge1ptm", "_TZE200_anv5ujhv", "_TZE200_xlppj4f5" };

// z2m exposes: water_flow / last_watering_duration / remaining_watering_time
// are STATE; valve_state / valve_state_auto_shutdown / shutdown_timer are
// STATE_SET (writable); battery is STATE.
constexpr Expose kExp_QT_05M[] = {
    { "water_flow",                ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0 },
    { "last_watering_duration",    ExposeType::Numeric, ::zhc::Access::State,    "sec", nullptr, nullptr, 0 },
    { "remaining_watering_time",   ExposeType::Numeric, ::zhc::Access::State,    "sec", nullptr, nullptr, 0 },
    { "valve_state",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",   nullptr, nullptr, 0 },
    { "valve_state_auto_shutdown", ExposeType::Numeric, ::zhc::Access::StateSet, "%",   nullptr, nullptr, 0 },
    { "shutdown_timer",            ExposeType::Numeric, ::zhc::Access::StateSet, "sec", nullptr, nullptr, 0 },
    { "battery",                   ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_QT_05M[] = {
    { 1, 0xEF00 },
};
}  // namespace

extern const PreparedDefinition kDef_QT_05M{
    .zigbee_models=kModels_QT_05M, .zigbee_models_count=sizeof(kModels_QT_05M)/sizeof(kModels_QT_05M[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_QT_05M, .manufacturer_names_count=sizeof(kManus_QT_05M)/sizeof(kManus_QT_05M[0]),
    .model="QT-05M", .vendor="Qoto",
    .meta=nullptr, .exposes=kExp_QT_05M, .exposes_count=sizeof(kExp_QT_05M)/sizeof(kExp_QT_05M[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_QT_05M, .from_zigbee_count=sizeof(kFz_QT_05M)/sizeof(kFz_QT_05M[0]),
    .to_zigbee=kTz_QT_05M, .to_zigbee_count=sizeof(kTz_QT_05M)/sizeof(kTz_QT_05M[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings                 = kBind_QT_05M,
    .bindings_count           = sizeof(kBind_QT_05M)/sizeof(kBind_QT_05M[0]),
};

}  // namespace zhc::devices::qoto
