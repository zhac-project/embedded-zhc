// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Viessmann converters. See _shared.hpp for the list of
// emitted keys + attribute IDs and z2m source provenance.
//
// Pattern mirrors `definitions/danfoss/_shared.cpp`:
//   * `tz_zcl_write_attr` (in `_generic/_shared.cpp`) already encodes
//     fc=0x14 + mfg-code header — we just hand it a `ZclWriteSpec`.
//   * `fz_viessmann_thermostat` walks the decoded payload and emits
//     both the standard hvacThermostat surface (local_temperature
//     0x0000, occupied_heating_setpoint 0x0012, system_mode 0x001C —
//     matching z2m's `fz.thermostat` key set) and the Viessmann
//     manu-specific attrs (window_open / window_open_force /
//     assembly_mode). The generic `kFzThermostat` is not used on the
//     TRV because it emits 0x0012 under a different key.
#include "definitions/viessmann/_shared.hpp"

namespace zhc::devices::viessmann {

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

// ── fz_viessmann_thermostat ───────────────────────────────────────
// z2m `fzLocal.viessmann_thermostat` chains `fz.thermostat.convert(...)`
// (the standard surface) with three manu-specific extras. The standard
// `fz.thermostat` emits attr 0x0012 as `occupied_heating_setpoint` (the
// key the ZK03840 expose + z2m `.withSetpoint("occupied_heating_
// setpoint",…)` declares). The generic `::zhc::generic::kFzThermostat`
// instead emits 0x0012 as `current_heating_setpoint`, so we decode the
// whole standard surface here with z2m-faithful keys and DROP the
// generic decoder from this def — no phantom `current_heating_setpoint`.
//
// Standard attrs (z2m fz.thermostat):
//   0x0000 LocalTemperature        → local_temperature        (s16, raw 1/100 °C)
//   0x0012 OccupiedHeatingSetpoint → occupied_heating_setpoint (s16, raw 1/100 °C)
//   0x001C SystemMode              → system_mode               (enum8)
// (0x0008 pi_heating_demand is intentionally NOT emitted — z2m deletes it.)
//
// Manu-specific extras:
//   `window_open`       = (viessmannWindowOpenInternal === 3 || === 4)
//   `window_open_force` =  msg.data.viessmannWindowOpenForce === 1
//   `assembly_mode`     =  msg.data.viessmannAssemblyMode      === 1
//
// viessmannWindowOpenInternal (ENUM8) raw codes per z2m:
//   0-2, 5 → unknown  → window NOT open
//   3      → window open (OO on display, no heating)
//   4      → window open (OO on display, heating)
// The earlier port flattened this to `!= 0`, which false-flagged codes
// 1/2/5 as open. Match z2m: only 3 and 4 mean open.
bool fz_viessmann_thermostat(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // attr 0x0000 — LocalTemperature (s16).
    if (const Value* v = msg.payload.find("0")) {
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Int;
            o.i = v->type == ValueType::Int ? v->i
                                            : static_cast<std::int64_t>(v->u);
            if (out.put("local_temperature", o)) emitted = true;
        }
    }

    // attr 0x0012 — OccupiedHeatingSetpoint (s16) → occupied_heating_setpoint.
    if (const Value* v = msg.payload.find("18")) {
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Int;
            o.i = v->type == ValueType::Int ? v->i
                                            : static_cast<std::int64_t>(v->u);
            if (out.put("occupied_heating_setpoint", o)) emitted = true;
        }
    }

    // attr 0x001C — SystemMode (u8 enum).
    if (const Value* v = msg.payload.find("28")) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            if (out.put("system_mode", o)) emitted = true;
        }
    }

    // attr 0x4000 — viessmannWindowOpenInternal (ENUM8).
    if (const Value* v = msg.payload.find("16384")) {
        std::int64_t raw = 0;
        bool have = true;
        if      (v->type == ValueType::Uint) raw = static_cast<std::int64_t>(v->u);
        else if (v->type == ValueType::Int)  raw = v->i;
        else if (v->type == ValueType::Bool) raw = v->b ? 1 : 0;
        else                                 have = false;
        if (have) {
            Value o{}; o.type = ValueType::Bool; o.b = (raw == 3 || raw == 4);
            if (out.put("window_open", o)) emitted = true;
        }
    }

    // attr 0x4003 — viessmannWindowOpenForce (BOOLEAN).
    if (const Value* v = msg.payload.find("16387")) {
        bool b = false;
        if      (v->type == ValueType::Bool) b = v->b;
        else if (v->type == ValueType::Uint) b = v->u == 1;
        else if (v->type == ValueType::Int)  b = v->i == 1;
        Value o{}; o.type = ValueType::Bool; o.b = b;
        if (out.put("window_open_force", o)) emitted = true;
    }

    // attr 0x4012 — viessmannAssemblyMode (BOOLEAN).
    if (const Value* v = msg.payload.find("16402")) {
        bool b = false;
        if      (v->type == ValueType::Bool) b = v->b;
        else if (v->type == ValueType::Uint) b = v->u == 1;
        else if (v->type == ValueType::Int)  b = v->i == 1;
        Value o{}; o.type = ValueType::Bool; o.b = b;
        if (out.put("assembly_mode", o)) emitted = true;
    }

    return emitted;
}

