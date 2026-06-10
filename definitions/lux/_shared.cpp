// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared LUX converter implementations.
// See definitions/lux/_shared.hpp for the cluster table.

#include "definitions/lux/_shared.hpp"

#include <cstdint>

namespace zhc::lux {

namespace {

// ── fz: hvacThermostat extras (0x0011 cooling sp + 0x0029 running
//    state + 0x001E running mode) ─────────────────────────────────
//
// Delegates to the generic thermostat decoder for local_temperature
// (0x0000), current_heating_setpoint (0x0012) and system_mode
// (0x001C), then adds the attributes z2m's `fz.thermostat` decodes
// that the generic converter drops for this device:
//
//   0x0011 OccupiedCoolingSetpoint (s16, key "17")  → occupied_cooling_setpoint
//   0x0029 ThermostatRunningState  (bitmap16, "41") → running_state (string)
//   0x001E ThermostatRunningMode   (enum8, "30")    → running_mode  (string)
//
// running_state / running_mode emit z2m's string form (matching
// fz.thermostat's getFromLookup against constants.thermostatRunning-
// States / thermostatRunningMode). occupied_cooling_setpoint emits the
// raw s16 (downstream applies the ×100 scale, same contract as
// local_temperature / current_heating_setpoint in the generic decoder).
bool fz_lux_thermostat_extras(const ::zhc::DecodedMessage& msg,
                              const ::zhc::FzConverter& self,
                              const ::zhc::PreparedDefinition& def,
                              ::zhc::RuntimeContext& ctx,
                              ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = ::zhc::generic::kFzThermostat.fn.zcl_fn(msg, self, def, ctx, out);

    // 0x0011 OccupiedCoolingSetpoint (s16) — raw int, like the generic
    // local_temperature / current_heating_setpoint branches.
    if (const ::zhc::Value* v = msg.payload.find("17")) {
        if (v->type == ::zhc::ValueType::Int || v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Int;
            o.i = v->type == ::zhc::ValueType::Int
                      ? v->i
                      : static_cast<std::int64_t>(v->u);
            out.put("occupied_cooling_setpoint", o);
            emitted = true;
        }
    }

    // 0x0029 ThermostatRunningState (bitmap16) — z2m
    // constants.thermostatRunningStates getFromLookup. Bit0=heat,
    // bit1=cool, bit2=fan_only; the device expose declares
    // {idle, heat, cool}. Unknown values are dropped (lookup miss).
    if (const ::zhc::Value* v = msg.payload.find("41")) {
        if (v->type == ::zhc::ValueType::Uint) {
            const char* label = nullptr;
            switch (v->u) {
                case 0x0000: label = "idle"; break;
                case 0x0001: label = "heat"; break;
                case 0x0002: label = "cool"; break;
                case 0x0004: label = "fan_only"; break;
                case 0x0005: label = "heat"; break;
                case 0x0006: label = "cool"; break;
                case 0x8000: label = "idle"; break;
                case 0x8001: label = "heat"; break;
                default: break;
            }
            if (label) {
                ::zhc::Value o{};
                o.type = ::zhc::ValueType::StringRef;
                o.str  = label;
                out.put("running_state", o);
                emitted = true;
            }
        }
    }

    // 0x001E ThermostatRunningMode (enum8) — z2m
    // constants.thermostatRunningMode: 0→off, 3→cool, 4→heat. Unknown
    // values are dropped (getFromLookup miss).
    if (const ::zhc::Value* v = msg.payload.find("30")) {
        if (v->type == ::zhc::ValueType::Uint) {
            const char* label = nullptr;
            switch (v->u) {
                case 0: label = "off";  break;
                case 3: label = "cool"; break;
                case 4: label = "heat"; break;
                default: break;
            }
            if (label) {
                ::zhc::Value o{};
                o.type = ::zhc::ValueType::StringRef;
                o.str  = label;
                out.put("running_mode", o);
                emitted = true;
            }
        }
    }

    return emitted;
}

}  // namespace

extern const FzConverter kFzLuxThermostatExtras{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lux_thermostat_extras },
    .user_config       = nullptr,
};

}  // namespace zhc::lux
