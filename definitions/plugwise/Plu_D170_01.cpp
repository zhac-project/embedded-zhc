// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Plugwise 170-01 — uses shared plugwise converters.
// Emma Wired Pro / Emma Wireless OpenTherm gateway thermostat.
// z2m-source: plugwise.ts #170-01.
//
// Note: 170-01 ships with no battery (Emma Wired Pro is mains-powered;
// Wireless variant uses USB-C). The original auto-port wired
// kFzBattery, which is harmless but never fires — we drop it here.
//
// Decoder coverage:
//   - kFzTemperature           -> local_temperature (msTemperatureMeasurement)
//   - kFzHumidity              -> humidity (msRelativeHumidity)
//   - kFzThermostat            -> (generic) local_temperature,
//                                 current_heating_setpoint, system_mode
//                                 (only 0x0000 / 0x0012 / 0x001C).
//   - kFzPlugwiseThermostat    -> occupied_cooling_setpoint (0x0011),
//                                 running_state (0x0029),
//                                 local_temperature_calibration (0x0010),
//                                 outdoor_temperature (0x0001),
//                                 boiler_water_temperature, dhw_temperature,
//                                 return_water_temperature, application_fault_code,
//                                 oem_fault_code, max_dhw_setpoint,
//                                 max_boiler_setpoint, external_heat_demand,
//                                 external_heat_demand_timeout.
//                                 NOTE: the four standard attrs above were
//                                 DEAD exposes — generic kFzThermostat skips
//                                 them — until kFzPlugwiseThermostat was
//                                 extended to decode them.
//   - fz_d170_boiler_setpoint  -> boiler_setpoint (0x0008 pIHeatingDemand).
//                                 z2m names this attr "boiler_setpoint" (°C,
//                                 intended boiler water temp) on Emma, not
//                                 the TRV's "pi_heating_demand".
//
// Writer coverage:
//   - kTzThermostat                  -> occupied_heating_setpoint, current_heating_setpoint, system_mode
//   - kTzPlugwiseMaxDhwSetpoint      -> max_dhw_setpoint
//   - kTzPlugwiseMaxBoilerSetpoint   -> max_boiler_setpoint
//   - kTzPlugwiseExternalHeatDemand  -> external_heat_demand
//   - kTzPlugwiseExternalHeatDemandTo-> external_heat_demand_timeout
//
// Not yet ported: occupied_cooling_setpoint and local_temperature_calibration
// WRITES — the generic kTzThermostat only handles 0x0012/0x001C, so a richer
// "tz_thermostat_full" or dedicated ZclWriteSpec instances would be needed.
// These are manuSpec-free standard writes w/o an existing converter = INFRA,
// deferred. Reads of both attrs now decode (above).
#include "definitions/_generic/_shared.hpp"
#include "definitions/plugwise/_shared.hpp"

namespace zhc::devices::plugwise {
namespace {

// Emma overloads the standard pIHeatingDemand attribute (0x0008) as the
// "intended boiler water temperature" setpoint. z2m surfaces it via
// `m.numeric({name: "boiler_setpoint", attribute: "pIHeatingDemand", ...})`
// — a different expose key than the TRV's "pi_heating_demand". Decode it
// here as a raw u8 so the renamed expose is populated.
bool fz_d170_boiler_setpoint(const DecodedMessage& msg,
                             const FzConverter&,
                             const PreparedDefinition&,
                             RuntimeContext&,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("8");  // 8 == 0x0008 PIHeatingDemand
    if (!v) return false;
    Value o{};
    if (v->type == ValueType::Uint) { o.type = ValueType::Uint; o.u = v->u; }
    else if (v->type == ValueType::Int && v->i >= 0) {
        o.type = ValueType::Uint; o.u = static_cast<std::uint64_t>(v->i);
    } else {
        return false;
    }
    return out.put("boiler_setpoint", o);
}

const FzConverter kFzD170BoilerSetpoint{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_d170_boiler_setpoint },
    .user_config       = nullptr,
};

const FzConverter* const kFz_D170_01[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzThermostat,
    &kFzPlugwiseThermostat,
    &kFzD170BoilerSetpoint,
};
const TzConverter* const kTz_D170_01[] = {
    &::zhc::generic::kTzThermostat,
    &kTzPlugwiseMaxDhwSetpoint,
    &kTzPlugwiseMaxBoilerSetpoint,
    &kTzPlugwiseExternalHeatDemand,
    &kTzPlugwiseExternalHeatDemandTo,
};

constexpr const char* kModels_D170_01[] = { "170-01" };

}  // namespace


// --- exposes ---
constexpr Expose kAutoExposes[] = {
    {"local_temperature",          ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"humidity",                   ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"outdoor_temperature",        ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"occupied_heating_setpoint",  ExposeType::Numeric, Access::StateSet, "C",  nullptr, nullptr, 0},
    {"occupied_cooling_setpoint",  ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"local_temperature_calibration", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"system_mode",                ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"running_state",              ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"boiler_setpoint",            ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"boiler_water_temperature",   ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"dhw_temperature",            ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"return_water_temperature",   ExposeType::Numeric, Access::State,    "C",  nullptr, nullptr, 0},
    {"max_dhw_setpoint",           ExposeType::Numeric, Access::StateSet, "C",  nullptr, nullptr, 0},
    {"max_boiler_setpoint",        ExposeType::Numeric, Access::StateSet, "C",  nullptr, nullptr, 0},
    {"external_heat_demand",       ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"external_heat_demand_timeout", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"application_fault_code",     ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0},
    {"oem_fault_code",             ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},   // hvacThermostat
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0405},   // msRelativeHumidity
};
// --- end ---

extern const PreparedDefinition kDef_D170_01{
    .zigbee_models=kModels_D170_01, .zigbee_models_count=sizeof(kModels_D170_01)/sizeof(kModels_D170_01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="170-01", .vendor="Plugwise",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D170_01, .from_zigbee_count=sizeof(kFz_D170_01)/sizeof(kFz_D170_01[0]),
    .to_zigbee=kTz_D170_01, .to_zigbee_count=sizeof(kTz_D170_01)/sizeof(kTz_D170_01[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::plugwise