// ── fz_co2 ─────────────────────────────────────────────────────────
// `msCO2.measuredValue` is a single-precision float fraction (e.g.
// 0.000420 for 420 ppm). z2m emits `co2 = floor(value * 1e6)` so we
// match.
bool fz_co2(const DecodedMessage& msg, const FzConverter&,
             const PreparedDefinition&, RuntimeContext&,
             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");
    if (!v) return false;
    double frac = 0.0;
    if      (v->type == ValueType::Float) frac = v->f;
    else if (v->type == ValueType::Uint)  frac = static_cast<double>(v->u);
    else if (v->type == ValueType::Int)   frac = static_cast<double>(v->i);
    else return false;
    Value o{}; o.type = ValueType::Int;
    o.i = static_cast<std::int64_t>(frac * 1000000.0);
    out.put("co2", o);
    return true;
}

// ── fz_keypad_lockout ──────────────────────────────────────────────
// `hvacUserInterfaceCfg.keypadLockout` is attr 0x0001 (ENUM8). We
// emit it as a raw uint — z2m's `constants.keypadLockoutMode` lookup
// falls back to the raw number for unknown codes anyway.
bool fz_keypad_lockout(const DecodedMessage& msg, const FzConverter&,
                        const PreparedDefinition&, RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("1");
    if (!v) return false;
    Value o{}; o.type = ValueType::Uint;
    if      (v->type == ValueType::Uint) o.u = v->u;
    else if (v->type == ValueType::Int)  o.u = static_cast<std::uint64_t>(v->i);
    else if (v->type == ValueType::Bool) o.u = v->b ? 1 : 0;
    else return false;
    out.put("keypad_lockout", o);
    return true;
}

// ── ZclWriteSpec — manuSpec writes ────────────────────────────────
constexpr ::zhc::generic::ZclWriteSpec kSpecWindowOpenForce{
    "window_open_force", 0x4003, 0x10, kViessmannMfg, nullptr, 0,
};

// Non-manu writeAttributes spec for keypadLockout (mfg=0).
constexpr ::zhc::generic::ZclWriteSpec kSpecKeypadLockout{
    "keypad_lockout", 0x0001, 0x30, 0, nullptr, 0,
};

}  // namespace

// ── FzConverters ──────────────────────────────────────────────────
extern const FzConverter kFzViessmannThermostat{
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
    .fn                = { .zcl_fn = &fz_viessmann_thermostat },
    .user_config       = nullptr,
};

extern const FzConverter kFzCo2{
    .family            = FrameFamily::Zcl,
    .cluster           = "msCO2",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_co2 },
    .user_config       = nullptr,
};

extern const FzConverter kFzKeypadLockout{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacUserInterfaceCfg",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_keypad_lockout },
    .user_config       = nullptr,
};

// ── TzConverters ──────────────────────────────────────────────────
extern const TzConverter kTzViessmannWindowOpenForce{
    .key         = "window_open_force",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,                 // writeAttributes
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecWindowOpenForce,
};

extern const TzConverter kTzKeypadLockout{
    .key         = "keypad_lockout",
    .cluster     = "hvacUserInterfaceCfg",
    .cluster_id  = 0x0204,
    .command_id  = 0x02,                 // writeAttributes
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecKeypadLockout,
};

}  // namespace zhc::devices::viessmann
