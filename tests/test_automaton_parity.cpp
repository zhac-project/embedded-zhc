// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Automaton (Tuya-OEM) family. Found by a
// z2m<->embedded-zhc parity pass over the single automaton def.
//
// AUT000069 is a 5-zone underfloor-heating / irrigation valve controller
// (TS011F, _TZ3000_j0ktmul1). z2m wires
// `tuya.modernExtend.tuyaOnOff({endpoints: ["l1".."l5"]})` with
// `endpoint: {l1:1, l2:2, l3:3, l4:4, l5:5}` and `meta.multiEndpoint`, i.e.
// five independent genOnOff channels. The auto-port collapsed this to a
// single bare `state` expose with one EP1 binding and NO endpoint_map, so
// all five zones collided on one key and zones 2..5 were never bound.
//
// The fix adds the endpoint_map (l1..l5), per-EP state exposes and genOnOff
// bindings on EP1..EP5. This test pins that an onOff attribute report on
// each source endpoint emits the correctly suffixed key (state_l1..state_l5)
// — the runtime dispatcher suffixes when the def carries an endpoint_map,
// while the exposes stay declared as state_lN.
//
// z2m-source: zigbee-herdsman-converters/src/devices/automaton.ts #AUT000069.

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "zhc/devices/automaton_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> automaton_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::automaton::kAutomatonRegistry,
        devices::automaton::kAutomatonRegistryCount);
}

const PreparedDefinition& resolve() {
    const auto* d = find_definition("TS011F", "_TZ3000_j0ktmul1",
                                    automaton_registry());
    assert(d && "automaton AUT000069 must resolve from its fingerprint");
    assert(d->model && std::strcmp(d->model, "AUT000069") == 0);
    return *d;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

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

// genOnOff onOff (attr 0x0000, bool) Report Attributes frame:
//   fc=0x18 (profile-wide, S->C) | tsn | cmd=0x0A (report) |
//   attr 0x0000 LE | type 0x10 (bool) | value (0/1).
DispatchResult report_on_off(const PreparedDefinition& def,
                             std::uint8_t src_ep, bool on) {
    const std::array<std::uint8_t, 7> rep = {
        0x18, 0x01, 0x0A, 0x00, 0x00, 0x10,
        static_cast<std::uint8_t>(on ? 0x01 : 0x00),
    };
    auto raw = make_frame(0x0006 /* genOnOff */, src_ep,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// On endpoint N, an onOff report must surface as the suffixed key
// "state_l<N>" (Bool) and NOT as the bare "state" (which would mean the
// channels collide).
void assert_zone(const PreparedDefinition& def, std::uint8_t ep,
                 const char* suffixed, bool on) {
    auto r = report_on_off(def, ep, on);
    assert(r.any_matched);

    const Value* v = r.merged.find(suffixed);
    assert(v && v->type == ValueType::Bool && v->b == on &&
           "per-endpoint onOff must emit the suffixed state_l<N> key");

    // The bare collapsed key must be gone now that the endpoint_map suffixes.
    assert(r.merged.find("state") == nullptr &&
           "bare 'state' must not leak — channels would collide");
}

void test_automaton_aut000069() {
    const auto& d = resolve();

    // Five per-endpoint state exposes are declared (the bare single 'state'
    // must be gone).
    assert(def_exposes(d, "state_l1"));
    assert(def_exposes(d, "state_l2"));
    assert(def_exposes(d, "state_l3"));
    assert(def_exposes(d, "state_l4"));
    assert(def_exposes(d, "state_l5"));
    assert(!def_exposes(d, "state"));
    assert(d.exposes_count == 5);

    // endpoint_map + bindings cover all five zones (z2m binds genOnOff on
    // EP1..EP5).
    assert(d.endpoint_map != nullptr && d.endpoint_map_count == 5);
    assert(d.bindings != nullptr && d.bindings_count == 5);

    // Each zone routes to its own suffixed key.
    assert_zone(d, 1, "state_l1", true);
    assert_zone(d, 2, "state_l2", false);
    assert_zone(d, 3, "state_l3", true);
    assert_zone(d, 4, "state_l4", false);
    assert_zone(d, 5, "state_l5", true);

    // Write path (genOnOff on/off setter) still wired.
    assert(d.to_zigbee_count >= 1 && d.to_zigbee != nullptr);
}

}  // namespace

int main() {
    test_automaton_aut000069();
    return 0;
}
