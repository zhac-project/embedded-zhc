// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Immax 07046L — hand-rewritten (keyfob action via genScenes commandRecall).
// 4-Touch single click buttons
// z2m-source: immax.ts #07046L.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::immax {
namespace {
const FzConverter* const kFz_D07046L[] = {
    &::zhc::generic::kFzCommandRecall,
};
constexpr const char* kModels_D07046L[] = { "Keyfob-ZB3.0" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0005},
};

extern const PreparedDefinition kDef_D07046L{
    .zigbee_models=kModels_D07046L, .zigbee_models_count=sizeof(kModels_D07046L)/sizeof(kModels_D07046L[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="07046L", .vendor="Immax",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D07046L, .from_zigbee_count=sizeof(kFz_D07046L)/sizeof(kFz_D07046L[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::immax
