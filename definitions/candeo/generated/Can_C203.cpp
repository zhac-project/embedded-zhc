// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Candeo C203 (uses shared candeo switch_type).
// Zigbee micro smart dimmer
// z2m-source: candeo.ts #C203.
#include "definitions/_generic/_shared.hpp"
#include "definitions/candeo/_shared.hpp"

namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C203[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::candeo::kFzExternalSwitchType
};
const TzConverter* const kTz_C203[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::candeo::kTzExternalSwitchType
};
constexpr const char* kModels_C203[] = { "C203", "HK-LN-DIM-A" };

constexpr Expose kAutoExposes[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"external_switch_type", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008}
};

}  // namespace

extern const PreparedDefinition kDef_C203{
    .zigbee_models=kModels_C203, .zigbee_models_count=sizeof(kModels_C203)/sizeof(kModels_C203[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C203", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C203, .from_zigbee_count=sizeof(kFz_C203)/sizeof(kFz_C203[0]),
    .to_zigbee=kTz_C203, .to_zigbee_count=sizeof(kTz_C203)/sizeof(kTz_C203[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
