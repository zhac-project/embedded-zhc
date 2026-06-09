// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Ubisys (mfg 0x10F2) converter implementations.
// See definitions/ubisys/_shared.hpp for the cluster table.

#include "definitions/ubisys/_shared.hpp"

#include <cstdint>

namespace zhc::ubisys {

namespace {

// ── DimmerSetup write specs (mfgcode 0x10F2, BITMAP8 = 0x18) ──────
constexpr ::zhc::generic::ZclWriteSpec kSpecDimmerCapabilities{
    "capabilities", 0x0000, 0x18, kMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDimmerStatus{
    "status",       0x0001, 0x18, kMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDimmerMode{
    "mode",         0x0002, 0x18, kMfgCode, nullptr, 0,
};

// ── hvacThermostat extension write specs (mfgcode 0x10F2) ─────────
// INT8 = 0x28, INT16 = 0x29, UINT16 = 0x21, BOOLEAN = 0x10.
constexpr ::zhc::generic::ZclWriteSpec kSpecLocalTempOffset{
    "local_temperature_offset", 0x0010, 0x28, kMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecVacationMode{
    "vacation_mode",            0x0012, 0x10, kMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRemoteTemperature{
    "remote_temperature",       0x0013, 0x29, kMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRemoteTempDuration{
    "remote_temperature_duration", 0x0014, 0x21, kMfgCode, nullptr, 0,
};

}  // namespace

#define ZHC_UBISYS_TZ(var, spec_ref, key_str, cluster_str, cluster_hex)  \
    extern const TzConverter var{                                        \
        .key         = key_str,                                          \
        .cluster     = cluster_str,                                      \
        .cluster_id  = cluster_hex,                                      \
        .command_id  = 0x02,    /* writeAttributes */                    \
        .fn          = &::zhc::generic::tz_zcl_write_attr,               \
        .user_config = &spec_ref,                                        \
    };

ZHC_UBISYS_TZ(kTzUbisysDimmerCapabilities, kSpecDimmerCapabilities,
              "capabilities",
              "manuSpecificUbisysDimmerSetup", 0xFC01)
ZHC_UBISYS_TZ(kTzUbisysDimmerStatus,       kSpecDimmerStatus,
              "status",
              "manuSpecificUbisysDimmerSetup", 0xFC01)
ZHC_UBISYS_TZ(kTzUbisysDimmerMode,         kSpecDimmerMode,
              "mode",
              "manuSpecificUbisysDimmerSetup", 0xFC01)

ZHC_UBISYS_TZ(kTzUbisysLocalTempOffset,    kSpecLocalTempOffset,
              "local_temperature_offset",
              "hvacThermostat", 0x0201)
ZHC_UBISYS_TZ(kTzUbisysVacationMode,       kSpecVacationMode,
              "vacation_mode",
              "hvacThermostat", 0x0201)
ZHC_UBISYS_TZ(kTzUbisysRemoteTemperature,  kSpecRemoteTemperature,
              "remote_temperature",
              "hvacThermostat", 0x0201)
ZHC_UBISYS_TZ(kTzUbisysRemoteTempDuration, kSpecRemoteTempDuration,
              "remote_temperature_duration",
              "hvacThermostat", 0x0201)

#undef ZHC_UBISYS_TZ

// ── fz: dimmer-setup BITMAP8 decoder (0xFC01 attrs 0/1/2) ─────────
//
// Looks up attribute "0" (capabilities), "1" (status), "2" (mode) and
// fans out booleans per the z2m bit layout. We accept whichever of
// the three is present in this report — z2m's converter does the
// same (single attribute report per call).
bool fz_ubisys_dimmer_setup(const DecodedMessage& msg,
                             const FzConverter&,
                             const PreparedDefinition&,
                             RuntimeContext&,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    auto emit_bool = [&](const char* key, std::uint64_t v, std::uint8_t bit) {
        Value o{};
        o.type = ValueType::Bool;
        o.b    = ((v >> bit) & 1u) != 0;
        out.put(key, o);
    };

    auto u_of = [](const Value& v) -> std::uint64_t {
        if (v.type == ValueType::Uint) return v.u;
        if (v.type == ValueType::Int)  return static_cast<std::uint64_t>(v.i);
        return 0;
    };

    bool any = false;

    if (const Value* cap = msg.payload.find("0"); cap) {
        const std::uint64_t v = u_of(*cap);
        emit_bool("capabilities_forward_phase_control", v, 0);
        emit_bool("capabilities_reverse_phase_control", v, 1);
        emit_bool("capabilities_reactance_discriminator", v, 5);
        emit_bool("capabilities_configurable_curve",      v, 6);
        emit_bool("capabilities_overload_detection",      v, 7);
        any = true;
    }
    if (const Value* st = msg.payload.find("1"); st) {
        const std::uint64_t v = u_of(*st);
        emit_bool("status_forward_phase_control",   v, 0);
        emit_bool("status_reverse_phase_control",   v, 1);
        emit_bool("status_overload",                v, 2);
        emit_bool("status_capacitive_load",         v, 3);
        emit_bool("status_inductive_load",          v, 4);
        any = true;
    }
    if (const Value* md = msg.payload.find("2"); md) {
        const std::uint64_t v = u_of(*md);
        emit_bool("mode_phase_control_auto",        v, 0);
        emit_bool("mode_forward_phase_control",     v, 1);
        emit_bool("mode_reverse_phase_control",     v, 2);
        any = true;
    }

    return any;
}

extern const FzConverter kFzUbisysDimmerSetup{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificUbisysDimmerSetup",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_ubisys_dimmer_setup },
    .user_config       = nullptr,
};

// ── fz: hvacThermostat extras (0x0008 demand + 0x001E running mode) ─
//
// Decodes the two standard hvacThermostat attributes the generic
// `kFzThermostat` skips. local_temperature / current_heating_setpoint
// / system_mode are still emitted by the generic converter wired
// alongside this one on the H1 / H10 defs.
bool fz_ubisys_thermostat_extras(const DecodedMessage& msg,
                                 const FzConverter&,
                                 const PreparedDefinition&,
                                 RuntimeContext&,
                                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // attr 0x0008 — PIHeatingDemand (u8). z2m maps 0-255 → 0-100 %
    // (Ubisys does not set dontMapPIHeatingDemand). Use half-up
    // rounding to mirror the integer `%` expose.
    if (const Value* v = msg.payload.find("8")) {
        std::uint64_t raw = 0;
        bool ok = false;
        if (v->type == ValueType::Uint) { raw = v->u; ok = true; }
        else if (v->type == ValueType::Int && v->i >= 0) {
            raw = static_cast<std::uint64_t>(v->i); ok = true;
        }
        if (ok) {
            if (raw > 255) raw = 255;
            Value o{};
            o.type = ValueType::Uint;
            o.u    = (raw * 100u + 127u) / 255u;
            if (out.put("pi_heating_demand", o)) emitted = true;
        }
    }

    // attr 0x001E — ThermostatRunningMode (enum8). z2m lookup:
    // 0 → "off", 3 → "cool", 4 → "heat" (constants.thermostatRunningMode).
    // Unknown values are dropped, matching getFromLookup's miss.
    if (const Value* v = msg.payload.find("30")) {
        std::uint64_t mode = 0;
        bool ok = false;
        if (v->type == ValueType::Uint) { mode = v->u; ok = true; }
        else if (v->type == ValueType::Int && v->i >= 0) {
            mode = static_cast<std::uint64_t>(v->i); ok = true;
        }
        if (ok) {
            const char* label = nullptr;
            switch (mode) {
                case 0: label = "off";  break;
                case 3: label = "cool"; break;
                case 4: label = "heat"; break;
                default: break;
            }
            if (label) {
                Value o{};
                o.type = ValueType::StringRef;
                o.str  = label;
                if (out.put("running_mode", o)) emitted = true;
            }
        }
    }

    return emitted;
}

extern const FzConverter kFzUbisysThermostatExtras{
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
    .fn                = { .zcl_fn = &fz_ubisys_thermostat_extras },
    .user_config       = nullptr,
};

// ── fz: closuresWindowCovering lift + tilt ────────────────────────
//
// Emits "position" (attr 0x0008) and "tilt" (attr 0x0009). Values of
// 0xFF (255) are the J1's "unknown" sentinel and are skipped (z2m's
// `<= 100` guard). Raw 0-100 passthrough — same convention as the
// generic `kFzCoverPosition` (no inversion at this layer).
bool fz_ubisys_cover_position_tilt(const DecodedMessage& msg,
                                   const FzConverter&,
                                   const PreparedDefinition&,
                                   RuntimeContext&,
                                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    auto emit_pct = [&](const char* attr_key, const char* out_key) {
        if (const Value* v = msg.payload.find(attr_key)) {
            std::uint64_t pct = 0;
            bool ok = false;
            if (v->type == ValueType::Uint) { pct = v->u; ok = true; }
            else if (v->type == ValueType::Int && v->i >= 0) {
                pct = static_cast<std::uint64_t>(v->i); ok = true;
            }
            if (ok && pct <= 100) {
                Value o{};
                o.type = ValueType::Uint;
                o.u    = pct;
                if (out.put(out_key, o)) emitted = true;
            }
        }
    };

    emit_pct("8", "position");  // CurrentPositionLiftPercentage
    emit_pct("9", "tilt");      // CurrentPositionTiltPercentage
    return emitted;
}

extern const FzConverter kFzUbisysCoverPositionTilt{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresWindowCovering",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_ubisys_cover_position_tilt },
    .user_config       = nullptr,
};

}  // namespace zhc::ubisys
