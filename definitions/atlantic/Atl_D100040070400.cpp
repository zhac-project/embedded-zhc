// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Atlantic Group 100040070400, Thermor 400704 thermostat for electric
// underfloor heating (new in z2m v26.129.0).
// m.thermostat: local temperature, heating setpoint 7..28, system_mode off/heat.
// running_state does not come from runningState (0x0029, unsupported by the
// device): it follows the Atlantic heatingDemand duty cycle, heat while > 0,
// else idle (z2m v26.133.0 fix, triage_zcl R135).
// seMetering energy in Wh (z2m forces multiplier 1 / divisor 1000).
// Manufacturer-specific hvacThermostat attributes (ATLANTIC_GROUP 0x125B, not
// reportable upstream): window_detection 0x428A (Bool, writable),
// pi_heating_demand 0x42AC (%), connected_load 0x4286 (W) and
// device_temperature 0x42B3 (power board, /100 °C).
// Not ported: identify; the reads behind STATE_GET (running_state,
// pi_heating_demand, connected_load, device_temperature).
// z2m-source: atlantic.ts #100040070400.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::atlantic {
namespace {

constexpr std::uint16_t kAtlanticMfgCode = 0x125B;  // Zcl.ManufacturerCode.ATLANTIC_GROUP

// R135: heatingDemand (0x42AC = 17068) > 0 -> "heat", else "idle".
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
    { 0x42AC, "pi_heating_demand" },
    { 0x4286, "connected_load" },
    { 0x42B3, "device_temperature", 100 },
};
constexpr ::zhc::generic::ZclAttrMap kManuMap{ kManuRows, sizeof(kManuRows)/sizeof(kManuRows[0]), kAtlanticMfgCode };
constexpr FzConverter kFzManu = ::zhc::generic::zcl_attr_fz("hvacThermostat", &kManuMap);
constexpr ::zhc::generic::ZclWriteSpec kWindowDetectionSpec{ "window_detection", 0x428A, 0x10, kAtlanticMfgCode, nullptr, 0 };
constexpr TzConverter kTzWindowDetection = ::zhc::generic::zcl_write_tz("hvacThermostat", 0x0201, &kWindowDetectionSpec);

// currentSummDelivered is in Wh (z2m forces multiplier 1 / divisor 1000).
constexpr ::zhc::generic::ZclAttrRow kMeterRows[] = { { 0x0000, "energy", 1000 } };
constexpr ::zhc::generic::ZclAttrMap kMeterMap{ kMeterRows, sizeof(kMeterRows)/sizeof(kMeterRows[0]) };
constexpr FzConverter kFzEnergyWh = ::zhc::generic::zcl_attr_fz("seMetering", &kMeterMap);

const FzConverter* const kFz[] = {
    &::zhc::generic::kFzThermostat,
    &kFzRunningState,
    &kFzManu,
    &kFzEnergyWh,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzThermostat,
    &kTzWindowDetection,
};
constexpr const char* kSystemModes[] = { "off", "heat" };
constexpr const char* kRunningStates[] = { "idle", "heat" };
constexpr Expose kExp[] = {
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
     ExposeCategory::State, 7, 28, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModes, 2},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kRunningStates, 2},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr,
     "Stops heating when an open window is detected", nullptr, 0, ExposeCategory::Config},
    {"pi_heating_demand", ExposeType::Numeric, Access::State, "%", "Heating duty cycle", nullptr, 0},
    {"connected_load", ExposeType::Numeric, Access::State, "W",
     "Power of the heating mat set at commissioning", nullptr, 0, ExposeCategory::Diagnostic},
    {"device_temperature", ExposeType::Numeric, Access::State, "°C",
     "Temperature of the power board", nullptr, 0, ExposeCategory::Diagnostic},
};
constexpr const char* kM[] = { "100040070400" };
constexpr BindingSpec kBind[] = {
    {1, 0x0201},
    {1, 0x0702},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0201, 0x0000, 0x29, 0, 3600, 10, 0},    // localTemp
    {1, 0x0201, 0x0012, 0x29, 0, 3600, 10, 0},    // occupiedHeatingSetpoint
    {1, 0x0201, 0x001C, 0x30, 0, 3600, 0, 0},     // systemMode
    {1, 0x0702, 0x0000, 0x25, 10, 65000, 100, 0}, // currentSummDelivered, 0.1 kWh
};
}  // namespace

extern const PreparedDefinition kDef_D100040070400{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="100040070400", .vendor="Atlantic",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::atlantic
