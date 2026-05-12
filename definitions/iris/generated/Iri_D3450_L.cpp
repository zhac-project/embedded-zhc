// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Iris 3450-L — hand-rewritten 2026-04-28q.
// 4-button smart key fob (zigbeeModels "3450-L", "3450-L2", "3450-Geu").
// z2m-source: iris.ts #3450-L.
//   fromZigbee: [fz.command_on_presence, fz.command_off, fz.battery, fz.checkin_presence]
//   toZigbee:   []
//   exposes:    [action: ["on_1".."off_4"], battery, presence]
//   meta:       multiEndpoint (4 EPs); voltageToPercentage 3V_2100; checkinInterval write
//
// Mapped:
//   kFzCommandOn   — genOnOff cmd 0x01 → action "on"
//   kFzCommandOff  — genOnOff cmd 0x00 → action "off"
//   kFzBattery     — genPowerCfg → battery + voltage
//
// Bindings on EP 1-4 + genPowerCfg + genPollCtrl on EP 1.
//
// Known parity gaps (documented in IRIS_PARITY.md):
//   * `action` is in `kAlwaysGlobalKeys[]` blocklist so the multi-EP
//     dispatcher cannot suffix per-button (`on_1`/`off_1`/...). All four
//     buttons collapse to bare `on`/`off` until either the blocklist is
//     widened or a custom multi-EP-aware Fz is written.
//   * `command_on_presence` is `command_on` plus an implicit `presence: true`
//     toggle — z2m emits both `action` and `presence`. We don't yet have a
//     `kFzCommandOnPresence` so the `presence` stream is dropped.
//   * `checkin_presence` (genPollCtrl Checkin → presence: true) similarly
//     dropped; would need a new fz + an `on_event` write of `checkinInterval`.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iris {
namespace {

const FzConverter* const kFz_D3450_L[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_D3450_L[] = { "3450-L", "3450-L2", "3450-Geu" };

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff EP 1
    {2, 0x0006},  // genOnOff EP 2
    {3, 0x0006},  // genOnOff EP 3
    {4, 0x0006},  // genOnOff EP 4
    {1, 0x0001},  // genPowerCfg
    {1, 0x0020},  // genPollCtrl
};

constexpr ::zhc::EndpointLabel kEndpoints_D3450_L[] = {
    {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4},
};

}  // namespace

extern const PreparedDefinition kDef_D3450_L{
    .zigbee_models=kModels_D3450_L, .zigbee_models_count=sizeof(kModels_D3450_L)/sizeof(kModels_D3450_L[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3450-L", .vendor="Iris",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3450_L, .from_zigbee_count=sizeof(kFz_D3450_L)/sizeof(kFz_D3450_L[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map=kEndpoints_D3450_L,
    .endpoint_map_count=sizeof(kEndpoints_D3450_L)/sizeof(kEndpoints_D3450_L[0]),
    .endpoint_action_suffix = true,
};

}  // namespace zhc::devices::iris
