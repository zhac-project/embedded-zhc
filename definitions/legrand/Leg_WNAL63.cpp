// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Legrand WNAL63 — battery action remote, not an on/off switch.
//
// Netatmo remote dimmer pushbutton, no local relay. Same fix as 067773:
// the generated kFzOnOff + on/off TZ were dead; the device emits
// genOnOff / genLevelCtrl client->server commands. Re-wired to the
// generic command-action decoders.
//
// z2m-source: legrand.ts #WNAL63 —
//   fromZigbee: [fz.identify, fz.command_on, fz.command_off,
//                fz.command_toggle, fz.command_move, fz.command_stop,
//                fz.battery]
//   exposes: e.action(["identify","on","off","toggle",
//                       "brightness_move_up","brightness_move_down",
//                       "brightness_stop"]), e.battery()
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::legrand {
namespace {
const FzConverter* const kFz_WNAL63[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_WNAL63[] = { "Remote dimmer switch" };

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_WNAL63{
    .zigbee_models=kModels_WNAL63, .zigbee_models_count=sizeof(kModels_WNAL63)/sizeof(kModels_WNAL63[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WNAL63", .vendor="Legrand",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WNAL63, .from_zigbee_count=sizeof(kFz_WNAL63)/sizeof(kFz_WNAL63[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::legrand
