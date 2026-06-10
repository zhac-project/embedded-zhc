// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: TheLightGroup S57007 — hand-curated (was wrong-bundled).
// SLC SmartOne Zigbee remote control IP20 — battery-powered 3-button scene
// remote, NOT a controllable on/off light. The auto-generator saw genOnOff
// and emitted kFzOnOff + kTzOnOff + a settable `state`, so every button press
// was dead (decoded to nothing) and the device falsely advertised a writable
// relay.
//
// Reality: z2m builds it from m.commandsOnOff + m.commandsScenes over three
// device endpoints {1,2,3} (m.deviceEndpoints). It only emits genOnOff /
// genScenes cluster commands → action on/off/recall_<n>/store_<n>. We carry
// the per-endpoint identity via the existing endpoint_map {1,2,3} plus
// endpoint_action_suffix (which the auto-port omitted), so `action` becomes
// action_1..action_3 with the command label in the value (mirrors paulmann
// 501.40). The phantom settable `state` + on/off toolbox are dropped.
//
// z2m extend: m.deviceEndpoints({1,2,3}), m.battery(),
//             m.commandsOnOff({endpointNames:[1,2,3]}),
//             m.commandsScenes({endpointNames:[1,2,3]}).
// z2m toZigbee: [] (cannot be controlled).
//
// z2m-source: the_light_group.ts #S57007.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::the_light_group {
namespace {
const FzConverter* const kFz_S57007[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandStore,
    &::zhc::generic::kFzBattery,
};
// Toolbox is empty — the remote has no controllable state (z2m toZigbee: []).
constexpr const char* kModels_S57007[] = { "S57007" };

// Per-endpoint label map: EP1..EP3 → "1".."3", driving the action_<n> suffix
// (paired with endpoint_action_suffix) so each button keeps its identity.
constexpr ::zhc::EndpointLabel kEndpoints_S57007[] = { {"1", 1}, {"2", 2}, {"3", 3} };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    // `action` value carries on/off/recall_<n>/store_<n>; the dispatcher
    // suffixes the key per source endpoint → action_1..action_3.
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff    (EP1)
    {1, 0x0005},   // genScenes   (EP1)
    {1, 0x0001},   // genPowerCfg (EP1, battery)
    {2, 0x0006},   // genOnOff    (EP2)
    {2, 0x0005},   // genScenes   (EP2)
    {3, 0x0006},   // genOnOff    (EP3)
    {3, 0x0005},   // genScenes   (EP3)
};

extern const PreparedDefinition kDef_S57007{
    .zigbee_models=kModels_S57007, .zigbee_models_count=sizeof(kModels_S57007)/sizeof(kModels_S57007[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S57007", .vendor="TheLightGroup",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_S57007, .from_zigbee_count=sizeof(kFz_S57007)/sizeof(kFz_S57007[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_S57007,
    .endpoint_map_count = sizeof(kEndpoints_S57007)/sizeof(kEndpoints_S57007[0]),
    .endpoint_action_suffix=true,
};

}  // namespace zhc::devices::the_light_group
