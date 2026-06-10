// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Somfy (roller-shutter / blind motor) family. The
// auto-generated ports carried two real gaps pinned here after the fix:
//
//   Multi-button remote missing endpoint_action_suffix —
//     * 1871154 (Ysia 5): z2m wires m.commandsOnOff + m.commandsWindowCovering,
//       BOTH with endpointNames ["1".."5"], so each command is postfixed with
//       its endpoint (on_1, open_2, ...). The port carried an endpoint_map for
//       EPs 1..5 but NOT endpoint_action_suffix, so dispatch kept "action" in
//       kAlwaysGlobalKeys (bare) and all five channels collided on one key.
//       Setting the flag makes dispatch rewrite action -> action_<n>.
//
//   electricityMeter cluster:"both" — 0x0B04 electrical half dropped —
//     * 1871215B: z2m uses m.electricityMeter() (no args) which defaults to
//       cluster:"both" + ac, decoding seMetering (0x0702: energy) AND
//       haElectricalMeasurement (0x0B04: power/voltage/current). The port wired
//       only kFzMetering (0x0702), dropping voltage/current. Added
//       kFzElectricalMeasurement + the voltage/current exposes + 0x0B04 bind.
//
// The single-channel Ysia 1 (1871157) is also asserted to remain bare-action
// (no endpoint_map, no suffix) — z2m has no endpointNames there.

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

namespace zhc::devices::somfy {
extern const PreparedDefinition kDef_D1871154;   // Ysia 5: multi-button remote
extern const PreparedDefinition kDef_D1871157;   // Ysia 1: single-button remote
extern const PreparedDefinition kDef_D1871215B;  // plug: metering + electrical
}  // namespace zhc::devices::somfy

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

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// Keep `ctx` alive while the result is read: multi-endpoint suffixing allocates
// in RuntimeContext scratch, so result pointers alias `ctx`.
DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn, cmd.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> payload = {}) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : payload) v.push_back(b);
    return v;
}

// ZCL attribute report (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool action_is(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

// ── Ysia 5 (1871154): per-endpoint action suffixing. ───────────────────────
void test_ysia5_multibutton() {
    using namespace zhc::devices::somfy;
    const auto& def = kDef_D1871154;

    // Shape: action remote, no settable state, no write path.
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);
    assert(def.from_zigbee_count > 0);

    // The fix: endpoint_action_suffix + a 5-entry endpoint_map.
    assert(def.endpoint_action_suffix);
    assert(def.endpoint_map && def.endpoint_map_count == 5);

    // genOnOff On (cmd 0x01) on EP3 -> action_3 = "on" (suffixed, not bare).
    RuntimeContext c1{};
    auto on3 = dispatch_zcl_ep(c1, def, 0x0006, 3, "genOnOff", cmd_frame(0x01));
    assert(on3.any_matched);
    assert(action_is(on3, "action_3", "on"));
    assert(on3.merged.find("action") == nullptr);

    // genOnOff Off (cmd 0x00) on EP1 -> action_1 = "off".
    RuntimeContext c2{};
    auto off1 = dispatch_zcl_ep(c2, def, 0x0006, 1, "genOnOff", cmd_frame(0x00));
    assert(action_is(off1, "action_1", "off"));

    // closuresWindowCovering UpOpen (cmd 0x00) on EP2 -> action_2 = "open".
    RuntimeContext c3{};
    auto open2 = dispatch_zcl_ep(c3, def, 0x0102, 2, "closuresWindowCovering", cmd_frame(0x00));
    assert(open2.any_matched);
    assert(action_is(open2, "action_2", "open"));

    // DownClose (cmd 0x01) on EP4 -> action_4 = "close".
    RuntimeContext c4{};
    auto close4 = dispatch_zcl_ep(c4, def, 0x0102, 4, "closuresWindowCovering", cmd_frame(0x01));
    assert(action_is(close4, "action_4", "close"));

    // Stop (cmd 0x02) on EP5 -> action_5 = "stop".
    RuntimeContext c5{};
    auto stop5 = dispatch_zcl_ep(c5, def, 0x0102, 5, "closuresWindowCovering", cmd_frame(0x02));
    assert(action_is(stop5, "action_5", "stop"));
    assert(stop5.merged.find("action") == nullptr);

    // Per-EP binds for genOnOff + closuresWindowCovering on every channel.
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 5, 0x0006));
    assert(def_binds(def, 1, 0x0102));
    assert(def_binds(def, 5, 0x0102));
    assert(def_binds(def, 1, 0x0001));   // battery
    assert(def_exposes(def, "battery"));
}

// ── Ysia 1 (1871157): single channel -> bare action, no suffix. ────────────
void test_ysia1_single() {
    using namespace zhc::devices::somfy;
    const auto& def = kDef_D1871157;
    assert(def_exposes(def, "action"));
    assert(!def.endpoint_action_suffix);              // single EP -> no suffix
    assert(def.endpoint_map_count == 0);

    // genOnOff On -> bare action "on" (not action_1).
    RuntimeContext c1{};
    auto on = dispatch_zcl_ep(c1, def, 0x0006, 1, "genOnOff", cmd_frame(0x01));
    assert(on.any_matched);
    assert(action_is(on, "action", "on"));
    assert(on.merged.find("action_1") == nullptr);

    // closuresWindowCovering UpOpen -> bare action "open".
    RuntimeContext c2{};
    auto op = dispatch_zcl_ep(c2, def, 0x0102, 1, "closuresWindowCovering", cmd_frame(0x00));
    assert(action_is(op, "action", "open"));
}

// ── 1871215B plug: electrical half (voltage/current) regained. ─────────────
void test_plug_electrical() {
    using namespace zhc::devices::somfy;
    const auto& def = kDef_D1871215B;

    // Both halves exposed: metering energy (kept) + electrical V/I (added).
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));   // electrical half added
    assert(def_exposes(def, "current"));   // electrical half added
    assert(def_exposes(def, "state"));     // controllable relay kept
    assert(def.to_zigbee_count == 1);      // tz.on_off kept
    assert(def_binds(def, 1, 0x0702));
    assert(def_binds(def, 1, 0x0B04));     // electrical-measurement bind added

    // rmsVoltage (0x0505, u16) on haElectricalMeasurement -> "voltage".
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> v = {0xE6, 0x00};   // 230 LE
    auto vr = dispatch_zcl_ep(c1, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x0505, 0x21, std::span<const std::uint8_t>(v.data(), v.size())));
    assert(vr.any_matched);
    assert(vr.merged.find("voltage") != nullptr);

    // rmsCurrent (0x0508, u16) -> "current".
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> i = {0xF4, 0x01};   // 500 LE
    auto ir = dispatch_zcl_ep(c2, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x0508, 0x21, std::span<const std::uint8_t>(i.data(), i.size())));
    assert(ir.any_matched);
    assert(ir.merged.find("current") != nullptr);

    // seMetering currentSummDelivered (0x0000, u48) -> "energy" still works.
    RuntimeContext c3{};
    const std::array<std::uint8_t, 6> e = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00};   // 16 LE
    auto er = dispatch_zcl_ep(c3, def, 0x0702, 1, "seMetering",
                              attr_report(0x0000, 0x25, std::span<const std::uint8_t>(e.data(), e.size())));
    assert(er.any_matched);
    assert(er.merged.find("energy") != nullptr);
}

}  // namespace

int main() {
    test_ysia5_multibutton();
    test_ysia1_single();
    test_plug_electrical();
    return 0;
}
