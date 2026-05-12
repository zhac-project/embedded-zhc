// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Robb ROB_200-025-0 — hand-rewritten from a wrong on/off bundle.
// Zigbee 8 button wall switch.
// z2m-source: robb.ts #ROB_200-025-0.
//
// Actions: on_<n>, off_<n>, brightness_move_up_<n>, brightness_move_down_<n>, brightness_stop_<n> for n in 1..4.
// No to_zigbee path — this is a battery-powered remote/scene controller,
// the device is the genOnOff/genLevelCtrl client and we never write back.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::robb {
namespace {
const FzConverter* const kFz_ROB_200_025_0[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_ROB_200_025_0[] = { "ROB_200-025-0" };

constexpr ::zhc::EndpointLabel kEndpoints_ROB_200_025_0[] = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4} };
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0008},
};

extern const PreparedDefinition kDef_ROB_200_025_0{
    .zigbee_models=kModels_ROB_200_025_0, .zigbee_models_count=sizeof(kModels_ROB_200_025_0)/sizeof(kModels_ROB_200_025_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ROB_200-025-0", .vendor="Robb",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ROB_200_025_0, .from_zigbee_count=sizeof(kFz_ROB_200_025_0)/sizeof(kFz_ROB_200_025_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_ROB_200_025_0,
    .endpoint_map_count = sizeof(kEndpoints_ROB_200_025_0)/sizeof(kEndpoints_ROB_200_025_0[0]),
};

}  // namespace zhc::devices::robb
