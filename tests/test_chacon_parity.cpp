// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Chacon roller-shutter family.
//
//   ZB-ERSM-01 (roller shutter module) — m.commandsWindowCovering dropped —
//     z2m wires m.windowCovering({controls:["lift"], coverInverted, coverMode})
//     PLUS m.commandsWindowCovering({commands:["open","close","stop"]}). The
//     auto-port kept only the position half and dropped the second block, so
//     the device's own button-press commands (closuresWindowCovering
//     0x00/0x01/0x02) decoded to nothing. The fix wires
//     kFzCommandCoverOpen/Close/Stop + an `action` enum expose. Single channel
//     -> bare action (no endpoint_map). Position decode (attr 0x0008) is
//     asserted to still work.
//
//   ZB-PM-01 (on/off lighting module) — FALSE-by-category: m.onOff correctly
//     ported as a state binary; asserted here as a regression guard.

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

namespace zhc::devices::chacon {
extern const PreparedDefinition kDef_ZB_ERSM_01;  // roller shutter module
extern const PreparedDefinition kDef_ZB_PM_01;    // on/off module
}  // namespace zhc::devices::chacon

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

// ── ZB-ERSM-01: position state + window-cover command actions. ──────────────
void test_ersm_roller_shutter() {
    using namespace zhc::devices::chacon;
    const auto& def = kDef_ZB_ERSM_01;

    // Shape: a cover with position state (settable) AND an action expose
    // (the dropped m.commandsWindowCovering, now restored).
    assert(def_exposes(def, "position"));
    assert(def_exposes(def, "action"));
    assert(def.to_zigbee_count == 1);          // tz cover position kept
    assert(!def.endpoint_action_suffix);       // single channel -> bare action
    assert(def.endpoint_map_count == 0);
    assert(def_binds(def, 1, 0x0102));         // closuresWindowCovering bind

    // closuresWindowCovering UpOpen (cmd 0x00) -> action = "open".
    RuntimeContext c1{};
    auto op = dispatch_zcl_ep(c1, def, 0x0102, 1, "closuresWindowCovering", cmd_frame(0x00));
    assert(op.any_matched);
    assert(action_is(op, "action", "open"));

    // DownClose (cmd 0x01) -> action = "close".
    RuntimeContext c2{};
    auto cl = dispatch_zcl_ep(c2, def, 0x0102, 1, "closuresWindowCovering", cmd_frame(0x01));
    assert(action_is(cl, "action", "close"));

    // Stop (cmd 0x02) -> action = "stop".
    RuntimeContext c3{};
    auto st = dispatch_zcl_ep(c3, def, 0x0102, 1, "closuresWindowCovering", cmd_frame(0x02));
    assert(action_is(st, "action", "stop"));

    // Position attribute report (0x0008 currentPositionLiftPercentage, u8)
    // -> "position" still decodes (the kept half).
    RuntimeContext c4{};
    const std::array<std::uint8_t, 1> p = {0x32};   // 50 %
    auto pr = dispatch_zcl_ep(c4, def, 0x0102, 1, "closuresWindowCovering",
                              attr_report(0x0008, 0x20, std::span<const std::uint8_t>(p.data(), p.size())));
    assert(pr.any_matched);
    assert(pr.merged.find("position") != nullptr);
}

// ── ZB-PM-01: plain on/off module (regression guard). ──────────────────────
void test_pm_onoff() {
    using namespace zhc::devices::chacon;
    const auto& def = kDef_ZB_PM_01;

    assert(def_exposes(def, "state"));
    assert(!def_exposes(def, "action"));
    assert(def.to_zigbee_count == 1);          // tz on/off
    assert(def_binds(def, 1, 0x0006));

    // genOnOff onOff attribute report (0x0000 bool, true) -> "state".
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto r = dispatch_zcl_ep(c1, def, 0x0006, 1, "genOnOff",
                             attr_report(0x0000, 0x10, std::span<const std::uint8_t>(on.data(), on.size())));
    assert(r.any_matched);
    assert(r.merged.find("state") != nullptr);
}

}  // namespace

int main() {
    test_ersm_roller_shutter();
    test_pm_onoff();
    return 0;
}
