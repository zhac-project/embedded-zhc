// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Niko 552-720X4 — hand-rewritten (was wrong-bundled).
// Battery switch with 4 buttons
//
// z2m-source: niko.ts #552-720X4.
//
// 4-endpoint scene controller (top_left=ep1, bottom_left=ep2,
// top_right=ep3, bottom_right=ep4). Same converter set as 720X1 but
// with bindings on every endpoint. No toZigbee.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::niko {
namespace {
const FzConverter* const kFz_D552_720X4[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D552_720X4[] = { "Battery switch, 4 button" };

}  // namespace


// --- hand-authored exposes ---
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0001},
    {2, 0x0006},
    {2, 0x0008},
    {3, 0x0006},
    {3, 0x0008},
    {4, 0x0006},
    {4, 0x0008},
};
// --- end ---

extern const PreparedDefinition kDef_D552_720X4{
    .zigbee_models=kModels_D552_720X4, .zigbee_models_count=sizeof(kModels_D552_720X4)/sizeof(kModels_D552_720X4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="552-720X4", .vendor="Niko",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D552_720X4, .from_zigbee_count=sizeof(kFz_D552_720X4)/sizeof(kFz_D552_720X4[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::niko
