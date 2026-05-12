// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Niko 552-720X2 — hand-rewritten (was wrong-bundled).
// Battery switch with 2 buttons
//
// z2m-source: niko.ts #552-720X2.
//
// 2-endpoint scene controller (left=ep1, right=ep2). Same converter
// set as 720X1 but with bindings on both endpoints. No toZigbee.
// Multi-endpoint mapping (`{left:1,right:2}`) is metadata only — the
// Fz converters wildcard the endpoint and the runtime tags the action
// with the originating endpoint id.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::niko {
namespace {
const FzConverter* const kFz_D552_720X2[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D552_720X2[] = { "Battery switch, 2 button" };

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
};
// --- end ---

extern const PreparedDefinition kDef_D552_720X2{
    .zigbee_models=kModels_D552_720X2, .zigbee_models_count=sizeof(kModels_D552_720X2)/sizeof(kModels_D552_720X2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="552-720X2", .vendor="Niko",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D552_720X2, .from_zigbee_count=sizeof(kFz_D552_720X2)/sizeof(kFz_D552_720X2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::niko
