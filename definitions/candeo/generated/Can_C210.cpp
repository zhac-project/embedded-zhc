// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Candeo C210.
// Zigbee dimming smart plug
// z2m-source: candeo.ts #C210.
#include "definitions/_generic/_shared.hpp"


namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C210[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzPowerOnBehavior1
};
const TzConverter* const kTz_C210[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzPowerOnBehavior1
};
constexpr const char* kModels_C210[] = { "Dimmer-Switch-ZB3.0", "C210" };

constexpr Expose kAutoExposes[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008}
};

}  // namespace

extern const PreparedDefinition kDef_C210{
    .zigbee_models=kModels_C210, .zigbee_models_count=sizeof(kModels_C210)/sizeof(kModels_C210[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C210", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C210, .from_zigbee_count=sizeof(kFz_C210)/sizeof(kFz_C210[0]),
    .to_zigbee=kTz_C210, .to_zigbee_count=sizeof(kTz_C210)/sizeof(kTz_C210[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
