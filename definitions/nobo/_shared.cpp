// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Nobø shared converters — see _shared.hpp for rationale.
//
// z2m-source: nobo.ts via converters/fromZigbee.ts #thermostat.
#include "definitions/nobo/_shared.hpp"

namespace zhc::devices::nobo {
namespace {

// ── Vendor FZ: Nobø thermostat decoder (hvacThermostat 0x0201) ───────
// Standalone (does NOT delegate to the generic kFzThermostat) because
// the generic decoder emits attr 0x0012 under "current_heating_setpoint"
// — the wrong key for the Nobø climate expose, which declares
// "occupied_heating_setpoint" — and ignores 0x0014 / 0x001E entirely.
bool fz_nobo_thermostat(const ::zhc::DecodedMessage& msg,
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

    // 0x001E runningMode — z2m thermostatRunningMode: 0=off, 3=cool,
    // 4=heat. These devices expose only {off, heat}; map heat=4, else
    // off (cool is not in the device's value set).
    if (const ::zhc::Value* v = msg.payload.find("30")) {
        if (v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::StringRef;
            o.str  = (v->u == 4) ? "heat" : "off";
            out.put("running_mode", o);
            emitted = true;
        }
    }

    return emitted;
}

}  // namespace

const ::zhc::FzConverter kFzNoboThermostat{
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
    .fn                = { .zcl_fn = fz_nobo_thermostat },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::nobo
