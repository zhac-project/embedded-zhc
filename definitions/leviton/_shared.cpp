// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Leviton vendor-shared runtime. Decodes the standard hvacThermostat
// (0x0201) attributes that the generic `kFzThermostat` ignores but that
// the RC-2000WH (Omnistat2) thermostat exposes via z2m's `fz.thermostat`:
//
//   * 0x0008 PIHeatingDemand            → pi_heating_demand
//   * 0x0010 LocalTemperatureCalibration → local_temperature_calibration
//   * 0x0011 OccupiedCoolingSetpoint     → occupied_cooling_setpoint
//
// Pattern cloned from `definitions/danfoss/_shared.cpp`. The standard
// local_temperature / current_heating_setpoint / system_mode attrs are
// still handled by the generic `kFzThermostat`, which is wired alongside
// this converter on the device.
//
// z2m-source: zigbee-herdsman-converters/src/devices/leviton.ts #RC-2000WH
// (fz.thermostat). Leviton does NOT set `dontMapPIHeatingDemand`, so
// pi_heating_demand is `mapNumberRange(raw, 0, 255, 0, 100)` (round-half-up).
#include "definitions/leviton/_shared.hpp"

namespace zhc::devices::leviton {

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

// Coerce a decoded numeric Value into a signed integer (handles the
// decoder emitting either Int or Uint for the same attribute).
bool as_int(const Value& v, std::int64_t& out_i) {
    if (v.type == ValueType::Int)  { out_i = v.i; return true; }
    if (v.type == ValueType::Uint) { out_i = static_cast<std::int64_t>(v.u); return true; }
    return false;
}

bool fz_leviton_thermostat(const DecodedMessage& msg,
                           const FzConverter&,
                           const PreparedDefinition&,
                           RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // attr 0x0008 — PIHeatingDemand (u8 0-255). z2m maps to a 0-100 %
    // via mapNumberRange(raw, 0, 255, 0, 100) since Leviton does not set
    // dontMapPIHeatingDemand. Integer round-half-up: (raw*100 + 127)/255.
    if (const Value* v = msg.payload.find("8")) {
        std::int64_t raw = 0;
        if (as_int(*v, raw) && raw >= 0) {
            if (raw > 255) raw = 255;
            const std::int64_t pct = (raw * 100 + 127) / 255;
            Value o{}; o.type = ValueType::Uint;
            o.u = static_cast<std::uint64_t>(pct);
            if (out.put("pi_heating_demand", o)) emitted = true;
        }
    }

    // attr 0x0010 — LocalTemperatureCalibration (s8, raw 1/10 °C step in
    // z2m precisionRound(v,2)/10; we forward the raw integer to match the
    // generic kFzThermostat numeric convention — no host-side scaling).
    if (const Value* v = msg.payload.find("16")) {
        std::int64_t cal = 0;
        if (as_int(*v, cal)) {
            Value o{}; o.type = ValueType::Int; o.i = cal;
            if (out.put("local_temperature_calibration", o)) emitted = true;
        }
    }

    // attr 0x0011 — OccupiedCoolingSetpoint (s16, raw 1/100 °C).
    if (const Value* v = msg.payload.find("17")) {
        std::int64_t sp = 0;
        if (as_int(*v, sp)) {
            Value o{}; o.type = ValueType::Int; o.i = sp;
            if (out.put("occupied_cooling_setpoint", o)) emitted = true;
        }
    }

    return emitted;
}

}  // namespace

extern const FzConverter kFzLevitonThermostat{
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
    .fn                = { .zcl_fn = fz_leviton_thermostat },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::leviton
