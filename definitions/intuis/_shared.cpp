// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Intuis shared converters — see _shared.hpp for rationale.
//
// z2m-source: intuis.ts via converters/fromZigbee.ts
//             #thermostat + #hvac_user_interface.
#include "definitions/intuis/_shared.hpp"

namespace zhc::devices::intuis {
namespace {

// ── Vendor FZ: Intuis thermostat decoder (hvacThermostat 0x0201) ─────
// Standalone (does NOT delegate to the generic kFzThermostat) because
// the generic decoder emits attr 0x0012 under "current_heating_setpoint"
// — the wrong key for the intuisradiator, which exposes
// "occupied_heating_setpoint" — and ignores 0x0014 / 0x0029 entirely.
bool fz_intuis_thermostat(const ::zhc::DecodedMessage& msg,
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

    // 0x0029 runningState — z2m thermostatRunningStates: 0=idle, 1=heat.
    // The intuisradiator exposes only {idle, heat}.
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

// ── Vendor FZ: hvacUserInterfaceCfg decoder (0x0204) ─────────────────
// Closes z2m fz.hvac_user_interface for the two attrs the
// intuisradiator exposes. keypad_lockout maps via
// constants.keypadLockoutMode (0→unlock, 1..5→lock1..lock5);
// temperature_display_mode maps 0→celsius, 1→fahrenheit. Unknown enum
// values are dropped (matching z2m's lookup-miss path, which would
// otherwise publish the raw number — we keep the string contract).
bool fz_intuis_hvac_ui(const ::zhc::DecodedMessage& msg,
                       const ::zhc::FzConverter&,
                       const ::zhc::PreparedDefinition&,
                       ::zhc::RuntimeContext&,
                       ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // 0x0000 keypadLockout (enum8).
    if (const ::zhc::Value* v = msg.payload.find("0")) {
        if (v->type == ::zhc::ValueType::Uint) {
            const char* label = nullptr;
            switch (v->u) {
                case 0: label = "unlock"; break;
                case 1: label = "lock1";  break;
                case 2: label = "lock2";  break;
                case 3: label = "lock3";  break;
                case 4: label = "lock4";  break;
                case 5: label = "lock5";  break;
                default: break;
            }
            if (label) {
                ::zhc::Value o{};
                o.type = ::zhc::ValueType::StringRef;
                o.str  = label;
                out.put("keypad_lockout", o);
                emitted = true;
            }
        }
    }

    // 0x0001 tempDisplayMode (enum8): 0=celsius, 1=fahrenheit.
    if (const ::zhc::Value* v = msg.payload.find("1")) {
        if (v->type == ::zhc::ValueType::Uint) {
            const char* label = nullptr;
            switch (v->u) {
                case 0: label = "celsius";    break;
                case 1: label = "fahrenheit"; break;
                default: break;
            }
            if (label) {
                ::zhc::Value o{};
                o.type = ::zhc::ValueType::StringRef;
                o.str  = label;
                out.put("temperature_display_mode", o);
                emitted = true;
            }
        }
    }

    return emitted;
}

}  // namespace

const ::zhc::FzConverter kFzIntuisThermostat{
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
    .fn                = { .zcl_fn = fz_intuis_thermostat },
    .user_config       = nullptr,
};

const ::zhc::FzConverter kFzIntuisHvacUi{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "hvacUserInterfaceCfg",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_intuis_hvac_ui },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::intuis
