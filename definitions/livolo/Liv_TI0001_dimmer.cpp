// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Livolo TI0001-dimmer — custom-cluster dimmer.
// Zigbee dimmer
// z2m-source: livolo.ts #TI0001-dimmer — fz.livolo_dimmer_state decodes
// raw genPowerCfg frames: data[10]==7/13 -> on/off "state", data[10]==5
// -> "brightness" (data[14]*10 mapped 0..1000 -> 0..255). The auto-port
// wired generic kFzOnOff/kFzBrightness (standard 0x0006/0x0008) which
// never match the raw frame. Wired the custom decoder + Livolo state
// encoder. Brightness *write* uses z2m's bespoke tz.livolo_dimmer_level
// (a distinct manuSpec genLevelCtrl frame) — left read-only here; the
// dimmer-level encoder is INFRA, deferred.
#include "definitions/_generic/_shared.hpp"
#include "definitions/livolo/_shared.hpp"

namespace zhc::devices::livolo {
namespace {
const FzConverter* const kFz_TI0001_dimmer[] = {
    &::zhc::devices::livolo::kFzLivoloDimmerState,
};
const TzConverter* const kTz_TI0001_dimmer[] = {
    &::zhc::devices::livolo::kTzLivoloState,
};
constexpr const char* kModels_TI0001_dimmer[] = { "TI0001-dimmer" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
};

extern const PreparedDefinition kDef_TI0001_dimmer{
    .zigbee_models=kModels_TI0001_dimmer, .zigbee_models_count=sizeof(kModels_TI0001_dimmer)/sizeof(kModels_TI0001_dimmer[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TI0001-dimmer", .vendor="Livolo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TI0001_dimmer, .from_zigbee_count=sizeof(kFz_TI0001_dimmer)/sizeof(kFz_TI0001_dimmer[0]),
    .to_zigbee=kTz_TI0001_dimmer, .to_zigbee_count=sizeof(kTz_TI0001_dimmer)/sizeof(kTz_TI0001_dimmer[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::livolo
