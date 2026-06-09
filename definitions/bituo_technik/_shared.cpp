// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0

#include "definitions/bituo_technik/_shared.hpp"

#include <cstdint>

namespace zhc::bituo_technik {

namespace {

// Raw unsigned pass-through (u16/u24/u48 wire types). Matches the
// generic kFzMetering/kFzElectricalMeasurement convention; the runtime
// applies the cluster multiplier/divisor downstream.
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

// haElectricalMeasurement (0x0B04): the per-phase, total, frequency,
// power-factor and reactive/apparent channels z2m's
// `m.electricityMeter` + `bituo_fz.total_power` decode but the generic
// `kFzElectricalMeasurement` (power/voltage/current only) drops.
bool fz_bituo_elec_extras(const DecodedMessage& msg,
                          const FzConverter&,
                          const PreparedDefinition&,
                          RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // attr 0x0300 (768) — ACFrequency (u16). z2m key "ac_frequency".
    if (put_uint(out, "ac_frequency", msg.payload.find("768"))) emitted = true;

    // attr 0x0510 (1296) — PowerFactor (s16). z2m key "power_factor".
    // Note: z2m leaves powerFactor un-scaled by acPowerDivisor; raw.
    if (put_int(out, "power_factor", msg.payload.find("1296"))) emitted = true;

    // attr 0x050E (1294) — ReactivePower (s16) -> "power_reactive".
    if (put_int(out, "power_reactive", msg.payload.find("1294"))) emitted = true;

    // attr 0x050F (1295) — ApparentPower (u16) -> "power_apparent".
    if (put_uint(out, "power_apparent", msg.payload.find("1295"))) emitted = true;

    // Total power (bituo_fz.total_power):
    //   attr 0x0304 (772)  TotalActivePower   (s32) -> "total_power"
    //   attr 0x0305 (773)  TotalReactivePower (s32) -> "total_power_reactive"
    //   attr 0x0306 (774)  TotalApparentPower (u32) -> "total_power_apparent"
    if (put_int(out, "total_power", msg.payload.find("772"))) emitted = true;
    if (put_int(out, "total_power_reactive", msg.payload.find("773")))
        emitted = true;
    if (put_uint(out, "total_power_apparent", msg.payload.find("774")))
        emitted = true;

    // Per-phase B/C (threePhase meters: SDM01W / SPM02; SDM02 explicitly
    // surfaces the phase-B trio too):
    //   activePowerPhB 0x090B (2315) / PhC 0x0A0B (2571) -> power_phase_b/c
    //   rmsVoltagePhB  0x0905 (2309) / PhC 0x0A05 (2565) -> voltage_phase_b/c
    //   rmsCurrentPhB  0x0908 (2312) / PhC 0x0A08 (2568) -> current_phase_b/c
    if (put_int(out, "power_phase_b", msg.payload.find("2315"))) emitted = true;
    if (put_int(out, "power_phase_c", msg.payload.find("2571"))) emitted = true;
    if (put_uint(out, "voltage_phase_b", msg.payload.find("2309")))
        emitted = true;
    if (put_uint(out, "voltage_phase_c", msg.payload.find("2565")))
        emitted = true;
    if (put_uint(out, "current_phase_b", msg.payload.find("2312")))
        emitted = true;
    if (put_uint(out, "current_phase_c", msg.payload.find("2568")))
        emitted = true;

    return emitted;
}

// seMetering (0x0702): produced energy and the per-phase tier-summation
// counters z2m's `m.electricityMeter` (producedEnergy) +
// `bituo_fz.phase_energy` decode but the generic `kFzMetering`
// (energy 0x0000 / power 0x0400 only) drops.
bool fz_bituo_metering_extras(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // attr 0x0001 (1) — CurrentSummReceived (u48) -> "produced_energy".
    if (put_uint(out, "produced_energy", msg.payload.find("1"))) emitted = true;

    // bituo_fz.phase_energy — per-phase tier summations (u48 each):
    //   0x0102 (258) Tier1SummDelivered -> energy_phase_a
    //   0x0103 (259) Tier1SummReceived  -> produced_energy_phase_a
    //   0x0104 (260) Tier2SummDelivered -> energy_phase_b
    //   0x0105 (261) Tier2SummReceived  -> produced_energy_phase_b
    //   0x0106 (262) Tier3SummDelivered -> energy_phase_c
    //   0x0107 (263) Tier3SummReceived  -> produced_energy_phase_c
    if (put_uint(out, "energy_phase_a", msg.payload.find("258"))) emitted = true;
    if (put_uint(out, "produced_energy_phase_a", msg.payload.find("259")))
        emitted = true;
    if (put_uint(out, "energy_phase_b", msg.payload.find("260"))) emitted = true;
    if (put_uint(out, "produced_energy_phase_b", msg.payload.find("261")))
        emitted = true;
    if (put_uint(out, "energy_phase_c", msg.payload.find("262"))) emitted = true;
    if (put_uint(out, "produced_energy_phase_c", msg.payload.find("263")))
        emitted = true;

    return emitted;
}

}  // namespace

extern const FzConverter kFzBituoElectricalMeasurementExtras{
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
    .fn                = { .zcl_fn = &fz_bituo_elec_extras },
    .user_config       = nullptr,
};

extern const FzConverter kFzBituoMeteringExtras{
    .family            = FrameFamily::Zcl,
    .cluster           = "seMetering",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_bituo_metering_extras },
    .user_config       = nullptr,
};

}  // namespace zhc::bituo_technik
