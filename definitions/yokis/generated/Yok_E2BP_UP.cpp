// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yokis E2BP-UP — parity-extended 2026-04-28.
// Flush-mounted independent 2-channel transmitter (battery)
// z2m-source: yokis.ts #E2BP-UP.
//
// z2m extend list:
//   m.deviceEndpoints({endpoints: {ep1:1, ep2:2}})
//   m.identify()
//   m.commandsOnOff(), m.commandsLevelCtrl(), m.commandsWindowCovering()
//   YokisDeviceExtend, YokisInputExtend, YokisChannelExtend
//
// Runtime gap (manuSpec yokis clusters not yet implemented in ZHC):
//   manuSpecificYokisDevice (0xFC01) — ConfigurationChanged attr, reset/openNet cmds
//   manuSpecificYokisInput (0xFC02) — InputMode/ContactMode/sendPress/sendRelease
//   manuSpecificYokisChannel (0xFC0B) — onOffClusterMode, channel routing
// → these are config/diagnostic surfaces; the device still behaves as a button
//   transmitter without them. Hardware testing required before claiming parity.
// TODO 2026-04-28: wire manuSpecificYokis* converters from
//   definitions/yokis/_shared.hpp (kFzYokis{Input,Entry,Channel,LightControl,
//   Dimmer,SubSystem,PilotWire} + matching kTz*) — see Yok_MTR500E_UP.cpp /
//   Yok_TLC2_UP.cpp / Yok_MFP_UP.cpp for canonical wiring patterns.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yokis {
namespace {
const FzConverter* const kFz_E2BP_UP[] = {
    &::zhc::generic::kFzOnOff,
    // Cluster-command action decoders (commandsOnOff/LevelCtrl/WindowCovering)
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
};
const TzConverter* const kTz_E2BP_UP[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_E2BP_UP[] = { "E2BP-UP" };

constexpr Expose kAutoExposes[] = {
    {"state",  ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum,   Access::State,    nullptr, "Triggered button action", nullptr, 0},
};

// Bind on/off, level, window-covering on both endpoints for command actions.
constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006}, {1, 0x0008}, {1, 0x0102},
    {2, 0x0006}, {2, 0x0008}, {2, 0x0102},
};
}  // namespace

extern const PreparedDefinition kDef_E2BP_UP{
    .zigbee_models=kModels_E2BP_UP, .zigbee_models_count=sizeof(kModels_E2BP_UP)/sizeof(kModels_E2BP_UP[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E2BP-UP", .vendor="Yokis",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E2BP_UP, .from_zigbee_count=sizeof(kFz_E2BP_UP)/sizeof(kFz_E2BP_UP[0]),
    .to_zigbee=kTz_E2BP_UP, .to_zigbee_count=sizeof(kTz_E2BP_UP)/sizeof(kTz_E2BP_UP[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yokis
