// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecodim ED-10010 — wall remote (hand-rewritten, 2026-04-28q).
// Zigbee 2 button wall switch - white.
// z2m-source: ecodim.ts #ED-10010.
//
// fromZigbee: command_on / command_off / command_move / command_stop / battery.
// exposes: action ["on","off","brightness_move_up","brightness_move_down",
//                  "brightness_stop"], battery.
// toZigbee: [].
// meta: {multiEndpoint: true} — 2 endpoints (one per button-pair).
//
// Note: action key is in `kAlwaysGlobalKeys` so endpoint suffixing
// is not applied; current runtime emits the bare label. Per-EP
// suffixing inside the value (e.g. "on_1") is the same gap shared
// with easyiot/ZB-WB0x and other multi-button remotes.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ecodim {
namespace {
const FzConverter* const kFz_ED_10010[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ED_10010[] = { "ED-10010" };

constexpr Expose kExposes_ED_10010[] = {
    {"action",  ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ED_10010[] = {
    {1, 0x0001},
    {1, 0x0006}, {1, 0x0008},
    {2, 0x0006}, {2, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_ED_10010{
    .zigbee_models=kModels_ED_10010, .zigbee_models_count=sizeof(kModels_ED_10010)/sizeof(kModels_ED_10010[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ED-10010", .vendor="Ecodim",
    .meta=nullptr, .exposes=kExposes_ED_10010, .exposes_count=sizeof(kExposes_ED_10010)/sizeof(kExposes_ED_10010[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ED_10010, .from_zigbee_count=sizeof(kFz_ED_10010)/sizeof(kFz_ED_10010[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ED_10010, .bindings_count=sizeof(kBindings_ED_10010)/sizeof(kBindings_ED_10010[0]),
};

}  // namespace zhc::devices::ecodim
