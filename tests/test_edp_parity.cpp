// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the EDP re:dy plug family.
// z2m-source: edp.ts — PLUG EDP RE:DY is fz.on_off + fz.metering (seMetering
// 0x0702 only; z2m does NOT use fz.electrical_measurement, so there is no
// 0x0B04 channel). exposes: switch + power + energy.
//
// Bug fixed: z2m configures on `device.getEndpoint(85)` and binds genOnOff +
// seMetering on endpoint 85 ("re:dy plug uses ep relay"). The auto-port bound
// on endpoint 1, so the coordinator never bound the relay/metering endpoint
// the device actually reports on → state/power/energy stayed dead. The two
// bindings were repointed to endpoint 85.
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * the def exposes state + power + energy (and no phantom channels),
//   * both bindings (genOnOff 0x0006, seMetering 0x0702) target endpoint 85,
//   * a seMetering report from ep 85 still yields energy + power,
//   * an onOff report from ep 85 still yields state.

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

namespace zhc::devices::edp {
extern const PreparedDefinition kDef_PLUG_EDP_RE_DY;   // switch + power + energy
extern const PreparedDefinition kDef_SWITCH_EDP_RE_DY; // bare on/off
}  // namespace zhc::devices::edp

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

// True iff the def binds `cluster` on `endpoint`.
bool def_binds_on(const PreparedDefinition& def, std::uint16_t cluster,
                  std::uint8_t endpoint) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster &&
            def.bindings[i].endpoint == endpoint)
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

// ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A=ReportAttributes,
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

// ── PLUG EDP RE:DY: the real fix — bindings on the relay endpoint (85). ──
void check_plug() {
    using namespace zhc::devices::edp;
    const PreparedDefinition& def = kDef_PLUG_EDP_RE_DY;

    // Static shape: z2m exposes switch + power + energy; no phantom channels.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "energy"));
    assert(!def_exposes(def, "voltage"));   // no 0x0B04 channel in z2m
    assert(!def_exposes(def, "current"));
    assert(!def_exposes(def, "brightness")); // not a light

    // The fix: both bindings target the re:dy relay endpoint 85, not 1.
    assert(def_binds_on(def, 0x0006, 85));  // genOnOff   @ ep 85
    assert(def_binds_on(def, 0x0702, 85));  // seMetering @ ep 85
    // Regression guard: must NOT keep binding the wrong endpoint 1.
    assert(!def_binds_on(def, 0x0006, 1));
    assert(!def_binds_on(def, 0x0702, 1));

    // seMetering (0x0702) report from ep 85 still yields energy + power.
    // attr 0x0000 CurrentSummationDelivered, uint48 = 1234.
    std::array<std::uint8_t, 6> energy_b{0xD2, 0x04, 0x00, 0x00, 0x00, 0x00};
    auto re = dispatch_zcl(def, 0x0702, "seMetering", 85,
                           attr_report(0x0000, 0x25, energy_b));
    assert(re.any_matched);
    assert(re.merged.find("energy") != nullptr);

    // attr 0x0400 InstantaneousDemand, int24s = 150 W.
    std::array<std::uint8_t, 3> demand_b{0x96, 0x00, 0x00};
    auto rd = dispatch_zcl(def, 0x0702, "seMetering", 85,
                           attr_report(0x0400, 0x2A, demand_b));
    assert(rd.any_matched);
    assert(rd.merged.find("power") != nullptr);

    // onOff (0x0006) attr 0x0000 bool from ep 85 → state still decodes.
    std::array<std::uint8_t, 1> on_b{0x01};
    auto rs = dispatch_zcl(def, 0x0006, "genOnOff", 85,
                           attr_report(0x0000, 0x10, on_b));
    assert(rs.any_matched);
    assert(rs.merged.find("state") != nullptr);
}

// ── SWITCH EDP RE:DY: bare on/off (z2m m.onOff()) — unchanged regression. ──
void check_switch() {
    using namespace zhc::devices::edp;
    const PreparedDefinition& def = kDef_SWITCH_EDP_RE_DY;

    assert(def_exposes(def, "state"));
    assert(!def_exposes(def, "power"));
    assert(!def_exposes(def, "energy"));

    std::array<std::uint8_t, 1> on_b{0x01};
    auto rs = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                           attr_report(0x0000, 0x10, on_b));
    assert(rs.any_matched);
    assert(rs.merged.find("state") != nullptr);
}

}  // namespace

int main() {
    check_plug();
    check_switch();
    return 0;
}
