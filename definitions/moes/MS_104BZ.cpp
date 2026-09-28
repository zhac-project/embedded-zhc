// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Moes MS-104BZ smart light switch module, 2 gang (graduated from
// generated/Moe_MS_104BZ.cpp).
//
// z2m tuyaBase() + tuyaOnOff({endpoints: [l1, l2]}), endpoint {l1: 1, l2: 2}:
// state_l1 / state_l2 and one power_on_behavior on genOnOff 0x8002
// (moesStartUpOnOff {0 off, 1 on, 2 previous}), written to EP1. A 0x8002
// report comes out as power_on_behavior_l<n>, as z2m's multiEndpoint
// postfix names it. Configure = bind + reporting.onOff on EP1 and EP2, and
// tuyaBase's magic packet. tuyaOnOff's configureSetPowerSourceWhenUnknown
// (only fills an unknown power source) is not ported.
//
// z2m-source: moes.ts #MS-104BZ.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::moes {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::tuya::kFzTuyaPowerOnBehavior,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::tuya::kTzTuyaPowerOnBehavior,
};
constexpr const char* kModels[] = { "TS011F" };
constexpr const char* kManus[]  = { "_TZ3000_pmz6mjyu", "_TZ3000_iv6ph5tr" };
constexpr const char* kPowerOnValues[] = { "off", "previous", "on" };

constexpr Expose kExposes[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kPowerOnValues, 3, ExposeCategory::Config},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
};

// Local copies of tuya::kReportsOnOff_2ep / kConfigStepsTuyaMagicPacket: their
// counts are extern, which would make this definition dynamically initialised.
constexpr ReportingSpec kReports[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 3600, 0, 0},
    {2, 0x0006, 0x0000, 0x10, 0, 3600, 0, 0},
};
constexpr ConfigStep kConfigSteps[] = {
    { ConfigStepOp::Read, 1, 0x0000, 0x00, 0,
      ::zhc::tuya::kTuyaMagicPacketAttrs, sizeof(::zhc::tuya::kTuyaMagicPacketAttrs), 0 },
};

constexpr ::zhc::EndpointLabel kEndpoints[] = { {"l1", 1}, {"l2", 2} };

constexpr WhiteLabel kWhiteLabels[] = {
    {"KnockautX", "FMS2C017"},   // _TZ3000_iv6ph5tr
};
}  // namespace

extern const PreparedDefinition kDef_MS_104BZ{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="MS-104BZ", .vendor="Moes",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=kWhiteLabels, .white_labels_count=sizeof(kWhiteLabels)/sizeof(kWhiteLabels[0]),
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .reports=kReports, .reports_count=sizeof(kReports)/sizeof(kReports[0]),
    .config_steps=kConfigSteps, .config_steps_count=sizeof(kConfigSteps)/sizeof(kConfigSteps[0]),
    .endpoint_map=kEndpoints, .endpoint_map_count=sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

}  // namespace zhc::devices::moes
