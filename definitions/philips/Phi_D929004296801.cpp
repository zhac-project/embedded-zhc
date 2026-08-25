// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 929004296801 (z2m v26.97.0 parity).
// Hue wired wall switch module 2 channel
// z2m-source: philips.ts #929004296801.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D929004296801[] = { "LOM014" };

// z2m: `m.deviceEndpoints({endpoints: {l1: 11, l2: 12}}) +
// philips.m.onOff({endpointNames: ["l1", "l2"], powerOnBehavior: true})`.
// Two relays on endpoints 11 and 12; `powerOnBehavior` adds the standard
// genOnOff startUpOnOff config expose, suffixed per endpoint by the
// runtime.
constexpr ::zhc::EndpointLabel kEndpoints_D929004296801[] = {
    {"l1", 11}, {"l2", 12},
};

constexpr const char* kPowerOnBehaviourValues[] = {
    "off", "on", "toggle", "previous",
};

constexpr Expose kExposes_D929004296801[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior_l1", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kPowerOnBehaviourValues,
     sizeof(kPowerOnBehaviourValues)/sizeof(kPowerOnBehaviourValues[0]),
     ExposeCategory::Config},
    {"power_on_behavior_l2", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kPowerOnBehaviourValues,
     sizeof(kPowerOnBehaviourValues)/sizeof(kPowerOnBehaviourValues[0]),
     ExposeCategory::Config},
};

const FzConverter* const kFz_D929004296801[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior,
};
const TzConverter* const kTz_D929004296801[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};

constexpr BindingSpec kBindings_D929004296801[] = {
    {11, 0x0006},   // genOnOff
    {12, 0x0006},   // genOnOff
};

}  // namespace

extern const PreparedDefinition kDef_D929004296801{
    .zigbee_models           = kModels_D929004296801,
    .zigbee_models_count     = sizeof(kModels_D929004296801)/sizeof(kModels_D929004296801[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "929004296801",
    .vendor                  = "Philips",
    .meta                    = nullptr,
    .exposes                 = kExposes_D929004296801,
    .exposes_count           = sizeof(kExposes_D929004296801)/sizeof(kExposes_D929004296801[0]),
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFz_D929004296801,
    .from_zigbee_count       = sizeof(kFz_D929004296801)/sizeof(kFz_D929004296801[0]),
    .to_zigbee               = kTz_D929004296801,
    .to_zigbee_count         = sizeof(kTz_D929004296801)/sizeof(kTz_D929004296801[0]),
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindings_D929004296801,
    .bindings_count          = sizeof(kBindings_D929004296801)/sizeof(kBindings_D929004296801[0]),
    .endpoint_map            = kEndpoints_D929004296801,
    .endpoint_map_count      = sizeof(kEndpoints_D929004296801)/sizeof(kEndpoints_D929004296801[0]),
};

}  // namespace zhc::devices::philips
