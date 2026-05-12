// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yokis TLC2-UP — parity-extended 2026-04-28.
// Keyring Design series 2-button remote control
// z2m-source: yokis.ts #TLC2-UP.
//
// Runtime: cluster-cmd action emission + the manuSpecificYokis{Input,Entry,
//   Channel,Device-stub} runtime from definitions/yokis/_shared. Channel
//   routing and Input/Entry config are now writable. manuSpecificYokisDevice
//   reset/openNetwork still TODO (no key payload encoded yet).
#include "definitions/_generic/_shared.hpp"
#include "definitions/yokis/_shared.hpp"

namespace zhc::devices::yokis {
namespace {
const FzConverter* const kFz_TLC2_UP[] = {
    &::zhc::generic::kFzOnOff,
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
    &::zhc::yokis::kFzYokisInput,
    &::zhc::yokis::kFzYokisEntry,
    &::zhc::yokis::kFzYokisChannel,
};
const TzConverter* const kTz_TLC2_UP[] = {
    &::zhc::generic::kTzOnOff,
    // Input + Entry timing config (transmitter knobs).
    &::zhc::yokis::kTzYokisInputMode,
    &::zhc::yokis::kTzYokisContactMode,
    &::zhc::yokis::kTzYokisEShortPress,
    &::zhc::yokis::kTzYokisELongPress,
    &::zhc::yokis::kTzYokisLongPressDuration,
    &::zhc::yokis::kTzYokisTimeBetweenPress,
};
constexpr const char* kModels_TLC2_UP[] = { "TLC2-UP" };

constexpr Expose kAutoExposes[] = {
    {"state",  ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum,   Access::State,    nullptr, "Triggered button action", nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006}, {1, 0x0008}, {1, 0x0102},
    {2, 0x0006}, {2, 0x0008}, {2, 0x0102},
};
}  // namespace

extern const PreparedDefinition kDef_TLC2_UP{
    .zigbee_models=kModels_TLC2_UP, .zigbee_models_count=sizeof(kModels_TLC2_UP)/sizeof(kModels_TLC2_UP[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TLC2-UP", .vendor="Yokis",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TLC2_UP, .from_zigbee_count=sizeof(kFz_TLC2_UP)/sizeof(kFz_TLC2_UP[0]),
    .to_zigbee=kTz_TLC2_UP, .to_zigbee_count=sizeof(kTz_TLC2_UP)/sizeof(kTz_TLC2_UP[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yokis
