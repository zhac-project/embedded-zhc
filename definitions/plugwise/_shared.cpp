// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Plugwise hvacThermostat manuSpec runtime. See `_shared.hpp` for the
// attribute table. Manufacturer code: 0x1172 (PLUGWISE_B_V).
#include "definitions/plugwise/_shared.hpp"

#include <cstdio>
#include <cstring>

namespace zhc::devices::plugwise {

using ::zhc::FzConverter;
using ::zhc::TzConverter;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::Direction;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::FixedPayload;
using ::zhc::DecodedMessage;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::type_bit;

namespace {

constexpr std::uint16_t kPlugwiseMfg = 0x1172;

// ── Lookup tables for force / radio_strength ─────────────────────
constexpr ::zhc::generic::ZclWriteLookup kForceLookup[] = {
    { "standard",  0       },
    { "high",      393216  },
    { "very_high", 458752  },
};
constexpr ::zhc::generic::ZclWriteLookup kRadioStrengthLookup[] = {
    { "normal", 0 },
    { "high",   1 },
};

// ── ZclWriteSpec instances (writeAttributes, mfg 0x1172) ─────────
// ZCL data type bytes:
//   0x10 BOOLEAN, 0x20 UINT8, 0x21 UINT16, 0x23 UINT32,
//   0x29 INT16.
constexpr ::zhc::generic::ZclWriteSpec kSpecValvePosition{
    "valve_position", 0x4001, 0x20, kPlugwiseMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecPushForce{
    "force", 0x4012, 0x23, kPlugwiseMfg,
    kForceLookup, sizeof(kForceLookup) / sizeof(kForceLookup[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRadioStrength{
    "radio_strength", 0x4014, 0x10, kPlugwiseMfg,
    kRadioStrengthLookup, sizeof(kRadioStrengthLookup) / sizeof(kRadioStrengthLookup[0]),
};
// The OpenTherm temperatures are written in °C (z2m `scale: 100`), refused
// outside z2m's ranges: external_heat_demand 0..90 °C, the max setpoints
// 0..100 °C.
constexpr ::zhc::generic::ZclWriteSpec kSpecExternalHeatDemand{
    "external_heat_demand", 0xF000, 0x21, kPlugwiseMfg, nullptr, 0, 0, 100, 0, 9000,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecExternalHeatDemandTo{
    "external_heat_demand_timeout", 0xF001, 0x21, kPlugwiseMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMaxDhwSetpoint{
    "max_dhw_setpoint", 0xF007, 0x29, kPlugwiseMfg, nullptr, 0, 0, 100, 0, 10000,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMaxBoilerSetpoint{
    "max_boiler_setpoint", 0xF008, 0x29, kPlugwiseMfg, nullptr, 0, 0, 100, 0, 10000,
};

// ── fz_plugwise_thermostat — manuSpec attribute decoder ──────────
//
// Walks the decoded payload (keyed by decimal attr id) and emits
// human-readable expose keys for every Plugwise attribute we know
// about. The standard attrs (local_temperature 0x0000,
// pIHeatingDemand 0x0008, occupiedHeatingSetpoint 0x0012,
// systemMode 0x001C) are already covered by the generic
// `kFzThermostat`, so we leave them alone here.

enum class PType : std::uint8_t {
    Uint,            // Pass-through unsigned
    Int,             // Pass-through signed
    Celsius,         // raw / 100 -> °C Float (z2m precisionRound/100, scale 100)
    CelsiusValid,    // Celsius, below -273.15 dropped (fz.thermostat outdoorTemp)
    DeciCelsius,     // raw / 10 -> °C Float (localTemperatureCalibration)
    ForceLookup,     // u32 -> string label via kForceLookup
    RadioLookup,     // u8/bool -> string label via kRadioStrengthLookup
    RunningState,    // map16 -> string label via constants.thermostatRunningStates
};

struct PEntry {
    std::uint16_t attr_id;
    const char*   key;
    PType         type;
};

constexpr PEntry kAttrs[] = {
    // Standard hvacThermostat attrs that the generic kFzThermostat drops
    // (it handles only 0x0000 / 0x0012 / 0x001C). z2m decodes all of
    // these via base fz.thermostat — without them the Tom/Emma exposes
    // (pi_heating_demand, occupied_cooling_setpoint, running_state,
    // local_temperature_calibration, outdoor_temperature) are dead.
    // Temperatures are °C, scaled as z2m's fz.thermostat / m.numeric do.
    { 0x0001, "outdoor_temperature",           PType::CelsiusValid },  // OutdoorTemperature (s16)
    { 0x0008, "pi_heating_demand",             PType::Uint         },  // PIHeatingDemand (u8, 0..100 on Plugwise)
    { 0x0010, "local_temperature_calibration", PType::DeciCelsius  },  // LocalTemperatureCalibration (s8)
    { 0x0011, "occupied_cooling_setpoint",     PType::Celsius      },  // OccupiedCoolingSetpoint (s16)
    { 0x0029, "running_state",                 PType::RunningState },  // ThermostatRunningState (map16)
    // Plugwise manufacturer-specific attrs (0x1172 mfg code):
    { 0x4001, "valve_position",                PType::Uint         },
    { 0x4002, "error_status",                  PType::Uint         },
    { 0x4003, "current_heating_setpoint",      PType::Celsius      },
    { 0x4008, "plugwise_t_diff",               PType::Int          },
    { 0x4012, "force",                         PType::ForceLookup  },
    { 0x4014, "radio_strength",                PType::RadioLookup  },
    { 0xF000, "external_heat_demand",          PType::Celsius      },  // u16, z2m scale 100
    { 0xF001, "external_heat_demand_timeout",  PType::Uint         },
    { 0xF002, "boiler_water_temperature",      PType::Celsius      },
    { 0xF003, "dhw_temperature",               PType::Celsius      },
    { 0xF004, "return_water_temperature",      PType::Celsius      },
    { 0xF005, "application_fault_code",        PType::Uint         },
    { 0xF006, "oem_fault_code",                PType::Uint         },
    { 0xF007, "max_dhw_setpoint",              PType::Celsius      },
    { 0xF008, "max_boiler_setpoint",           PType::Celsius      },
};

const char* lookup_label(const ::zhc::generic::ZclWriteLookup* table,
                          std::size_t n, std::uint64_t v) {
    for (std::size_t i = 0; i < n; ++i) {
        if (table[i].value == v) return table[i].label;
    }
    return nullptr;
}

bool fz_plugwise_thermostat(const DecodedMessage& msg,
                             const FzConverter&,
                             const PreparedDefinition&,
                             RuntimeContext&,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    char keybuf[8];
    for (const auto& e : kAttrs) {
        std::snprintf(keybuf, sizeof(keybuf), "%u",
                      static_cast<unsigned>(e.attr_id));
        const Value* v = msg.payload.find(keybuf);
        if (!v) continue;

        Value o{};
        switch (e.type) {
            case PType::Uint: {
                if (v->type == ValueType::Uint) {
                    o.type = ValueType::Uint; o.u = v->u;
                } else if (v->type == ValueType::Int && v->i >= 0) {
                    o.type = ValueType::Uint;
                    o.u   = static_cast<std::uint64_t>(v->i);
                } else if (v->type == ValueType::Bool) {
                    o.type = ValueType::Uint; o.u = v->b ? 1 : 0;
                } else { continue; }
                break;
            }
            case PType::Int: {
                if (v->type == ValueType::Int) {
                    o.type = ValueType::Int; o.i = v->i;
                } else if (v->type == ValueType::Uint) {
                    o.type = ValueType::Int;
                    o.i   = static_cast<std::int64_t>(v->u);
                } else { continue; }
                break;
            }
            case PType::Celsius:
            case PType::CelsiusValid:
            case PType::DeciCelsius:
                if (::zhc::generic::put_thermostat_celsius(
                        out, e.key, v, e.type == PType::DeciCelsius ? 10 : 100,
                        e.type == PType::CelsiusValid))
                    emitted = true;
                continue;
            case PType::ForceLookup: {
                std::uint64_t raw = 0;
                if      (v->type == ValueType::Uint) raw = v->u;
                else if (v->type == ValueType::Int)  raw = static_cast<std::uint64_t>(v->i);
                else continue;
                const char* label = lookup_label(
                    kForceLookup,
                    sizeof(kForceLookup) / sizeof(kForceLookup[0]),
                    raw);
                if (!label) continue;
                o.type = ValueType::StringRef; o.str = label;
                break;
            }
            case PType::RadioLookup: {
                std::uint64_t raw = 0;
                if      (v->type == ValueType::Bool) raw = v->b ? 1 : 0;
                else if (v->type == ValueType::Uint) raw = v->u;
                else if (v->type == ValueType::Int)  raw = static_cast<std::uint64_t>(v->i);
                else continue;
                const char* label = lookup_label(
                    kRadioStrengthLookup,
                    sizeof(kRadioStrengthLookup) / sizeof(kRadioStrengthLookup[0]),
                    raw);
                if (!label) continue;
                o.type = ValueType::StringRef; o.str = label;
                break;
            }
            case PType::RunningState: {
                // ThermostatRunningState (map16). z2m looks the raw value
                // up in constants.thermostatRunningStates. The Plugwise
                // heating family runs the common low states (Emma exposes
                // running_state over [idle, heat, cool]); cover the bits
                // z2m maps and drop unknowns (matching getFromLookup miss).
                std::uint64_t st = 0;
                if      (v->type == ValueType::Uint) st = v->u;
                else if (v->type == ValueType::Int && v->i >= 0)
                    st = static_cast<std::uint64_t>(v->i);
                else continue;
                const char* label = nullptr;
                switch (st) {
                    case 0x00: label = "idle";     break;
                    case 0x01: label = "heat";     break;
                    case 0x02: label = "cool";     break;
                    case 0x04: label = "fan_only"; break;
                    default: break;
                }
                if (!label) continue;
                o.type = ValueType::StringRef; o.str = label;
                break;
            }
        }
        if (out.put(e.key, o)) emitted = true;
    }
    return emitted;
}

// ── tz_plugwise_calibrate_valve — empty mfg-specific cluster command ─
//
// Sends the `plugwiseCalibrateValve` cluster command (id 0xA0) with
// no payload. Frame control: cluster-specific (bit 0=1) +
// manufacturer-specific (bit 2=1) + disable-default-response
// (bit 4=1) = 0x15. Header layout:
//
//   [0] fc=0x15
//   [1..2] mfg LE
//   [3] tsn placeholder (0)
//   [4] cmd id 0xA0
bool tz_plugwise_calibrate_valve(std::string_view key,
                                  const Value&,
                                  const TzConverter&,
                                  const PreparedDefinition&,
                                  RuntimeContext&,
                                  std::span<std::uint8_t> out_frame,
                                  std::size_t& out_size) {
    out_size = 0;
    if (key != "calibrate_valve") return false;
    if (out_frame.size() < 5) return false;
    out_frame[0] = 0x15;
    out_frame[1] = static_cast<std::uint8_t>(kPlugwiseMfg & 0xFF);
    out_frame[2] = static_cast<std::uint8_t>((kPlugwiseMfg >> 8) & 0xFF);
    out_frame[3] = 0x00;
    out_frame[4] = 0xA0;
    out_size = 5;
    return true;
}

}  // namespace

// ── FzConverter — manuSpec hvacThermostat decoder ────────────────
extern const FzConverter kFzPlugwiseThermostat{
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
    .fn                = { .zcl_fn = fz_plugwise_thermostat },
    .user_config       = nullptr,
};

// ── TzConverters — manuSpec writes (cluster 0x0201, cmd 0x02) ────
#define ZHC_PLUGWISE_TZ(var, spec_ref, key_str)                          \
    extern const TzConverter var{                                        \
        .key         = key_str,                                          \
        .cluster     = "hvacThermostat",                                 \
        .cluster_id  = 0x0201,                                           \
        .command_id  = 0x02,    /* writeAttributes */                    \
        .fn          = &::zhc::generic::tz_zcl_write_attr,               \
        .user_config = &spec_ref,                                        \
    };

ZHC_PLUGWISE_TZ(kTzPlugwiseValvePosition,        kSpecValvePosition,        "valve_position")
ZHC_PLUGWISE_TZ(kTzPlugwisePushForce,            kSpecPushForce,            "force")
ZHC_PLUGWISE_TZ(kTzPlugwiseRadioStrength,        kSpecRadioStrength,        "radio_strength")
ZHC_PLUGWISE_TZ(kTzPlugwiseExternalHeatDemand,   kSpecExternalHeatDemand,   "external_heat_demand")
ZHC_PLUGWISE_TZ(kTzPlugwiseExternalHeatDemandTo, kSpecExternalHeatDemandTo, "external_heat_demand_timeout")
ZHC_PLUGWISE_TZ(kTzPlugwiseMaxDhwSetpoint,       kSpecMaxDhwSetpoint,       "max_dhw_setpoint")
ZHC_PLUGWISE_TZ(kTzPlugwiseMaxBoilerSetpoint,    kSpecMaxBoilerSetpoint,    "max_boiler_setpoint")

#undef ZHC_PLUGWISE_TZ

// ── TzConverter — plugwiseCalibrateValve cluster command ──────────
extern const TzConverter kTzPlugwiseCalibrateValve{
    .key         = "calibrate_valve",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0xA0,
    .fn          = tz_plugwise_calibrate_valve,
    .user_config = nullptr,
};

}  // namespace zhc::devices::plugwise
