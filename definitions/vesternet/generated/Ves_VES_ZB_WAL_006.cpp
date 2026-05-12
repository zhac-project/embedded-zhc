// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Vesternet VES-ZB-WAL-006 — hand-curated.
// Zigbee wall controller, 2 button — battery-powered remote, NO state.
// Auto-generator wrong-bundled this as on/off; reality is button-event
// only. See Ves_VES_ZB_REM_013.cpp comment for the semantic mapping.
//
// z2m fromZigbee: fz.command_on, fz.command_off, fz.command_move,
//                 fz.command_stop, fz.battery
// z2m toZigbee: [] (cannot be controlled).
// z2m exposes: battery + action ["on_1","off_1","stop_1",
//              "brightness_move_up_1","brightness_move_down_1",
//              "brightness_stop_1"].
// z2m meta: {multiEndpoint: true, battery: {dontDividePercentage: true}}
//
// z2m-source: vesternet.ts #VES-ZB-WAL-006.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vesternet {
namespace {
const FzConverter* const kFz_VES_ZB_WAL_006[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_VES_ZB_WAL_006[] = { "ZG2833K2_EU07" };

}  // namespace


// --- hand-curated exposes / bindings ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0001},   // genPowerCfg
};
// --- end ---

extern const PreparedDefinition kDef_VES_ZB_WAL_006{
    .zigbee_models=kModels_VES_ZB_WAL_006, .zigbee_models_count=sizeof(kModels_VES_ZB_WAL_006)/sizeof(kModels_VES_ZB_WAL_006[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VES-ZB-WAL-006", .vendor="Vesternet",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VES_ZB_WAL_006, .from_zigbee_count=sizeof(kFz_VES_ZB_WAL_006)/sizeof(kFz_VES_ZB_WAL_006[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::vesternet
