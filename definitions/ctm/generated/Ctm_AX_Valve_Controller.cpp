// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ctm AX_Valve_Controller — auto-generated.
// AX Valve Controller, water shutoff valve controller
// z2m-source: ctm.ts #AX_Valve_Controller.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
const FzConverter* const kFz_AX_Valve_Controller[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};
const TzConverter* const kTz_AX_Valve_Controller[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_AX_Valve_Controller[] = { "AX Valve Controller" };

}  // namespace


// Manual: water_leak label (z2m `ctm_water_leak_alarm` semantics).
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_AX_Valve_Controller{
    .zigbee_models=kModels_AX_Valve_Controller, .zigbee_models_count=sizeof(kModels_AX_Valve_Controller)/sizeof(kModels_AX_Valve_Controller[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AX_Valve_Controller", .vendor="Ctm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AX_Valve_Controller, .from_zigbee_count=sizeof(kFz_AX_Valve_Controller)/sizeof(kFz_AX_Valve_Controller[0]),
    .to_zigbee=kTz_AX_Valve_Controller, .to_zigbee_count=sizeof(kTz_AX_Valve_Controller)/sizeof(kTz_AX_Valve_Controller[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ctm
