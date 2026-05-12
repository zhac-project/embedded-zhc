// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Orvibo T40W1Z — uses orvibo/_shared.
// MixSwitch 1 gang
// z2m-source: orvibo.ts #T40W1Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/orvibo/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_T40W1Z[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_T40W1Z[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::orvibo::kTzOrviboPowerOnBehavior,
};
constexpr const char* kModels_T40W1Z[] = { "2ae011fb6d0542f58705d6861064eb5f", "71a0b275d9ba4895afdaf400bc7e3a0d", "b7313321dbe74da384d136a2a3fa2005" };

}  // namespace

constexpr const char* kEnum_T40W1Z_1[] = { "off", "previous" };

constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kEnum_T40W1Z_1, 2},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0xFF00},
};

extern const PreparedDefinition kDef_T40W1Z{
    .zigbee_models=kModels_T40W1Z, .zigbee_models_count=sizeof(kModels_T40W1Z)/sizeof(kModels_T40W1Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T40W1Z", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_T40W1Z, .from_zigbee_count=sizeof(kFz_T40W1Z)/sizeof(kFz_T40W1Z[0]),
    .to_zigbee=kTz_T40W1Z, .to_zigbee_count=sizeof(kTz_T40W1Z)/sizeof(kTz_T40W1Z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
