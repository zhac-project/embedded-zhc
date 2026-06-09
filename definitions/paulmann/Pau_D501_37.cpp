// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Paulmann 501.37 — hand-curated (was wrong-bundled).
// Smart switch 4 buttons white — battery-powered scene remote, NOT a
// controllable on/off light. The auto-generator saw genOnOff and emitted
// kFzOnOff + kTzOnOff + a settable `state`, so every button press was dead
// (decoded to nothing) and the device falsely advertised a writable relay.
//
// Reality: the remote only emits genOnOff/genLevelCtrl cluster commands
// which the action decoders translate to on/off/brightness_move_up/
// brightness_move_down/brightness_move_stop. z2m `meta: {multiEndpoint}`
// routes the two rocker pairs to EP1/EP2; z2m labels them on_1/on_2/…
// We carry the per-endpoint identity via endpoint_map {1,2} +
// endpoint_action_suffix, so `action` becomes action_1 / action_2 with the
// command label in the value (mirrors the vesternet REM-013 fix).
//
// z2m fromZigbee: fz.command_on, fz.command_off, fz.battery,
//                 fz.command_move, fz.command_stop
// z2m toZigbee: [] (cannot be controlled).
// z2m exposes: e.battery(), e.action([on_1,off_1,on_2,off_2,
//   brightness_move_up_1,brightness_move_down_1,brightness_move_stop_1,
//   brightness_move_up_2,brightness_move_down_2,brightness_move_stop_2]).
// z2m meta: {multiEndpoint: true}.
//
// z2m-source: paulmann.ts #501.37.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
const FzConverter* const kFz_D501_37[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
// Toolbox is empty — the remote has no controllable state.
constexpr const char* kModels_D501_37[] = { "501.37" };

// Per-endpoint label map: EP1/EP2 → "1"/"2", driving the action_<n> suffix
// (paired with endpoint_action_suffix) so each rocker keeps its identity.
constexpr ::zhc::EndpointLabel kEndpoints_D501_37[] = {
    {"1", 1}, {"2", 2},
};

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    // `action` value carries on/off/brightness_move_up/brightness_move_down/
    // brightness_move_stop; the dispatcher suffixes the key per source
    // endpoint → action_1 / action_2 (z2m on_1..off_2 / brightness_move_*_N).
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff   (EP1)
    {1, 0x0008},   // genLevelCtrl (EP1)
    {1, 0x0001},   // genPowerCfg (EP1, battery)
    {2, 0x0006},   // genOnOff   (EP2)
    {2, 0x0008},   // genLevelCtrl (EP2)
};

extern const PreparedDefinition kDef_D501_37{
    .zigbee_models=kModels_D501_37, .zigbee_models_count=sizeof(kModels_D501_37)/sizeof(kModels_D501_37[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="501.37", .vendor="Paulmann",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D501_37, .from_zigbee_count=sizeof(kFz_D501_37)/sizeof(kFz_D501_37[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map=kEndpoints_D501_37,
    .endpoint_map_count=sizeof(kEndpoints_D501_37)/sizeof(kEndpoints_D501_37[0]),
    .endpoint_action_suffix=true,
};

}  // namespace zhc::devices::paulmann
