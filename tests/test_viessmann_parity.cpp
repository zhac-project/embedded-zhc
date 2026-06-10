// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Viessmann (ViCare) family. Pins the real gap the
// auto-generated port carried against the z2m wire shape:
//
//   * ViCare radiator thermostat ZK03840 decoded the manuSpec attr 0x4000
//     viessmannWindowOpenInternal (ENUM8) as a flat `!= 0` boolean. z2m
//     (`fzLocal.viessmann_thermostat`) only treats raw 3 and 4 as
//     "window open"; raw 0-2 and 5 are documented "unknown" → window NOT
//     open. The `!= 0` flattening false-flagged codes 1/2/5 as open.
//     Fixed in `kFzViessmannThermostat` (raw == 3 || raw == 4).
//
// The remaining manuSpec extras (window_open_force 0x4003, assembly_mode
// 0x4012), the keypad_lockout read (hvacUserInterfaceCfg 0x0001), the
// core thermostat surface (generic kFzThermostat: local_temperature /
// occupied_heating_setpoint / system_mode), and the CO2 sensor decode
// (msCO2.measuredValue → co2 ppm) are regression-guarded here too.
//
// z2m-source: zigbee-herdsman-converters/src/devices/viessmann.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::viessmann {
extern const PreparedDefinition kDef_ZK03840;    // ViCare radiator TRV
extern const PreparedDefinition kDef_D7377019;   // ViCare CO2/temp/hum sensor
}  // namespace zhc::devices::viessmann

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

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then records ─
// (Plain attribute report. Viessmann manu-spec attrs carry a mfg header
// on the wire, but the host decoder keys decoded values by attr id, so a
// plain report exercises the converter the same way — matching the
// danfoss parity-test convention.)
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

// ── ZK03840: window_open semantics (the real fix) ────────────────────
void check_window_open() {
    const auto& def = devices::viessmann::kDef_ZK03840;

    assert(def_exposes(def, "window_open"));
    assert(def_exposes(def, "window_open_force"));
    assert(def_exposes(def, "keypad_lockout"));
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "occupied_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));

    // attr 0x4000 viessmannWindowOpenInternal (ENUM8, type 0x30).
    // Only raw 3 and 4 are "window open"; everything else is NOT open.
    for (std::uint8_t raw : {std::uint8_t{0}, std::uint8_t{1},
                             std::uint8_t{2}, std::uint8_t{5}}) {
        const std::uint8_t val[] = {raw};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x4000, 0x30, val));
        assert(r.any_matched);
        // Regression: the old `!= 0` decode raised window_open here.
        assert(b_false(r.merged.find("window_open")));
    }

    for (std::uint8_t raw : {std::uint8_t{3}, std::uint8_t{4}}) {
        const std::uint8_t val[] = {raw};
        auto r = dispatch_zcl(def, 0x0201, "hvacThermostat",
                              attr_report(0x4000, 0x30, val));
        assert(r.any_matched);
        assert(b_true(r.merged.find("window_open")));
    }
}

// ── ZK03840: manuSpec extras still decode (regression) ───────────────
void check_manu_extras() {
    const auto& def = devices::viessmann::kDef_ZK03840;

    // attr 0x4003 viessmannWindowOpenForce (BOOLEAN, type 0x10) = 1 → true.
    const std::uint8_t force_on[] = {0x01};
    auto rf = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x4003, 0x10, force_on));
    assert(rf.any_matched);
    assert(b_true(rf.merged.find("window_open_force")));

    const std::uint8_t force_off[] = {0x00};
    auto rf0 = dispatch_zcl(def, 0x0201, "hvacThermostat",
                            attr_report(0x4003, 0x10, force_off));
    assert(b_false(rf0.merged.find("window_open_force")));

    // attr 0x4012 viessmannAssemblyMode (BOOLEAN, type 0x10) = 1 → true.
    const std::uint8_t asm_on[] = {0x01};
    auto ra = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x4012, 0x10, asm_on));
    assert(b_true(ra.merged.find("assembly_mode")));
}

// ── ZK03840: keypad_lockout via hvacUserInterfaceCfg 0x0001 ──────────
void check_keypad_lockout() {
    const auto& def = devices::viessmann::kDef_ZK03840;
    // attr 0x0001 keypadLockout (ENUM8, type 0x30) = 1 → raw 1.
    const std::uint8_t lock[] = {0x01};
    auto r = dispatch_zcl(def, 0x0204, "hvacUserInterfaceCfg",
                          attr_report(0x0001, 0x30, lock));
    assert(r.any_matched);
    const Value* k = r.merged.find("keypad_lockout");
    assert(k && k->type == ValueType::Uint && k->u == 1);
}

// ── ZK03840: core thermostat surface via generic kFzThermostat ───────
void check_core_surface() {
    const auto& def = devices::viessmann::kDef_ZK03840;

    // attr 0x0000 LocalTemperature, s16 (type 0x29) = 0x0834 = 2100 → 21.00 °C raw.
    const std::uint8_t lt[] = {0x34, 0x08};
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    // attr 0x0012 OccupiedHeatingSetpoint, s16 = 0x07D0 = 2000.
    const std::uint8_t sp[] = {0xD0, 0x07};
    auto rs = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0012, 0x29, sp));
    const Value* sv = rs.merged.find("occupied_heating_setpoint");
    assert(sv && sv->type == ValueType::Int && sv->i == 2000);

    // attr 0x001C SystemMode, u8 enum = 4 (heat).
    const std::uint8_t sm[] = {0x04};
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x001C, 0x30, sm));
    const Value* mv = rm.merged.find("system_mode");
    assert(mv && mv->type == ValueType::Uint && mv->u == 4);
}

// ── 7377019: CO2 decode (msCO2.measuredValue float → ppm) ────────────
void check_co2() {
    const auto& def = devices::viessmann::kDef_D7377019;
    assert(def_exposes(def, "co2"));
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));

    // msCO2 0x040D attr 0x0000 measuredValue, single float (type 0x39).
    // z2m: floor(measuredValue * 1e6). 0.000420f → 420 ppm.
    float frac = 0.000420f;
    std::array<std::uint8_t, 4> fbytes{};
    std::memcpy(fbytes.data(), &frac, sizeof(frac));
    auto r = dispatch_zcl(def, 0x040D, "msCO2",
                          attr_report(0x0000, 0x39, fbytes));
    assert(r.any_matched);
    const Value* c = r.merged.find("co2");
    assert(c && c->type == ValueType::Int);
    // float rounding tolerance around 420 ppm.
    assert(c->i >= 419 && c->i <= 421);
}

}  // namespace

int main() {
    check_window_open();
    check_manu_extras();
    check_keypad_lockout();
    check_core_surface();
    check_co2();
    return 0;
}
