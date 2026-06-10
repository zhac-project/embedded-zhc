// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0

#include "definitions/imhotepcreation/_shared.hpp"

#include <cstdint>

namespace zhc::imhotepcreation {

namespace {

// Decodes the standard hvacThermostat attributes the generic
// `kFzThermostat` skips. local_temperature / current_heating_setpoint
// / system_mode are still emitted by the generic converter wired
// alongside this one on the E-Ctrl / BRI4P defs.
//
// Every value is a raw s16 pass-through as Int (the runtime scales /100
// downstream), mirroring the generic heating-setpoint branch and z2m's
// `precisionRound(x, 2) / 100` in fz.thermostat.
bool fz_imhotep_thermostat_extras(const DecodedMessage& msg,
                                  const FzConverter&,
                                  const PreparedDefinition&,
                                  RuntimeContext&,
                                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    struct AttrKey {
        const char* attr;  // decimal-string attribute id
        const char* key;   // emitted semantic key
    };
    // attr 0x0011 = 17, 0x0015 = 21, 0x0016 = 22, 0x0017 = 23, 0x0018 = 24.
    static constexpr AttrKey kMap[] = {
        {"17", "current_cooling_setpoint"},  // OccupiedCoolingSetpoint
        {"21", "min_heat_setpoint_limit"},   // MinHeatSetpointLimit
        {"22", "max_heat_setpoint_limit"},   // MaxHeatSetpointLimit
        {"23", "min_cool_setpoint_limit"},   // MinCoolSetpointLimit
        {"24", "max_cool_setpoint_limit"},   // MaxCoolSetpointLimit
    };

    for (const auto& m : kMap) {
        const Value* v = msg.payload.find(m.attr);
        if (!v) continue;
        if (v->type != ValueType::Int && v->type != ValueType::Uint) continue;
        Value o{};
        o.type = ValueType::Int;
        o.i    = v->type == ValueType::Int
                     ? v->i
                     : static_cast<std::int64_t>(v->u);
        if (out.put(m.key, o)) emitted = true;
    }

    return emitted;
}

}  // namespace

extern const FzConverter kFzImhotepThermostatExtras{
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
    .fn                = { .zcl_fn = &fz_imhotep_thermostat_extras },
    .user_config       = nullptr,
};

}  // namespace zhc::imhotepcreation
