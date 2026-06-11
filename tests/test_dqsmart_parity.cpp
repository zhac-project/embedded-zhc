// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the DQSmart dqhome.re4 4-gang switch.
//
//   Incomplete per-endpoint bindings — the auto-port bound genOnOff
//   (0x0006) only on endpoint 1, so gangs l2/l3/l4 never got reporting
//   configured. z2m's m.onOff({endpointNames:["l1".."l4"]}) runs
//   setupAttributes() per endpoint with the cluster (getEndpointsWithCluster
//   → ep1..ep4), binding + configuring 0x0006 on every gang. The fix binds
//   0x0006 on all four endpoints.
//
//   Decode parity — the endpoint_map (l1..l4 → ep1..ep4) is present, so a
//   genOnOff report on each gang's source endpoint suffixes the bare "state"
//   key to state_l<n>. This was already correct; the test pins it so a
//   future regen can't silently drop the map.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::dqsmart {
extern const PreparedDefinition kDef_dqhome_re4;  // 4-gang on/off
}  // namespace zhc::devices::dqsmart

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

DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster,
                        std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

std::uint8_t ep_of(const PreparedDefinition& def, const char* label) {
    for (std::uint8_t i = 0; i < def.endpoint_map_count; ++i)
        if (std::strcmp(def.endpoint_map[i].label, label) == 0) return def.endpoint_map[i].endpoint;
    return 0xFF;  // not mapped
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

// genOnOff On/Off attribute report: attr 0x0000 (bool) = value.
std::array<std::uint8_t, 7> onoff_report(bool on) {
    return {0x18, 0x00, 0x0A,        // fc(server->client report), tsn, cmd=Report
            0x00, 0x00,              // attr 0x0000
            0x10,                    // type bool
            static_cast<std::uint8_t>(on ? 0x01 : 0x00)};
}

constexpr std::uint16_t CL_ONOFF = 0x0006;

}  // namespace

// ── Endpoint map: all four gangs mapped l1..l4 → ep1..ep4. ─────────────
static void test_endpoint_map() {
    const auto& def = devices::dqsmart::kDef_dqhome_re4;
    assert(def.endpoint_map_count == 4);
    assert(ep_of(def, "l1") == 1);
    assert(ep_of(def, "l2") == 2);
    assert(ep_of(def, "l3") == 3);
    assert(ep_of(def, "l4") == 4);
}

// ── Bindings: genOnOff bound on every gang (the fix). ──────────────────
static void test_bindings_all_endpoints() {
    const auto& def = devices::dqsmart::kDef_dqhome_re4;
    assert(def_binds(def, 1, CL_ONOFF));
    assert(def_binds(def, 2, CL_ONOFF));   // <-- the fix: was missing
    assert(def_binds(def, 3, CL_ONOFF));   // <-- the fix: was missing
    assert(def_binds(def, 4, CL_ONOFF));   // <-- the fix: was missing
}

// ── Exposes: a single bare "state" (endpoint_map suffixes at runtime). ─
static void test_exposes_bare_state() {
    const auto& def = devices::dqsmart::kDef_dqhome_re4;
    assert(def_exposes(def, "state"));
    // No phantom power_on_behavior (z2m powerOnBehavior:false).
    assert(!def_exposes(def, "power_on_behavior"));
    // No phantom Light bundle.
    assert(!def_exposes(def, "brightness"));
    assert(!def_exposes(def, "color_temp"));
}

// ── Decode: a genOnOff report on each gang suffixes state → state_l<n>. ─
static void test_onoff_suffixed_per_gang() {
    const auto& def = devices::dqsmart::kDef_dqhome_re4;

    struct Case { std::uint8_t ep; const char* key; };
    const Case cases[] = {
        {1, "state_l1"}, {2, "state_l2"}, {3, "state_l3"}, {4, "state_l4"},
    };
    for (const auto& c : cases) {
        auto on = onoff_report(true);
        auto ron = dispatch(def, CL_ONOFF, c.ep,
                            std::span<const std::uint8_t>(on.data(), on.size()));
        assert(ron.any_matched);
        assert(b_true(ron.merged.find(c.key)));
        // The bare key must not leak when an endpoint_map is present.
        assert(ron.merged.find("state") == nullptr);

        auto off = onoff_report(false);
        auto roff = dispatch(def, CL_ONOFF, c.ep,
                             std::span<const std::uint8_t>(off.data(), off.size()));
        assert(roff.any_matched);
        assert(b_false(roff.merged.find(c.key)));
    }
}

int main() {
    test_endpoint_map();
    test_bindings_all_endpoints();
    test_exposes_bare_state();
    test_onoff_suffixed_per_gang();
    return 0;
}
