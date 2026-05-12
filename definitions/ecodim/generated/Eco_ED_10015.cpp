// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecodim ED-10015 — wall remote (hand-rewritten, 2026-04-28q).
// Zigbee 8 button wall switch - black (4 endpoints).
// z2m-source: ecodim.ts #ED-10015.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ecodim {
namespace {
const FzConverter* const kFz_ED_10015[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ED_10015[] = { "ED-10015" };

constexpr Expose kExposes_ED_10015[] = {
    {"action",  ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ED_10015[] = {
    {1, 0x0001},
    {1, 0x0006}, {1, 0x0008},
    {2, 0x0006}, {2, 0x0008},
    {3, 0x0006}, {3, 0x0008},
    {4, 0x0006}, {4, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_ED_10015{
    .zigbee_models=kModels_ED_10015, .zigbee_models_count=sizeof(kModels_ED_10015)/sizeof(kModels_ED_10015[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ED-10015", .vendor="Ecodim",
    .meta=nullptr, .exposes=kExposes_ED_10015, .exposes_count=sizeof(kExposes_ED_10015)/sizeof(kExposes_ED_10015[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ED_10015, .from_zigbee_count=sizeof(kFz_ED_10015)/sizeof(kFz_ED_10015[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ED_10015, .bindings_count=sizeof(kBindings_ED_10015)/sizeof(kBindings_ED_10015[0]),
};

}  // namespace zhc::devices::ecodim
