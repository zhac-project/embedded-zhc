// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lonsonho VM-Zigbee-S02-0-10V — auto-generated.
// 2 channel Zigbee 0-10V dimmer module
// z2m-source: lonsonho.ts #VM-Zigbee-S02-0-10V.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lonsonho {
namespace {
// TZE600 Tuya MCU device — runtime decoding requires Tuya DP plumbing
// not yet wired in ZHC. Exposes match z2m so UI surfaces the correct
// controls; transport is best-effort via generic OnOff until Tuya DP
// dispatch lands.
const FzConverter* const kFz_VM_Zigbee_S02_0_10V[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_VM_Zigbee_S02_0_10V[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_VM_Zigbee_S02_0_10V[] = { "TS0603" };
constexpr const char* kManus_VM_Zigbee_S02_0_10V[] = { "_TZE600_wxq8dpha" };
// z2m endpoint: {l1:1, l2:1, l3:1} — dispatch by Tuya DP id.
constexpr ::zhc::EndpointLabel kEndpoints_VM_Zigbee_S02_0_10V[] = { {"l1", 1}, {"l2", 1} };

// z2m exposes mirror DPs 1..14: light brightness w/ min/max + countdown +
// switch_type per channel + power_on_behavior.
constexpr Expose kExposes_VM_Zigbee_S02_0_10V[] = {
    { "state_l1",          ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness_l1",     ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "min_brightness_l1", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "max_brightness_l1", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "switch_type_l1",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "countdown_l1",      ExposeType::Numeric, ::zhc::Access::StateSet, "s",  nullptr, nullptr, 0 },
    { "state_l2",          ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness_l2",     ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "min_brightness_l2", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "max_brightness_l2", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "switch_type_l2",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "countdown_l2",      ExposeType::Numeric, ::zhc::Access::StateSet, "s",  nullptr, nullptr, 0 },
    { "power_on_behavior", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBindings_VM_Zigbee_S02_0_10V[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
}  // namespace

extern const PreparedDefinition kDef_VM_Zigbee_S02_0_10V{
    .zigbee_models=kModels_VM_Zigbee_S02_0_10V, .zigbee_models_count=sizeof(kModels_VM_Zigbee_S02_0_10V)/sizeof(kModels_VM_Zigbee_S02_0_10V[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_VM_Zigbee_S02_0_10V, .manufacturer_names_count=sizeof(kManus_VM_Zigbee_S02_0_10V)/sizeof(kManus_VM_Zigbee_S02_0_10V[0]),
    .model="VM-Zigbee-S02-0-10V", .vendor="Lonsonho",
    .meta=nullptr, .exposes=kExposes_VM_Zigbee_S02_0_10V, .exposes_count=sizeof(kExposes_VM_Zigbee_S02_0_10V)/sizeof(kExposes_VM_Zigbee_S02_0_10V[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VM_Zigbee_S02_0_10V, .from_zigbee_count=sizeof(kFz_VM_Zigbee_S02_0_10V)/sizeof(kFz_VM_Zigbee_S02_0_10V[0]),
    .to_zigbee=kTz_VM_Zigbee_S02_0_10V, .to_zigbee_count=sizeof(kTz_VM_Zigbee_S02_0_10V)/sizeof(kTz_VM_Zigbee_S02_0_10V[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindings_VM_Zigbee_S02_0_10V,
    .bindings_count           = sizeof(kBindings_VM_Zigbee_S02_0_10V)/sizeof(kBindings_VM_Zigbee_S02_0_10V[0]),
    .endpoint_map       = kEndpoints_VM_Zigbee_S02_0_10V,
    .endpoint_map_count = sizeof(kEndpoints_VM_Zigbee_S02_0_10V)/sizeof(kEndpoints_VM_Zigbee_S02_0_10V[0]),
};

}  // namespace zhc::devices::lonsonho
