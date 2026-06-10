// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Spotmau on/off switch family.
// z2m-source: spotmau.ts.
//
// Bug fixed (verify-caught, CLEAN verdict was wrong): SP-PS1-02 (1-gang) and
// SP-WS-02 (socket) declare z2m `endpoint:()=>({default:16})` with a single
// m.onOff() — the genOnOff cluster lives on endpoint 16 and publishes a bare
// `state` (the default endpoint is never suffixed). The auto-port bound
// genOnOff on ep1 and left default_endpoint=0, so reporting was configured on
// the wrong endpoint (reads dead) and outbound on/off routed to ep1 (control
// dead). Fix: bind {16,0x0006} + default_endpoint=16, NO endpoint_map (a
// 1-entry map would wrongly suffix state→state_default). Same shape as the
// ge PTAPT-WH02 / frient SMRZB-153 default_endpoint fix.
//
// SP-PS2-02 / SP-PS3-02 (multi-gang) already carry the correct
// {left:16,right:17[,center:18]} endpoint_map (z2m deviceEndpoints +
// endpointNames) — sanity-checked here.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::spotmau {
extern const PreparedDefinition kDef_SP_PS1_02;
extern const PreparedDefinition kDef_SP_WS_02;
extern const PreparedDefinition kDef_SP_PS2_02;
extern const PreparedDefinition kDef_SP_PS3_02;
}  // namespace zhc::devices::spotmau

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

// ── PS1 / WS: default endpoint 16, bare `state`, no endpoint_map ──────
void check_default_ep16(const PreparedDefinition& def) {
    assert(def.default_endpoint == 16);          // outbound on/off → ep16
    assert(def.endpoint_map_count == 0);          // bare state (not state_default)
    assert(binds(def, 0x0006, 16));               // reporting bound on ep16
    assert(!binds(def, 0x0006, 1));               // not the wrong endpoint

    // A genOnOff report from ep16 decodes to the bare `state` key.
    auto r = onoff_report(def, 16);
    assert(r.any_matched);
    const Value* s = r.merged.find("state");
    assert(s && s->type == ValueType::Bool && s->b == true);
    assert(r.merged.find("state_default") == nullptr);
}

// ── PS2 / PS3: per-gang endpoint_map intact ──────────────────────────
void check_multi_gang() {
    const auto& ps2 = devices::spotmau::kDef_SP_PS2_02;
    assert(ps2.endpoint_map_count == 2);
    auto r2 = onoff_report(ps2, 16);              // left = ep16
    assert(r2.any_matched);
    assert(r2.merged.find("state_left") != nullptr);
    assert(r2.merged.find("state") == nullptr);

    const auto& ps3 = devices::spotmau::kDef_SP_PS3_02;
    assert(ps3.endpoint_map_count == 3);
}

}  // namespace

int main() {
    check_default_ep16(devices::spotmau::kDef_SP_PS1_02);
    check_default_ep16(devices::spotmau::kDef_SP_WS_02);
    check_multi_gang();
    return 0;
}
