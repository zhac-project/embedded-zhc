// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya _TZ3000_mw1pqqqt / _TZ3210_n0wbkysi — 3 channel USB switch.
// Hand-upgraded from the generated 1-endpoint stub for z2m v26.133.0, which
// adds `_TZ3210_n0wbkysi` (#13761).
//
// z2m tuyaBase() + tuyaOnOff({onOffCountdown, endpoints: [l1, l2, l3]}),
// endpoint {l1: 1, l2: 2, l3: 3}:
//   state_l1 … state_l3          genOnOff 0x0000 on EP1 … EP3
//   countdown_l1 … countdown_l3  genOnOff onTime 0x4001 / onWithTimedOff
//   power_on_behavior            genOnOff 0x8002 moesStartUpOnOff (tuyaOnOff's
//                                default power_on_behavior_1); a report comes
//                                out power_on_behavior_l<n>, as z2m's
//                                multiEndpoint postfix names it
// Configure = tuyaBase's magic packet (upstream has no configure of its own;
// the generator's genOnOff bind on EP1 is kept).
//
// z2m-source: tuya.ts #_TZ3000_mw1pqqqt.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {
const FzConverter* const kFz__TZ3000_mw1pqqqt[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzOnOffCountdown,
    &::zhc::tuya::kFzTuyaPowerOnBehavior,
};
const TzConverter* const kTz__TZ3000_mw1pqqqt[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzOnWithTimedOff,
    &::zhc::tuya::kTzTuyaPowerOnBehavior,
};
constexpr const char* kModels__TZ3000_mw1pqqqt[] = { "TS0003" };
constexpr const char* kManus__TZ3000_mw1pqqqt[] = { "_TZ3000_mw1pqqqt", "_TZ3210_n0wbkysi" };

// z2m tuyaExposes.countdown(): 0-43200 s.
constexpr Expose countdown(const char* name) {
    return {name, ExposeType::Numeric, Access::StateSet, "s",
            "Toggle the device after a set duration (one time action)", nullptr, 0,
            ExposeCategory::State, 0, 43200, 1};
}
constexpr const char* kPobOpts[] = { "off", "previous", "on" };
constexpr Expose kExposes[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    countdown("countdown_l1"),
    countdown("countdown_l2"),
    countdown("countdown_l3"),
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kPobOpts, 3, ExposeCategory::Config},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0006},
};
constexpr ::zhc::EndpointLabel kEndpoints[] = { {"l1", 1}, {"l2", 2}, {"l3", 3} };
}  // namespace

extern const PreparedDefinition kDef__TZ3000_mw1pqqqt{
    .zigbee_models=kModels__TZ3000_mw1pqqqt, .zigbee_models_count=sizeof(kModels__TZ3000_mw1pqqqt)/sizeof(kModels__TZ3000_mw1pqqqt[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus__TZ3000_mw1pqqqt, .manufacturer_names_count=sizeof(kManus__TZ3000_mw1pqqqt)/sizeof(kManus__TZ3000_mw1pqqqt[0]),
    .model="_TZ3000_mw1pqqqt", .vendor="Tuya",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz__TZ3000_mw1pqqqt, .from_zigbee_count=sizeof(kFz__TZ3000_mw1pqqqt)/sizeof(kFz__TZ3000_mw1pqqqt[0]),
    .to_zigbee=kTz__TZ3000_mw1pqqqt, .to_zigbee_count=sizeof(kTz__TZ3000_mw1pqqqt)/sizeof(kTz__TZ3000_mw1pqqqt[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .config_steps=::zhc::tuya::kConfigStepsTuyaMagicPacket,
    .config_steps_count=::zhc::tuya::kConfigStepsTuyaMagicPacketCount,
    .endpoint_map=kEndpoints, .endpoint_map_count=sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

}  // namespace zhc::devices::tuya
