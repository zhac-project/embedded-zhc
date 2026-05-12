// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo SE21.
// Smart emergency button
// z2m-source: orvibo.ts #SE21.
// BLOCKED — same as SE20-O; z2m emits off/single/double/hold actions.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_SE21[] = {
    &::zhc::generic::kFzOnOff,
};
constexpr const char* kModels_SE21[] = { "72bd56c539ca4c7fba73a9be0ae0d19f" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_SE21{
    .zigbee_models=kModels_SE21, .zigbee_models_count=sizeof(kModels_SE21)/sizeof(kModels_SE21[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SE21", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SE21, .from_zigbee_count=sizeof(kFz_SE21)/sizeof(kFz_SE21[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
