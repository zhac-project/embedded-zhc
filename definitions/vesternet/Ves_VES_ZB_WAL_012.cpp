// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Vesternet VES-ZB-WAL-012 — hand-curated.
// Zigbee wall controller, 8 button — battery-powered remote, NO state.
// Auto-generator wrong-bundled this as on/off; reality is button-event
// only. See Ves_VES_ZB_REM_013.cpp comment for the semantic mapping.
//
// PARITY FIX (lost per-button identity): the command decoders emit a bare
// `action` ("on", "brightness_move_up", …), which is a kAlwaysGlobalKey, so
// without endpoint_action_suffix all four rocker pairs collapsed onto the same
// `action` key and the originating endpoint was thrown away — z2m instead
// distinguishes them as on_1..on_4/…/brightness_stop_4. Added the per-endpoint
// label map ({1,2,3,4}) and set endpoint_action_suffix so the dispatcher
// rewrites the key to `action_<n>` per endpoint. Same convention as the robb/
// sunricher rebrand of this 8-button hardware (Rob_ROB_200_007_0).
//
// z2m fromZigbee: fz.command_on, fz.command_off, fz.command_move,
//                 fz.command_stop, fz.battery
// z2m toZigbee: [] (cannot be controlled).
// z2m exposes: battery + action [on_1..4, off_1..4, stop_1..4,
//              brightness_move_up_1..4, brightness_move_down_1..4,
//              brightness_stop_1..4] → 24 labels for 8 buttons (paired).
// z2m meta: {multiEndpoint: true, battery: {dontDividePercentage: true}}
// z2m configure: bind genOnOff/genLevelCtrl on EP1-4, +genPowerCfg on EP1.
//
// z2m-source: vesternet.ts #VES-ZB-WAL-012.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vesternet {
namespace {
const FzConverter* const kFz_VES_ZB_WAL_012[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_VES_ZB_WAL_012[] = { "ZG2833K8_EU05" };

// Per-endpoint label map: EP1..4 → "1".."4". Drives the `action_<n>` suffix
// rewrite (paired with endpoint_action_suffix) so each rocker keeps identity.
constexpr ::zhc::EndpointLabel kEndpoints_VES_ZB_WAL_012[] = {
    {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4},
};

}  // namespace


// --- hand-curated exposes / bindings ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0001},
    {2, 0x0006},
    {2, 0x0008},
    {3, 0x0006},
    {3, 0x0008},
    {4, 0x0006},
    {4, 0x0008},
};
// --- end ---


constexpr WhiteLabel kWhiteLabels_VES_ZB_WAL_012[] = {
    {"Sunricher","SR-ZG9001K8-DIM"},
};
extern const PreparedDefinition kDef_VES_ZB_WAL_012{
    .zigbee_models=kModels_VES_ZB_WAL_012, .zigbee_models_count=sizeof(kModels_VES_ZB_WAL_012)/sizeof(kModels_VES_ZB_WAL_012[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VES-ZB-WAL-012", .vendor="Vesternet",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_VES_ZB_WAL_012, .white_labels_count=sizeof(kWhiteLabels_VES_ZB_WAL_012)/sizeof(kWhiteLabels_VES_ZB_WAL_012[0]),
    .from_zigbee=kFz_VES_ZB_WAL_012, .from_zigbee_count=sizeof(kFz_VES_ZB_WAL_012)/sizeof(kFz_VES_ZB_WAL_012[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map=kEndpoints_VES_ZB_WAL_012,
    .endpoint_map_count=sizeof(kEndpoints_VES_ZB_WAL_012)/sizeof(kEndpoints_VES_ZB_WAL_012[0]),
    .endpoint_action_suffix=true,
};

}  // namespace zhc::devices::vesternet
