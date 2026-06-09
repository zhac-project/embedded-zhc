// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0

#include "definitions/amina/_shared.hpp"

#include <cstdint>

namespace zhc::devices::amina {

namespace {

using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::FzConverter;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::type_bit;

// Raw unsigned pass-through (u16 wire types). Matches the generic
// kFzElectricalMeasurement convention; the runtime applies the cluster
// multiplier/divisor downstream.
bool put_uint(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key,
              const Value* v) {
    if (!v || v->type != ValueType::Uint) return false;
    Value o{};
    o.type = ValueType::Uint;
    o.u    = v->u;
    return out.put(key, o);
}

// Raw signed pass-through (s16/s32 wire types). Accepts an unsigned
// record too (some stacks surface small magnitudes as Uint), widening
// to the signed slot — mirroring the generic active-power branch.
bool put_int(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key,
             const Value* v) {
    if (!v) return false;
    if (v->type != ValueType::Int && v->type != ValueType::Uint) return false;
    Value o{};
    o.type = ValueType::Int;
    o.i    = v->type == ValueType::Int ? v->i
                                       : static_cast<std::int64_t>(v->u);
    return out.put(key, o);
}

// haElectricalMeasurement (0x0B04): the frequency, total-active-power and
// per-phase b/c channels z2m's `m.electricityMeter`
// (cluster:"electrical", acFrequency, threePhase) + the standalone
// `total_active_power` numeric decode but the generic
// `kFzElectricalMeasurement` (power 0x050B / voltage 0x0505 /
// current 0x0508 only) drops.
bool fz_amina_elec_extras(const DecodedMessage& msg,
                          const FzConverter&,
                          const PreparedDefinition&,
                          RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // attr 0x0300 (768) — ACFrequency (u16). z2m key "ac_frequency".
    if (put_uint(out, "ac_frequency", msg.payload.find("768"))) emitted = true;

    // attr 0x0304 (772) — TotalActivePower (s32). z2m amina
    // `total_active_power` numeric. The device reports its instantaneous
    // power here (not via activePower 0x050B), so this is the primary
    // power channel — the generic ELM's `power` (0x050B) stays wired for
    // completeness but is not what this firmware populates.
    if (put_int(out, "total_active_power", msg.payload.find("772"))) emitted = true;

    // Per-phase B/C (threePhase:true). Power s16, voltage/current u16.
    //   activePowerPhB 0x090B (2315) / PhC 0x0A0B (2571) -> power_phase_b/c
    //   rmsVoltagePhB  0x0905 (2309) / PhC 0x0A05 (2565) -> voltage_phase_b/c
    //   rmsCurrentPhB  0x0908 (2312) / PhC 0x0A08 (2568) -> current_phase_b/c
    if (put_int(out, "power_phase_b", msg.payload.find("2315"))) emitted = true;
    if (put_int(out, "power_phase_c", msg.payload.find("2571"))) emitted = true;
    if (put_uint(out, "voltage_phase_b", msg.payload.find("2309"))) emitted = true;
    if (put_uint(out, "voltage_phase_c", msg.payload.find("2565"))) emitted = true;
    if (put_uint(out, "current_phase_b", msg.payload.find("2312"))) emitted = true;
    if (put_uint(out, "current_phase_c", msg.payload.find("2568"))) emitted = true;

    return emitted;
}

}  // namespace

extern const FzConverter kFzAminaElectricalMeasurementExtras{
    .family            = FrameFamily::Zcl,
    .cluster           = "haElectricalMeasurement",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_amina_elec_extras },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::amina
