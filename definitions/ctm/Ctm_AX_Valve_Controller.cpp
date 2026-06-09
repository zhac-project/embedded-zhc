// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ctm AX_Valve_Controller — graduated from generated/ for an
// IAS-zone bit-mapping fix.
// AX Valve Controller, water shutoff valve controller.
//
// Parity fix: same as AX_Water_Sensor — z2m's `ctm_water_leak_alarm`
// puts the leak on zoneStatus bit 1 ("water_leak"), bit 0 is
// "active_water_leak", bit 3 is "battery_low", with no "tamper". The
// auto-port used the generic `kFzIasWaterLeakAlarm` (bit 0) and a
// spurious `tamper`. On/off control (genOnOff) is correct — z2m wires
// `fz.on_off` + `tz.on_off` + `e.switch()`, NOT a cover. Re-wired the
// leak decoder onto `kFzCtmWaterLeak`.
// z2m-source: ctm.ts #AX_Valve_Controller.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ctm/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
const FzConverter* const kFz_AX_Valve_Controller[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::ctm::kFzCtmWaterLeak,
};
const TzConverter* const kTz_AX_Valve_Controller[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_AX_Valve_Controller[] = { "AX Valve Controller" };

}  // namespace


// Manual: leak on bit 1 (water_leak) + bit 0 (active_water_leak),
// battery_low on bit 3 — matches z2m `ctm_water_leak_alarm`. No tamper.
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"active_water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
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
