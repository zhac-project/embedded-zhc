// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Paulmann 501.34 — hand-curated (was wrong-bundled).
// Smart switch 4 buttons white — sibling of 501.37 (two rocker pairs on
// EP1/EP2). The auto-generator mis-bundled it as a settable on/off light
// (kFzOnOff + kTzOnOff + writable `state`), dropping every button press.
// Replaced with the genOnOff/genLevelCtrl command decoders + an `action`
// enum + endpoint_map {1,2} so each rocker keeps its identity.
//
// Note vs 501.37: z2m labels the level-stop action "brightness_stop" here
// (not "brightness_move_stop"); the generic kFzCommandStop already emits
// the bare "brightness_stop" value, matching this model exactly.
//
// z2m fromZigbee: fz.command_on, fz.command_off, fz.battery,
//                 fz.command_move, fz.command_stop
// z2m toZigbee: [] (cannot be controlled).
// z2m exposes: e.battery(), e.action([on_1,off_1,on_2,off_2,
//   brightness_move_up_1,brightness_move_down_1,brightness_stop_1,
//   brightness_move_up_2,brightness_move_down_2,brightness_stop_2]).
// z2m meta: {multiEndpoint: true}.
//
// z2m-source: paulmann.ts #501.34.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
const FzConverter* const kFz_D501_34[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D501_34[] = { "501.34" };

constexpr ::zhc::EndpointLabel kEndpoints_D501_34[] = {
    {"1", 1}, {"2", 2},
};

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff   (EP1)
    {1, 0x0008},   // genLevelCtrl (EP1)
    {1, 0x0001},   // genPowerCfg (EP1, battery)
    {2, 0x0006},   // genOnOff   (EP2)
    {2, 0x0008},   // genLevelCtrl (EP2)
};

extern const PreparedDefinition kDef_D501_34{
    .zigbee_models=kModels_D501_34, .zigbee_models_count=sizeof(kModels_D501_34)/sizeof(kModels_D501_34[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="501.34", .vendor="Paulmann",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D501_34, .from_zigbee_count=sizeof(kFz_D501_34)/sizeof(kFz_D501_34[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map=kEndpoints_D501_34,
    .endpoint_map_count=sizeof(kEndpoints_D501_34)/sizeof(kEndpoints_D501_34[0]),
    .endpoint_action_suffix=true,
};

}  // namespace zhc::devices::paulmann
