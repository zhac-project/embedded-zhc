// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Woox R7060 — smart garden irrigation control (TS0101 Tuya MCU).
// z2m-source: woox.ts #R7060 (legacy.fromZigbee.woox_R7060 DP-stream).
//   DP 102 (wooxSwitch): u32, value == 2 → state OFF, else → ON.
//   DP 101: u32, battery %.
// Note: state DP semantics are inverted relative to a normal bool; we
// flag the entry with kTuyaDpFlagInvertBool so 2 maps to OFF.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::woox {
namespace {

constexpr ::zhc::tuya::TuyaDpMapEntry kDps_R7060[] = {
    // dp_id, key,        type,                    divisor, enum_table, count, flags
    { 102,   "state",     ::zhc::TuyaDpType::Bool,    1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertBool },
    { 101,   "battery",   ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kDpMap_R7060{
    kDps_R7060, sizeof(kDps_R7060)/sizeof(kDps_R7060[0])
};

constexpr FzConverter kFzDp_R7060{
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
    .user_config       = &kDpMap_R7060,
};
constexpr TzConverter kTzDp_R7060{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kDpMap_R7060,
};

const FzConverter* const kFz_R7060[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::tuya::kFzIgnoreTuyaSetTime,
    &kFzDp_R7060,
};
const TzConverter* const kTz_R7060[] = { &kTzDp_R7060 };

constexpr const char* kModels_R7060[] = { "TS0101" };
constexpr const char* kManus_R7060[] = { "_TZ3210_eymunffl" };

constexpr Expose kExp_R7060[] = {
    {"state",   ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_R7060[] = {
    {1, 0xEF00},  // manuSpecificTuya
};

}  // namespace

extern const PreparedDefinition kDef_R7060{
    .zigbee_models=kModels_R7060, .zigbee_models_count=sizeof(kModels_R7060)/sizeof(kModels_R7060[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_R7060, .manufacturer_names_count=sizeof(kManus_R7060)/sizeof(kManus_R7060[0]),
    .model="R7060", .vendor="Woox",
    .meta=nullptr, .exposes=kExp_R7060, .exposes_count=sizeof(kExp_R7060)/sizeof(kExp_R7060[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_R7060, .from_zigbee_count=sizeof(kFz_R7060)/sizeof(kFz_R7060[0]),
    .to_zigbee=kTz_R7060,   .to_zigbee_count=sizeof(kTz_R7060)/sizeof(kTz_R7060[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_R7060,
    .bindings_count=sizeof(kBind_R7060)/sizeof(kBind_R7060[0]),
};

}  // namespace zhc::devices::woox
