// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Somfy 1871157 — hand-rewritten 2026-04-29e.
// Ysia 1 channel blinds remote. z2m uses `m.commandsOnOff` +
// `m.commandsWindowCovering` — wire command-receiver Fz, no Tz state
// path (no local relay/motor).
// z2m-source: somfy.ts #1871157.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::somfy {
namespace {
const FzConverter* const kFz_D1871157[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D1871157[] = { "Ysia 1 Zigbee Europe" };

}  // namespace


// --- hand-edited exposes/bindings ---
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0102},
};
// --- end hand-edited block ---

extern const PreparedDefinition kDef_D1871157{
    .zigbee_models=kModels_D1871157, .zigbee_models_count=sizeof(kModels_D1871157)/sizeof(kModels_D1871157[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="1871157", .vendor="Somfy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D1871157, .from_zigbee_count=sizeof(kFz_D1871157)/sizeof(kFz_D1871157[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::somfy
