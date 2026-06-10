// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Nyce NCZ-30xx IAS / multi-sensor family.
// z2m-source: nyce.ts.
//
// Bugs fixed (all five defs were generated as battery + generic kFzIasZone,
// which emits a bare `alarm` key and reads the wrong zoneStatus bit):
//
//   * Contact sensors (NCZ-3010 hinge, NCZ-3011-HA door/window) are wired in
//     z2m to fz.ias_contact_alarm_1 (zoneStatus bit 0 -> `contact`). The
//     generated defs emitted `alarm`. Re-wired to typed kFzIasContactAlarm.
//
//   * Motion sensors (NCZ-3041/3043/3045-HA) are multi-sensors: z2m wires
//     fz.occupancy (msOccupancySensing 0x0406), fz.temperature (0x0402),
//     fz.humidity (0x0405), fz.battery AND fz.ias_occupancy_alarm_2
//     (zoneStatus **bit 1** -> `occupancy`). The generated defs dropped
//     temperature/humidity/occupancy entirely AND read bit 0 via the generic
//     kFzIasZone. Re-wired all channels + kFzIasMotionAlarm2 (the bit-1
//     variant emitting `occupancy`).
//
// These tests pin, on real wire shapes:
//   1. Contact decodes `contact` from IAS bit 0 with z2m polarity
//      (contact = !bit0: bit0 SET -> open/false, clear -> closed/true) and
//      exposes `contact`, not `alarm`; tamper/battery_low ride the shared bits.
//   2. Motion decodes `occupancy` from IAS bit 1 (alarm_2) and NOT from bit 0
//      — the wrong-bit guard.
//   3. Motion also decodes occupancy from msOccupancySensing 0x0406, plus
//      temperature (0x0402, /100) and humidity (0x0405, /100) — the dropped
//      channels.

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::nyce {
extern const PreparedDefinition kDef_NCZ_3010;     // contact (alarm_1 / bit 0)
extern const PreparedDefinition kDef_NCZ_3011_HA;  // contact + tamper
extern const PreparedDefinition kDef_NCZ_3041_HA;  // motion multi-sensor
extern const PreparedDefinition kDef_NCZ_3043_HA;  // motion multi-sensor
extern const PreparedDefinition kDef_NCZ_3045_HA;  // motion multi-sensor
}  // namespace zhc::devices::nyce

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// 9-byte IAS Zone Status Change Notification:
//   fc(1)=0x09 tsn(1) cmd(1)=0x00 zoneStatus_le(2) extStatus(1) zoneId(1)
//   delay_le(2)
DispatchResult dispatch_ias(const PreparedDefinition& def,
                            std::uint16_t zone_status) {
    const std::array<std::uint8_t, 9> bytes = {
        0x09, 0x42, 0x00,
        static_cast<std::uint8_t>(zone_status & 0xFF),
        static_cast<std::uint8_t>(zone_status >> 8),
        0x00, 0x00, 0x00, 0x00};
    auto raw = make_frame(0x0500,
                          std::span<const std::uint8_t>(bytes.data(), bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Profile-wide attribute report (S->C):
//   fc(1)=0x18 tsn(1) cmd(1)=0x0A attrId(2,LE) type(1) value(LE).
DispatchResult report_u16(const PreparedDefinition& def, std::uint16_t cluster,
                          std::uint16_t value) {
    const std::array<std::uint8_t, 8> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 measuredValue
        0x21,                  // type 0x21 = uint16
        static_cast<std::uint8_t>(value & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF)};
    auto raw = make_frame(cluster,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// msOccupancySensing 0x0406 attr 0x0000 occupancy bitmap8 (type 0x18).
DispatchResult report_occupancy(const PreparedDefinition& def,
                                std::uint8_t bitmap) {
    const std::array<std::uint8_t, 7> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 occupancy
        0x18,                  // type 0x18 = bitmap8
        bitmap};
    auto raw = make_frame(0x0406,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }
bool near(float a, float b)  { return std::fabs(a - b) < 0.01f; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// Contact: z2m publishes `contact = !(zoneStatus bit0)` for zoneType:"contact"
// (fz.ias_contact_alarm_1) — bit0 SET (alarmed/open) -> contact:false; bit0
// CLEAR (closed) -> contact:true. Never `alarm`.
void test_contact(const PreparedDefinition& def, bool expect_tamper_expose) {
    assert(def_exposes(def, "contact"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "battery_low"));
    assert(!def_exposes(def, "alarm"));
    assert(!def_exposes(def, "voltage"));
    assert(def_exposes(def, "tamper") == expect_tamper_expose);

    auto on = dispatch_ias(def, 0x0001);   // alarm_1 / bit 0 SET -> open -> contact:false
    assert(on.any_matched);
    assert(b_false(on.merged.find("contact")));
    assert(on.merged.find("alarm") == nullptr);

    auto off = dispatch_ias(def, 0x0000);  // bit 0 CLEAR -> closed -> contact:true
    assert(off.any_matched);
    assert(b_true(off.merged.find("contact")));

    // tamper(bit2)+battery_low(bit3) ride the shared bits.
    auto tb = dispatch_ias(def, 0x000C);
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// Motion: occupancy from IAS bit 1 (alarm_2), NOT bit 0; plus the restored
// msOccupancySensing / temperature / humidity channels.
void test_motion(const PreparedDefinition& def) {
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "tamper"));
    assert(!def_exposes(def, "alarm"));
    assert(!def_exposes(def, "voltage"));

    // IAS alarm_2 / bit 1 -> occupancy true.
    auto on = dispatch_ias(def, 0x0002);
    assert(on.any_matched);
    assert(b_true(on.merged.find("occupancy")));
    assert(on.merged.find("alarm") == nullptr);

    // bit 0 (alarm_1) must NOT trip occupancy — wrong-bit guard.
    auto bit0 = dispatch_ias(def, 0x0001);
    assert(bit0.any_matched);
    assert(b_false(bit0.merged.find("occupancy")));

    // tamper(bit2)+battery_low(bit3) ride the shared bits.
    auto tb = dispatch_ias(def, 0x000C);
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));

    // msOccupancySensing 0x0406 path (fz.occupancy) also decodes occupancy.
    auto occ = report_occupancy(def, 0x01);
    assert(occ.any_matched);
    assert(b_true(occ.merged.find("occupancy")));
    auto occ0 = report_occupancy(def, 0x00);
    assert(occ0.any_matched);
    assert(b_false(occ0.merged.find("occupancy")));

    // temperature 0x0402, /100 (was dropped entirely).
    auto t = report_u16(def, 0x0402, 2150);
    assert(t.any_matched);
    const Value* tv = t.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float && near(tv->f, 21.50f));

    // humidity 0x0405, /100 (was dropped entirely).
    auto h = report_u16(def, 0x0405, 4567);
    assert(h.any_matched);
    const Value* hv = h.merged.find("humidity");
    assert(hv && hv->type == ValueType::Float && near(hv->f, 45.67f));
}

}  // namespace

int main() {
    using namespace zhc::devices::nyce;

    test_contact(kDef_NCZ_3010, /*expect_tamper_expose=*/false);
    test_contact(kDef_NCZ_3011_HA, /*expect_tamper_expose=*/true);

    test_motion(kDef_NCZ_3041_HA);
    test_motion(kDef_NCZ_3043_HA);
    test_motion(kDef_NCZ_3045_HA);

    std::printf("nyce parity tests passed\n");
    return 0;
}
