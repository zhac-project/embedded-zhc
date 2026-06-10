// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Lupus (LUPUSEC) family. The auto-generated ports of the
// two roller-shutter models carried the same wrong-cluster cover bug fixed for
// the Climax SCM family:
//
//   12031 / LS12128 — SCM-family roller shutters that z2m drives through the
//   brightness/on-off proxy (fz.cover_position_via_brightness +
//   fz.cover_state_via_onoff + tz.cover_via_brightness). The port wired the
//   generic kFzCoverPosition / kTzCoverPosition against closuresWindowCovering
//   (0x0102) — a cluster these devices never speak — so position decode AND
//   control were both dead, and the z2m cover_position() `state` expose was
//   dropped. Rewired to the via-brightness converters (genLevelCtrl 0x0008 +
//   genOnOff 0x0006) and restored the `state` expose.
//
// The 12127 two-channel relay is also pinned here: its l1/l2 endpoint_map must
// drive runtime key suffixing (state_l1 / state_l2) so the two channels do not
// collide on a bare `state` key.

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

namespace zhc::devices::lupus {
extern const PreparedDefinition kDef_D12031;    // cover via brightness
extern const PreparedDefinition kDef_LS12128;   // cover via brightness (OTA)
extern const PreparedDefinition kDef_D12127;    // 2-channel relay (l1/l2)
}  // namespace zhc::devices::lupus

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

bool str_is(const Value* v, const char* expected) {
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, expected) == 0;
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

// ZCL attribute report builder (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── Shared cover assertions for the SCM-family via-brightness ports. ────────
void check_cover(const PreparedDefinition& def) {
    assert(def_exposes(def, "position"));
    assert(def_exposes(def, "state"));
    // Wiring moved off the (dead) window-covering cluster onto level + on/off.
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0006));
    assert(!def_binds(def, 1, 0x0102));

    // currentLevel (genLevelCtrl 0x0000, u8) = 128 -> position 50 %, state OPEN.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> lvl = {128};
    auto pr = dispatch_zcl_ep(c1, def, 0x0008, 1, "genLevelCtrl",
                              attr_report(0x0000, 0x20, std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    assert(pr.any_matched);
    const Value* pos = pr.merged.find("position");
    assert(pos && pos->type == ValueType::Uint && pos->u == 50);
    assert(str_is(pr.merged.find("state"), "OPEN"));

    // currentLevel 0 -> position 0, state CLOSE.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> zero = {0};
    auto zr = dispatch_zcl_ep(c2, def, 0x0008, 1, "genLevelCtrl",
                              attr_report(0x0000, 0x20, std::span<const std::uint8_t>(zero.data(), zero.size())));
    assert(zr.any_matched);
    const Value* zpos = zr.merged.find("position");
    assert(zpos && zpos->type == ValueType::Uint && zpos->u == 0);
    assert(str_is(zr.merged.find("state"), "CLOSE"));

    // onOff (genOnOff 0x0000, bool) = 1 -> state OPEN via fz_cover_state_via_onoff.
    RuntimeContext c3{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto sr = dispatch_zcl_ep(c3, def, 0x0006, 1, "genOnOff",
                              attr_report(0x0000, 0x10, std::span<const std::uint8_t>(on.data(), on.size())));
    assert(sr.any_matched);
    assert(str_is(sr.merged.find("state"), "OPEN"));

    // tz: position 100 % -> moveToLevelWithOnOff (cmd 0x04) level 255 on 0x0008.
    RuntimeContext c4{};
    std::array<std::uint8_t, 16> frame{};
    Value full{}; full.type = ValueType::Uint; full.u = 100;
    auto tz = dispatch_to_zigbee(def, "position", full, c4,
                                 std::span<std::uint8_t>(frame.data(), frame.size()));
    assert(tz.ok);
    assert(tz.cluster_id == 0x0008);
    assert(tz.command_id == 0x04);
    assert(tz.frame_size == 6);
    assert(frame[2] == 0x04);    // moveToLevelWithOnOff
    assert(frame[3] == 255);     // 100 % -> 255 level
}

void test_covers() {
    using namespace zhc::devices::lupus;
    check_cover(kDef_D12031);
    check_cover(kDef_LS12128);
}

// ── 12127: two-channel relay endpoint_map → key suffixing. ──────────────────
void test_12127_endpoints() {
    using namespace zhc::devices::lupus;
    const auto& def = kDef_D12127;

    // endpoint_map present so the two on/off channels suffix to state_l1/state_l2.
    assert(def.endpoint_map != nullptr);
    assert(def.endpoint_map_count == 2);

    // genOnOff report on endpoint 1 -> state_l1 (suffixed via the l1 label).
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto r1 = dispatch_zcl_ep(c1, def, 0x0006, 1, "genOnOff",
                              attr_report(0x0000, 0x10, std::span<const std::uint8_t>(on.data(), on.size())));
    assert(r1.any_matched);
    const Value* l1 = r1.merged.find("state_l1");
    assert(l1 && l1->type == ValueType::Bool && l1->b);

    // genOnOff report on endpoint 2 -> state_l2.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> off = {0x00};
    auto r2 = dispatch_zcl_ep(c2, def, 0x0006, 2, "genOnOff",
                              attr_report(0x0000, 0x10, std::span<const std::uint8_t>(off.data(), off.size())));
    assert(r2.any_matched);
    const Value* l2 = r2.merged.find("state_l2");
    assert(l2 && l2->type == ValueType::Bool && !l2->b);
}

}  // namespace

int main() {
    test_covers();
    test_12127_endpoints();
    return 0;
}
