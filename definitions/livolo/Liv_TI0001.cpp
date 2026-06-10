// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Livolo TI0001 — multi-gang switch.
// Zigbee switch (1, 2, 3, 4 gang)
// z2m-source: livolo.ts #TI0001 — exposes switch().withEndpoint(left/
// right/bottom_left/bottom_right). The fz `kFzLivoloNewSwitchState4Gang`
// emits state_left/state_right/state_bottom_left/state_bottom_right, but
// the auto-port declared only a bare "state" expose (none of the decoder
// keys had a home). Exposes corrected to the four keyed switches.
#include "definitions/_generic/_shared.hpp"
#include "definitions/livolo/_shared.hpp"

namespace zhc::devices::livolo {
namespace {
const FzConverter* const kFz_TI0001[] = {
    &::zhc::devices::livolo::kFzLivoloNewSwitchState4Gang,
};
const TzConverter* const kTz_TI0001[] = {
    &::zhc::devices::livolo::kTzLivoloState,
};
constexpr const char* kModels_TI0001[] = { "TI0001          " };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state_left",         ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_right",        ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_bottom_left",  ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_bottom_right", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_TI0001{
    .zigbee_models=kModels_TI0001, .zigbee_models_count=sizeof(kModels_TI0001)/sizeof(kModels_TI0001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TI0001", .vendor="Livolo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TI0001, .from_zigbee_count=sizeof(kFz_TI0001)/sizeof(kFz_TI0001[0]),
    .to_zigbee=kTz_TI0001, .to_zigbee_count=sizeof(kTz_TI0001)/sizeof(kTz_TI0001[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::livolo
