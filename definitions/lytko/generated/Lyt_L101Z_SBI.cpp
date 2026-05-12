// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lytko L101Z-SBI — auto-generated.
// Single channel Zigbee thermostat
// z2m-source: lytko.ts #L101Z-SBI.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lytko/_shared.hpp"

namespace zhc::devices::lytko {
namespace {
const FzConverter* const kFz_L101Z_SBI[] = {
    &::zhc::generic::kFzThermostat,
    // TODO: lytko humidity/temperature endpoint l2 + lytko-local fz for
    // attrs 0x7700/0x7701 (sensor_type, target_temp_first) on
    // hvacThermostat / hvacUserInterfaceCfg.
};
const TzConverter* const kTz_L101Z_SBI[] = {
    &::zhc::generic::kTzThermostat,
    &kTzLytkoSensorType,
    &kTzLytkoTargetTempFirst,
    &kTzLytkoOccupiedSetback,
    &kTzLytkoKeypadLockout,
    &kTzLytkoBrightnessActive,
    &kTzLytkoBrightnessStandby,
};
constexpr const char* kModels_L101Z_SBI[] = { "L101Z-SBI" };

}  // namespace


// Exposes mirror lytko.ts #L101Z-SBI. Endpoint suffixes (l1/l2/l3)
// are not encoded yet — runtime treats keys as flat. Humidity /
// temperature on l2 fall back to standard kFzThermostat-emitted keys.
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
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},  // hvacThermostat
    {1, 0x0204},  // hvacUserInterfaceCfg (keypad / brightness)
    {2, 0x0402},  // msTemperatureMeasurement on l2
    {2, 0x0405},  // msRelativeHumidity     on l2
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_L101Z_SBI{
    .zigbee_models=kModels_L101Z_SBI, .zigbee_models_count=sizeof(kModels_L101Z_SBI)/sizeof(kModels_L101Z_SBI[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L101Z-SBI", .vendor="Lytko",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_L101Z_SBI, .from_zigbee_count=sizeof(kFz_L101Z_SBI)/sizeof(kFz_L101Z_SBI[0]),
    .to_zigbee=kTz_L101Z_SBI, .to_zigbee_count=sizeof(kTz_L101Z_SBI)/sizeof(kTz_L101Z_SBI[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lytko
