// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nodon IRB-4-1-00 — hand-rewrite (climate enum keys + humidity).
// IR Blaster (climate-style controller; uses fz.thermostat + fz.fan).
//
// z2m wires the standard ZCL thermostat + fan + an extra `m.humidity()`.
// The auto-generated port had `system_mode` and `fan_mode` typed as
// `Binary` — they are enum-of-strings (off/heat/cool/auto/dry/fan_only
// and off/low/medium/high/auto). Cooling setpoint, min/max heat/cool
// setpoint limits, control_sequence_of_operation and
// ac_louver_position are not yet wired — see TODO.
//
// z2m-source: nodon.ts #IRB-4-1-00.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_IRB_4_1_00[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzFanMode,
    &::zhc::generic::kFzHumidity,
};
const TzConverter* const kTz_IRB_4_1_00[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_IRB_4_1_00[] = { "IRB-4-1-00" };

}  // namespace


// Hand-aligned to z2m exposes (cooling/min/max setpoints + louver TODO).
constexpr Expose kAutoExposes[] = {
    {"local_temperature",        ExposeType::Numeric, Access::State,    "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode",              ExposeType::Enum,    Access::StateSet, nullptr,
        "off / heat / cool / auto / dry / fan_only", nullptr, 0},
    {"fan_mode",                 ExposeType::Enum,    Access::StateSet, nullptr,
        "off / low / medium / high / auto", nullptr, 0},
    {"humidity",                 ExposeType::Numeric, Access::State,    "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},
    {1, 0x0202},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_IRB_4_1_00{
    .zigbee_models=kModels_IRB_4_1_00, .zigbee_models_count=sizeof(kModels_IRB_4_1_00)/sizeof(kModels_IRB_4_1_00[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="IRB-4-1-00", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_IRB_4_1_00, .from_zigbee_count=sizeof(kFz_IRB_4_1_00)/sizeof(kFz_IRB_4_1_00[0]),
    .to_zigbee=kTz_IRB_4_1_00, .to_zigbee_count=sizeof(kTz_IRB_4_1_00)/sizeof(kTz_IRB_4_1_00[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
