// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecodim ED-10013 — wall remote (hand-rewritten, 2026-04-28q).
// Zigbee 4 button wall switch - black (2 endpoints).
// z2m-source: ecodim.ts #ED-10013.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ecodim {
namespace {
const FzConverter* const kFz_ED_10013[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ED_10013[] = { "ED-10013" };

constexpr Expose kExposes_ED_10013[] = {
    {"action",  ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ED_10013[] = {
    {1, 0x0001},
    {1, 0x0006}, {1, 0x0008},
    {2, 0x0006}, {2, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_ED_10013{
    .zigbee_models=kModels_ED_10013, .zigbee_models_count=sizeof(kModels_ED_10013)/sizeof(kModels_ED_10013[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ED-10013", .vendor="Ecodim",
    .meta=nullptr, .exposes=kExposes_ED_10013, .exposes_count=sizeof(kExposes_ED_10013)/sizeof(kExposes_ED_10013[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ED_10013, .from_zigbee_count=sizeof(kFz_ED_10013)/sizeof(kFz_ED_10013[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ED_10013, .bindings_count=sizeof(kBindings_ED_10013)/sizeof(kBindings_ED_10013[0]),
};

}  // namespace zhc::devices::ecodim
