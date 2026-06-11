// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Atsmart Z6 3-gang smart wall switch.
// z2m-source: atsmart.ts #Z6 —
//   extend: [ m.deviceEndpoints({left:1, center:2, right:3}),
//             m.onOff({endpointNames:["left","center","right"]}) ]
//
// Bug fixed (MISSING per-endpoint bindings + collapsed exposes): the
// generated Tier-1 def carried the endpoint_map {left:1, center:2, right:3}
// and the generic onOff converter, but
//   (a) bound genOnOff (0x0006) on endpoint 1 ONLY — z2m m.onOff with three
//       endpointNames binds the cluster on all three endpoints, so the
//       center/right gangs never had their reporting configured; and
//   (b) collapsed the three per-gang exposes into a single bare "state" —
//       with an endpoint_map present, dispatch suffixes inbound onOff keys
//       to state_<label>, so the surface keys are state_left/state_center/
//       state_right and the bare "state" expose matched no runtime key.
//
// The def was graduated to a Tier-2 override that binds 0x0006 on endpoints
// 1/2/3 and declares the per-gang exposes. These tests pin:
//   * all three labelled endpoints are in the endpoint_map;
//   * each gang carries a suffixed state expose (no bare "state");
//   * genOnOff is bound on every endpoint;
//   * an inbound onOff report from endpoint 2 decodes to state_center and
//     from endpoint 3 to state_right (the suffix rewrite drives the key).

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/devices/atsmart_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> atsmart_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::atsmart::kAtsmartRegistry, devices::atsmart::kAtsmartRegistryCount);
}

const PreparedDefinition& resolve(const char* model) {
    const auto* d = find_definition(model, nullptr, atsmart_registry());
    assert(d && "atsmart model must resolve to a def");
    assert(d->model && std::strcmp(d->model, "Z6") == 0);
    return *d;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_has_endpoint(const PreparedDefinition& def, const char* label) {
    for (std::size_t i = 0; i < def.endpoint_map_count; ++i)
        if (def.endpoint_map[i].label && std::strcmp(def.endpoint_map[i].label, label) == 0)
            return true;
    return false;
}

bool def_binds_ep(const PreparedDefinition& def, std::uint16_t cluster, std::uint8_t ep) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster && def.bindings[i].endpoint == ep) return true;
    return false;
}

// ── ZCL genOnOff attribute-report harness ────────────────────────────
// Frame: ZCL header {0x18, seq, 0x0A=ReportAttributes} + attr 0x0000 onOff
// type 0x10 (bool) = value, decoded via decode_frame then dispatched. The
// cluster name is stamped post-decode (see test_ctm_parity convention).
DispatchResult decode_onoff(const PreparedDefinition& def, std::uint8_t src_ep,
                            std::uint8_t value) {
    const std::uint8_t bytes[] = {
        0x18, 0x10, 0x0A,        // ZCL header: server->client, seq, ReportAttributes
        0x00, 0x00, 0x10, value, // attr 0x0000 onOff, type 0x10 bool
    };
    InboundApsFrame raw{};
    raw.cluster_id   = 0x0006;
    raw.src_endpoint = src_ep;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = std::span<const std::uint8_t>(bytes, sizeof(bytes));

    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(0x0006);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

// ── endpoint_map carries all three named gangs ───────────────────────
static void test_z6_endpoint_map() {
    const auto& def = resolve("Z601");
    assert(def.endpoint_map_count == 3);
    assert(def_has_endpoint(def, "left"));
    assert(def_has_endpoint(def, "center"));
    assert(def_has_endpoint(def, "right"));
}

// ── per-gang suffixed state exposes; no bare "state" ─────────────────
static void test_z6_per_gang_exposes() {
    const auto& def = resolve("Z604");
    assert(def_exposes(def, "state_left"));
    assert(def_exposes(def, "state_center"));
    assert(def_exposes(def, "state_right"));
    assert(!def_exposes(def, "state") &&
           "3-gang switch must not carry a bare state (it would collide)");
}

// ── genOnOff bound on every endpoint — the core fix ──────────────────
static void test_z6_binds_all_endpoints() {
    const auto& def = resolve("Z602");
    assert(def_binds_ep(def, 0x0006, 1));
    assert(def_binds_ep(def, 0x0006, 2) &&
           "center gang (ep2) must bind genOnOff");
    assert(def_binds_ep(def, 0x0006, 3) &&
           "right gang (ep3) must bind genOnOff");
}

// ── runtime onOff per endpoint decodes to the suffixed key ───────────
static void test_z6_endpoint_suffixed_decode() {
    const auto& def = resolve("Z603");

    auto r2 = decode_onoff(def, 2, 0x01);
    assert(r2.any_matched && "onOff report from ep2 must match");
    assert(r2.merged.find("state_center") && "ep2 onOff must surface as state_center");
    assert(!r2.merged.find("state") && "key must be suffixed, not bare");

    auto r3 = decode_onoff(def, 3, 0x00);
    assert(r3.any_matched && "onOff report from ep3 must match");
    assert(r3.merged.find("state_right") && "ep3 onOff must surface as state_right");

    auto r1 = decode_onoff(def, 1, 0x01);
    assert(r1.any_matched && "onOff report from ep1 must match");
    assert(r1.merged.find("state_left") && "ep1 onOff must surface as state_left");
}

int main() {
    test_z6_endpoint_map();
    test_z6_per_gang_exposes();
    test_z6_binds_all_endpoints();
    test_z6_endpoint_suffixed_decode();
    return 0;
}
