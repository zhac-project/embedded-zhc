// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Stelpro vendor converters. See _shared.hpp for the list of
// manu-specific attributes covered.

#include "definitions/stelpro/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

#include <cstring>

namespace zhc::stelpro {

namespace {

constexpr std::uint16_t kStelproMfg = 0x1185;

// fz_stelpro_thermostat: mirrors fzLocal.stelpro_thermostat in z2m.
//
// First runs the generic thermostat decoder to populate
// local_temperature / current_heating_setpoint / system_mode, then
// applies the two Stelpro tweaks:
//   * if attr 0x401C (stelproSystemMode raw key "16412") == 5, override
//     system_mode → "auto" (Eco mode).
//   * if attr 0x0008 (pIHeatingDemand, key "8") is present, derive
//     running_state: >= 10 → "heat", else → "idle".
bool fz_stelpro_thermostat(const ::zhc::DecodedMessage& msg,
                            const ::zhc::FzConverter& self,
                            const ::zhc::PreparedDefinition& def,
                            ::zhc::RuntimeContext& ctx,
                            ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Delegate the standard attrs first.
    bool emitted = ::zhc::generic::kFzThermostat.fn.zcl_fn(msg, self, def, ctx, out);

    // 0x401C stelproSystemMode (uint key "16412") == 5 ⇒ system_mode = "auto"
    if (const ::zhc::Value* v = msg.payload.find("16412")) {
        if (v->type == ::zhc::ValueType::Uint && v->u == 5) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::StringRef;
            o.str  = "auto";
            out.put("system_mode", o);
            emitted = true;
        }
    }

    // 0x0008 pIHeatingDemand (uint key "8") ⇒ running_state
    if (const ::zhc::Value* v = msg.payload.find("8")) {
        if (v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::StringRef;
            o.str  = (v->u >= 10) ? "heat" : "idle";
            out.put("running_state", o);
            emitted = true;
        }
    }

    return emitted;
}

// fz_stelpro_power: HT402 attr 0x4008 (power, u16 W). Key "16392".
bool fz_stelpro_power(const ::zhc::DecodedMessage& msg,
                       const ::zhc::FzConverter&,
                       const ::zhc::PreparedDefinition&,
                       ::zhc::RuntimeContext&,
                       ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (const ::zhc::Value* v = msg.payload.find("16392")) {
        if (v->type == ::zhc::ValueType::Uint || v->type == ::zhc::ValueType::Int) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Uint;
            o.u = v->type == ::zhc::ValueType::Uint ? v->u
                                                    : static_cast<std::uint64_t>(v->i);
            out.put("power", o);
            return true;
        }
    }
    return false;
}

// fz_stelpro_energy: HT402 attr 0x4009 (energy, Wh). Key "16393".
// z2m divides by 1000 to publish kWh; we mirror that.
bool fz_stelpro_energy(const ::zhc::DecodedMessage& msg,
                        const ::zhc::FzConverter&,
                        const ::zhc::PreparedDefinition&,
                        ::zhc::RuntimeContext&,
                        ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (const ::zhc::Value* v = msg.payload.find("16393")) {
        if (v->type == ::zhc::ValueType::Uint || v->type == ::zhc::ValueType::Int) {
            const std::uint64_t raw = v->type == ::zhc::ValueType::Uint
                                          ? v->u
                                          : static_cast<std::uint64_t>(v->i);
            ::zhc::Value o{};
            // Embed as float to preserve fractional kWh.
            o.type = ::zhc::ValueType::Float;
            o.f = static_cast<float>(raw) / 1000.0f;
            out.put("energy", o);
            return true;
        }
    }
    return false;
}

// ── Tz outdoor temperature spec (manu 0x1185) ──────────────────────
//
// z2m multiplies the user value by 100 (°C → 1/100 °C). The shared
// `tz_zcl_write_attr` does not currently scale, so we do it inline
// via a tiny adapter that produces a temporary scaled spec call.
constexpr ::zhc::generic::ZclWriteSpec kSpecStelproOutdoorTemp{
    "outdoor_temperature_display",  // input key
    0x4001,                         // attr_id (stelproOutdoorTemp)
    0x29,                           // attr_type int16
    kStelproMfg,                    // mfgcode 0x1185
    nullptr, 0,
};

