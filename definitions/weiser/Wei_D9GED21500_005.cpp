// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2 (2026-06-11): full lock channel restored. The generated def was a
// battery-only stub (kFzBattery + battery/voltage exposes, binding {1,0x0001})
// that DROPPED the entire doorlock channel and bound the wrong endpoint.
// z2m `m.lock({pinCodeCount:30, readPinCodeOnProgrammingEvent:true})` +
// `m.battery()` with `endpoint: () => ({default: 2})` puts closuresDoorLock
// (0x0101) + genPowerCfg (0x0001) on endpoint 2. Wired the generic lock-PIN
// runtime (lock_state + battery + operation/programming event action stream +
// PIN/sound/auto-relock encoders), re-endpointed bindings ep1->ep2, and added
// .default_endpoint=2 — mirrors the sibling Kwikset SmartCode family (same
// hardware; kwikset 99140-031 even lists the _W3 variants of this model).
// 30-slot pin code count. SmartCode 10 Touch.
// z2m-source: weiser.ts #9GED21500-005.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::weiser {
namespace {
const FzConverter* const kFz_D9GED21500_005[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzLock,
    &::zhc::generic::kFzLockProgrammingEvent,
    &::zhc::generic::kFzLockOperationEvent,
    &::zhc::generic::kFzLockUserStatusResponse,
};
const TzConverter* const kTz_D9GED21500_005[] = {
    &::zhc::generic::kTzLock,
    &::zhc::generic::kTzLockPinCode,
    &::zhc::generic::kTzLockSoundVolume,
    &::zhc::generic::kTzLockAutoRelockTime,
};
constexpr const char* kModels_D9GED21500_005[] = { "SMARTCODE_DEADBOLT_10T" };

}  // namespace


// --- Tier-2 expose/binding surface (mirrors kwikset SmartCode) ---
// Lock vocabulary (zigbee2mqtt words): commands on `state`, the reported
// `lock_state`, and the operation-event `action` stream.
static constexpr const char* kLockCmdOpts[]   = {"LOCK", "UNLOCK"};
static constexpr const char* kLockStateOpts[] = {"not_fully_locked", "locked", "unlocked"};
static constexpr const char* kLockActionOpts[] = {"unknown", "lock", "unlock", "lock_failure_invalid_pin_or_id", "lock_failure_invalid_schedule", "unlock_failure_invalid_pin_or_id", "unlock_failure_invalid_schedule", "one_touch_lock", "key_lock", "key_unlock", "auto_lock", "schedule_lock", "schedule_unlock", "manual_lock", "manual_unlock", "non_access_user_event"};
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"state", ExposeType::Enum, Access::Set, nullptr, nullptr, kLockCmdOpts, 2},
    {"lock_state", ExposeType::Enum, Access::State, nullptr, nullptr, kLockStateOpts, 3},
    {"pin_code", ExposeType::String, Access::Set, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kLockActionOpts, 16},
    {"action_user", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_source", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_source_name", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {2, 0x0001},  // genPowerCfg     (z2m endpoint: {default: 2})
    {2, 0x0101},  // closuresDoorLock
};
// --- end block ---

extern const PreparedDefinition kDef_D9GED21500_005{
    .zigbee_models=kModels_D9GED21500_005, .zigbee_models_count=sizeof(kModels_D9GED21500_005)/sizeof(kModels_D9GED21500_005[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="9GED21500-005", .vendor="Weiser",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D9GED21500_005, .from_zigbee_count=sizeof(kFz_D9GED21500_005)/sizeof(kFz_D9GED21500_005[0]),
    .to_zigbee=kTz_D9GED21500_005, .to_zigbee_count=sizeof(kTz_D9GED21500_005)/sizeof(kTz_D9GED21500_005[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
.default_endpoint=2,
};

}  // namespace zhc::devices::weiser
