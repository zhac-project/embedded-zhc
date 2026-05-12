// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Easyiot ZB-WB02 — auto-generated.
// 2-button remote control
// z2m-source: easyiot.ts #ZB-WB02.
//
// Two-endpoint button remote (ep1 / ep2). Same notes as ZB-WB01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
const FzConverter* const kFz_ZB_WB02[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ZB_WB02[] = { "ZB-WB02" };

constexpr Expose kExposes_ZB_WB02[] = {
    // TODO(easyiot): per-endpoint action mapping
    // (1_single..2_long) needs runtime support.
    {"action",  ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_WB02[] = {
    {1, 0x0001},
    {1, 0x0006},
    {2, 0x0006},
};

}  // namespace

extern const PreparedDefinition kDef_ZB_WB02{
    .zigbee_models=kModels_ZB_WB02, .zigbee_models_count=sizeof(kModels_ZB_WB02)/sizeof(kModels_ZB_WB02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-WB02", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_WB02, .exposes_count=sizeof(kExposes_ZB_WB02)/sizeof(kExposes_ZB_WB02[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB_WB02, .from_zigbee_count=sizeof(kFz_ZB_WB02)/sizeof(kFz_ZB_WB02[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_WB02, .bindings_count=sizeof(kBindings_ZB_WB02)/sizeof(kBindings_ZB_WB02[0]),
};

}  // namespace zhc::devices::easyiot
