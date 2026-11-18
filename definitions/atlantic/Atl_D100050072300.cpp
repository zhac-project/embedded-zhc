// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Atlantic Group 100050072300, Sauter Kartala connected radiator
// horizontal 1000 W (new in z2m v26.126.0..v26.133.0).
// m.thermostat: local temperature, heating setpoint 7..28, system_mode off/heat;
// running_state follows the Atlantic heatingDemand duty cycle (heat while > 0,
// else idle), not runningState. m.occupancy; power from haElectricalMeasurement
// activePower (W, multiplier/divisor 1) and energy from seMetering in Wh.
// Manufacturer-specific hvacThermostat attributes (ATLANTIC_GROUP 0x125B):
// window_detection 0x428A (Bool, writable), window_open 0x4298 (Bool),
// effective_heating_setpoint 0x42AB (/100 °C), pi_heating_demand 0x42AC (%),
// internal_temperature 0x42B4 (/100 °C).
// keypad_lockout: hvacUserInterfaceCfg keypadLockout with the device's own
// lookup {unlocked 0, partial 1, settings_locked 2, full 5} (enum8).
// Not ported: identify; the running_state read behind STATE_GET.
// z2m-source: atlantic.ts #100050072300.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::atlantic {
namespace {

constexpr std::uint16_t kAtlanticMfgCode = 0x125B;  // Zcl.ManufacturerCode.ATLANTIC_GROUP

// heatingDemand (0x42AC = 17068) > 0 -> "heat", else "idle".
bool fz_running_state(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                      RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (!msg.manufacturer_specific || msg.manufacturer_code != kAtlanticMfgCode) return false;
    const Value* v = msg.payload.find("17068");
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::StringRef; o.str = v->u > 0 ? "heat" : "idle";
    return out.put("running_state", o);
}
constexpr FzConverter kFzRunningState = [] {
    FzConverter c = ::zhc::generic::zcl_attr_fz("hvacThermostat", nullptr);
    c.fn.zcl_fn = fz_running_state;
    return c;
}();

constexpr ::zhc::generic::ZclAttrRow kManuRows[] = {
    { 0x428A, "window_detection", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x4298, "window_open", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x42AB, "effective_heating_setpoint", 100 },
    { 0x42AC, "pi_heating_demand" },
    { 0x42B4, "internal_temperature", 100 },
};
constexpr ::zhc::generic::ZclAttrMap kManuMap{ kManuRows, sizeof(kManuRows)/sizeof(kManuRows[0]), kAtlanticMfgCode };
constexpr FzConverter kFzManu = ::zhc::generic::zcl_attr_fz("hvacThermostat", &kManuMap);
constexpr ::zhc::generic::ZclWriteSpec kWindowDetectionSpec{ "window_detection", 0x428A, 0x10, kAtlanticMfgCode, nullptr, 0 };
constexpr TzConverter kTzWindowDetection = ::zhc::generic::zcl_write_tz("hvacThermostat", 0x0201, &kWindowDetectionSpec);

constexpr ::zhc::generic::ZclWriteLookup kKeypadModes[] = {
    {"unlocked", 0}, {"partial", 1}, {"settings_locked", 2}, {"full", 5} };
constexpr ::zhc::generic::ZclAttrRow kUiRows[] = {
    { 0x0001, "keypad_lockout", 1, kKeypadModes, sizeof(kKeypadModes)/sizeof(kKeypadModes[0]) },
};
constexpr ::zhc::generic::ZclAttrMap kUiMap{ kUiRows, sizeof(kUiRows)/sizeof(kUiRows[0]) };
constexpr FzConverter kFzKeypad = ::zhc::generic::zcl_attr_fz("hvacUserInterfaceCfg", &kUiMap);
constexpr ::zhc::generic::ZclWriteSpec kKeypadSpec{
    "keypad_lockout", 0x0001, 0x30, 0, kKeypadModes, sizeof(kKeypadModes)/sizeof(kKeypadModes[0]) };
constexpr TzConverter kTzKeypad = ::zhc::generic::zcl_write_tz("hvacUserInterfaceCfg", 0x0204, &kKeypadSpec);

// currentSummDelivered is in Wh (z2m forces multiplier 1 / divisor 1000).
constexpr ::zhc::generic::ZclAttrRow kMeterRows[] = { { 0x0000, "energy", 1000 } };
constexpr ::zhc::generic::ZclAttrMap kMeterMap{ kMeterRows, sizeof(kMeterRows)/sizeof(kMeterRows[0]) };
constexpr FzConverter kFzEnergyWh = ::zhc::generic::zcl_attr_fz("seMetering", &kMeterMap);
constexpr ::zhc::generic::ZclAttrRow kPowerRows[] = { { 0x050B, "power" } };
constexpr ::zhc::generic::ZclAttrMap kPowerMap{ kPowerRows, sizeof(kPowerRows)/sizeof(kPowerRows[0]) };
constexpr FzConverter kFzPower = ::zhc::generic::zcl_attr_fz("haElectricalMeasurement", &kPowerMap);

const FzConverter* const kFz[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzOccupancy,
    &kFzRunningState,
    &kFzManu,
    &kFzKeypad,
    &kFzPower,
    &kFzEnergyWh,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzThermostat,
    &kTzWindowDetection,
    &kTzKeypad,
};
constexpr const char* kSystemModes[] = { "off", "heat" };
constexpr const char* kRunningStates[] = { "idle", "heat" };
constexpr const char* kKeypadLabels[] = { "unlocked", "partial", "settings_locked", "full" };
constexpr Expose kExp[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
     ExposeCategory::State, 7, 28, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModes, 2},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kRunningStates, 2},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr,
     "Stops heating when an open window is detected", nullptr, 0, ExposeCategory::Config},
    {"window_open", ExposeType::Binary, Access::State, nullptr, "Open window detected", nullptr, 0},
    {"pi_heating_demand", ExposeType::Numeric, Access::State, "%", "Heating duty cycle", nullptr, 0},
    {"effective_heating_setpoint", ExposeType::Numeric, Access::State, "°C",
     "Setpoint actually applied by the radiator (7 °C while a window is open)", nullptr, 0},
    {"internal_temperature", ExposeType::Numeric, Access::State, "°C",
     "Internal temperature of the radiator", nullptr, 0, ExposeCategory::Diagnostic},
    {"keypad_lockout", ExposeType::Enum, Access::StateSet, nullptr,
     "Restricted access, can only be changed over Zigbee", kKeypadLabels, 4},
};
constexpr const char* kM[] = { "100050072300" };
constexpr BindingSpec kBind[] = {
    {1, 0x0201},
    {1, 0x0406},
    {1, 0x0B04},
    {1, 0x0702},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0201, 0x0000, 0x29, 0, 3600, 10, 0},    // localTemp
    {1, 0x0201, 0x0012, 0x29, 0, 3600, 10, 0},    // occupiedHeatingSetpoint
    {1, 0x0201, 0x001C, 0x30, 0, 3600, 0, 0},     // systemMode
    {1, 0x0406, 0x0000, 0x18, 0, 3600, 0, 0},     // occupancy
    {1, 0x0B04, 0x050B, 0x29, 10, 65000, 5, 0},   // activePower, 5 W
    {1, 0x0702, 0x0000, 0x25, 10, 3600, 100, 0},  // currentSummDelivered, 0.1 kWh, max 1_HOUR
};
}  // namespace

extern const PreparedDefinition kDef_D100050072300{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="100050072300", .vendor="Atlantic",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::atlantic
