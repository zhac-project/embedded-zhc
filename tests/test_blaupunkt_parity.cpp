// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Blaupunkt SCM-S1 roller shutter. The auto-generated port
// carried a wrong-cluster cover bug this fixture pins after the fix:
//
//   z2m drives this shutter through the brightness/on-off proxy
//   (fz.cover_position_via_brightness + fz.cover_state_via_onoff +
//   tz.cover_via_brightness), NOT closuresWindowCovering. The port wired the
//   generic kFzCoverPosition / kTzCoverPosition against 0x0102 — a cluster this
//   device never speaks — so position decode AND control were both dead, and the
//   `state` channel (z2m cover_position().setAccess("state", ea.ALL)) was
//   dropped. Rewired to the generic via-brightness converters
//   (genLevelCtrl 0x0008 + genOnOff 0x0006). Mirrors the Climax SCM-5ZBS fix.

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

namespace zhc::devices::blaupunkt {
extern const PreparedDefinition kDef_SCM_S1;   // cover via brightness
}  // namespace zhc::devices::blaupunkt

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

// ── SCM-S1: position-via-brightness roller shutter. ────────────────────────
void test_scm_s1() {
    using namespace zhc::devices::blaupunkt;
    const auto& def = kDef_SCM_S1;

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

}  // namespace

int main() {
    test_scm_s1();
    return 0;
}
