// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the eZEX ECW-100-A03 3-gang switch.
// z2m-source: ezex.ts.
//
// Bug fixed: ECW-100-A03 is z2m m.deviceEndpoints({top:1,center:2,bottom:3})
// + m.onOff({endpointNames:["top","center","bottom"]}). z2m's m.onOff()
// configure calls setupAttributes(device,...) which iterates
// getEndpointsWithCluster(device,"genOnOff","input") and binds genOnOff on
// EVERY endpoint exposing it (1/2/3). The auto-port bound only {1,0x0006}, so
// the center/bottom gangs were never bound at join and never reported state.
// Fix: bind {1,2,3}×0x0006. The endpoint_map {top,center,bottom} was already
// present and correctly drives the state→state_<label> suffixing on decode.

#include <array>
#include <cassert>
#include <cstdint>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::ezex {
extern const PreparedDefinition kDef_ECW_100_A03;
}  // namespace zhc::devices::ezex

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

// genOnOff (0x0006) Report Attributes: onOff (attr 0x0000, bool 0x10) = 1.
DispatchResult onoff_report(const PreparedDefinition& def, std::uint8_t src_ep) {
    const std::array<std::uint8_t, 7> rep = {0x18, 0x01, 0x0A, 0x00, 0x00, 0x10, 0x01};
    auto raw = make_frame(0x0006, src_ep,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool binds(const PreparedDefinition& def, std::uint16_t cluster, std::uint8_t ep) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster && def.bindings[i].endpoint == ep) return true;
    return false;
}

// ── genOnOff bound on all 3 gang endpoints (z2m setupAttributes parity) ──
void check_bindings() {
    const auto& def = devices::ezex::kDef_ECW_100_A03;
    assert(binds(def, 0x0006, 1));   // top
    assert(binds(def, 0x0006, 2));   // center  (was missing pre-fix)
    assert(binds(def, 0x0006, 3));   // bottom  (was missing pre-fix)
}

// ── endpoint_map drives per-gang state suffixing on decode ───────────────
void check_endpoint_suffix() {
    const auto& def = devices::ezex::kDef_ECW_100_A03;
    assert(def.endpoint_map_count == 3);

    auto top = onoff_report(def, 1);
    assert(top.any_matched);
    assert(top.merged.find("state_top") != nullptr);
    assert(top.merged.find("state") == nullptr);   // never collides on bare key

    auto center = onoff_report(def, 2);
    assert(center.any_matched);
    assert(center.merged.find("state_center") != nullptr);

    auto bottom = onoff_report(def, 3);
    assert(bottom.any_matched);
    assert(bottom.merged.find("state_bottom") != nullptr);
}

}  // namespace

int main() {
    check_bindings();
    check_endpoint_suffix();
    return 0;
}
