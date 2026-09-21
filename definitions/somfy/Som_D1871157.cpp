// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: hand-authored z2m v26.76.0 parity port (Phase B).
// Somfy 1871157 — Ysia 1 channel blinds remote (z2m v26.76.0 parity).
//
// z2m-source: somfy.ts #1871157. Graduated from the hand-edited generated stub
// to add the second zigbeeModel spelling "Ysia 1 HP Zigbee" (delta v26.35->
// v26.76 added it to the def's zigbeeModel list). Converter surface unchanged:
// z2m uses m.commandsOnOff + m.commandsWindowCovering + m.battery — wire the
// command-receiver Fz set, no Tz state path (no local relay/motor).
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
constexpr const char* kModels_D1871157[] = { "Ysia 1 Zigbee Europe", "Ysia 1 HP Zigbee" };

}  // namespace

static constexpr const char* kActionOpts_Som_D1871157[] = {"on_1", "on_2", "on_3", "on_4", "on_5", "off_1", "off_2", "off_3", "off_4", "off_5", "toggle_1", "toggle_2", "toggle_3", "toggle_4", "toggle_5", "on", "off", "toggle", "open_1", "open_2", "open_3", "open_4", "open_5", "close_1", "close_2", "close_3", "close_4", "close_5", "stop_1", "stop_2", "stop_3", "stop_4", "stop_5", "open", "close", "stop"};
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActionOpts_Som_D1871157, 36},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0102},
};

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
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::somfy