// Custom Tz: scale (°C * 100) before delegating to tz_zcl_write_attr.
bool tz_stelpro_outdoor_temp(std::string_view key,
                              const ::zhc::Value& input,
                              const ::zhc::TzConverter& self,
                              const ::zhc::PreparedDefinition& def,
                              ::zhc::RuntimeContext& ctx,
                              std::span<std::uint8_t> out_frame,
                              std::size_t& out_size) {
    out_size = 0;
    if (key != "outdoor_temperature_display") return false;
    if (input.type != ::zhc::ValueType::Int &&
        input.type != ::zhc::ValueType::Uint &&
        input.type != ::zhc::ValueType::Float) return false;

    // Range-check (z2m: -32..119 °C).
    double celsius = 0.0;
    if (input.type == ::zhc::ValueType::Int)        celsius = static_cast<double>(input.i);
    else if (input.type == ::zhc::ValueType::Uint)  celsius = static_cast<double>(input.u);
    else                                            celsius = input.f;
    if (celsius < -32.0 || celsius > 119.0) return false;

    ::zhc::Value scaled{};
    scaled.type = ::zhc::ValueType::Int;
    scaled.i = static_cast<std::int64_t>(celsius * 100.0);

    return ::zhc::generic::tz_zcl_write_attr(
        key, scaled, self, def, ctx, out_frame, out_size);
}

// fz_stelpro_keypad_lockout: hvacUserInterfaceCfg.keypadLockout is attr
// 0x0001 (ENUM8). Mirrors z2m fz.hvac_user_interface — emit the raw
// ENUM8 as a uint (z2m's constants.keypadLockoutMode lookup falls back
// to the raw number for unknown codes anyway).
bool fz_stelpro_keypad_lockout(const ::zhc::DecodedMessage& msg,
                                const ::zhc::FzConverter&,
                                const ::zhc::PreparedDefinition&,
                                ::zhc::RuntimeContext&,
                                ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const ::zhc::Value* v = msg.payload.find("1");  // 0x0001 keypadLockout
    if (!v) return false;
    ::zhc::Value o{};
    o.type = ::zhc::ValueType::Uint;
    if      (v->type == ::zhc::ValueType::Uint) o.u = v->u;
    else if (v->type == ::zhc::ValueType::Int)  o.u = static_cast<std::uint64_t>(v->i);
    else if (v->type == ::zhc::ValueType::Bool) o.u = v->b ? 1 : 0;
    else return false;
    out.put("keypad_lockout", o);
    return true;
}

// keypad_lockout: non-manu ENUM8 write to hvacUserInterfaceCfg attr
// 0x0001 (mfg=0). Mirrors z2m tz.thermostat_keypad_lockout.
constexpr ::zhc::generic::ZclWriteSpec kSpecStelproKeypadLockout{
    "keypad_lockout", 0x0001, 0x30, 0, nullptr, 0,
};

// peak_demand_icon: HT402 only. Standard manu-write to attr 0x4012
// (u8) at mfgcode 0x1185. The TS calls this "stelpro_peak_demand_
// event_icon" — encoded as a writeAttributes here for v1.
constexpr ::zhc::generic::ZclWriteSpec kSpecStelproPeakDemandIcon{
    "peak_demand_icon", 0x4012, 0x20, kStelproMfg, nullptr, 0,
};

}  // namespace

extern const ::zhc::FzConverter kFzStelproThermostat{
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
    .fn                = { .zcl_fn = fz_stelpro_thermostat },
    .user_config       = nullptr,
};

extern const ::zhc::FzConverter kFzStelproPower{
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
    .fn                = { .zcl_fn = fz_stelpro_power },
    .user_config       = nullptr,
};

extern const ::zhc::FzConverter kFzStelproEnergy{
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
    .fn                = { .zcl_fn = fz_stelpro_energy },
    .user_config       = nullptr,
};

extern const ::zhc::TzConverter kTzStelproOutdoorTemp{
    .key         = "outdoor_temperature_display",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = tz_stelpro_outdoor_temp,
    .user_config = &kSpecStelproOutdoorTemp,
};

extern const ::zhc::TzConverter kTzStelproPeakDemandIcon{
    .key         = "peak_demand_icon",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecStelproPeakDemandIcon,
};

extern const ::zhc::FzConverter kFzStelproKeypadLockout{
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
    .fn                = { .zcl_fn = fz_stelpro_keypad_lockout },
    .user_config       = nullptr,
};

extern const ::zhc::TzConverter kTzStelproKeypadLockout{
    .key         = "keypad_lockout",
    .cluster     = "hvacUserInterfaceCfg",
    .cluster_id  = 0x0204,
    .command_id  = 0x02,                 // writeAttributes
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecStelproKeypadLockout,
};

}  // namespace zhc::stelpro
