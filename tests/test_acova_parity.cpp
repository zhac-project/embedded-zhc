// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Acova electric-radiator thermostats
// (ALCANTARA2, TAFFETAS2/PERCALE2, IHC-Enki). Pins the gap fixed when
// the defs were graduated out of generated/:
//
//   * The generic kFzThermostat only decodes hvacThermostat 0x0000 /
//     0x0012 / 0x001C, and — critically — emits attr 0x0012 under the
//     key "current_heating_setpoint". All three Acova defs expose
//     "occupied_heating_setpoint" (z2m fz.thermostat / e.climate().
//     withSetpoint("occupied_heating_setpoint")), so the occupied
//     setpoint was a DEAD key. The unoccupied setpoint (0x0014),
//     running_state (0x0029) and local_temperature_calibration (0x0010)
//     were not decoded at all.
//   * Now routed through kFzAcovaThermostatExtras, which decodes the
//     full Acova surface with z2m's keys.
//
// z2m-source: zigbee-herdsman-converters/src/devices/acova.ts +
//             converters/fromZigbee.ts #thermostat.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::acova {
extern const PreparedDefinition kDef_ALCANTARA2;
extern const PreparedDefinition kDef_TAFFETAS2_PERCALE2;
extern const PreparedDefinition kDef_IHC_Enki;
}  // namespace zhc::devices::acova

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t src_ep,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then one record.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool str_is(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}

bool int_is(const Value* v, std::int64_t i) {
    return v && v->type == ValueType::Int && v->i == i;
}

}  // namespace

int main() {
    using namespace zhc::devices::acova;

    // ── ALCANTARA2 ───────────────────────────────────────────────────
    {
        const PreparedDefinition& def = kDef_ALCANTARA2;
        assert(def_exposes(def, "local_temperature"));
        assert(def_exposes(def, "occupied_heating_setpoint"));
        assert(def_exposes(def, "unoccupied_heating_setpoint"));
        assert(def_exposes(def, "system_mode"));
        assert(def_exposes(def, "running_state"));

        // local_temperature (0x0000 s16) = 2100 (21.00 °C raw).
        {
            const std::uint8_t b[] = {0x34, 0x08};
            auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                                  attr_report(0x0000, 0x29, b));
            assert(r.any_matched);
            assert(int_is(r.merged.find("local_temperature"), 2100));
        }
        // occupied_heating_setpoint (0x0012 s16) = 2000 — NOT
        // current_heating_setpoint (the generic-decoder dead key).
        {
            const std::uint8_t b[] = {0xD0, 0x07};
            auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                                  attr_report(0x0012, 0x29, b));
            assert(r.any_matched);
            assert(int_is(r.merged.find("occupied_heating_setpoint"), 2000));
            assert(r.merged.find("current_heating_setpoint") == nullptr);
        }
        // unoccupied_heating_setpoint (0x0014 s16) = 1600.
        {
            const std::uint8_t b[] = {0x40, 0x06};
            auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                                  attr_report(0x0014, 0x29, b));
            assert(r.any_matched);
            assert(int_is(r.merged.find("unoccupied_heating_setpoint"), 1600));
        }
        // running_state (0x0029) 1 → heat, 0 → idle.
        {
            const std::uint8_t heat[] = {0x01};
            auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                                  attr_report(0x0029, 0x30, heat));
            assert(r.any_matched);
            assert(str_is(r.merged.find("running_state"), "heat"));

            const std::uint8_t idle[] = {0x00};
            auto r2 = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                                   attr_report(0x0029, 0x30, idle));
            assert(str_is(r2.merged.find("running_state"), "idle"));
        }
        // system_mode (0x001C u8 enum) raw passthrough still works.
        {
            const std::uint8_t sm[] = {0x04};  // heat
            auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                                  attr_report(0x001C, 0x30, sm));
            assert(r.any_matched);
            assert(r.merged.find("system_mode") != nullptr);
        }
    }

    // ── TAFFETAS2/PERCALE2 ───────────────────────────────────────────
    {
        const PreparedDefinition& def = kDef_TAFFETAS2_PERCALE2;
        assert(def_exposes(def, "occupied_heating_setpoint"));
        assert(def_exposes(def, "unoccupied_heating_setpoint"));
        assert(def_exposes(def, "local_temperature_calibration"));
        assert(def_exposes(def, "running_state"));
        assert(def_exposes(def, "occupancy"));

        // local_temperature_calibration (0x0010 s8/s16) = -20 (−2.0 °C raw).
        {
            const std::uint8_t b[] = {0xEC, 0xFF};  // -20 LE int16
            auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                                  attr_report(0x0010, 0x29, b));
            assert(r.any_matched);
            assert(int_is(r.merged.find("local_temperature_calibration"), -20));
        }
        // occupied_heating_setpoint (0x0012) decodes to the right key.
        {
            const std::uint8_t b[] = {0xD0, 0x07};
            auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                                  attr_report(0x0012, 0x29, b));
            assert(r.any_matched);
            assert(int_is(r.merged.find("occupied_heating_setpoint"), 2000));
            assert(r.merged.find("current_heating_setpoint") == nullptr);
        }
        // running_state (0x0029).
        {
            const std::uint8_t heat[] = {0x01};
            auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                                  attr_report(0x0029, 0x30, heat));
            assert(str_is(r.merged.find("running_state"), "heat"));
        }
        // occupancy still decodes on EP2 (msOccupancySensing 0x0406).
        {
            const std::uint8_t occ[] = {0x01};
            auto r = dispatch_zcl(def, 0x0406, "msOccupancySensing", 2,
                                  attr_report(0x0000, 0x18, occ));
            assert(r.any_matched);
            assert(r.merged.find("occupancy") != nullptr);
        }
    }

    // ── IHC-Enki ─────────────────────────────────────────────────────
    {
        const PreparedDefinition& def = kDef_IHC_Enki;
        assert(def_exposes(def, "occupied_heating_setpoint"));
        assert(def_exposes(def, "local_temperature_calibration"));
        // z2m has no on/off, battery or voltage — none must be present.
        assert(!def_exposes(def, "state"));
        assert(!def_exposes(def, "battery"));
        assert(!def_exposes(def, "voltage"));

        // occupied_heating_setpoint (0x0012) → right key.
        {
            const std::uint8_t b[] = {0xD0, 0x07};
            auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                                  attr_report(0x0012, 0x29, b));
            assert(r.any_matched);
            assert(int_is(r.merged.find("occupied_heating_setpoint"), 2000));
            assert(r.merged.find("current_heating_setpoint") == nullptr);
        }
        // local_temperature_calibration (0x0010) decodes.
        {
            const std::uint8_t b[] = {0x14, 0x00};  // +20 (2.0 °C raw)
            auto r = dispatch_zcl(def, 0x0201, "hvacThermostat", 1,
                                  attr_report(0x0010, 0x29, b));
            assert(r.any_matched);
            assert(int_is(r.merged.find("local_temperature_calibration"), 20));
        }
    }

    return 0;
}
