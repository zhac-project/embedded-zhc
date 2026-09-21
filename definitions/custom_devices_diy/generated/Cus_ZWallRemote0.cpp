// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy ZWallRemote0 — auto-generated.
// Matts Wall Switch Remote
// z2m-source: custom_devices_diy.ts #ZWallRemote0.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: fromZigbee=[fz.command_toggle], toZigbee=[], exposes=[e.action(["toggle"])].
// Remote-only device: emits genOnOff "toggle" command (cmd 0x02), no on/off state.
const FzConverter* const kFz_ZWallRemote0[] = {
    &::zhc::generic::kFzCommandToggle,
};
constexpr const char* kModels_ZWallRemote0[] = { "ZWallRemote0" };

}  // namespace


constexpr const char* kAutoOpts_ZWallRemote0_action[] = {"toggle"};
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0): action ---
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts_ZWallRemote0_action, 1},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_ZWallRemote0{
    .zigbee_models=kModels_ZWallRemote0, .zigbee_models_count=sizeof(kModels_ZWallRemote0)/sizeof(kModels_ZWallRemote0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZWallRemote0", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZWallRemote0, .from_zigbee_count=sizeof(kFz_ZWallRemote0)/sizeof(kFz_ZWallRemote0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::custom_devices_diy
