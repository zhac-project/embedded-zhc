// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ctm mTouch_One — auto-generated.
// mTouch One OP, touch thermostat
// z2m-source: ctm.ts #mTouch_One.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ctm/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
// mTouch One z2m: hvacThermostat with extensive ctm* manuSpec attrs.
// kFzThermostat already decodes localTemp / occupiedHeatingSetpoint /
// systemMode; ctm-specific keys (load, mean_power, floor_temp, etc.)
// surface as raw attribute IDs until ZHC gains a vendor fz_ctm_thermo
// decoder. Setpoint writes (load, display_text, child_lock, frost_guard,
// preset) wire through the canonical ZclWriteSpec converters.
const FzConverter* const kFz_mTouch_One[] = {
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_mTouch_One[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::ctm::kTzCtmLoad,
    &::zhc::ctm::kTzCtmDisplayText,
    &::zhc::ctm::kTzCtmFrostGuard,
    &::zhc::ctm::kTzCtmChildLockThermostat,
    &::zhc::ctm::kTzCtmPreset,
};
constexpr const char* kModels_mTouch_One[] = { "mTouch One" };

}  // namespace


// Manual: add ctm-specific writeable surfaces. Read-only ctm attrs
// (mean_power, floor_temp, regulation_mode, regulator_setpoint,
// frost_guard_setpoint, external_temp, external_sensor_source, air_temp,
// floor_sensor_error, air_sensor_error, weekly_timer, sensor,
// regulator_mode, night_switching, power_status) need a vendor fz
// decoder — TODO.
constexpr Expose kAutoExposes[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"load", ExposeType::Numeric, Access::StateSet, "W", nullptr, nullptr, 0},
    {"display_text", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"frost_guard", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},
};

extern const PreparedDefinition kDef_mTouch_One{
    .zigbee_models=kModels_mTouch_One, .zigbee_models_count=sizeof(kModels_mTouch_One)/sizeof(kModels_mTouch_One[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="mTouch_One", .vendor="Ctm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_mTouch_One, .from_zigbee_count=sizeof(kFz_mTouch_One)/sizeof(kFz_mTouch_One[0]),
    .to_zigbee=kTz_mTouch_One, .to_zigbee_count=sizeof(kTz_mTouch_One)/sizeof(kTz_mTouch_One[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ctm
