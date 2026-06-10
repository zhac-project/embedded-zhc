// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Senoro.Win window alarm (TS0601 / _TZE200_ytx9fudw).
//
// Bug fixed:
//   * The auto-port dropped DP 101 `opening_state` entirely (the device's
//     primary channel) and carried two phantom exposes (`state`, `action`)
//     that no DP ever drives. The real z2m exposes are opening_state /
//     alarm / battery. DP 16 `alarm` is a z2m `e.binary` so it decodes as
//     Bool, not Numeric.
//   * A sibling auto-stub (Senoro.Win, same TS0601 / _TZE200_ytx9fudw
//     fingerprint) was misrouted to the generic IAS-zone (ssIasZone 0x0500)
//     emitting bare alarm/tamper/battery_low. z2m has no IAS path for this
//     device — it is a pure Tuya-MCU 0xEF00 DP-stream device. That dup is
//     removed; this Tier-2 def is the single canonical port.
//
// z2m-source: senoro.ts #Senoro.Win — tuyaDatapoints:
//   [101, opening_state, lookup{open:0,closed:1,tilted:2}]
//   [16,  alarm,         raw]   (e.binary STATE_SET)
//   [2,   battery,       raw]
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::senoro {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ytx9fudw_opening_state_dp101[] = {
    { 0, "open" },
    { 1, "closed" },
    { 2, "tilted" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_ytx9fudw[] = {
    { 101, "opening_state", ::zhc::TuyaDpType::Enum, 1,
      kEnum__TZE200_ytx9fudw_opening_state_dp101,
      sizeof(kEnum__TZE200_ytx9fudw_opening_state_dp101) /
          sizeof(kEnum__TZE200_ytx9fudw_opening_state_dp101[0]) },
    { 16, "alarm",   ::zhc::TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 2,  "battery", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_ytx9fudw{
    kEntries__TZE200_ytx9fudw,
    sizeof(kEntries__TZE200_ytx9fudw) / sizeof(kEntries__TZE200_ytx9fudw[0]) };
constexpr FzConverter kFzDp__TZE200_ytx9fudw{
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
    .user_config       = &kMap__TZE200_ytx9fudw,
};
const FzConverter* const kFz__TZE200_ytx9fudw[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_ytx9fudw,
};
constexpr TzConverter kTzDp__TZE200_ytx9fudw{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_ytx9fudw,
};
const TzConverter* const kTz__TZE200_ytx9fudw[] = { &kTzDp__TZE200_ytx9fudw };
constexpr const char* kM__TZE200_ytx9fudw[] = { "TS0601" };
constexpr const char* kN__TZE200_ytx9fudw[] = { "_TZE200_ytx9fudw" };

constexpr const char* kOpeningStateValues[] = { "open", "closed", "tilted" };
}  // namespace

// --- Tier 2 hand-corrected exposes (was: phantom state/action, no opening_state) ---
constexpr Expose kAutoExposes[] = {
    {"opening_state", ExposeType::Enum, Access::State, nullptr, "Opening state",
     kOpeningStateValues, sizeof(kOpeningStateValues) / sizeof(kOpeningStateValues[0])},
    {"alarm", ExposeType::Binary, Access::StateSet, nullptr, "Alarm state", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};

extern const PreparedDefinition kDefSen__TZE200_ytx9fudw{
    .zigbee_models=kM__TZE200_ytx9fudw,.zigbee_models_count=sizeof(kM__TZE200_ytx9fudw)/sizeof(kM__TZE200_ytx9fudw[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_ytx9fudw,.manufacturer_names_count=1,
    .model="Senoro.Win",.vendor="Senoro",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_ytx9fudw,
    .from_zigbee_count=sizeof(kFz__TZE200_ytx9fudw)/sizeof(kFz__TZE200_ytx9fudw[0]),
    .to_zigbee=kTz__TZE200_ytx9fudw,
    .to_zigbee_count=sizeof(kTz__TZE200_ytx9fudw)/sizeof(kTz__TZE200_ytx9fudw[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::senoro
