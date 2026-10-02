// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Atlantic Group Nirvana+ horizontal radiator 750 W (z2m v26.115.1 window),
// white labels 100052992500 (1000 W) / 100052992700 (1500 W); the firmware may
// append a NUL to the model id, which ends the C string here anyway.
// z2m nirvanaExtend({horizontal: true}): thermostat 7..30 on endpoint 1,
// occupancy, power (haElectricalMeasurement, W) and energy (Wh), and the
// user-interface cluster (keypad lockout, display unit) on endpoint 230.
// z2m-source: atlantic.ts #100052992400.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::atlantic {
namespace {
// currentSummDelivered is in Wh (z2m forces multiplier 1 / divisor 1000).
constexpr ::zhc::generic::ZclAttrRow kMeterRows[] = { { 0x0000, "energy", 1000 } };
constexpr ::zhc::generic::ZclAttrMap kMeterMap{ kMeterRows, 1 };
constexpr FzConverter kFzEnergyWh = ::zhc::generic::zcl_attr_fz("seMetering", &kMeterMap);
constexpr ::zhc::generic::ZclAttrRow kPowerRows[] = { { 0x050B, "power" } };
constexpr ::zhc::generic::ZclAttrMap kPowerMap{ kPowerRows, 1 };
constexpr FzConverter kFzPower = ::zhc::generic::zcl_attr_fz("haElectricalMeasurement", &kPowerMap);
constexpr TzConverter kTzKeypad230 = ::zhc::generic::zcl_write_tz("hvacUserInterfaceCfg", 0x0204, &::zhc::generic::kKeypadLockoutSpec, 230);
constexpr TzConverter kTzDisplay230 = ::zhc::generic::zcl_write_tz("hvacUserInterfaceCfg", 0x0204, &::zhc::generic::kTemperatureDisplayModeSpec, 230);
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzHvacUserInterface,
    &kFzPower,
    &kFzEnergyWh,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzThermostat,
    &kTzKeypad230,
    &kTzDisplay230,
};
constexpr const char* kOpts2[] = { "off", "heat" };
constexpr const char* kOpts4[] = { "unlock", "lock1", "lock2", "lock3", "lock4", "lock5" };
constexpr const char* kOpts5[] = { "celsius", "fahrenheit" };
constexpr Expose kExp[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 7, 30, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts2, 2},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"keypad_lockout", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts4, 6, ExposeCategory::Config},
    {"temperature_display_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts5, 2, ExposeCategory::Config},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "100052992400", "100052992500", "100052992700" };
constexpr WhiteLabel kWL[] = { {"Atlantic", "100052992500"}, {"Atlantic", "100052992700"} };
constexpr BindingSpec kBind[] = {
    {1, 0x0201},
    {1, 0x0406},
    {1, 0x0B04},
    {1, 0x0702},
    {230, 0x0204},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0201, 0x0000, 0x29, 0, 3600, 10, 0},
    {1, 0x0201, 0x0012, 0x29, 0, 3600, 10, 0},
    {1, 0x0201, 0x001C, 0x30, 10, 3600, 0, 0},
    {1, 0x0406, 0x0000, 0x18, 0, 3600, 0, 0},
    {1, 0x0B04, 0x050B, 0x29, 10, 65000, 5, 0},
    {1, 0x0702, 0x0000, 0x25, 10, 3600, 100, 0},
    {230, 0x0204, 0x0001, 0x30, 10, 3600, 0, 0},
};
}  // namespace

extern const PreparedDefinition kDef_D100052992400{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="100052992400", .vendor="Atlantic",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL, .white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::atlantic
