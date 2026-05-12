// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Niko 552-720X1 — hand-rewritten (was wrong-bundled).
// Battery switch with 1 button
//
// z2m-source: niko.ts #552-720X1.
//
// This is a stateless Zigbee scene controller. The previous auto-bundle
// used `kFzOnOff` + `kTzOnOff` + a `state` expose, which is wrong: the
// device has no on/off attribute — it sends `command_on` /
// `command_off` cluster commands plus `command_move` / `command_stop`
// for press-and-hold dimming. There is no toZigbee converter.
//
// Exposes: e.action(["on", "off", "brightness_move_up",
// "brightness_move_down", "brightness_stop"]) + e.battery().
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::niko {
namespace {
const FzConverter* const kFz_D552_720X1[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D552_720X1[] = { "Battery switch, 1 button" };

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
};
// --- end ---

extern const PreparedDefinition kDef_D552_720X1{
    .zigbee_models=kModels_D552_720X1, .zigbee_models_count=sizeof(kModels_D552_720X1)/sizeof(kModels_D552_720X1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="552-720X1", .vendor="Niko",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D552_720X1, .from_zigbee_count=sizeof(kFz_D552_720X1)/sizeof(kFz_D552_720X1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::niko
