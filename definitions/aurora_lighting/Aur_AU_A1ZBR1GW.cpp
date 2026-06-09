// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AuroraLighting AU-A1ZBR1GW — hand-curated battery rotary dimmer.
// AOne one gang wireless battery rotary dimmer — single-endpoint (EP1).
//
// z2m builds it from the `batteryRotaryDimmer(1)` helper:
//   fromZigbee=[fz.battery, fz.command_on, fz.command_off, fz.command_step,
//               fz.command_step_color_temperature], toZigbee=[],
//   exposes=[battery, action(["on","off","brightness_step_up",
//     "brightness_step_down","color_temperature_step_up",
//     "color_temperature_step_down"])].
// The auto-generator dropped the entire action stream — it wired kFzOnOff
// (a genOnOff *attribute* decoder, which this remote never reports) plus a
// phantom settable `state`, so every rotary turn / press was dead.
//
// Fix (mirrors paulmann 501.41): swap to the genOnOff / genLevelCtrl(step)
// / lightingColorCtrl(stepColorTemp) command decoders + battery, expose
// `action`, drop the phantom state. Single endpoint → `action` is bare.
//
// z2m-source: aurora_lighting.ts #AU-A1ZBR1GW (batteryRotaryDimmer(1)).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
const FzConverter* const kFz_AU_A1ZBR1GW[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,            // brightness_step_up / _down
    &::zhc::generic::kFzCommandStepColorTemp,   // color_temperature_step_up / _down
};
constexpr const char* kModels_AU_A1ZBR1GW[] = { "1GBatteryDimmer50AU" };

constexpr Expose kExp_AU_A1ZBR1GW[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_AU_A1ZBR1GW[] = {
    {1, 0x0001},  // genPowerCfg (battery)
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl (command_step)
    {1, 0x0300},  // lightingColorCtrl (command_step_color_temperature)
};

}  // namespace

extern const PreparedDefinition kDef_AU_A1ZBR1GW{
    .zigbee_models=kModels_AU_A1ZBR1GW, .zigbee_models_count=sizeof(kModels_AU_A1ZBR1GW)/sizeof(kModels_AU_A1ZBR1GW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1ZBR1GW", .vendor="AuroraLighting",
    .meta=nullptr, .exposes=kExp_AU_A1ZBR1GW, .exposes_count=sizeof(kExp_AU_A1ZBR1GW)/sizeof(kExp_AU_A1ZBR1GW[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AU_A1ZBR1GW, .from_zigbee_count=sizeof(kFz_AU_A1ZBR1GW)/sizeof(kFz_AU_A1ZBR1GW[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_AU_A1ZBR1GW, .bindings_count=sizeof(kBind_AU_A1ZBR1GW)/sizeof(kBind_AU_A1ZBR1GW[0]),
};

}  // namespace zhc::devices::aurora_lighting
