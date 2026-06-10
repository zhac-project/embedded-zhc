// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Livolo TI0001-curtain-switch — custom-cluster curtain switch.
// Zigbee curtain switch (can only read status, control does not work yet)
// z2m-source: livolo.ts #TI0001-curtain-switch — fz.livolo_curtain_switch_state
// decodes raw genPowerCfg frames into state_left/state_right with a
// non-bitmask polarity (s==1 -> left ON, s==0 -> right ON). The auto-port
// wired the generic kFzOnOff (standard genOnOff 0x0006), which never
// matches the raw frame and emits the wrong key. Wired the custom decoder.
#include "definitions/_generic/_shared.hpp"
#include "definitions/livolo/_shared.hpp"

namespace zhc::devices::livolo {
namespace {
const FzConverter* const kFz_TI0001_curtain_switch[] = {
    &::zhc::devices::livolo::kFzLivoloCurtainState,
};
const TzConverter* const kTz_TI0001_curtain_switch[] = {
    &::zhc::devices::livolo::kTzLivoloState,
};
constexpr const char* kModels_TI0001_curtain_switch[] = { "TI0001-curtain-switch" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state_left",  ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_right", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_TI0001_curtain_switch{
    .zigbee_models=kModels_TI0001_curtain_switch, .zigbee_models_count=sizeof(kModels_TI0001_curtain_switch)/sizeof(kModels_TI0001_curtain_switch[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TI0001-curtain-switch", .vendor="Livolo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TI0001_curtain_switch, .from_zigbee_count=sizeof(kFz_TI0001_curtain_switch)/sizeof(kFz_TI0001_curtain_switch[0]),
    .to_zigbee=kTz_TI0001_curtain_switch, .to_zigbee_count=sizeof(kTz_TI0001_curtain_switch)/sizeof(kTz_TI0001_curtain_switch[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::livolo
