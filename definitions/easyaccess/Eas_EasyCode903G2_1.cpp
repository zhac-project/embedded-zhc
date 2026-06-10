// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: EasyAccess EasyCode903G2.1 (EasyFinger V2) door lock.
//
// Graduated from generated/. The auto-port carried a phantom genOnOff
// bundle (kFzOnOff + kTzOnOff + a 0x0006 binding) — this device is a
// closuresDoorLock (0x0101), it has no on/off cluster. It also dropped
// the entire `easycode_action` event stream (keypad / RFID / zigbee
// unlock) and the sound_volume / auto_relock config. Fixed to match
// z2m exactly.
//
// z2m-source: easyaccess.ts #EasyCode903G2.1
//   fromZigbee: [fz.lock, fz.easycode_action, fz.battery]
//   toZigbee:   [tz.lock, tz.easycode_auto_relock, tz.lock_sound_volume]
//   exposes:    lock, battery, sound_volume,
//               action(zigbee_unlock/lock/rfid_unlock/keypad_unlock),
//               auto_relock
//   configure:  bind ep11 closuresDoorLock + genPowerCfg
#include "definitions/_generic/_shared.hpp"
#include "definitions/easyaccess/_shared.hpp"

namespace zhc::devices::easyaccess {
namespace {
const FzConverter* const kFz_EasyCode903G2_1[] = {
    &::zhc::generic::kFzLock,            // closuresDoorLock lockState -> lock_state
    &kFzEasycodeAction,                  // RAW unlock event -> action
    &kFzEasycodeSoundVolume,             // soundVolume 0x0024 -> sound_volume
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_EasyCode903G2_1[] = {
    &::zhc::generic::kTzLock,            // "state" -> closuresDoorLock lock/unlock
    &kTzEasycodeSoundVolume,             // "sound_volume"
    &kTzEasycodeAutoRelock,              // "auto_relock"
};
constexpr const char* kModels_EasyCode903G2_1[] = { "EasyCode903G2.1" };

constexpr const char* kActionValues[] = {
    "zigbee_unlock", "lock", "rfid_unlock", "keypad_unlock",
};
constexpr const char* kSoundVolumeValues[] = {
    "silent_mode", "low_volume", "high_volume",
};

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"lock_state", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"sound_volume", ExposeType::Enum, Access::StateSet, nullptr, nullptr,
     kSoundVolumeValues, sizeof(kSoundVolumeValues)/sizeof(kSoundVolumeValues[0])},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActionValues, sizeof(kActionValues)/sizeof(kActionValues[0])},
    {"auto_relock", ExposeType::Binary, Access::StateSet, nullptr,
     "Auto relock after 7 seconds.", nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {11, 0x0101},  // closuresDoorLock
    {11, 0x0001},  // genPowerCfg
};


constexpr WhiteLabel kWhiteLabels_EasyCode903G2_1[] = {
    {"Datek Wireless","EasyCode903G2.1"},
};
extern const PreparedDefinition kDef_EasyCode903G2_1{
    .zigbee_models=kModels_EasyCode903G2_1, .zigbee_models_count=sizeof(kModels_EasyCode903G2_1)/sizeof(kModels_EasyCode903G2_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EasyCode903G2.1", .vendor="Easyaccess",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_EasyCode903G2_1, .white_labels_count=sizeof(kWhiteLabels_EasyCode903G2_1)/sizeof(kWhiteLabels_EasyCode903G2_1[0]),
    .from_zigbee=kFz_EasyCode903G2_1, .from_zigbee_count=sizeof(kFz_EasyCode903G2_1)/sizeof(kFz_EasyCode903G2_1[0]),
    .to_zigbee=kTz_EasyCode903G2_1, .to_zigbee_count=sizeof(kTz_EasyCode903G2_1)/sizeof(kTz_EasyCode903G2_1[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::easyaccess
