// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Legrand 067694 — battery action remote, not an on/off switch.
//
// The generated def wired kFzOnOff + a controllable on/off TZ onto a
// device that has no local relay. 067694 is a wireless wall-mount
// pushbutton: it only *sends* genOnOff cluster commands (client->server
// On/Off/Toggle) to its bound group. kFzOnOff decodes attribute
// *reports* (server->client) and so never fired — the "state" expose was
// dead and the on/off TZ was meaningless. Re-wired to the generic
// command-action decoders so the button events surface as `action`.
//
// z2m-source: legrand.ts #067694 —
//   fromZigbee: [fz.identify, fz.command_on, fz.command_off,
//                fz.command_toggle, fz.battery]
//   exposes: e.action(["identify","on","off","toggle"]), e.battery()
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::legrand {
namespace {
const FzConverter* const kFz_D067694[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D067694[] = { " Remote toggle switch" };

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_D067694{
    .zigbee_models=kModels_D067694, .zigbee_models_count=sizeof(kModels_D067694)/sizeof(kModels_D067694[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="067694", .vendor="Legrand",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D067694, .from_zigbee_count=sizeof(kFz_D067694)/sizeof(kFz_D067694[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::legrand
