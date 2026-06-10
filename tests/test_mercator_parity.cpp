// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Mercator Ikuü SSWF01G AC fan controller.
// z2m-source: mercator.ts #SSWF01G — fromZigbee:[fz.on_off, fz.fan].
//
// Bug fixed: z2m's fz.fan decodes hvacFanCtrl attr 0x0000 (fanMode, enum8)
// and publishes BOTH `fan_mode` (enum key) AND `fan_state` ("OFF"/"ON"). The
// generated port wired the generic kFzFanMode, which emits ONLY `fan_mode`,
// while still declaring a `fan_state` expose — so that key was DEAD (nothing
// ever populated it). Graduated to a Tier-2 parent with an inlined vendor
// converter (kFzMercatorFan) that emits `fan_mode` (u8) + `fan_state` (Bool:
// off/0 -> false) from the same 0x0000 report, mirroring z2m.
//
// These tests pin, on a real hvacFanCtrl Report-Attributes wire shape, that:
//   - fanMode==0 (off) -> fan_mode==0 AND fan_state==false
//   - fanMode==2 (medium) -> fan_mode==2 AND fan_state==true
//   - the on/off relay still decodes on genOnOff (no converter cross-talk)
//   - fan_state is declared as an expose (regression guard for the dead key)

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::mercator {
extern const PreparedDefinition kDef_SSWF01G;  // AC fan controller
}  // namespace zhc::devices::mercator

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

bool u_eq(const Value* v, std::uint64_t want) {
    return v && v->type == ValueType::Uint && v->u == want;
}
bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// hvacFanCtrl (0x0202) Report-Attributes for attr 0x0000 (fanMode, enum8 0x30).
//   fc(1)=0x18  tsn(1)  cmd(1)=0x0A  attr_le(2)=0x0000  type(1)=0x30  val(1)
DispatchResult dispatch_fan(const PreparedDefinition& def, std::uint8_t fan_mode) {
    const std::uint8_t report[] = {
        0x18, 0x42, 0x0A,   // fc, tsn, cmd=Report Attributes
        0x00, 0x00,         // attr 0x0000
        0x30,               // type enum8
        fan_mode,
    };
    auto raw = make_frame(0x0202, 1, report);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacFanCtrl";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// genOnOff (0x0006) Report-Attributes for attr 0x0000 (onOff, bool).
DispatchResult dispatch_onoff(const PreparedDefinition& def, bool on) {
    const std::uint8_t report[] = {
        0x18, 0x42, 0x0A,   // fc, tsn, cmd=Report Attributes
        0x00, 0x00,         // attr 0x0000
        0x10,               // type bool
        static_cast<std::uint8_t>(on ? 0x01 : 0x00),
    };
    auto raw = make_frame(0x0006, 1, report);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) { std::printf("  FAIL: %s\n", what); ++g_failures; }
}

// ── fan_mode + fan_state both decode from one hvacFanCtrl report ──────
void test_fan_decode() {
    const auto& def = devices::mercator::kDef_SSWF01G;

    // off (0): fan_mode==0, fan_state==false
    auto off = dispatch_fan(def, 0);
    check(off.any_matched,                       "fan off matched");
    check(u_eq(off.merged.find("fan_mode"), 0),  "fan off fan_mode==0");
    check(b_false(off.merged.find("fan_state")), "fan off fan_state==false");

    // medium (2): fan_mode==2, fan_state==true
    auto med = dispatch_fan(def, 2);
    check(med.any_matched,                        "fan medium matched");
    check(u_eq(med.merged.find("fan_mode"), 2),   "fan medium fan_mode==2");
    check(b_true(med.merged.find("fan_state")),   "fan medium fan_state==true");

    // high (3): fan_state still true
    auto high = dispatch_fan(def, 3);
    check(u_eq(high.merged.find("fan_mode"), 3),  "fan high fan_mode==3");
    check(b_true(high.merged.find("fan_state")),  "fan high fan_state==true");
}

// ── the on/off relay still decodes; fan keys do not leak onto it ─────
void test_onoff_still_works() {
    const auto& def = devices::mercator::kDef_SSWF01G;
    auto on = dispatch_onoff(def, true);
    check(on.any_matched,                        "onoff matched");
    check(b_true(on.merged.find("state")),       "onoff state==true");
    check(on.merged.find("fan_mode")  == nullptr, "onoff no fan_mode leak");
    check(on.merged.find("fan_state") == nullptr, "onoff no fan_state leak");
}

// ── exposes: state + fan_mode + fan_state declared (dead-key guard) ──
void test_exposes() {
    const auto& def = devices::mercator::kDef_SSWF01G;
    check(def_exposes(def, "state"),     "exposes state");
    check(def_exposes(def, "fan_mode"),  "exposes fan_mode");
    check(def_exposes(def, "fan_state"), "exposes fan_state");
}

}  // namespace

int main() {
    std::printf("test_mercator_parity\n");
    test_fan_decode();
    test_onoff_still_works();
    test_exposes();
    if (g_failures == 0) { std::printf("  OK\n"); return 0; }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
