// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Vimar vendor-shared runtime. Decodes the standard hvacThermostat
// (0x0201) cooling-setpoint attribute that the generic `kFzThermostat`
// ignores but that the 02973.B thermostat exposes via z2m's
// `fz.thermostat`:
//
//   * 0x0011 OccupiedCoolingSetpoint → occupied_cooling_setpoint
//
// Pattern cloned from `definitions/leviton/_shared.cpp`. The standard
// local_temperature / current_heating_setpoint / system_mode attrs are
// still handled by the generic `kFzThermostat`, which is wired alongside
// this converter on the device. The raw int16 (1/100 °C) is forwarded
// unscaled, matching the generic kFzThermostat numeric convention.
//
// z2m-source: zigbee-herdsman-converters/src/devices/vimar.ts #02973.B
// (fz.thermostat → occupied_cooling_setpoint).
#include "definitions/vimar/_shared.hpp"

namespace zhc::devices::vimar {

using ::zhc::FzConverter;
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

bool fz_vimar_thermostat_extras(const DecodedMessage& msg,
                                const FzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext&,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attr 0x0011 — OccupiedCoolingSetpoint (s16, raw 1/100 °C).
    if (const Value* v = msg.payload.find("17")) {
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Int;
            o.i = v->type == ValueType::Int ? v->i
                                            : static_cast<std::int64_t>(v->u);
            if (out.put("occupied_cooling_setpoint", o)) return true;
        }
    }
    return false;
}

}  // namespace

extern const FzConverter kFzVimarThermostatExtras{
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
    .fn                = { .zcl_fn = fz_vimar_thermostat_extras },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::vimar
