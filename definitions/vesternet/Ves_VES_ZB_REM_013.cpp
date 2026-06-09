// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Vesternet VES-ZB-REM-013 — hand-curated.
// Zigbee 12-button remote — battery-powered, NO on/off state. The
// auto-generator wrong-bundled this as a generic on/off switch; reality
// is that the device only emits commandOn/Off/Move/Stop/Recall ZCL
// cluster commands which the action-decoders translate into "on",
// "off", "brightness_move_up", "brightness_move_down",
// "brightness_stop", "recall_<scene>" semantic action labels.
//
// PARITY FIX (lost per-endpoint identity): the command decoders emit a bare
// `action`, which is a kAlwaysGlobalKey, so without endpoint_action_suffix
// every one of the four rocker groups collapsed onto the same `action` key
// and the originating endpoint was thrown away. z2m distinguishes them per
// endpoint (on_1..on_4, …, recall_1_1..recall_2_4). Added the per-endpoint
// label map ({1,2,3,4}) and set endpoint_action_suffix so the dispatcher
// rewrites the key to `action_<n>` per endpoint; the recall scene rides in
// the value (`recall_<scene>`), so both endpoint and scene survive.
//
// z2m fromZigbee: fz.command_on, fz.command_off, fz.command_move,
//                 fz.command_stop, fz.command_recall, fzLocal.battery
// z2m toZigbee: [] (it cannot be controlled).
// z2m exposes: e.battery(), e.action([32 labels]) — on/off/stop/
//   brightness_move_up/brightness_move_down/brightness_stop × ep 1-4,
//   plus recall_1_1..recall_1_4 and recall_2_1..recall_2_4.
// z2m meta: {multiEndpoint: true, battery: {dontDividePercentage: true},
//            publishDuplicateTransaction: true}
// z2m configure: bind genOnOff/genLevelCtrl/genScenes on EP1-4,
//                +genPowerCfg on EP1.
//
// z2m-source: vesternet.ts #VES-ZB-REM-013.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vesternet {
namespace {
const FzConverter* const kFz_VES_ZB_REM_013[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzBattery,
};
// Toolbox is empty — the remote has no controllable state.
constexpr const char* kModels_VES_ZB_REM_013[] = { "ZGRC-KEY-013" };

// Per-endpoint label map: EP1..4 → "1".."4". Drives the `action_<n>` suffix
// rewrite (paired with endpoint_action_suffix) so each rocker keeps identity.
constexpr ::zhc::EndpointLabel kEndpoints_VES_ZB_REM_013[] = {
    {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4},
};

}  // namespace


// --- hand-curated exposes / bindings ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    // `action` is the conventional event label key; values are emitted by
    // kFzCommand* converters and the dispatcher suffixes the key per source
    // endpoint (endpoint_action_suffix) → action_1..action_4 carrying values
    // on/off/brightness_move_up/brightness_move_down/brightness_stop and
    // recall_<scene> (z2m equivalents on_1..on_4 … recall_1_1..recall_2_4).
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

// z2m configure: bind genOnOff/genLevelCtrl/genScenes on EP1-4, +genPowerCfg
// on EP1. Configure callbacks are deferred to vendor-specific configure() in
// v2; for now we only declare the binding rows so the adapter can emit ZDO
// bind requests.
constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0001},   // genPowerCfg
    {1, 0x0005},   // genScenes (for fz.command_recall)
    {2, 0x0006},
    {2, 0x0008},
    {2, 0x0005},
    {3, 0x0006},
    {3, 0x0008},
    {3, 0x0005},
    {4, 0x0006},
    {4, 0x0008},
    {4, 0x0005},
};
// --- end ---


constexpr WhiteLabel kWhiteLabels_VES_ZB_REM_013[] = {
    {"Sunricher","SR-ZG9001K12-DIM-Z4"},
};
extern const PreparedDefinition kDef_VES_ZB_REM_013{
    .zigbee_models=kModels_VES_ZB_REM_013, .zigbee_models_count=sizeof(kModels_VES_ZB_REM_013)/sizeof(kModels_VES_ZB_REM_013[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VES-ZB-REM-013", .vendor="Vesternet",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_VES_ZB_REM_013, .white_labels_count=sizeof(kWhiteLabels_VES_ZB_REM_013)/sizeof(kWhiteLabels_VES_ZB_REM_013[0]),
    .from_zigbee=kFz_VES_ZB_REM_013, .from_zigbee_count=sizeof(kFz_VES_ZB_REM_013)/sizeof(kFz_VES_ZB_REM_013[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map=kEndpoints_VES_ZB_REM_013,
    .endpoint_map_count=sizeof(kEndpoints_VES_ZB_REM_013)/sizeof(kEndpoints_VES_ZB_REM_013[0]),
    .endpoint_action_suffix=true,
};

}  // namespace zhc::devices::vesternet
