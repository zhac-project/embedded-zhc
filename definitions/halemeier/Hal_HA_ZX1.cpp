// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Halemeier HA-ZX1 — hand-rewritten command-style remote.
// X-Mitter smart remote control.
// z2m: extend=[battery, identify],
// fromZigbee=[command_off, command_on, command_stop, command_move],
// exposes=[action(recall_*/on/off/brightness_move_up/brightness_move_down)].
// The auto-generator emitted kFzOnOff + kTzOnOff (a controllable on/off
// switch with a `state` attribute), but this is a battery REMOTE: it emits
// genOnOff / genLevelCtrl cluster commands, it has no on/off state.
// Fix → command_* converters + `action` expose.
// z2m-source: halemeier.ts #HA-ZX1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::halemeier {
namespace {
const FzConverter* const kFz_HA_ZX1[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};

constexpr Expose kExposes_HA_ZX1[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_HA_ZX1[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl (move / stop)
    {1, 0x0003},  // genIdentify
    {1, 0x0001},  // genPowerCfg (battery)
};

constexpr const char* kModels_HA_ZX1[] = { "HA-ZX1" };

}  // namespace

extern const PreparedDefinition kDef_HA_ZX1{
    .zigbee_models=kModels_HA_ZX1, .zigbee_models_count=sizeof(kModels_HA_ZX1)/sizeof(kModels_HA_ZX1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HA-ZX1", .vendor="Halemeier",
    .meta=nullptr,
    .exposes=kExposes_HA_ZX1, .exposes_count=sizeof(kExposes_HA_ZX1)/sizeof(kExposes_HA_ZX1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HA_ZX1, .from_zigbee_count=sizeof(kFz_HA_ZX1)/sizeof(kFz_HA_ZX1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_HA_ZX1, .bindings_count=sizeof(kBindings_HA_ZX1)/sizeof(kBindings_HA_ZX1[0]),
};

}  // namespace zhc::devices::halemeier
