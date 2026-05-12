// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AuroraLighting AU-A1ZBR2GW — multi-EP battery rotary dimmer.
// AOne two gang wireless battery rotary dimmer
// z2m-source: aurora_lighting.ts #AU-A1ZBR2GW  (endpoint(): {right:1, left:2})
//
// Note z2m's labels are reversed (right=1, left=2). Per-gang exposes
// (`state_left`, `state_right`) flow through the runtime's multi-endpoint
// suffix path. fz.command_* / fz.command_step are not yet ported to a
// generic remote-style fz, so for now we expose battery + state mirror.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
constexpr const char* kModels_AU_A1ZBR2GW[] = { "2GBatteryDimmer50AU" };

const FzConverter* const kFz_AU_A1ZBR2GW[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_AU_A1ZBR2GW[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_AU_A1ZBR2GW[] = {
    { 1, 0x0001 }, { 1, 0x0006 }, { 2, 0x0006 },
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
};

}  // namespace zhc::devices::aurora_lighting
