// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: TheLightGroup S57003 — hand-curated (was wrong-bundled).
// SLC SmartOne Zigbee wall remote 4-channels — battery-powered scene/dimmer
// remote, NOT a controllable on/off light. The auto-generator saw genOnOff
// and emitted kFzOnOff + kTzOnOff + a settable `state`, so every button press
// was dead (decoded to nothing) and the device falsely advertised a writable
// relay. It also dropped the endpoint_map entirely, so the four rockers
// would have collided on one key.
//
// Reality: the remote only emits genOnOff / genLevelCtrl cluster commands
// which the action decoders translate to on/off/brightness_move_up/
// brightness_move_down/brightness_stop. z2m `meta: {multiEndpoint}` routes
// the four rocker pairs to EP1..EP4; z2m labels them on_l1..off_l4 /
// brightness_move_*_l<n>. We carry the per-endpoint identity via
// endpoint_map {1,2,3,4} + endpoint_action_suffix, so `action` becomes
// action_1..action_4 with the command label in the value (mirrors the
// paulmann 501.37 fix).
//
// z2m fromZigbee: fz.command_on, fz.command_off, fz.battery,
//                 fz.command_move, fz.command_stop
// z2m toZigbee: [] (cannot be controlled).
// z2m exposes: e.battery(), e.action([on_l1..off_l4, brightness_move_up_l<n>,
//   brightness_move_down_l<n>, brightness_stop_l<n>]).
// z2m meta: {multiEndpoint: true}, endpoint {l1:1,l2:2,l3:3,l4:4}.
//
// z2m-source: the_light_group.ts #S57003.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::the_light_group {
namespace {
const FzConverter* const kFz_S57003[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
// Toolbox is empty — the remote has no controllable state (z2m toZigbee: []).
constexpr const char* kModels_S57003[] = { "S57003" };

// Per-endpoint label map: EP1..EP4 → "1".."4", driving the action_<n> suffix
// (paired with endpoint_action_suffix) so each rocker keeps its identity.
constexpr ::zhc::EndpointLabel kEndpoints_S57003[] = {
    {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4},
};

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    // `action` value carries on/off/brightness_move_up/brightness_move_down/
    // brightness_stop; the dispatcher suffixes the key per source endpoint →
    // action_1..action_4 (z2m on_l1..off_l4 / brightness_move_*_l<n>).
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff     (EP1)
    {1, 0x0008},   // genLevelCtrl (EP1)
    {1, 0x0001},   // genPowerCfg  (EP1, battery)
    {2, 0x0006},   // genOnOff     (EP2)
    {2, 0x0008},   // genLevelCtrl (EP2)
    {3, 0x0006},   // genOnOff     (EP3)
    {3, 0x0008},   // genLevelCtrl (EP3)
    {4, 0x0006},   // genOnOff     (EP4)
    {4, 0x0008},   // genLevelCtrl (EP4)
};

extern const PreparedDefinition kDef_S57003{
    .zigbee_models=kModels_S57003, .zigbee_models_count=sizeof(kModels_S57003)/sizeof(kModels_S57003[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S57003", .vendor="TheLightGroup",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_S57003, .from_zigbee_count=sizeof(kFz_S57003)/sizeof(kFz_S57003[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map=kEndpoints_S57003,
    .endpoint_map_count=sizeof(kEndpoints_S57003)/sizeof(kEndpoints_S57003[0]),
    .endpoint_action_suffix=true,
};

}  // namespace zhc::devices::the_light_group
