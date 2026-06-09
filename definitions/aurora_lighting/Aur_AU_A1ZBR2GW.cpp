// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AuroraLighting AU-A1ZBR2GW — multi-EP battery rotary dimmer.
// AOne two gang wireless battery rotary dimmer
// z2m-source: aurora_lighting.ts #AU-A1ZBR2GW  (endpoint(): {right:1, left:2})
//
// z2m builds it from the `batteryRotaryDimmer(1, 2)` helper with
// meta.multiEndpoint and endpoint {right:1, left:2}:
//   fromZigbee=[fz.battery, fz.command_on, fz.command_off, fz.command_step,
//               fz.command_step_color_temperature], toZigbee=[],
//   exposes=[battery, action([... per gang ...])].
// The previous port only mirrored battery + a phantom settable `state` and
// explicitly deferred the action stream ("fz.command_* not yet ported") —
// but the generic genOnOff / genLevelCtrl / lightingColorCtrl command
// decoders DO exist (see paulmann 501.40), so every rotary turn / press
// was dead.
//
// Fix: wire the command decoders + battery and turn on endpoint_action_suffix
// so each gang keeps identity — a press on EP1 surfaces `action_right`, EP2
// surfaces `action_left` (z2m's labels are reversed: right=1, left=2).
// Battery is reported on the first endpoint only and stays a global key.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
constexpr const char* kModels_AU_A1ZBR2GW[] = { "2GBatteryDimmer50AU" };

const FzConverter* const kFz_AU_A1ZBR2GW[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,            // brightness_step_up / _down
    &::zhc::generic::kFzCommandStepColorTemp,   // color_temperature_step_up / _down
};
constexpr Expose kExp_AU_A1ZBR2GW[] = {
    // Per-gang action surfaces as action_right (EP1) / action_left (EP2).
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};
constexpr BindingSpec kBind_AU_A1ZBR2GW[] = {
    {1, 0x0001},  // genPowerCfg (battery — first EP only)
    {1, 0x0006}, {1, 0x0008}, {1, 0x0300},  // gang 1 (right): onOff/level/colorCtrl
    {2, 0x0006}, {2, 0x0008}, {2, 0x0300},  // gang 2 (left)
};
constexpr ::zhc::EndpointLabel kEndpoints_AU_A1ZBR2GW[] = {
    {"right", 1}, {"left", 2},
};
}  // namespace

extern const PreparedDefinition kDef_AU_A1ZBR2GW{
    .zigbee_models=kModels_AU_A1ZBR2GW, .zigbee_models_count=sizeof(kModels_AU_A1ZBR2GW)/sizeof(kModels_AU_A1ZBR2GW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1ZBR2GW", .vendor="AuroraLighting",
    .meta=nullptr, .exposes=kExp_AU_A1ZBR2GW, .exposes_count=sizeof(kExp_AU_A1ZBR2GW)/sizeof(kExp_AU_A1ZBR2GW[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AU_A1ZBR2GW, .from_zigbee_count=sizeof(kFz_AU_A1ZBR2GW)/sizeof(kFz_AU_A1ZBR2GW[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings       = kBind_AU_A1ZBR2GW,
    .bindings_count = sizeof(kBind_AU_A1ZBR2GW)/sizeof(kBind_AU_A1ZBR2GW[0]),
    .endpoint_map       = kEndpoints_AU_A1ZBR2GW,
    .endpoint_map_count = sizeof(kEndpoints_AU_A1ZBR2GW)/sizeof(kEndpoints_AU_A1ZBR2GW[0]),
    .endpoint_action_suffix = true,
};

}  // namespace zhc::devices::aurora_lighting
