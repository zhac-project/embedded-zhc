// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Danalock V3-BTZB/V3-BTZBE — hand-upgraded for full z2m parity.
// BT/ZB smartlock. The generated port wired only kFzLock + kFzBattery and
// dropped the lock action stream + PIN write path. z2m wires
// fz.lock_operation_event + fz.lock_programming_event (the `action` /
// `action_user` / `action_source` / `action_source_name` stream) and
// tz.pincode_lock (the `pin_code` write). Both decoders + the PIN encoder
// already exist as the generic closuresDoorLock helpers used cross-vendor
// by datek 0402946 / onesti / kwikset — wired here. The
// fz.lock_pin_code_response (commandGetPinCodeRsp) read-back has no generic
// decoder yet → deferred (INFRA), as is the userstatus read.
// z2m-source: danalock.ts #V3-BTZB/V3-BTZBE.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::danalock {
namespace {
const FzConverter* const kFz_V3_BTZB_V3_BTZBE[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzLock,
    &::zhc::generic::kFzLockOperationEvent,
    &::zhc::generic::kFzLockProgrammingEvent,
};
const TzConverter* const kTz_V3_BTZB_V3_BTZBE[] = {
    &::zhc::generic::kTzLock,
    &::zhc::generic::kTzLockPinCode,
};
constexpr const char* kModels_V3_BTZB_V3_BTZBE[] = { "V3-BTZB", "V3-BTZBE" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"lock_state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // e.lock_action() / e.lock_action_source_name() / e.lock_action_user()
    // — the fz.lock_operation_event + fz.lock_programming_event stream.
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_user", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_source", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_source_name", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    // e.pincode() — the tz.pincode_lock write path (kTzLockPinCode).
    {"pin_code", ExposeType::String, Access::Set, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0101},  // closuresDoorLock
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_V3_BTZB_V3_BTZBE{
    .zigbee_models=kModels_V3_BTZB_V3_BTZBE, .zigbee_models_count=sizeof(kModels_V3_BTZB_V3_BTZBE)/sizeof(kModels_V3_BTZB_V3_BTZBE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="V3-BTZB/V3-BTZBE", .vendor="Danalock",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_V3_BTZB_V3_BTZBE, .from_zigbee_count=sizeof(kFz_V3_BTZB_V3_BTZBE)/sizeof(kFz_V3_BTZB_V3_BTZBE[0]),
    .to_zigbee=kTz_V3_BTZB_V3_BTZBE, .to_zigbee_count=sizeof(kTz_V3_BTZB_V3_BTZBE)/sizeof(kTz_V3_BTZB_V3_BTZBE[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::danalock
