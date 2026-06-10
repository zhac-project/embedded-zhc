// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for Insta / Gira / Jung devices.
// z2m-source: insta.ts (#57008000, #57005000, #57004000, #InstaRemote).
//
// Bug fixed (cover-tilt follow-up, 2026-06-10):
//   57008000 — z2m's cover_position_tilt reports BOTH
//     currentPositionLiftPercentage (attr 0x0008) AND
//     currentPositionTiltPercentage (attr 0x0009) on EP6, and the def
//     declared a `tilt` expose. But only the generic kFzCoverPosition
//     (lift, 0x0008) was wired, so tilt reports decoded to nothing.
//     Now wires kFzCoverTilt (0x0009 → tilt) and default_endpoint=6 so
//     outbound cover commands route to the cover EP.
//
// The other three defs are regression-guarded for their command/action
// decode shapes (these were already correct; assertions lock them in).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::insta {
extern const PreparedDefinition kDef_D57008000;
extern const PreparedDefinition kDef_D57005000;
extern const PreparedDefinition kDef_D57004000;
extern const PreparedDefinition kDef_InstaRemote;
}  // namespace zhc::devices::insta

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("  FAIL: %s\n", what);
        ++g_failures;
    }
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

// ZCL attribute report (cluster-specific=false, Report=0x0A).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
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

bool uint_is(const DispatchResult& r, const char* key, std::uint64_t expect) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Uint && v->u == expect;
}
bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
}
bool def_exposes(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (std::strcmp(def.exposes[i].name, name) == 0) return true;
    return false;
}

// ── 57008000 — lift + tilt cover (cover-tilt follow-up) ──────────────
void test_57008000_lift_tilt() {
    const auto& def = devices::insta::kDef_D57008000;

    // currentPositionLiftPercentage (attr 0x0008, u8) → position.
    auto liftbytes = attr_report(0x0008, 0x20,
                                 std::span<const std::uint8_t>(
                                     std::array<std::uint8_t, 1>{0x2A}.data(), 1));  // 42 %
    auto rp = dispatch_zcl(def, 0x0102, 6,
                           std::span<const std::uint8_t>(liftbytes.data(), liftbytes.size()));
    check(uint_is(rp, "position", 42), "57008000 decodes position 42 (attr 0x0008)");

    // currentPositionTiltPercentage (attr 0x0009, u8) → tilt. THIS is the
    // previously-dead channel the follow-up fix wires up.
    auto tiltbytes = attr_report(0x0009, 0x20,
                                 std::span<const std::uint8_t>(
                                     std::array<std::uint8_t, 1>{0x4B}.data(), 1));  // 75 %
    auto rt = dispatch_zcl(def, 0x0102, 6,
                           std::span<const std::uint8_t>(tiltbytes.data(), tiltbytes.size()));
    check(uint_is(rt, "tilt", 75), "57008000 decodes tilt 75 (attr 0x0009)");

    // Exposes: position + tilt, no phantom on/off `state`.
    check(def_exposes(def, "position"), "57008000 exposes position");
    check(def_exposes(def, "tilt"),     "57008000 exposes tilt");
    check(!def_exposes(def, "state"),   "57008000 has no phantom state expose");

    // Cover routing: default_endpoint==6 (z2m endpoint:{default:6}).
    check(def.default_endpoint == 6, "57008000 default_endpoint == 6");
    bool ep6_cover = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == 6 && def.bindings[i].cluster_id == 0x0102)
            ep6_cover = true;
    check(ep6_cover, "57008000 binds closuresWindowCovering on EP6");

    // Wall-switch input: closuresWindowCovering open command → action.
    std::array<std::uint8_t, 3> openhdr{0x01, 0x42, 0x00};  // cluster-specific cmd 0x00
    auto ro = dispatch_zcl(def, 0x0102, 7,
                           std::span<const std::uint8_t>(openhdr.data(), openhdr.size()));
    check(has_key(ro, "action"), "57008000 cover open command emits action");
}

// ── 57005000 — switching actuator (on/off + command passthrough) ─────
void test_57005000_onoff() {
    const auto& def = devices::insta::kDef_D57005000;

    auto onbytes = attr_report(0x0000, 0x10,
                               std::span<const std::uint8_t>(
                                   std::array<std::uint8_t, 1>{0x01}.data(), 1));
    auto ron = dispatch_zcl(def, 0x0006, 1,
                            std::span<const std::uint8_t>(onbytes.data(), onbytes.size()));
    check(has_key(ron, "state"), "57005000 genOnOff report emits state");

    check(def_exposes(def, "state"),  "57005000 exposes state");
    check(def_exposes(def, "action"), "57005000 exposes action");
    check(def.to_zigbee_count >= 1,   "57005000 has on/off setter");
}

// ── 57004000 — pushbutton interface (multi-endpoint action suffix) ───
void test_57004000_endpoints() {
    const auto& def = devices::insta::kDef_D57004000;

    check(def_exposes(def, "action"),       "57004000 exposes action");
    check(def.endpoint_map_count == 3,      "57004000 has 3-entry endpoint_map");
    check(def.endpoint_action_suffix,       "57004000 suffixes action per-EP");
    check(def.to_zigbee_count == 0,         "57004000 is action-only (no tz)");

    // genOnOff `on` command on EP4 → suffixed action key `action_e1`.
    std::array<std::uint8_t, 3> onhdr{0x01, 0x42, 0x01};  // cluster-specific cmd 0x01 (on)
    auto r = dispatch_zcl(def, 0x0006, 4,
                          std::span<const std::uint8_t>(onhdr.data(), onhdr.size()));
    check(has_key(r, "action_e1"), "57004000 EP4 on → action_e1 (suffixed)");
}

// ── InstaRemote — ZLL transmitter (action-only) ──────────────────────
void test_insta_remote_action() {
    const auto& def = devices::insta::kDef_InstaRemote;

    check(def_exposes(def, "action"), "InstaRemote exposes action");
    check(def.from_zigbee_count >= 5, "InstaRemote wires command converters");
    check(def.to_zigbee_count == 0,   "InstaRemote is read-only (no tz)");
    check(!def_exposes(def, "state"), "InstaRemote has no phantom state expose");

    // genOnOff `on` command → action="on".
    std::array<std::uint8_t, 3> onhdr{0x01, 0x42, 0x01};
    auto r = dispatch_zcl(def, 0x0006, 1,
                          std::span<const std::uint8_t>(onhdr.data(), onhdr.size()));
    check(has_key(r, "action"), "InstaRemote on command emits action");
}

}  // namespace

int main() {
    std::printf("test_insta_parity\n");
    test_57008000_lift_tilt();
    test_57005000_onoff();
    test_57004000_endpoints();
    test_insta_remote_action();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
