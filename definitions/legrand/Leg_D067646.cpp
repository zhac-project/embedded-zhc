// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Legrand 067646 — battery shutter remote, not an on/off switch.
//
// Wireless wall shutter remote, no local relay. The generated def wired
// kFzOnOff + an on/off TZ (both dead). The device emits
// closuresWindowCovering client->server commands (upOpen/downClose/stop).
// Re-wired to the generic cover-command action decoders so the button
// events surface as action open/close/stop.
//
// Note: z2m additionally emits action "moving"/"stopped" via
// fz.legrand_binary_input_moving (a Legrand genBinaryInput presentValue
// remap). No generic decoder maps a binary-input present-value to those
// action labels, so the moving/stopped sub-actions are not surfaced here
// (open/close/stop — the primary button stream — are).
//
// z2m-source: legrand.ts #067646 —
//   fromZigbee: [fz.identify, fz.command_cover_open, fz.command_cover_close,
//                fz.command_cover_stop, fz.battery,
//                fz.legrand_binary_input_moving, fzLegrand.stop_poll_on_checkin]
//   exposes: e.action(["identify","open","close","stop","moving","stopped"]),
//            e.battery()
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::legrand {
namespace {
const FzConverter* const kFz_D067646[] = {
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D067646[] = { " Shutters central remote switch" };

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0102},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_D067646{
    .zigbee_models=kModels_D067646, .zigbee_models_count=sizeof(kModels_D067646)/sizeof(kModels_D067646[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="067646", .vendor="Legrand",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D067646, .from_zigbee_count=sizeof(kFz_D067646)/sizeof(kFz_D067646[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::legrand
