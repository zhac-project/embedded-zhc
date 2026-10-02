// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: uses shared sinope converters.
// Ouellet OTH3600-GA-ZB floor heating thermostat (z2m v26.115.1 window).
//
// Ported: thermostat (local temperature, heating setpoint 7..30, system mode
// off/heat), pi_heating_demand and z2m's running_state (heat at a demand of 10 %
// or more), the temperature sensor, keypad lockout / display unit, the
// Sinopé secondary display (second_display_mode, outdoor temperature and its
// timeout on 0xFF01), rmsVoltage and energy. Configure only reads, as z2m: the
// device rejects reporting for system mode and keypad lockout.
// Not ported, see the R8 worklist: z2m derives `current` (rmsCurrent / 100,
// forced to 0 while the heating demand is 0) and `power` (voltage x current)
// from attribute values cached across two clusters.
// z2m-source: sinope.ts #OTH3600-GA-ZB.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sinope/_shared.hpp"

namespace zhc::devices::sinope {
namespace {

// hvacThermostat pIHeatingDemand (0x0008, u8 %) -> pi_heating_demand +
// running_state.
bool fz_ouellet_heating_demand(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                               RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("8");
    if (!v || (v->type != ValueType::Uint && v->type != ValueType::Int)) return false;
    const std::int64_t demand = v->type == ValueType::Uint ? static_cast<std::int64_t>(v->u) : v->i;
    Value d{}; d.type = ValueType::Uint; d.u = static_cast<std::uint64_t>(demand < 0 ? 0 : demand);
    out.put("pi_heating_demand", d);
    Value s{}; s.type = ValueType::StringRef; s.str = demand >= 10 ? "heat" : "idle";
    out.put("running_state", s);
    return true;
}
constexpr FzConverter kFzHeatingDemand{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = type_bit(MessageType::AttributeReport) | type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_ouellet_heating_demand },
    .user_config       = nullptr,
};
constexpr ::zhc::generic::ZclAttrRow kVoltageRows[] = { { 0x0505, "voltage" } };
constexpr ::zhc::generic::ZclAttrMap kVoltageMap{ kVoltageRows, 1 };
constexpr FzConverter kFzVoltage = ::zhc::generic::zcl_attr_fz("haElectricalMeasurement", &kVoltageMap);

const FzConverter* const kFz[] = {
    &::zhc::generic::kFzThermostat,
    &kFzHeatingDemand,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHvacUserInterface,
    &::zhc::sinope::kFzSinopeManu,
    &kFzVoltage,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::generic::kTzKeypadLockout,
    &::zhc::generic::kTzTemperatureDisplayMode,
    &::zhc::sinope::kTzSinopeSecondDisplayMode,
    &::zhc::sinope::kTzSinopeOutdoorTemperature,
    &::zhc::sinope::kTzSinopeOutdoorTempTimeout,
};
constexpr const char* kSystemModes[] = { "off", "heat" };
constexpr const char* kRunningStates[] = { "idle", "heat" };
constexpr const char* kKeypadModes[] = { "unlock", "lock1", "lock2", "lock3", "lock4", "lock5" };
constexpr const char* kDisplayUnits[] = { "celsius", "fahrenheit" };
constexpr const char* kSecondDisplay[] = { "auto", "setpoint", "outdoor temp" };
constexpr Expose kExp[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "\u00b0C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModes, 2},
    {"pi_heating_demand", ExposeType::Numeric, Access::State, "%", "Position of the valve (= demanded heat) where 0% is fully closed and 100% is fully open", nullptr, 0},
    {"running_state", ExposeType::Enum, Access::State, nullptr, "Heating state calculated from PI heating demand", kRunningStates, 2},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"keypad_lockout", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kKeypadModes, 6, ExposeCategory::Config},
    {"temperature_display_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kDisplayUnits, 2, ExposeCategory::Config},
    {"second_display_mode", ExposeType::Enum, Access::StateSet, nullptr,
     "Displays the outdoor temperature and then returns to the set point in \"auto\" mode, or clears in \"outdoor temp\" mode when expired.",
     kSecondDisplay, 3},
    {"thermostat_outdoor_temperature", ExposeType::Numeric, Access::StateSet, "°C",
     "Outdoor temperature for the secondary display", nullptr, 0, ExposeCategory::State, -100, 100, 0},
    {"outdoor_temperature_timeout", ExposeType::Numeric, Access::StateSet, "s",
     "Time in seconds after which the outdoor temperature is considered to have expired", nullptr, 0,
     ExposeCategory::State, 30, 64800, 1},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "OTH3600-GA-ZB" };
constexpr BindingSpec kBind[] = {
    {1, 0x0201},
    {1, 0x0402},
    {1, 0x0702},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0201, 0x0000, 0x29, 0, 3600, 10, 0},       // localTemp
    {1, 0x0201, 0x0012, 0x29, 0, 3600, 10, 0},       // occupiedHeatingSetpoint
    {1, 0x0402, 0x0000, 0x29, 10, 3600, 100, 0},     // temperature
};
constexpr std::uint8_t kReadThermostat[] = { 0x00, 0x00, 0x12, 0x00, 0x1C, 0x00, 0x08, 0x00 };
constexpr std::uint8_t kReadUi[] = { 0x01, 0x00, 0x00, 0x00 };
constexpr std::uint8_t kReadElectrical[] = { 0x05, 0x05, 0x08, 0x05 };
constexpr std::uint8_t kReadMetering[] = { 0x00, 0x00, 0x01, 0x03, 0x02, 0x03 };   // summ, multiplier, divisor
constexpr std::uint8_t kReadSinope[] = { 0x10, 0x00, 0x11, 0x00, 0x12, 0x00 };
constexpr ConfigStep kSteps[] = {
    { ConfigStepOp::Read, 1, 0x0201, 0x00, 0, kReadThermostat, sizeof(kReadThermostat), 0 },
    { ConfigStepOp::Read, 1, 0x0204, 0x00, 0, kReadUi, sizeof(kReadUi), 0 },
    { ConfigStepOp::Read, 1, 0x0B04, 0x00, 0, kReadElectrical, sizeof(kReadElectrical), 0 },
    { ConfigStepOp::Read, 1, 0x0702, 0x00, 0, kReadMetering, sizeof(kReadMetering), 0 },
    { ConfigStepOp::Read, 1, 0xFF01, 0x00, 0, kReadSinope, sizeof(kReadSinope), 0, 0x119C },
};
}  // namespace

extern const PreparedDefinition kDef_OTH3600_GA_ZB{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="OTH3600-GA-ZB", .vendor="Ouellet",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
    .config_steps=kSteps, .config_steps_count=sizeof(kSteps)/sizeof(kSteps[0]),
};

}  // namespace zhc::devices::sinope
