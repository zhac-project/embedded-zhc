// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Easyiot ZB-WB08 — auto-generated.
// 8-button remote control
// z2m-source: easyiot.ts #ZB-WB08.
//
// Eight-endpoint button remote (ep1..ep8). Same notes as ZB-WB01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
const FzConverter* const kFz_ZB_WB08[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ZB_WB08[] = { "ZB-WB08" };

constexpr Expose kExposes_ZB_WB08[] = {
    // TODO(easyiot): per-endpoint action mapping
    // (1_single..8_long) needs runtime support.
    {"action",  ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_WB08[] = {
    {1, 0x0001},
    {1, 0x0006}, {2, 0x0006}, {3, 0x0006}, {4, 0x0006},
    {5, 0x0006}, {6, 0x0006}, {7, 0x0006}, {8, 0x0006},
};

}  // namespace

extern const PreparedDefinition kDef_ZB_WB08{
    .zigbee_models=kModels_ZB_WB08, .zigbee_models_count=sizeof(kModels_ZB_WB08)/sizeof(kModels_ZB_WB08[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-WB08", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_WB08, .exposes_count=sizeof(kExposes_ZB_WB08)/sizeof(kExposes_ZB_WB08[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB_WB08, .from_zigbee_count=sizeof(kFz_ZB_WB08)/sizeof(kFz_ZB_WB08[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_WB08, .bindings_count=sizeof(kBindings_ZB_WB08)/sizeof(kBindings_ZB_WB08[0]),
};

}  // namespace zhc::devices::easyiot
