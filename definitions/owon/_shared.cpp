// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0

#include "definitions/owon/_shared.hpp"

#include <cstdint>

namespace zhc::owon {

namespace {

// Decodes the two standard hvacThermostat attributes the generic
// `kFzThermostat` skips. local_temperature / current_heating_setpoint
// / system_mode are still emitted by the generic converter wired
// alongside this one on the AC201 / AC221 / PCT504 / PCT512 defs.
bool fz_owon_thermostat_extras(const DecodedMessage& msg,
                               const FzConverter&,
                               const PreparedDefinition&,
                               RuntimeContext&,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // attr 0x0011 — OccupiedCoolingSetpoint (s16, 0.01 °C wire units).
    // Raw pass-through as Int, mirroring the generic heating-setpoint
    // branch (the runtime scales /100 downstream). z2m key:
    // "occupied_cooling_setpoint"; the Owon climate exposes surface it
    // as "current_cooling_setpoint".
    if (const Value* v = msg.payload.find("17")) {
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value o{};
            o.type = ValueType::Int;
            o.i    = v->type == ValueType::Int
                         ? v->i
                         : static_cast<std::int64_t>(v->u);
            if (out.put("current_cooling_setpoint", o)) emitted = true;
        }
    }

    // attr 0x0029 — ThermostatRunningState (map16). z2m looks the raw
    // value up in constants.thermostatRunningStates. Owon fan-coils only
    // ever run the common low states, so cover 0/1/2/4 and drop unknowns
    // (matching getFromLookup's miss):
    //   0 → "idle", 1 → "heat", 2 → "cool", 4 → "fan_only".
    if (const Value* v = msg.payload.find("41")) {
        std::uint64_t st = 0;
        bool ok = false;
        if (v->type == ValueType::Uint) { st = v->u; ok = true; }
        else if (v->type == ValueType::Int && v->i >= 0) {
            st = static_cast<std::uint64_t>(v->i); ok = true;
        }
        if (ok) {
            const char* label = nullptr;
            switch (st) {
                case 0: label = "idle";     break;
                case 1: label = "heat";     break;
                case 2: label = "cool";     break;
                case 4: label = "fan_only"; break;
                default: break;
            }
            if (label) {
                Value o{};
                o.type = ValueType::StringRef;
                o.str  = label;
                if (out.put("running_state", o)) emitted = true;
            }
        }
    }

    return emitted;
}

}  // namespace

extern const FzConverter kFzOwonThermostatExtras{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_owon_thermostat_extras },
    .user_config       = nullptr,
};

}  // namespace zhc::owon
