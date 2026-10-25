// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0

#include "definitions/airzone_aidoo/_shared.hpp"

#include <cstdint>

namespace zhc::airzone_aidoo {

namespace {

// Decodes the standard hvacThermostat cooling-setpoint attribute the
// generic `kFzThermostat` skips. local_temperature /
// current_heating_setpoint / system_mode are still emitted by the
// generic converter wired alongside this one on the AZAI6ZBEMHI def.
bool fz_airzone_thermostat_extras(const DecodedMessage& msg,
                                  const FzConverter&,
                                  const PreparedDefinition&,
                                  RuntimeContext&,
                                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attr 0x0011 — OccupiedCoolingSetpoint (s16, 0.01 °C wire units) in
    // °C, like the generic heating setpoint. z2m key:
    // "occupied_cooling_setpoint"; the Airzone climate exposes surface
    // it as "current_cooling_setpoint".
    return generic::put_thermostat_celsius(out, "current_cooling_setpoint",
                                           msg.payload.find("17"), 100, false);
}

}  // namespace

extern const FzConverter kFzAirzoneThermostatExtras{
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
    .fn                = { .zcl_fn = &fz_airzone_thermostat_extras },
    .user_config       = nullptr,
};

}  // namespace zhc::airzone_aidoo
