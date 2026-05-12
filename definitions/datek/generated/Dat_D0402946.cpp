// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Datek 0402946 — hand-upgraded for full z2m parity.
// ID Lock 150 / 202 module — generic lock + battery + the manuSpec
//                            ID-lock attribute family (mfg=0x1337) +
//                            standard sound_volume write + PIN-code
//                            runtime via _generic kTzLockPinCode +
//                            kFzLockProgrammingEvent + kFzLockOperationEvent
//                            (landed 2026-04-28o).
// z2m-source: datek.ts #0402946.
#include "definitions/_generic/_shared.hpp"
#include "definitions/datek/_shared.hpp"

namespace zhc::devices::datek {
namespace {
const FzConverter* const kFz_D0402946[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzLock,
    &::zhc::generic::kFzLockProgrammingEvent,
    &::zhc::generic::kFzLockOperationEvent,
    &::zhc::datek::kFzIdlock,
};
const TzConverter* const kTz_D0402946[] = {
    &::zhc::generic::kTzLock,
    &::zhc::generic::kTzLockPinCode,
    &::zhc::datek::kTzLockSoundVolume,
    &::zhc::datek::kTzMasterPinMode,
    &::zhc::datek::kTzRfidEnable,
    &::zhc::datek::kTzServiceMode,
    &::zhc::datek::kTzLockMode,
    &::zhc::datek::kTzRelockEnabled,
};
constexpr const char* kModels_D0402946[] = { "ID Lock 150", "ID Lock 202" };

}  // namespace


// --- hand-curated exposes/bindings (z2m parity) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"lock_state", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"door_state", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"sound_volume", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"master_pin_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"rfid_enable", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"service_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"lock_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"relock_enabled", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"pin_code", ExposeType::String, Access::Set, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_user", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_source", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"action_source_name", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0101},  // closuresDoorLock
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_D0402946{
    .zigbee_models=kModels_D0402946, .zigbee_models_count=sizeof(kModels_D0402946)/sizeof(kModels_D0402946[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="0402946", .vendor="Datek",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D0402946, .from_zigbee_count=sizeof(kFz_D0402946)/sizeof(kFz_D0402946[0]),
    .to_zigbee=kTz_D0402946, .to_zigbee_count=sizeof(kTz_D0402946)/sizeof(kTz_D0402946[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::datek
