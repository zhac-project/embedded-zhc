// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Lellki (Tuya-OEM plug/switch/metering) family. Found by
// a z2m<->embedded-zhc parity pass over the 8 existing lellki defs.
//
// Bug fixed (gap class b — MISSING decoder/channel):
//   Two single-gang metering sockets, XF-EU-S100-1-M (_TZ3000_z6fgd73r) and
//   WK34-EU (_TZ3000_0yxeawjt), use z2m
//   tuya.modernExtend.tuyaOnOff({electricalMeasurements: true}), which pushes
//   fz.electrical_measurement (haElectricalMeasurement 0x0B04) + fz.metering
//   (seMetering 0x0702) and exposes power/current/voltage/energy. The auto port
//   lowered each to a bare kFzOnOff with a single `state` expose, dropping the
//   ENTIRE metering half. Graduated both to Tier-2 parent overrides that wire
//   kFzMetering + kFzElectricalMeasurement, add the four numeric exposes, and
//   bind 0x0702 + 0x0B04.
//
// The rest of the family was verified CLEAN: WP30-EU already wires both metering
// converters + an l1/l2/l3 endpoint_map; the multi-gang switches (WP33/WP34 5×,
// JZ-ZB-002 2×, JZ-ZB-003 3×) carry their endpoint_maps; CM001 / JZ-ZB-001 are
// plain on/off. (The power_on_behavior / power_outage_memory config enums are
// Tuya manuSpecific / genOnOff-0x8002 attrs with no existing generic converter —
// noted as INFRA, deferred.)
//
// z2m-source: zigbee-herdsman-converters/src/devices/lellki.ts
//             + lib/tuya.ts tuyaOnOff({electricalMeasurements}).

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/devices/lellki_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lellki {
extern const PreparedDefinition kDef_XF_EU_S100_1_M;
extern const PreparedDefinition kDef_WK34_EU;
extern const PreparedDefinition kDef_WP30_EU;
extern const PreparedDefinition kDef_WP33_EU_WP34_EU;
extern const PreparedDefinition kDef_CM001;
}  // namespace zhc::devices::lellki

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool def_has_endpoint(const PreparedDefinition& def, const char* label, std::uint8_t ep) {
    for (std::size_t i = 0; i < def.endpoint_map_count; ++i)
        if (def.endpoint_map[i].label && std::strcmp(def.endpoint_map[i].label, label) == 0 &&
            def.endpoint_map[i].endpoint == ep)
            return true;
    return false;
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

// ZCL attribute report: fc=0x18, tsn, cmd=0x0A=ReportAttributes,
// then one record: <attr_id LE><type><value LE>.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// Assert a metering socket decodes on/off + the full electrical/metering set.
void check_metering_socket(const PreparedDefinition& def) {
    // Static shape: state + the four numeric channels.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "current"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "energy"));
    assert(def_binds(def, 0x0006));   // genOnOff
    assert(def_binds(def, 0x0702));   // seMetering
    assert(def_binds(def, 0x0B04));   // haElectricalMeasurement
    // Single-gang: NO endpoint_map.
    assert(def.endpoint_map_count == 0);

    // onOff (0x0006) attr 0x0000 bool -> state.
    const std::uint8_t on_b[] = {0x01};
    auto ro = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                           attr_report(0x0000, 0x10, on_b));
    assert(ro.any_matched);
    assert(ro.merged.find("state") != nullptr);

    // seMetering (0x0702) attr 0x0000 CurrentSummationDelivered uint48 -> energy.
    const std::uint8_t energy_b[] = {0xD2, 0x04, 0x00, 0x00, 0x00, 0x00};
    auto re = dispatch_zcl(def, 0x0702, "seMetering", 1,
                           attr_report(0x0000, 0x25, energy_b));
    assert(re.any_matched);
    assert(re.merged.find("energy") != nullptr);

    // haElectricalMeasurement (0x0B04) attr 0x050B ActivePower (s16) -> power.
    const std::uint8_t power_b[] = {0x96, 0x00};
    auto rp = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x050B, 0x29, power_b));
    assert(rp.any_matched);
    assert(rp.merged.find("power") != nullptr);

    // attr 0x0505 RMSVoltage (u16) -> voltage.
    const std::uint8_t volt_b[] = {0xE6, 0x00};
    auto rv = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x0505, 0x21, volt_b));
    assert(rv.any_matched);
    assert(rv.merged.find("voltage") != nullptr);

    // attr 0x0508 RMSCurrent (u16) -> current.
    const std::uint8_t cur_b[] = {0x64, 0x00};
    auto rc = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x0508, 0x21, cur_b));
    assert(rc.any_matched);
    assert(rc.merged.find("current") != nullptr);
}

// ── 1. XF-EU-S100-1-M : metering restored ─────────────────────────────
void test_xf_eu_s100() {
    check_metering_socket(zhc::devices::lellki::kDef_XF_EU_S100_1_M);
}

// ── 2. WK34-EU : metering restored ────────────────────────────────────
void test_wk34_eu() {
    check_metering_socket(zhc::devices::lellki::kDef_WK34_EU);
}

// ── 3. Regression: WP30-EU (multi-gang metering) stays whole ──────────
void test_wp30_eu_unaffected() {
    using namespace zhc::devices::lellki;
    const PreparedDefinition& def = kDef_WP30_EU;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    assert(def_exposes(def, "energy"));
    assert(def_binds(def, 0x0702));
    assert(def_binds(def, 0x0B04));
    // l1/l2/l3 endpoint_map intact -> state keys suffix per endpoint.
    assert(def_has_endpoint(def, "l1", 1));
    assert(def_has_endpoint(def, "l2", 2));
    assert(def_has_endpoint(def, "l3", 3));
}

// ── 4. Regression: plain on/off devices did NOT grow a metering half ──
void test_plain_onoff_unaffected() {
    using namespace zhc::devices::lellki;
    // WP33/WP34: 5-gang switch, NO metering, endpoint_map l1..l5.
    const PreparedDefinition& mg = kDef_WP33_EU_WP34_EU;
    assert(def_exposes(mg, "state"));
    assert(!def_exposes(mg, "power"));
    assert(!def_binds(mg, 0x0702));
    assert(!def_binds(mg, 0x0B04));
    assert(def_has_endpoint(mg, "l1", 1));
    assert(def_has_endpoint(mg, "l5", 5));

    // CM001: single on/off circuit switch, no metering, no endpoint_map.
    const PreparedDefinition& cm = kDef_CM001;
    assert(def_exposes(cm, "state"));
    assert(!def_exposes(cm, "power"));
    assert(!def_binds(cm, 0x0702));
    assert(cm.endpoint_map_count == 0);
}

}  // namespace

int main() {
    test_xf_eu_s100();
    test_wk34_eu();
    test_wp30_eu_unaffected();
    test_plain_onoff_unaffected();
    return 0;
}
