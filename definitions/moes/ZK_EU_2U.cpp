// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Moes ZK-EU-2U dual USB wireless socket plug (graduated from
// generated/Moe_ZK_EU_2U.cpp).
//
// z2m m.onOff({endpointNames: [l1, l2]}): state_l1 / state_l2 and
// power_on_behavior_l1 / _l2 on the standard genOnOff 0x4003 startUpOnOff
// {0 off, 1 on, 2 toggle, 255 previous}. Configure, per port: bind genOnOff,
// onOff reporting (min 0, max 65000, change 1), read onOff + startUpOnOff.
// configureSetPowerSourceWhenUnknown (only fills an unknown power source) is
// not ported.
//
// Not mirrored: z2m's `l2: hasEndpoint2 ? 2 : 7`. The endpoint map, bindings
// and reports are static here, and Tz routing takes the first label match,
// so a unit with its second port on EP7 would need a per-device endpoint
// probe the library does not have.
//
// z2m-source: moes.ts #ZK-EU-2U.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::moes {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels[] = { "TS0112" };
constexpr const char* kPowerOnValues[] = { "off", "on", "toggle", "previous" };

constexpr Expose kExposes[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior_l1", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kPowerOnValues, 4, ExposeCategory::Config},
    {"power_on_behavior_l2", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kPowerOnValues, 4, ExposeCategory::Config},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
};

constexpr ReportingSpec kReports[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},
    {2, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},
};

constexpr std::uint8_t kReadAttrs[] = {
    0x00, 0x00,   // onOff
    0x03, 0x40,   // startUpOnOff (0x4003)
};
constexpr ConfigStep kConfigSteps[] = {
    { ConfigStepOp::Read, 1, 0x0006, 0x00, 0, kReadAttrs, sizeof(kReadAttrs), 0 },
    { ConfigStepOp::Read, 2, 0x0006, 0x00, 0, kReadAttrs, sizeof(kReadAttrs), 0 },
};

constexpr ::zhc::EndpointLabel kEndpoints[] = { {"l1", 1}, {"l2", 2} };
}  // namespace

extern const PreparedDefinition kDef_ZK_EU_2U{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZK-EU-2U", .vendor="Moes",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .reports=kReports, .reports_count=sizeof(kReports)/sizeof(kReports[0]),
    .config_steps=kConfigSteps, .config_steps_count=sizeof(kConfigSteps)/sizeof(kConfigSteps[0]),
    .endpoint_map=kEndpoints, .endpoint_map_count=sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

}  // namespace zhc::devices::moes
