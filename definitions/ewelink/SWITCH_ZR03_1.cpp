// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: eWeLink SWITCH-ZR03-1 Zigbee smart switch (graduated from
// generated/Ewe_SWITCH_ZR03_1.cpp).
//
// z2m m.onOff({skipDuplicateTransaction}): switch + power_on_behavior on
// the standard genOnOff 0x4003 startUpOnOff {0 off, 1 on, 2 toggle,
// 255 previous}; configure = onOff reporting (min 0, max 65000, change 1).
// z2m also tries startUpOnOff reporting and tolerates UNSUPPORTED — not
// ported. skipDuplicateTransaction / skipDefaultResponse are runtime-level.
//
// z2m-source: ewelink.ts #SWITCH-ZR03-1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ewelink {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels[] = { "SWITCH-ZR03-1" };
constexpr const char* kPowerOnValues[] = { "off", "on", "toggle", "previous" };
}  // namespace

constexpr Expose kExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kPowerOnValues, 4, ExposeCategory::Config},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0006},
};

constexpr ReportingSpec kReports[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},
};

extern const PreparedDefinition kDef_SWITCH_ZR03_1{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SWITCH-ZR03-1", .vendor="Ewelink",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .reports=kReports, .reports_count=sizeof(kReports)/sizeof(kReports[0]),
};

}  // namespace zhc::devices::ewelink
