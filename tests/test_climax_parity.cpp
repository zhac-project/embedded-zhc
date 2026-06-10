// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Climax family. The auto-generated ports carried two
// real bugs this fixture pins after the fix:
//
//   Wrong-cluster cover (SCM-5ZBS) — a roller shutter that z2m drives through
//   the brightness/on-off proxy (fz.cover_position_via_brightness +
//   fz.cover_state_via_onoff + tz.cover_via_brightness). The port wired the
//   generic kFzCoverPosition / kTzCoverPosition against closuresWindowCovering
//   (0x0102) — a cluster this device never speaks — so position decode AND
//   control were both dead. Rewired to the new generic via-brightness
//   converters (genLevelCtrl 0x0008 + genOnOff 0x0006).
//
//   IAS dead-key siren (SRAC-23B-ZBSR) — z2m fz.ias_siren is a
//   commandStatusChangeNotification decoder emitting `alarm` from zoneStatus
//   bit0. The port wired kFzIasZone, which reads only the ssIasZone *attribute*
//   report (attr 0x0002) and produces nothing on the status-change *command* a
//   siren sends → `alarm` was dead. Swapped to kFzIasGenericAlarm (command-path
//   bit0 → alarm). Also regained the dropped ias_wd max_duration channel
//   (kFzIasWdMaxDuration / kTzIasWdMaxDuration, ssIasWd 0x0502) and the missing
//   warning / squawk / max_duration exposes.

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

namespace zhc::devices::climax {
extern const PreparedDefinition kDef_SCM_5ZBS;        // cover via brightness
extern const PreparedDefinition kDef_SRAC_23B_ZBSR;   // siren
}  // namespace zhc::devices::climax

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

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

// IAS Zone Status Change Notification (ssIasZone cmd 0x00): fc=0x09, tsn, cmd,
// zonestatus u16 LE, ext-status u8, zoneid u8, delay u16 LE.
std::array<std::uint8_t, 9> ias_notif(std::uint16_t zone_status) {
    return { 0x09, 0x42, 0x00,
             static_cast<std::uint8_t>(zone_status & 0xFF),
             static_cast<std::uint8_t>(zone_status >> 8),
             0x00, 0x00, 0x00, 0x00 };
}

// ── SCM-5ZBS: position-via-brightness cover. ───────────────────────────────
void test_scm_5zbs() {
    using namespace zhc::devices::climax;
    const auto& def = kDef_SCM_5ZBS;

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

// ── SRAC-23B-ZBSR: siren IAS dead-key fix + max_duration channel. ──────────
void test_srac_23b() {
    using namespace zhc::devices::climax;
    const auto& def = kDef_SRAC_23B_ZBSR;

    assert(def_exposes(def, "alarm"));
    assert(def_exposes(def, "warning"));
    assert(def_exposes(def, "squawk"));
    assert(def_exposes(def, "max_duration"));
    assert(def_binds(def, 1, 0x0502));   // ssIasWd for max_duration

    // The fix: a Zone Status Change Notification *command* (cmd 0x00) with
    // bit0 set decodes `alarm` true. The old kFzIasZone (attribute-only) would
    // have produced nothing here.
    RuntimeContext c1{};
    auto on = dispatch_zcl_ep(c1, def, 0x0500, 1, "ssIasZone",
                              std::span<const std::uint8_t>(ias_notif(0x0001)));
    assert(on.any_matched);
    assert(b_true(on.merged.find("alarm")));

    // Clear notification -> alarm false.
    RuntimeContext c2{};
    auto off = dispatch_zcl_ep(c2, def, 0x0500, 1, "ssIasZone",
                               std::span<const std::uint8_t>(ias_notif(0x0000)));
    assert(off.any_matched);
    assert(b_false(off.merged.find("alarm")));

    // tamper (bit2) + battery_low (bit3) co-decode from the same command.
    RuntimeContext c3{};
    auto tb = dispatch_zcl_ep(c3, def, 0x0500, 1, "ssIasZone",
                              std::span<const std::uint8_t>(ias_notif(0x000C)));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("alarm")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));

    // max_duration decode: ssIasWd maxDuration (attr 0x0000, u16) = 300 s.
    RuntimeContext c4{};
    const std::array<std::uint8_t, 2> dur = {0x2C, 0x01};   // 300 LE
    auto mr = dispatch_zcl_ep(c4, def, 0x0502, 1, "ssIasWd",
                              attr_report(0x0000, 0x21, std::span<const std::uint8_t>(dur.data(), dur.size())));
    assert(mr.any_matched);
    const Value* md = mr.merged.find("max_duration");
    assert(md && md->type == ValueType::Uint && md->u == 300);

    // tz max_duration write routes to ssIasWd (0x0502).
    RuntimeContext c5{};
    std::array<std::uint8_t, 32> frame{};
    Value v{}; v.type = ValueType::Uint; v.u = 120;
    auto tz = dispatch_to_zigbee(def, "max_duration", v, c5,
                                 std::span<std::uint8_t>(frame.data(), frame.size()));
    assert(tz.ok);
    assert(tz.cluster_id == 0x0502);
}

}  // namespace

int main() {
    test_scm_5zbs();
    test_srac_23b();
    return 0;
}
