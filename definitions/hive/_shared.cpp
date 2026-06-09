// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0

#include "definitions/hive/_shared.hpp"

#include <cstdint>

namespace zhc::hive {

namespace {

// Decodes the standard hvacThermostat attribute the generic
// `kFzThermostat` skips. local_temperature / current_heating_setpoint /
// system_mode are still emitted by the generic converter wired alongside
// this one on the SLR1 / SLR1b / SLR1c / SLR1d / SLR2 / SLR2b / SLR2c /
// OTR1 receiver defs.
bool fz_hive_thermostat_extras(const DecodedMessage& msg,
                               const FzConverter&,
                               const PreparedDefinition&,
                               RuntimeContext&,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // attr 0x0029 — ThermostatRunningState (map16). z2m looks the raw
    // value up in constants.thermostatRunningStates. Hive receivers only
    // ever run heat / idle, but the full low-bit lookup is covered so any
    // reported value lands on the right z2m label; unknown values are
    // dropped (matching getFromLookup's miss).
    if (const Value* v = msg.payload.find("41")) {  // 41 == 0x0029
        std::uint64_t st = 0;
        bool ok = false;
        if (v->type == ValueType::Uint) { st = v->u; ok = true; }
        else if (v->type == ValueType::Int && v->i >= 0) {
            st = static_cast<std::uint64_t>(v->i); ok = true;
        }
        if (ok) {
            const char* label = nullptr;
            switch (st) {
                case 0x0000: label = "idle";     break;
                case 0x0001: label = "heat";     break;
                case 0x0002: label = "cool";     break;
                case 0x0004: label = "fan_only"; break;
                case 0x0005: label = "heat";     break;
                case 0x0006: label = "cool";     break;
                case 0x0008: label = "heat";     break;
                case 0x0009: label = "heat";     break;
                case 0x000A: label = "heat";     break;
                case 0x000D: label = "heat";     break;
                case 0x0010: label = "cool";     break;
                case 0x0012: label = "cool";     break;
                case 0x0014: label = "cool";     break;
                case 0x0015: label = "cool";     break;
                case 0x0022: label = "cool";     break;
                case 0x0033: label = "heat";     break;
                case 0x0034: label = "cool";     break;
                case 0x0065: label = "heat";     break;
                case 0x0066: label = "cool";     break;
                case 0x8000: label = "idle";     break;
                case 0x8001: label = "heat";     break;
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

extern const FzConverter kFzHiveThermostatExtras{
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
    .fn                = { .zcl_fn = &fz_hive_thermostat_extras },
    .user_config       = nullptr,
};

}  // namespace zhc::hive
