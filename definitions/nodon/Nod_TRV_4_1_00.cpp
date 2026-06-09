// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Nodon TRV-4-1-00 — hand-rewrite (manuSpec + extra exposes).
// Thermostatic Radiator Valve.
//
// z2m wires `m.battery() + nodonModernExtend.trvMode() +
//           nodonModernExtend.valvePosition() + fz.thermostat` plus
// 11 `tz.thermostat_*` setters. NodOn re-uses the eurotronic
// manuSpec attribute IDs (manu = NXP_SEMICONDUCTORS in z2m's source —
// the device speaks both NXP and NodOn manu codes). For trv_mode and
// valve_position we use the NODON manu code (0x128B) per
// `nodonModernExtend`.
//
// trv_mode (0x4000) / valve_position (0x4001) now decode via
// kFzNodonTrvExtras — z2m's enumLookup/numeric modernExtends generate a
// read path, but the generic kFzThermostat only handles 0x0000 /
// 0x0012 / 0x001C, so these were write-only (dead STATE_GET) before.
//
// Still-TODO: child_lock, mirror_display, eurotronic_error_status,
// unoccupied_heating_setpoint, min/max_heat_setpoint_limit,
// setpoint_raise_lower, control_sequence_of_operation,
// pi_heating_demand, local_temperature_calibration. The generic
// `kTzThermostat` only covers occupied_heating_setpoint + system_mode.
//
// z2m-source: nodon.ts #TRV-4-1-00.
#include "definitions/_generic/_shared.hpp"
#include "definitions/nodon/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_TRV_4_1_00[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzThermostat,
    &kFzNodonTrvExtras,
};
const TzConverter* const kTz_TRV_4_1_00[] = {
    &::zhc::generic::kTzThermostat,
    &kTzNodonTrvMode,
    &kTzNodonValvePosition,
};
constexpr const char* kModels_TRV_4_1_00[] = { "TRV-4-1-00" };

}  // namespace


// Hand-aligned to z2m exposes (subset — see header TODO list).
constexpr Expose kAutoExposes[] = {
    {"battery",                  ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage",                  ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"local_temperature",        ExposeType::Numeric, Access::State,    "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode",              ExposeType::Enum,    Access::StateSet, nullptr,
        "off / auto / heat", nullptr, 0},
    {"trv_mode",                 ExposeType::Enum,    Access::StateSet, nullptr,
        "auto / valve_position_mode / manual", nullptr, 0, ExposeCategory::Config},
    {"valve_position",           ExposeType::Numeric, Access::StateSet, "%",
        "Direct valve position (0..100) when trv_mode = valve_position_mode.",
        nullptr, 0, ExposeCategory::Config},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0201},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_TRV_4_1_00{
    .zigbee_models=kModels_TRV_4_1_00, .zigbee_models_count=sizeof(kModels_TRV_4_1_00)/sizeof(kModels_TRV_4_1_00[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TRV-4-1-00", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TRV_4_1_00, .from_zigbee_count=sizeof(kFz_TRV_4_1_00)/sizeof(kFz_TRV_4_1_00[0]),
    .to_zigbee=kTz_TRV_4_1_00, .to_zigbee_count=sizeof(kTz_TRV_4_1_00)/sizeof(kTz_TRV_4_1_00[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
