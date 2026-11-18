// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya `_TZ3000_a8mhmsol` TS0002 2-gang switch module (z2m v26.133.0).
// Before this def it fell to the model-only tuya/TS0002.cpp (on/off only).
//
// z2m tuyaBase() + tuyaOnOff({switchType, backlightModeOffOn, indicatorMode,
// onOffCountdown, inchingSwitch, endpoints: [l1, l2]}), endpoint {l1: 1, l2: 2},
// multiEndpointSkip [power_on_behavior]:
//   state_l1 / state_l2          genOnOff 0x0000 on EP1 / EP2
//   countdown_l1 / countdown_l2  genOnOff onTime 0x4001 / onWithTimedOff
//   power_on_behavior            genOnOff 0x8002 moesStartUpOnOff, EP1 only and
//                                global for both gangs (this firmware has no
//                                manuSpecificTuya3 powerOnBehavior)
//   switch_type                  0xE001 attr 0xD030 toggle / state / momentary
//   backlight_mode               genOnOff 0x5000 tuyaBacklightSwitch OFF / ON
//   indicator_mode               genOnOff 0x8001 tuyaBacklightMode
// Not ported: inching_control_set (manuSpecificTuya4 base64 composite).
// Reports from EP1 come out `_l1`-suffixed (the dispatcher suffixes every key
// of a mapped endpoint). Configure = magic packet + bind genOnOff EP1, EP2.
//
// z2m-source: tuya.ts #_TZ3000_a8mhmsol.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {
using ::zhc::generic::ZclAttrMap;
using ::zhc::generic::ZclAttrRow;
using ::zhc::generic::ZclWriteLookup;
using ::zhc::generic::ZclWriteSpec;

// z2m tuyaFz.backlight_mode_off_on / tuyaTz.backlight_indicator_mode_2.
constexpr ZclAttrRow kBacklightRows[] = {
    { 0x5000, "backlight_mode", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
};
constexpr ZclAttrMap kBacklightMap{ kBacklightRows, 1 };
constexpr FzConverter kFzBacklight = ::zhc::generic::zcl_attr_fz("genOnOff", &kBacklightMap);
constexpr ZclWriteLookup kBacklightLut[] = { {"OFF", 0}, {"ON", 1} };
constexpr ZclWriteSpec kSpecBacklight{ "backlight_mode", 0x5000, 0x30, 0, kBacklightLut, 2 };
constexpr TzConverter kTzBacklight = ::zhc::generic::zcl_write_tz("genOnOff", 0x0006, &kSpecBacklight);

// z2m tuyaTz.power_on_behavior_1, pinned to EP1 (multiEndpointSkip).
constexpr ZclWriteLookup kPobLut[] = { {"off", 0}, {"on", 1}, {"previous", 2} };
constexpr ZclWriteSpec kSpecPob{ "power_on_behavior", 0x8002, 0x30, 0, kPobLut, 3 };
constexpr TzConverter kTzPob = ::zhc::generic::zcl_write_tz("genOnOff", 0x0006, &kSpecPob, 1);

const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzOnOffCountdown,
    &::zhc::tuya::kFzTuyaPowerOnBehavior,
    &::zhc::tuya::kFzTuyaSwitchType,
    &kFzBacklight,
    &::zhc::tuya::kFzTuyaIndicatorMode,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzOnWithTimedOff,
    &kTzPob,
    &::zhc::tuya::kTzTuyaSwitchType,
    &kTzBacklight,
    &::zhc::tuya::kTzTuyaIndicatorMode,
};

// z2m tuyaExposes.countdown(): 0-43200 s.
constexpr Expose countdown(const char* name) {
    return {name, ExposeType::Numeric, Access::StateSet, "s",
            "Toggle the device after a set duration (one time action)", nullptr, 0,
            ExposeCategory::State, 0, 43200, 1};
}
constexpr const char* kPobOpts[]        = { "off", "previous", "on" };
constexpr const char* kSwitchTypeOpts[] = { "toggle", "state", "momentary" };
constexpr const char* kIndicatorOpts[]  = { "off", "off/on", "on/off", "on" };
constexpr Expose kExposes[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    countdown("countdown_l1"),
    countdown("countdown_l2"),
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kPobOpts, 3, ExposeCategory::Config},
    {"switch_type", ExposeType::Enum, Access::StateSet, nullptr,
     "Type of the switch", kSwitchTypeOpts, 3, ExposeCategory::Config},
    {"backlight_mode", ExposeType::Binary, Access::StateSet, nullptr,
     "Mode of the backlight", nullptr, 0, ExposeCategory::Config},
    {"indicator_mode", ExposeType::Enum, Access::StateSet, nullptr,
     "LED indicator mode", kIndicatorOpts, 4, ExposeCategory::Config},
};

constexpr const char* kModels[] = { "TS0002" };
constexpr const char* kManus[]  = { "_TZ3000_a8mhmsol" };
constexpr BindingSpec kBindings[] = { {1, 0x0006}, {2, 0x0006} };
constexpr ::zhc::EndpointLabel kEndpoints[] = { {"l1", 1}, {"l2", 2} };
}  // namespace

extern const PreparedDefinition kDef_TS0002_a8mhmsol{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="_TZ3000_a8mhmsol", .vendor="Tuya",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .config_steps=::zhc::tuya::kConfigStepsTuyaMagicPacket,
    .config_steps_count=::zhc::tuya::kConfigStepsTuyaMagicPacketCount,
    .endpoint_map=kEndpoints, .endpoint_map_count=sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

}  // namespace zhc::devices::tuya
