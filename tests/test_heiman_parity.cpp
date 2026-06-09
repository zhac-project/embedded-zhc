// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Heiman IAS-Zone sensor family.
// z2m-source: heiman.ts — the per-zone-type `fz.ias_<type>_alarm_{1,2}`
// decoders (and `fz.occupancy` for HS1MIS-3.0).
//
// Bug fixed: ~27 generated Heiman IAS defs lowered the generic
// kFzIasZone converter — which emits the bare key "alarm" — while their
// expose declared the *semantic* key (smoke / gas / water_leak /
// occupancy / contact / vibration / carbon_monoxide). With no rename
// layer the primary sensor state never reached the shadow. Each def was
// graduated to a Tier-2 parent override that swaps in the typed
// kFzIas<Type>Alarm converter (emitting the semantic key directly).
// HS1MIS-3.0 is a non-IAS msOccupancySensing device and was re-pointed
// at kFzOccupancy + a 0x0406 binding.
//
// These tests pin, on real Zone-Status-Change-Notification wire shapes,
// that each representative device now decodes its semantic key (and
// NOT the bare "alarm" key), and that the HS1MIS-3.0 occupancy path
// decodes from msOccupancySensing.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::heiman {
extern const PreparedDefinition kDef_HS1DS;        // contact (alarm_1)
extern const PreparedDefinition kDef_HS1SA_E;      // smoke   (alarm_1)
extern const PreparedDefinition kDef_HS1CG;        // gas     (alarm_1)
extern const PreparedDefinition kDef_HS3CG;        // gas     (alarm_2, bit 1)
extern const PreparedDefinition kDef_HS1WL_HS3WL;  // water_leak (alarm_1)
extern const PreparedDefinition kDef_HS1MS_EF;     // occupancy (alarm_1)
extern const PreparedDefinition kDef_HS1VS_EF;     // vibration (alarm_1)
extern const PreparedDefinition kDef_HS1CA_E;      // carbon_monoxide (alarm_1)
extern const PreparedDefinition kDef_HS1MIS_3_0;   // occupancy via msOccupancySensing
}  // namespace zhc::devices::heiman

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

// 9-byte IAS Zone Status Change Notification:
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
struct IasFrame { std::array<std::uint8_t, 9> bytes; };

IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = { 0x09, tsn, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

DispatchResult dispatch_ias(const PreparedDefinition& def, const IasFrame& f) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// Assert: alarm_1 (bit 0) asserted -> semantic key true, "alarm" absent,
// tamper/battery_low reflect bits 2/3.
void check_alarm1(const PreparedDefinition& def, const char* sem) {
    // semantic key must be a declared expose (regression guard against
    // generic kFzIasZone, whose "alarm" key never matched the expose).
    assert(def_exposes(def, sem));

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);       // bare key must be gone
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

}  // namespace

// ── per-type alarm_1 (bit 0) decoders ────────────────────────────────
static void test_contact()    { check_alarm1(devices::heiman::kDef_HS1DS,       "contact"); }
static void test_smoke()      { check_alarm1(devices::heiman::kDef_HS1SA_E,     "smoke"); }
static void test_gas_alarm1() { check_alarm1(devices::heiman::kDef_HS1CG,       "gas"); }
static void test_water()      { check_alarm1(devices::heiman::kDef_HS1WL_HS3WL, "water_leak"); }
static void test_occupancy()  { check_alarm1(devices::heiman::kDef_HS1MS_EF,    "occupancy"); }
static void test_vibration()  { check_alarm1(devices::heiman::kDef_HS1VS_EF,    "vibration"); }
static void test_co()         { check_alarm1(devices::heiman::kDef_HS1CA_E,     "carbon_monoxide"); }

// ── gas alarm_2 (bit 1) variant — HS3CG reads zoneStatus bit 1 ───────
static void test_gas_alarm2() {
    const auto& def = devices::heiman::kDef_HS3CG;
    assert(def_exposes(def, "gas"));

    // bit 0 set but bit 1 clear -> gas must stay false (alarm_2 reads bit 1).
    auto bit0 = dispatch_ias(def, ias_notif(0x0001));
    assert(bit0.any_matched);
    assert(b_false(bit0.merged.find("gas")));

    // bit 1 set -> gas true.
    auto bit1 = dispatch_ias(def, ias_notif(0x0002));
    assert(bit1.any_matched);
    assert(b_true(bit1.merged.find("gas")));
    assert(bit1.merged.find("alarm") == nullptr);
}

// ── HS1MIS-3.0 — occupancy via msOccupancySensing (NOT IAS) ──────────
static void test_hs1mis_occupancy_cluster() {
    const auto& def = devices::heiman::kDef_HS1MIS_3_0;
    assert(def_exposes(def, "occupancy"));

    // It must NOT be wired as an IAS device anymore: no ssIasZone fz, and
    // it must bind msOccupancySensing (0x0406).
    bool binds_occ = false, binds_ias = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i) {
        if (def.bindings[i].cluster_id == 0x0406) binds_occ = true;
        if (def.bindings[i].cluster_id == 0x0500) binds_ias = true;
    }
    assert(binds_occ);
    assert(!binds_ias);

    // msOccupancySensing (0x0406) Report Attributes: attr 0x0000 (occupancy,
    // bitmap8) bit 0 = occupied.
    const std::uint8_t kOccReport_present[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000
        0x18,                    // type bitmap8
        0x01,                    // value 1 = occupied
    };
    auto raw = make_frame(0x0406, 1, kOccReport_present);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msOccupancySensing";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    assert(b_true(r.merged.find("occupancy")));
    assert(r.merged.find("alarm") == nullptr);
}

int main() {
    test_contact();
    test_smoke();
    test_gas_alarm1();
    test_water();
    test_occupancy();
    test_vibration();
    test_co();
    test_gas_alarm2();
    test_hs1mis_occupancy_cluster();
    return 0;
}
