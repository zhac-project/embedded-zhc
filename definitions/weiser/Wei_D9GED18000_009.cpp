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
// 30-slot pin code count. SmartCode 10.
// z2m-source: weiser.ts #9GED18000-009.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::weiser {
namespace {
const FzConverter* const kFz_D9GED18000_009[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzLock,
    &::zhc::generic::kFzLockProgrammingEvent,
    &::zhc::generic::kFzLockOperationEvent,
    &::zhc::generic::kFzLockUserStatusResponse,
};
const TzConverter* const kTz_D9GED18000_009[] = {
    &::zhc::generic::kTzLock,
    &::zhc::generic::kTzLockPinCode,
    &::zhc::generic::kTzLockSoundVolume,
    &::zhc::generic::kTzLockAutoRelockTime,
};
constexpr const char* kModels_D9GED18000_009[] = { "SMARTCODE_DEADBOLT_10" };

}  // namespace


// --- Tier-2 expose/binding surface (mirrors kwikset SmartCode) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"lock_state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"pin_code", ExposeType::String, Access::Set, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_user", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_source", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_source_name", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {2, 0x0001},  // genPowerCfg     (z2m endpoint: {default: 2})
    {2, 0x0101},  // closuresDoorLock
};
// --- end block ---

extern const PreparedDefinition kDef_D9GED18000_009{
    .zigbee_models=kModels_D9GED18000_009, .zigbee_models_count=sizeof(kModels_D9GED18000_009)/sizeof(kModels_D9GED18000_009[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="9GED18000-009", .vendor="Weiser",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D9GED18000_009, .from_zigbee_count=sizeof(kFz_D9GED18000_009)/sizeof(kFz_D9GED18000_009[0]),
    .to_zigbee=kTz_D9GED18000_009, .to_zigbee_count=sizeof(kTz_D9GED18000_009)/sizeof(kTz_D9GED18000_009[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
.default_endpoint=2,
};

}  // namespace zhc::devices::weiser
