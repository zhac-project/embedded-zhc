// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lytko L101Ze-SBN — auto-generated.
// Single channel thermostat with big display (modernExtend definition).
// z2m-source: lytko.ts #L101Ze-SBN.
//
// Hand-rewritten 2026-04-28: replaces an incorrect battery+onOff stub
// emitted by the legacy generator. The L101Ze family is defined via
// `modernExtend`'s `thermostat({...})` builder + a few `lytkoExtend.*`
// custom-cluster wrappers. We map all of those onto the canonical
// generic kFzThermostat / kTzThermostat plus the shared Lytko
// manuSpec writes.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lytko/_shared.hpp"

namespace zhc::devices::lytko {
namespace {

constexpr const char* kModels_L101Ze_SBN[] = { "L101Ze-SBN" };

const FzConverter* const kFz_L101Ze_SBN[] = {
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_L101Ze_SBN[] = {
    &::zhc::generic::kTzThermostat,
    &kTzLytkoSensorType,
    &kTzLytkoTargetTempFirst,
    &kTzLytkoOccupiedSetback,
    &kTzLytkoKeypadLockout,
    &kTzLytkoBrightnessActive,
    &kTzLytkoBrightnessStandby,
};

constexpr Expose kExp_L101Ze_SBN[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_mode", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"sensor_type", ExposeType::Enum, Access::StateSet, nullptr, "External sensor resistance kOhm", nullptr, 0},
    {"target_temp_first", ExposeType::Binary, Access::StateSet, nullptr, "Display target/current first", nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, "Physical input lock", nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, "%", "Display brightness (active)", nullptr, 0},
    {"brightness_standby", ExposeType::Numeric, Access::StateSet, "%", "Display brightness (standby)", nullptr, 0},
    {"occupied_setback", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_L101Ze_SBN[] = {
    {3, 0x0201},  // hvacThermostat on endpoint 3
    {1, 0x0204},  // hvacUserInterfaceCfg on endpoint 1 (child_lock)
};
constexpr ::zhc::EndpointLabel kEndpoints_L101Ze_SBN[] = { {"3", 3} };

}  // namespace

extern const PreparedDefinition kDef_L101Ze_SBN{
    .zigbee_models=kModels_L101Ze_SBN, .zigbee_models_count=sizeof(kModels_L101Ze_SBN)/sizeof(kModels_L101Ze_SBN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L101Ze-SBN", .vendor="Lytko",
    .meta=nullptr, .exposes=kExp_L101Ze_SBN, .exposes_count=sizeof(kExp_L101Ze_SBN)/sizeof(kExp_L101Ze_SBN[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_L101Ze_SBN, .from_zigbee_count=sizeof(kFz_L101Ze_SBN)/sizeof(kFz_L101Ze_SBN[0]),
    .to_zigbee=kTz_L101Ze_SBN, .to_zigbee_count=sizeof(kTz_L101Ze_SBN)/sizeof(kTz_L101Ze_SBN[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_L101Ze_SBN, .bindings_count=sizeof(kBind_L101Ze_SBN)/sizeof(kBind_L101Ze_SBN[0]),
    .endpoint_map       = kEndpoints_L101Ze_SBN,
    .endpoint_map_count = sizeof(kEndpoints_L101Ze_SBN)/sizeof(kEndpoints_L101Ze_SBN[0]),
};

}  // namespace zhc::devices::lytko
