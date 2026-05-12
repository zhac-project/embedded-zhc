// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecodim ED-10011 — wall remote (hand-rewritten, 2026-04-28q).
// Zigbee 2 button wall switch - black.
// z2m-source: ecodim.ts #ED-10011.
//
// fromZigbee: command_on / command_off / command_move / command_stop / battery.
// exposes: action ["on_1","off_1","brightness_move_up_1",
//                  "brightness_move_down_1","brightness_stop_1"], battery.
// toZigbee: [].
// Single endpoint (no multiEndpoint meta) — emits action labels with
// `_1` suffix in z2m. Current ZHC runtime emits unsuffixed labels;
// see ED-10010 note.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ecodim {
namespace {
const FzConverter* const kFz_ED_10011[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ED_10011[] = { "ED-10011" };

constexpr Expose kExposes_ED_10011[] = {
    {"action",  ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ED_10011[] = {
    {1, 0x0001},
    {1, 0x0006}, {1, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_ED_10011{
    .zigbee_models=kModels_ED_10011, .zigbee_models_count=sizeof(kModels_ED_10011)/sizeof(kModels_ED_10011[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ED-10011", .vendor="Ecodim",
    .meta=nullptr, .exposes=kExposes_ED_10011, .exposes_count=sizeof(kExposes_ED_10011)/sizeof(kExposes_ED_10011[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ED_10011, .from_zigbee_count=sizeof(kFz_ED_10011)/sizeof(kFz_ED_10011[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ED_10011, .bindings_count=sizeof(kBindings_ED_10011)/sizeof(kBindings_ED_10011[0]),
};

}  // namespace zhc::devices::ecodim
