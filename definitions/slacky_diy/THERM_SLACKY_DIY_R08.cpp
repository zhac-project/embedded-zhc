// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: SlackyDiy THERM_SLACKY_DIY_R08 — Tuya floor-heating thermostat
// (custom firmware), zigbeeModel "Tuya_Thermostat_r08".
//
// z2m-source: slacky_diy.ts #THERM_SLACKY_DIY_R08.  z2m wires the device as a
// thermostat on a custom `hvacThermostat` cluster (ID 0x0201) via
// fromZigbee: localFromZigbeeThermostat, toZigbee: localToZigbeeThermostat,
// configure: configureCommon — identical to its 13 siblings R01..R0D. Its
// exposes include e.climate().withLocalTemperature().withSetpoint(
// "occupied_heating_setpoint",5,45,0.5).withSystemMode(["off","heat"]) plus
// child_lock / programming_operation_mode / setpoint limits / eco_mode /
// external_temperature_calibration / weekly schedules.
//
// The auto-generated def (definitions/slacky_diy/generated/
// Sla_THERM_SLACKY_DIY_R08.cpp) mis-lowered this device as a battery + on-off
// switch (kFzBattery + kFzOnOff, exposes state/battery/voltage, binds
// genPowerCfg 0x0001 + genOnOff 0x0006). That is the wrong device class
// entirely: the real R08 has no on/off or battery cluster — it surfaces a
// thermostat. A thermostat that reports a phantom switch and never surfaces
// its temperature/setpoint/mode is broken, so the def is graduated to a
// hand-maintained Tier-2 parent override that matches its R01..R0D siblings.
//
// The generic kFzThermostat / kTzThermostat converters decode/encode the
// three standard ZCL hvacThermostat attributes z2m's fz.thermostat /
// tz.thermostat_* cover:
//   * attr 0x0000 LocalTemperature        (s16, 0.01 °C) -> "local_temperature"
//   * attr 0x0012 OccupiedHeatingSetpoint (s16, 0.01 °C) -> "current_heating_setpoint"
//   * attr 0x001C SystemMode              (enum8)        -> "system_mode"
// This is exactly the parity surface the 13 sibling THERM defs ship. The
// device's richer controls (child_lock, schedules, eco_mode, frost/heat
// protect, calibration, deadzone, etc.) ride manufacturer-specific
// attributes of the custom 0x0201 cluster and z2m's `e.climate()` composite;
// the ZHC runtime has no generic decoder for those manuf attrs and ships
// climate flat (no ExposeType::Climate), so they are deferred here just as
// they are for every other slacky_diy / tuya thermostat — infrastructure
// work, not an R08-specific gap.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {

constexpr const char* kModels_THERM_SLACKY_DIY_R08[] = { "Tuya_Thermostat_r08" };

constexpr Expose kExposes_THERM_SLACKY_DIY_R08[] = {
    { "local_temperature",        ExposeType::Numeric, Access::State,
      "C", nullptr, nullptr, 0 },
    { "current_heating_setpoint", ExposeType::Numeric, Access::StateSet,
      "C", nullptr, nullptr, 0 },
    { "system_mode",              ExposeType::Binary,  Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};

const FzConverter* const kFz_THERM_SLACKY_DIY_R08[] = {
    &::zhc::generic::kFzThermostat,
};

const TzConverter* const kTz_THERM_SLACKY_DIY_R08[] = {
    &::zhc::generic::kTzThermostat,
};

constexpr BindingSpec kBindings_THERM_SLACKY_DIY_R08[] = {
    { 1, 0x0201 },   // hvacThermostat
};

}  // namespace

extern const PreparedDefinition kDef_THERM_SLACKY_DIY_R08{
    .zigbee_models            = kModels_THERM_SLACKY_DIY_R08,
    .zigbee_models_count      = sizeof(kModels_THERM_SLACKY_DIY_R08)/sizeof(kModels_THERM_SLACKY_DIY_R08[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "THERM_SLACKY_DIY_R08",
    .vendor                   = "SlackyDiy",
    .meta                     = nullptr,
    .exposes                  = kExposes_THERM_SLACKY_DIY_R08,
    .exposes_count            = sizeof(kExposes_THERM_SLACKY_DIY_R08)/sizeof(kExposes_THERM_SLACKY_DIY_R08[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_THERM_SLACKY_DIY_R08,
    .from_zigbee_count        = sizeof(kFz_THERM_SLACKY_DIY_R08)/sizeof(kFz_THERM_SLACKY_DIY_R08[0]),
    .to_zigbee                = kTz_THERM_SLACKY_DIY_R08,
    .to_zigbee_count          = sizeof(kTz_THERM_SLACKY_DIY_R08)/sizeof(kTz_THERM_SLACKY_DIY_R08[0]),
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_THERM_SLACKY_DIY_R08,
    .bindings_count           = sizeof(kBindings_THERM_SLACKY_DIY_R08)/sizeof(kBindings_THERM_SLACKY_DIY_R08[0]),
};

}  // namespace zhc::devices::slacky_diy
