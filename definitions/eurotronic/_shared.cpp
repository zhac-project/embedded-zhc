// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared eurotronic converters.
//
// z2m-source:
//   zigbee-herdsman-converters/src/converters/fromZigbee.ts eurotronic_thermostat
//   zigbee-herdsman-converters/src/converters/toZigbee.ts   eurotronic_*
//
// All Eurotronic manu-specific traffic goes on cluster `hvacThermostat`
// (0x0201) with manufacturer code 0x1037 (NXP_SEMICONDUCTORS).

#include "definitions/eurotronic/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

#include <cstdint>
#include <cstring>

#include "zhc/runtime/dispatch.hpp"

namespace zhc::eurotronic {

namespace {

// Decimal-string keys produced by the foundation parser for the
// Eurotronic manu-specific attribute IDs.
//
//   0x4000 = 16384  trv_mode
//   0x4001 = 16385  valve_position
//   0x4002 = 16386  error_status
//   0x4003 = 16387  current_heating_setpoint
//   0x4008 = 16392  host_flags
constexpr const char* kKey_TrvMode        = "16384";
constexpr const char* kKey_ValvePos       = "16385";
constexpr const char* kKey_ErrorStatus    = "16386";
constexpr const char* kKey_CurrentSP      = "16387";
constexpr const char* kKey_HostFlags      = "16392";

// host_flags bit layout (z2m eurotronic_host_flags / fromZigbee):
//   bit 0 always 1 (set by every encoder)
//   bit 1 (0x02) — mirror_display
//   bit 2 (0x04) — heat
//   bit 4 (0x10) — auto       (also reported as off on some devices)
//   bit 5 (0x20) — off
//   bit 7 (0x80) — child_lock
constexpr std::uint32_t kHostBit_Base         = 0x01;
constexpr std::uint32_t kHostBit_MirrorOn     = 0x02;
constexpr std::uint32_t kHostBit_Heat         = 0x04;
constexpr std::uint32_t kHostBit_Auto         = 0x10;
constexpr std::uint32_t kHostBit_Off          = 0x20;
constexpr std::uint32_t kHostBit_ChildLock    = 0x80;
constexpr std::uint32_t kHostBit_OffMask      = kHostBit_Auto | kHostBit_Off;

// Pull a Value's integer payload regardless of underlying type.
bool to_uint(const Value& in, std::uint32_t& out) {
    switch (in.type) {
        case ValueType::Int:   out = static_cast<std::uint32_t>(in.i); return true;
        case ValueType::Uint:  out = static_cast<std::uint32_t>(in.u); return true;
        case ValueType::Float: out = static_cast<std::uint32_t>(in.f); return true;
        case ValueType::Bool:  out = in.b ? 1u : 0u;                   return true;
        default: return false;
    }
}

// Encode a writeAttributes(0x02) frame with manu-specific fc=0x14 and
// `kEurotronicMfgCode`. `vlen` bytes of `val` are written little-endian.
bool emit_write(std::uint16_t attr_id,
                 std::uint8_t  attr_type,
                 std::uint32_t val,
                 std::size_t   vlen,
                 std::span<std::uint8_t> out_frame,
                 std::size_t&  out_size) {
    const std::size_t total = 1 + 2 + 1 + 1 + 2 + 1 + vlen;  // fc+mfg+tsn+cmd+attr+type+val
    if (out_frame.size() < total) return false;
    std::size_t p = 0;
    out_frame[p++] = 0x14;                                                  // fc
    out_frame[p++] = static_cast<std::uint8_t>(kEurotronicMfgCode & 0xFF);
    out_frame[p++] = static_cast<std::uint8_t>((kEurotronicMfgCode >> 8) & 0xFF);
    out_frame[p++] = 0x00;                                                  // tsn placeholder
    out_frame[p++] = 0x02;                                                  // cmd = writeAttributes
    out_frame[p++] = static_cast<std::uint8_t>(attr_id & 0xFF);
    out_frame[p++] = static_cast<std::uint8_t>((attr_id >> 8) & 0xFF);
    out_frame[p++] = attr_type;
    for (std::size_t i = 0; i < vlen; ++i) {
        out_frame[p++] = static_cast<std::uint8_t>((val >> (i * 8)) & 0xFF);
    }
    out_size = total;
    return true;
}

// Compose the 24-bit host_flags value from a single proposed input.
//
// LIMITATION: z2m does a real read-modify-write — it reads attr 0x4008
// first, then ORs in the requested change. We don't have shadow access
// from the encoder so we emit the nominal bitfield that matches the
// scalar in isolation. Other axes default to "auto + lights up" until
// the next attribute report refreshes the cache. This is good enough
// for one-axis-at-a-time UI control (which is how the SPA writes).
std::uint32_t compose_host_flags(const char* key,
                                  std::uint32_t scalar,
                                  bool is_str,
                                  const char* str) {
    std::uint32_t v = kHostBit_Base | kHostBit_Auto;  // default: auto, no mirror
    if (std::strcmp(key, "system_mode") == 0 && is_str && str) {
        v = kHostBit_Base;
        if      (std::strcmp(str, "off")  == 0) v |= kHostBit_Off;
        else if (std::strcmp(str, "heat") == 0) v |= kHostBit_Heat;
        else                                    v |= kHostBit_Auto;
    } else if (std::strcmp(key, "mirror_display") == 0) {
        if (is_str && str) {
            if (std::strcmp(str, "ON") == 0) v |= kHostBit_MirrorOn;
        } else if (scalar) {
            v |= kHostBit_MirrorOn;
        }
    } else if (std::strcmp(key, "child_lock") == 0) {
        if (is_str && str) {
            if (std::strcmp(str, "LOCK") == 0) v |= kHostBit_ChildLock;
        } else if (scalar) {
            v |= kHostBit_ChildLock;
        }
    } else if (std::strcmp(key, "eurotronic_host_flags") == 0) {
        // Caller has already composed the bitfield — pass straight through
        // but force bit 0 high to match the device contract.
        v = scalar | kHostBit_Base;
    }
    return v & 0x00FFFFFFu;  // map24
}

// ── tz_eurotronic_trv_mode (attr 0x4000, enum8) ────────────────────
bool tz_trv_mode(std::string_view key, const Value& input,
                  const TzConverter&, const PreparedDefinition&,
                  RuntimeContext&,
                  std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;
    if (key != "trv_mode" && key != "eurotronic_trv_mode") return false;
    std::uint32_t v;
    if (!to_uint(input, v)) return false;
    return emit_write(0x4000, 0x30, v & 0xFF, 1, out_frame, out_size);
}

// ── tz_eurotronic_valve_position (attr 0x4001, u8) ─────────────────
bool tz_valve_pos(std::string_view key, const Value& input,
                   const TzConverter&, const PreparedDefinition&,
                   RuntimeContext&,
                   std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;
    if (key != "valve_position" && key != "eurotronic_valve_position") return false;
    std::uint32_t v;
    if (!to_uint(input, v)) return false;
    return emit_write(0x4001, 0x20, v & 0xFF, 1, out_frame, out_size);
}

// ── tz_eurotronic_current_heating_setpoint (attr 0x4003, s16) ──────
//
// z2m: rounds value to nearest 0.5°C and multiplies by 100. We accept
// Int / Uint / Float and emit `round(input * 2) / 2 * 100` little-endian.
bool tz_current_setpoint(std::string_view key, const Value& input,
                          const TzConverter&, const PreparedDefinition&,
                          RuntimeContext&,
                          std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;
    if (key != "current_heating_setpoint") return false;
    double f;
    if      (input.type == ValueType::Float) f = input.f;
    else if (input.type == ValueType::Int)   f = static_cast<double>(input.i);
    else if (input.type == ValueType::Uint)  f = static_cast<double>(input.u);
    else return false;
    // Round to 0.5 °C — match z2m's `Math.round(value * 2) / 2`.
    const std::int32_t halves = static_cast<std::int32_t>(f * 2.0 + (f >= 0 ? 0.5 : -0.5));
    const std::int32_t raw    = halves * 50;  // (halves / 2) * 100
    return emit_write(0x4003, 0x29,
                       static_cast<std::uint32_t>(raw) & 0xFFFFu,
                       2, out_frame, out_size);
}

// ── tz_eurotronic_host_flags  (attr 0x4008, map24) ─────────────────
//
// One encoder serves "system_mode", "mirror_display", "child_lock",
// and the raw "eurotronic_host_flags" key. See compose_host_flags()
// for the read-modify-write caveat.
bool tz_host_flags(std::string_view key, const Value& input,
                    const TzConverter& self, const PreparedDefinition&,
                    RuntimeContext&,
                    std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;
    // The TzConverter wires `user_config` to one of the static keys
    // below — that lets a single function back four converter objects
    // without each carrying its own body.
    const char* expected = static_cast<const char*>(self.user_config);
    if (!expected) return false;
    if (key != expected) return false;

    std::uint32_t scalar = 0;
    const bool is_str = (input.type == ValueType::StringRef);
    if (!is_str) (void)to_uint(input, scalar);
    const std::uint32_t composed =
        compose_host_flags(expected, scalar, is_str, is_str ? input.str : nullptr);
    return emit_write(0x4008, 0x22, composed, 3, out_frame, out_size);
}

// Static keys handed to tz_host_flags via `user_config`.
constexpr char kKeyHostFlags[]    = "eurotronic_host_flags";
constexpr char kKeySystemMode[]   = "system_mode";
constexpr char kKeyMirror[]       = "mirror_display";
constexpr char kKeyChildLock[]    = "child_lock";

}  // namespace

// ── fz_eurotronic_thermostat ───────────────────────────────────────
//
// Decodes a hvacThermostat attribute report and emits both the
// generic thermostat keys (handled by the parent kFzThermostat at
// dispatch time) and the Eurotronic-specific extras.
bool fz_eurotronic_thermostat(const DecodedMessage& msg,
                               const FzConverter&,
                               const PreparedDefinition&,
                               RuntimeContext&,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // Standard thermostat attrs (mirror the kFzThermostat decoder so
    // a device can wire just kFzEurotronicThermostat).
    if (const Value* v = msg.payload.find("0")) {        // LocalTemperature
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Int;
            o.i = (v->type == ValueType::Int) ? v->i : static_cast<std::int64_t>(v->u);
            out.put("local_temperature", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find("18")) {       // OccupiedHeatingSetpoint
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Int;
            o.i = (v->type == ValueType::Int) ? v->i : static_cast<std::int64_t>(v->u);
            out.put("occupied_heating_setpoint", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find("28")) {       // SystemMode (enum8)
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("system_mode", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find("8")) {        // PI Heating Demand
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("pi_heating_demand", o); emitted = true;
            // Mirror z2m: pIHeatingDemand >= 10 → "heat" else "idle".
            Value rs{}; rs.type = ValueType::StringRef;
            rs.str = (v->u >= 10) ? "heat" : "idle";
            out.put("running_state", rs);
        }
    }

    // Eurotronic manu-specific extras (0x4000..0x4008).
    if (const Value* v = msg.payload.find(kKey_TrvMode)) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("trv_mode", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find(kKey_ValvePos)) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("valve_position", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find(kKey_ErrorStatus)) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("error_status", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find(kKey_CurrentSP)) {
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Int;
            o.i = (v->type == ValueType::Int) ? v->i : static_cast<std::int64_t>(v->u);
            out.put("current_heating_setpoint", o); emitted = true;
        }
    }
    if (const Value* v = msg.payload.find(kKey_HostFlags)) {
        if (v->type == ValueType::Uint) {
            const std::uint32_t hf = static_cast<std::uint32_t>(v->u);

            Value cl{}; cl.type = ValueType::StringRef;
            cl.str = (hf & kHostBit_ChildLock) ? "LOCK" : "UNLOCK";
            out.put("child_lock", cl);

            Value md{}; md.type = ValueType::StringRef;
            md.str = (hf & kHostBit_MirrorOn) ? "ON" : "OFF";
            out.put("mirror_display", md);

            // Match z2m heuristic: bit 4 or 5 set → reported as off.
            Value sm{}; sm.type = ValueType::StringRef;
            if      (hf & kHostBit_OffMask) sm.str = "off";
            else if (hf & kHostBit_Heat)    sm.str = "heat";
            else                            sm.str = "auto";
            out.put("system_mode", sm);

            emitted = true;
        }
    }
    return emitted;
}

extern const FzConverter kFzEurotronicThermostat{
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
    .fn                = { .zcl_fn = fz_eurotronic_thermostat },
    .user_config       = nullptr,
};

// ── TzConverter descriptors ────────────────────────────────────────

extern const TzConverter kTzEurotronicTrvMode{
    .key         = "trv_mode",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = tz_trv_mode,
    .user_config = nullptr,
};

extern const TzConverter kTzEurotronicValvePosition{
    .key         = "valve_position",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = tz_valve_pos,
    .user_config = nullptr,
};

extern const TzConverter kTzEurotronicCurrentSetpoint{
    .key         = "current_heating_setpoint",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = tz_current_setpoint,
    .user_config = nullptr,
};

extern const TzConverter kTzEurotronicHostFlags{
    .key         = "eurotronic_host_flags",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = tz_host_flags,
    .user_config = kKeyHostFlags,
};

extern const TzConverter kTzEurotronicMirrorDisplay{
    .key         = "mirror_display",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = tz_host_flags,
    .user_config = kKeyMirror,
};

extern const TzConverter kTzEurotronicChildLock{
    .key         = "child_lock",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = tz_host_flags,
    .user_config = kKeyChildLock,
};

}  // namespace zhc::eurotronic
