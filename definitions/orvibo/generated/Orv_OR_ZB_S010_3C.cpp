// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo OR_ZB_S010_3C.
// Smart socket
// z2m-source: orvibo.ts #OR_ZB_S010_3C.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_OR_ZB_S010_3C[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_OR_ZB_S010_3C[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_OR_ZB_S010_3C[] = { "ORVIBO Socket", "93e29b89b2ee45bea5bdbb7679d75d24" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_OR_ZB_S010_3C{
    .zigbee_models=kModels_OR_ZB_S010_3C, .zigbee_models_count=sizeof(kModels_OR_ZB_S010_3C)/sizeof(kModels_OR_ZB_S010_3C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="OR_ZB_S010_3C", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_OR_ZB_S010_3C, .from_zigbee_count=sizeof(kFz_OR_ZB_S010_3C)/sizeof(kFz_OR_ZB_S010_3C[0]),
    .to_zigbee=kTz_OR_ZB_S010_3C, .to_zigbee_count=sizeof(kTz_OR_ZB_S010_3C)/sizeof(kTz_OR_ZB_S010_3C[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
