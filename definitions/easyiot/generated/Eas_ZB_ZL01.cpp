// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Easyiot ZB-ZL01 — auto-generated.
// Smart door lock.
// z2m-source: easyiot.ts #ZB-ZL01.
//
// PARTIAL: battery + voltage decoded via the generic kFzBattery path.
// closuresDoorLock (0x0101) attr 0x0000 is decoded by ZHC's lock fz
// (kFzLock) into "lock_status".
//
// RUNTIME BLOCKER: every set-side action below builds custom ZCL
// command payloads that the generic kTzLock encoder doesn't support
// (length-prefixed PIN buffer, manuf-specific 0x180..0x183 commands,
// composite arg structs). Exposed here for UI but not wired:
//   - unlock_door (length-prefixed UTF-8 PIN)
//   - unlock_door_with_timeout (timeout u16 + PIN)
//   - ephemeral_pin_code (5-arg composite: start/end/userid/valid/pin)
//   - ephemeral_clear_pin_code (userid)
//   - ephemeral_clear_all_pin_code (no args)
// All require a vendor-specific tz helper for the closuresDoorLock
// custom commands defined inline in z2m's `EasyiotDoorLock` cluster
// override (manuf-specific cmd ids 0x10..0x16 + 0xB7..0xB9).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
const FzConverter* const kFz_ZB_ZL01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzLock,
};
constexpr const char* kModels_ZB_ZL01[] = { "ZB-ZL01" };

// TODO(easyiot/ZB-ZL01): vendor-specific closuresDoorLock encoder
// covering the 5 custom commands above.

constexpr Expose kExposes_ZB_ZL01[] = {
    {"lock_status",                ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",                    ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage",                    ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
    {"unlock_door",                ExposeType::String,  Access::Set,   nullptr, nullptr, nullptr, 0},
    // composite payloads flattened — ZHC has no composite expose
    {"unlock_door_with_timeout",   ExposeType::String,  Access::Set,   nullptr, nullptr, nullptr, 0},
    {"ephemeral_pin_code",         ExposeType::String,  Access::Set,   nullptr, nullptr, nullptr, 0},
    {"ephemeral_clear_pin_code",   ExposeType::String,  Access::Set,   nullptr, nullptr, nullptr, 0},
    {"ephemeral_clear_all_pin_code", ExposeType::Binary, Access::Set,  nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_ZL01[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0101},  // closuresDoorLock
};

}  // namespace

extern const PreparedDefinition kDef_ZB_ZL01{
    .zigbee_models=kModels_ZB_ZL01, .zigbee_models_count=sizeof(kModels_ZB_ZL01)/sizeof(kModels_ZB_ZL01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-ZL01", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_ZL01, .exposes_count=sizeof(kExposes_ZB_ZL01)/sizeof(kExposes_ZB_ZL01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB_ZL01, .from_zigbee_count=sizeof(kFz_ZB_ZL01)/sizeof(kFz_ZB_ZL01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_ZL01, .bindings_count=sizeof(kBindings_ZB_ZL01)/sizeof(kBindings_ZB_ZL01[0]),
};

}  // namespace zhc::devices::easyiot
