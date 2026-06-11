// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the TCL P2-WT water-leak sensor whose auto-port left the
// IAS zone-status decode wired to a dead key.
//
// z2m wires:
//   m.iasZoneAlarm({zoneType:"water_leak",
//                   zoneAttributes:["alarm_1","tamper","battery_low"]})
//   + m.battery()
// With a SINGLE alarm bit (no alarm_2), modernExtend uses the zoneType itself
// as the published key, so it emits the semantic `water_leak` (zoneStatus
// bit 0, NOT inverted — only zoneType "contact" inverts) plus `tamper`
// (bit 2) and `battery_low` (bit 3).
//
// The auto-port wired the generic kFzIasZone (which emits a bare `alarm`
// key) against an `alarm` expose, so the declared semantic `water_leak`
// key never populated. Repointed to the typed kFzIasWaterLeakAlarm
// converter + renamed the expose to `water_leak`.
//
// z2m-source: tcl.ts #P2-WT + lib/modernExtend.ts iasZoneAlarm.

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tcl {
extern const PreparedDefinition kDef_P2_WT;  // water leak (IAS bit0 → water_leak)
}  // namespace zhc::devices::tcl

using namespace zhc;

namespace {

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

// ── IAS zone harness ───────────────────────────────────────────────────
InboundApsFrame make_ias_raw(std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = 0x0500;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// 9-byte IAS Zone Status Change Notification:
//   fc(1)=0x09 tsn(1) cmd(1)=0x00 zoneStatus_le(2) extStatus(1) zoneId(1) delay_le(2)
struct IasFrame { std::array<std::uint8_t, 9> bytes; };

IasFrame ias_notif(std::uint16_t zone_status) {
    IasFrame f{};
    f.bytes = { 0x09, 0x42, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

DispatchResult dispatch_ias(const PreparedDefinition& def, const IasFrame& f) {
    auto raw = make_ias_raw(std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── tests ──────────────────────────────────────────────────────────────

// The dead bare `alarm` expose is replaced by the semantic `water_leak`;
// battery channel + IAS bind survive.
void test_p2wt_exposes_and_bindings() {
    const auto& d = devices::tcl::kDef_P2_WT;
    assert(std::strcmp(d.model, "P2-WT") == 0);
    assert(def_exposes(d, "water_leak"));
    assert(!def_exposes(d, "alarm"));      // the dead generic key is gone
    assert(def_exposes(d, "tamper"));
    assert(def_exposes(d, "battery_low"));
    assert(def_exposes(d, "battery"));
    assert(def_exposes(d, "voltage"));
    assert(def_binds(d, 0x0500));          // ssIasZone
    assert(def_binds(d, 0x0001));          // genPowerCfg (battery)
}

// zoneStatus bit 0 → water_leak (NOT inverted); tamper bit 2; battery_low bit 3.
void test_p2wt_alarm_bits() {
    const auto& d = devices::tcl::kDef_P2_WT;

    // bit 0 set → water_leak true (no inversion for water_leak).
    auto leak = dispatch_ias(d, ias_notif(0x0001));
    assert(leak.any_matched);
    assert(b_true(leak.merged.find("water_leak")));
    assert(leak.merged.find("alarm") == nullptr);   // never emits the dead key
    assert(b_false(leak.merged.find("tamper")));
    assert(b_false(leak.merged.find("battery_low")));

    // no bits → water_leak false (clear).
    auto clear = dispatch_ias(d, ias_notif(0x0000));
    assert(clear.any_matched);
    assert(b_false(clear.merged.find("water_leak")));

    // tamper (bit 2) + battery_low (bit 3), no leak.
    auto tb = dispatch_ias(d, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("water_leak")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

}  // namespace

int main() {
    test_p2wt_exposes_and_bindings();
    test_p2wt_alarm_bits();
    return 0;
}
