// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: FrankEver FK_V02 — Tuya-DP water valve (graduated from generated/).
//
// FrankEver "Zigbee smart water valve" (TS0601 / _TZE200_*). z2m v26.114.0
// (#13285) moved it off the legacy frankever_valve converters onto a plain
// datapoint table, renaming and rescaling on the way:
//
//   DP 1   -> state               Bool     ON/OFF
//   DP 9   -> countdown           Numeric  seconds (was `timer`, minutes = /60)
//   DP 27  -> power_off_state     Enum     off / on / maintain   (new)
//   DP 101 -> set_valve_position  Numeric  0..100 %, steps of 10 (was `threshold`)
//
// Rules on `threshold` / `timer` need the new keys; `countdown` is in seconds.
// z2m-source: frankever.ts #FK_V02.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::frankever {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kPowerOff_FK_V02[] = { {0,"off"}, {1,"on"}, {2,"maintain"} };
constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_FK_V02[] = {
    { 1,   "state",              ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 9,   "countdown",          ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 27,  "power_off_state",    ::zhc::TuyaDpType::Enum,    1, kPowerOff_FK_V02, 3, 0 },
    { 101, "set_valve_position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
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


// --- exposes ---
// Field order: {name, type, access, unit, description, enum_values, enum_count}.
constexpr const char* kPowerOffOpts_FK_V02[] = { "off", "on", "maintain" };
constexpr Expose kAutoExposes[] = {
    {"state",              ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_off_state",    ExposeType::Enum,    Access::StateSet, nullptr, "Power-off status behavior", kPowerOffOpts_FK_V02, 3},
    {"set_valve_position", ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 10},
    {"countdown",          ExposeType::Numeric, Access::StateSet, "s",     "Countdown timer in seconds", nullptr, 0, ExposeCategory::State, 0, 43200, 0},
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
