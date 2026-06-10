// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Net2Grid family. The real z2m<->ez gap caught here:
//
//   * N2G-SP (model SP31) is a metering power outlet that is BOTH a
//     controllable on/off plug AND a command source. z2m wires
//     fromZigbee:[fz.command_on, fz.command_off, fz.on_off, fz.metering]
//     and exposes [switch, power, energy]. The auto-generated port kept
//     only fz.on_off (attribute-report state) + fz.metering and dropped
//     the two command converters, so every genOnOff On/Off command the
//     plug emits (physical button / bound group) was lost. Fixed by adding
//     the generic kFzCommandOn / kFzCommandOff and an `action` enum, while
//     keeping the controllable on/off state + tz.on_off write path and the
//     seMetering 0x0702 energy+power channel.
//
//   * Metering completeness regression guard: z2m uses fz.metering, i.e.
//     seMetering 0x0702 ONLY (energy + power). There is no electricityMeter
//     / haElectricalMeasurement 0x0B04 half here, so the absence of
//     voltage/current is CORRECT (FALSE-by-category), not a dropped channel.
//
// z2m-source: zigbee-herdsman-converters/src/devices/net2grid.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::net2grid {
extern const PreparedDefinition kDef_N2G_SP;
}  // namespace zhc::devices::net2grid

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
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Cluster-specific command frame: fc=0x01, tsn, cmd, payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> body) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : body) v.push_back(b);
    return v;
}

// ZCL attribute report: fc=0x18, tsn, cmd=0x0A, then attr LE, type, value.
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
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool action_is(const DispatchResult& r, const char* expect) {
    const Value* v = r.merged.find("action");
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, expect) == 0;
}

constexpr std::uint16_t GEN_ON_OFF  = 0x0006;
constexpr std::uint16_t SE_METERING = 0x0702;

void test_n2g_sp() {
    const auto& def = devices::net2grid::kDef_N2G_SP;

    // Plug shape: controllable state + metering + a command-action channel.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "action"));
    // FALSE-by-category: fz.metering is 0x0702 only — no 0x0B04 axis.
    assert(!def_exposes(def, "voltage"));
    assert(!def_exposes(def, "current"));
    // Controllable plug — keeps its on/off write path (tz.on_off).
    assert(def.to_zigbee != nullptr && def.to_zigbee_count == 1);

    // ── command-action channel (the fixed gap) ──────────────────────────
    // genOnOff On (cmd 0x01) → action "on".
    auto ron = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x01, {}));
    assert(ron.any_matched);
    assert(action_is(ron, "on"));
    // genOnOff Off (cmd 0x00) → action "off".
    auto roff = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x00, {}));
    assert(roff.any_matched);
    assert(action_is(roff, "off"));
    // A command frame must NOT leak a controllable `state` onto the shadow.
    assert(ron.merged.find("state") == nullptr);

    // ── attribute-report on/off still drives `state` (kFzOnOff intact) ──
    const std::uint8_t on_val[] = {0x01};
    auto rstate = dispatch_zcl(def, GEN_ON_OFF, 1, attr_report(0x0000, 0x10, on_val));
    assert(rstate.any_matched);
    const Value* st = rstate.merged.find("state");
    assert(st != nullptr && st->type == ValueType::Bool && st->b == true);
    // An attribute report must NOT spuriously emit an `action`.
    assert(rstate.merged.find("action") == nullptr);

    // ── seMetering 0x0702: energy (CurrentSummDelivered) + power ────────
    // attr 0x0000 CurrentSummDelivered (u48, type 0x25) → energy.
    const std::uint8_t summ[] = {0x10, 0x27, 0x00, 0x00, 0x00, 0x00};  // 10000
    auto rene = dispatch_zcl(def, SE_METERING, 10, attr_report(0x0000, 0x25, summ));
    assert(rene.any_matched);
    const Value* en = rene.merged.find("energy");
    assert(en != nullptr && en->type == ValueType::Uint && en->u == 10000);
    // attr 0x0400 InstantaneousDemand (int24, type 0x2A) → power.
    const std::uint8_t demand[] = {0xE8, 0x03, 0x00};  // +1000
    auto rpow = dispatch_zcl(def, SE_METERING, 10, attr_report(0x0400, 0x2A, demand));
    assert(rpow.any_matched);
    const Value* pw = rpow.merged.find("power");
    assert(pw != nullptr && pw->type == ValueType::Int && pw->i == 1000);
}

}  // namespace

int main() {
    test_n2g_sp();
    return 0;
}
