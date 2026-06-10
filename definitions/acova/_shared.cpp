// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Acova shared converters — see _shared.hpp for rationale.
//
// z2m-source: acova.ts via converters/fromZigbee.ts #thermostat.
#include "definitions/acova/_shared.hpp"

namespace zhc::devices::acova {
namespace {

// ── Vendor FZ: Acova thermostat decoder ──────────────────────────────
// Standalone (does NOT delegate to the generic kFzThermostat) because the
// generic decoder emits attr 0x0012 under "current_heating_setpoint" — the
// wrong key for the Acova family, which exposes "occupied_heating_setpoint".
bool fz_acova_thermostat(const ::zhc::DecodedMessage& msg,
                         const ::zhc::FzConverter&,
                         const ::zhc::PreparedDefinition&,
                         ::zhc::RuntimeContext&,
                         ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // 0x0000 localTemp (s16, ×100) — raw int passthrough.
    if (const ::zhc::Value* v = msg.payload.find("0")) {
        if (v->type == ::zhc::ValueType::Int || v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Int;
            o.i = v->type == ::zhc::ValueType::Int ? v->i
                                                   : static_cast<std::int64_t>(v->u);
            out.put("local_temperature", o);
            emitted = true;
        }
    }

    // 0x0010 localTemperatureCalibration (s16, ×10) — raw int passthrough.
    if (const ::zhc::Value* v = msg.payload.find("16")) {
        if (v->type == ::zhc::ValueType::Int || v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Int;
            o.i = v->type == ::zhc::ValueType::Int ? v->i
                                                   : static_cast<std::int64_t>(v->u);
            out.put("local_temperature_calibration", o);
            emitted = true;
        }
    }

    // 0x0012 occupiedHeatingSetpoint (s16, ×100) — raw int passthrough.
    if (const ::zhc::Value* v = msg.payload.find("18")) {
        if (v->type == ::zhc::ValueType::Int || v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Int;
            o.i = v->type == ::zhc::ValueType::Int ? v->i
                                                   : static_cast<std::int64_t>(v->u);
            out.put("occupied_heating_setpoint", o);
            emitted = true;
        }
    }

    // 0x0014 unoccupiedHeatingSetpoint (s16, ×100) — raw int passthrough.
    if (const ::zhc::Value* v = msg.payload.find("20")) {
        if (v->type == ::zhc::ValueType::Int || v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Int;
            o.i = v->type == ::zhc::ValueType::Int ? v->i
                                                   : static_cast<std::int64_t>(v->u);
            out.put("unoccupied_heating_setpoint", o);
            emitted = true;
        }
    }

    // 0x001C systemMode (u8 enum) — raw passthrough (generic contract).
    if (const ::zhc::Value* v = msg.payload.find("28")) {
        if (v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Uint;
            o.u = v->u;
            out.put("system_mode", o);
            emitted = true;
        }
    }

    // 0x0029 runningState — z2m thermostatRunningStates: 0=idle, 1=heat.
    // The Acova family exposes only {idle, heat}.
    if (const ::zhc::Value* v = msg.payload.find("41")) {
        if (v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::StringRef;
            o.str  = (v->u == 0) ? "idle" : "heat";
            out.put("running_state", o);
            emitted = true;
        }
    }

    return emitted;
}

}  // namespace

const ::zhc::FzConverter kFzAcovaThermostatExtras{
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
    .fn                = { .zcl_fn = fz_acova_thermostat },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::acova
