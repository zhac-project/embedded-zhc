// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AuroraLighting AU-A1ZBRC — hand-curated (was wrong-bundled).
// AOne smart remote — single-endpoint battery remote.
//
// z2m drives it with fromZigbee=[fz.battery, fz.command_on, fz.command_off,
// fz.command_step, fz.command_recall, fz.command_store], toZigbee=[] and
// exposes action(["on","off","brightness_step_up","brightness_step_down",
// "recall_1","store_1"]) + battery. The auto-generator collapsed this into
// a settable on/off light (kFzOnOff + kTzOnOff + writable `state`) and
// dropped the `action` decode entirely — every button press was dead and
// the device falsely advertised a relay.
//
// Fix (mirrors paulmann 501.41 / icasa KPD14S): swap the on/off fz/tz for
// the genOnOff / genLevelCtrl(step) / genScenes(recall,store) command
// decoders, expose `action`, drop the phantom state + empty the toolbox.
// Single endpoint → `action` is emitted bare (no suffix).
//
// z2m-source: aurora_lighting.ts #AU-A1ZBRC.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
const FzConverter* const kFz_AU_A1ZBRC[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,    // brightness_step_up / _down
    &::zhc::generic::kFzCommandRecall,  // recall_<scene>
    &::zhc::generic::kFzCommandStore,   // store_<scene>
};
constexpr const char* kModels_AU_A1ZBRC[] = { "Remote50AU" };

constexpr Expose kExp_AU_A1ZBRC[] = {
    // action carries on / off / brightness_step_up / brightness_step_down /
    // recall_1 / store_1 (the z2m action set for this remote).
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_AU_A1ZBRC[] = {
    {1, 0x0006},  // genOnOff   (command_on / command_off)
    {1, 0x0008},  // genLevelCtrl (command_step)
    {1, 0x0001},  // genPowerCfg (battery)
    {1, 0x0005},  // genScenes  (command_recall / command_store)
};

}  // namespace

extern const PreparedDefinition kDef_AU_A1ZBRC{
    .zigbee_models=kModels_AU_A1ZBRC, .zigbee_models_count=sizeof(kModels_AU_A1ZBRC)/sizeof(kModels_AU_A1ZBRC[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1ZBRC", .vendor="AuroraLighting",
    .meta=nullptr, .exposes=kExp_AU_A1ZBRC, .exposes_count=sizeof(kExp_AU_A1ZBRC)/sizeof(kExp_AU_A1ZBRC[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AU_A1ZBRC, .from_zigbee_count=sizeof(kFz_AU_A1ZBRC)/sizeof(kFz_AU_A1ZBRC[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_AU_A1ZBRC, .bindings_count=sizeof(kBind_AU_A1ZBRC)/sizeof(kBind_AU_A1ZBRC[0]),
};

}  // namespace zhc::devices::aurora_lighting
