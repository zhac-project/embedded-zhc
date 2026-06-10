// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Envilar ZGR904-S — hand-rewritten for parity.
// Touchlink remote
// z2m-source: envilar.ts #ZGR904-S.
//
// z2m bundle: fz.command_recall, fz.command_on, fz.command_off,
//   fz.command_move, fz.command_stop, fz.battery; exposes
//   [battery(), action([recall_1, recall_2, on, off, brightness_stop,
//   brightness_move_up, brightness_move_down])]. toZigbee: [].
//
// ZGR904-S is a battery scene/dimmer remote (a SENDER), not a controllable
// load. The previous auto-port mis-ported it as a controllable on/off
// switch: it wired kFzOnOff (server-side attr read) + kTzOnOff and exposed
// a `state` binary, so every physical button press was dropped (dead
// action) and the SPA rendered a phantom toggle. Rewired to the generic
// command converters so each press surfaces as an `action` value, dropped
// the phantom `state`/kTzOnOff, and kept battery via kFzBattery.
//
// The command converters are single-endpoint here (z2m has no
// deviceEndpoints/multiEndpoint), so NO endpoint_map — action keys stay
// bare.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::envilar {
namespace {
const FzConverter* const kFz_ZGR904_S[] = {
    &::zhc::generic::kFzCommandRecall,   // recall_1 / recall_2
    &::zhc::generic::kFzCommandOn,       // on
    &::zhc::generic::kFzCommandOff,      // off
    &::zhc::generic::kFzCommandMove,     // brightness_move_up / brightness_move_down
    &::zhc::generic::kFzCommandStop,     // brightness_stop
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ZGR904_S[] = { "ZGR904-S" };

constexpr const char* kActionValues_ZGR904_S[] = {
    "recall_1", "recall_2", "on", "off",
    "brightness_stop", "brightness_move_up", "brightness_move_down",
};

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
        kActionValues_ZGR904_S,
        sizeof(kActionValues_ZGR904_S)/sizeof(kActionValues_ZGR904_S[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0005},
    {1, 0x0001},
};

extern const PreparedDefinition kDef_ZGR904_S{
    .zigbee_models=kModels_ZGR904_S, .zigbee_models_count=sizeof(kModels_ZGR904_S)/sizeof(kModels_ZGR904_S[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZGR904-S", .vendor="Envilar",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZGR904_S, .from_zigbee_count=sizeof(kFz_ZGR904_S)/sizeof(kFz_ZGR904_S[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::envilar
