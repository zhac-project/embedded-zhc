// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo CR11S8UZ.
// Smart sticker switch
// z2m-source: orvibo.ts #CR11S8UZ.
// BLOCKED — z2m fz.orvibo_raw_1 decodes a vendor-private raw cluster
// (0x0017) into 12 button_N_click/_hold/_release actions. ZHC has no raw-
// cluster decoder; shipped as state-only stub until a per-device fz lands.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_CR11S8UZ[] = {
    &::zhc::generic::kFzOnOff,
};
constexpr const char* kModels_CR11S8UZ[] = { "3c4e4fc81ed442efaf69353effcdfc5f", "51725b7bcba945c8a595b325127461e9" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_CR11S8UZ{
    .zigbee_models=kModels_CR11S8UZ, .zigbee_models_count=sizeof(kModels_CR11S8UZ)/sizeof(kModels_CR11S8UZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CR11S8UZ", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CR11S8UZ, .from_zigbee_count=sizeof(kFz_CR11S8UZ)/sizeof(kFz_CR11S8UZ[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
