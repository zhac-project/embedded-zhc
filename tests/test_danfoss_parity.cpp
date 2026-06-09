// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Danfoss thermostat family. One real gap in the
// auto-generated ports is pinned here against the z2m wire shape:
//
//   * Ally TRV 014G2461 declared a `pi_heating_demand` expose (z2m
//     `danfossExtend.danfossThermostat({piHeatingDemand:{values:true}})`,
//     decoded by the standard `fz.thermostat` from hvacThermostat attr
//     0x0008), but the def only wired the generic `kFzThermostat`, which
//     decodes 0x0000 / 0x0012 / 0x001C and skips 0x0008. The valve-demand
//     readout was therefore a dead expose — never populated in the shadow.
//     Fixed by extending `kFzDanfossThermostat` (already wired on this
//     device) to decode attr 0x0008. Danfoss sets
//     `dontMapPIHeatingDemand: true`, so the value is the raw 0-100 % with
//     no 0-255 → 0-100 remap.
//
// The standard control surface (local_temperature / current_heating_
// setpoint / system_mode) is decoded by the generic `kFzThermostat` and is
// regression-guarded here too.
//
// z2m-source: zigbee-herdsman-converters/src/devices/danfoss.ts #014G2461.

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

namespace zhc::devices::danfoss {
extern const PreparedDefinition kDef_D014G2461;   // Ally TRV
}  // namespace zhc::devices::danfoss

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
// (Plain attribute report; Danfoss manu-spec attrs are read with a mfg
// header on the wire, but 0x0008 / 0x0000 / 0x0012 / 0x001C are standard
// hvacThermostat attrs reported without one — matching z2m fz.thermostat.)
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── Ally TRV: pi_heating_demand (0x0008, u8) + core thermostat surface ─
void check_ally() {
    const auto& def = devices::danfoss::kDef_D014G2461;

    // Expose must be declared (regression: it always was, but the value
    // never reached it before the converter fix).
    assert(def_exposes(def, "pi_heating_demand"));
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));

    // attr 0x0008 PIHeatingDemand, u8 (type 0x20) = 55 → 55 % (identity,
    // dontMapPIHeatingDemand). This is the core fix.
    const std::uint8_t demand[] = {0x37};   // 55
    auto rd = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, demand));
    assert(rd.any_matched);
    const Value* pd = rd.merged.find("pi_heating_demand");
    assert(pd && pd->type == ValueType::Uint && pd->u == 55);

    // Boundary: 0 % must still be emitted (not swallowed as falsy).
    const std::uint8_t zero[] = {0x00};
    auto rz = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, zero));
    assert(rz.any_matched);
    const Value* pz = rz.merged.find("pi_heating_demand");
    assert(pz && pz->type == ValueType::Uint && pz->u == 0);

    // 100 % upper bound.
    const std::uint8_t full[] = {0x64};     // 100
    auto rf = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0008, 0x20, full));
    const Value* pf = rf.merged.find("pi_heating_demand");
    assert(pf && pf->u == 100);

    // ── Regression: core surface still decodes via generic kFzThermostat ──
    // attr 0x0000 LocalTemperature, s16 (type 0x29) = 0x0834 = 2100 → 21.00 °C raw.
    const std::uint8_t lt[] = {0x34, 0x08};
    auto rt = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0000, 0x29, lt));
    const Value* tv = rt.merged.find("local_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 2100);

    // attr 0x0012 OccupiedHeatingSetpoint, s16 = 0x07D0 = 2000 → current_heating_setpoint.
    const std::uint8_t sp[] = {0xD0, 0x07};
    auto rs = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x0012, 0x29, sp));
    const Value* sv = rs.merged.find("current_heating_setpoint");
    assert(sv && sv->type == ValueType::Int && sv->i == 2000);

    // attr 0x001C SystemMode, u8 enum = 4 (heat).
    const std::uint8_t sm[] = {0x04};
    auto rm = dispatch_zcl(def, 0x0201, "hvacThermostat",
                           attr_report(0x001C, 0x30, sm));
    const Value* mv = rm.merged.find("system_mode");
    assert(mv && mv->type == ValueType::Uint && mv->u == 4);
}

}  // namespace

int main() {
    check_ally();
    return 0;
}
