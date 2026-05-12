// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo SE20_O.
// Smart emergency button
// z2m-source: orvibo.ts #SE20_O.
// BLOCKED — z2m fz.command_status_change_notification_action emits action
// 'single' on ssIasZone. ZHC has kFzIasZoneStatusChange but not the
// command-status-change action mapper; stubbed.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_SE20_O[] = {
    &::zhc::generic::kFzOnOff,
};
constexpr const char* kModels_SE20_O[] = { "987b1869e4944218aa0034750d4ac585" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_SE20_O{
    .zigbee_models=kModels_SE20_O, .zigbee_models_count=sizeof(kModels_SE20_O)/sizeof(kModels_SE20_O[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SE20_O", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SE20_O, .from_zigbee_count=sizeof(kFz_SE20_O)/sizeof(kFz_SE20_O[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
