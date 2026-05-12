// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lytko L101Z-SBN — auto-generated.
// Single channel Zigbee thermostat
// z2m-source: lytko.ts #L101Z-SBN.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lytko/_shared.hpp"

namespace zhc::devices::lytko {
namespace {
const FzConverter* const kFz_L101Z_SBN[] = {
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_L101Z_SBN[] = {
    &::zhc::generic::kTzThermostat,
    &kTzLytkoSensorType,
    &kTzLytkoTargetTempFirst,
    &kTzLytkoOccupiedSetback,
    &kTzLytkoKeypadLockout,
    &kTzLytkoBrightnessActive,
    &kTzLytkoBrightnessStandby,
};
constexpr const char* kModels_L101Z_SBN[] = { "L101Z-SBN" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sensor_type", ExposeType::Enum, Access::StateSet, nullptr, "External sensor resistance kOhm", nullptr, 0},
    {"target_temp_first", ExposeType::Binary, Access::StateSet, nullptr, "Display target/current first", nullptr, 0},
    {"keypad_lockout", ExposeType::Enum, Access::StateSet, nullptr, "Physical input lock", nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, "%", "Display brightness (active)", nullptr, 0},
    {"brightness_standby", ExposeType::Numeric, Access::StateSet, "%", "Display brightness (standby)", nullptr, 0},
    {"occupied_setback", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},
    {1, 0x0204},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_L101Z_SBN{
    .zigbee_models=kModels_L101Z_SBN, .zigbee_models_count=sizeof(kModels_L101Z_SBN)/sizeof(kModels_L101Z_SBN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L101Z-SBN", .vendor="Lytko",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_L101Z_SBN, .from_zigbee_count=sizeof(kFz_L101Z_SBN)/sizeof(kFz_L101Z_SBN[0]),
    .to_zigbee=kTz_L101Z_SBN, .to_zigbee_count=sizeof(kTz_L101Z_SBN)/sizeof(kTz_L101Z_SBN[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lytko
