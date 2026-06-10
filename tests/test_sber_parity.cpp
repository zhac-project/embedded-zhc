// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for three Sber (SDevices) IAS Zone sensors whose
// auto-generated ports wired the generic kFzIasZone converter. That
// converter emits the raw zoneStatus bits ("alarm_1"/"alarm_2"/"tamper"/
// "battery_low"), but each def's expose list declared a bare key "alarm"
// that no converter ever produced — so the sensor's core state never
// reached the shadow. z2m decodes each via m.iasZoneAlarm({zoneType: ...}),
// which publishes a *semantic* key. There is no rename layer, so the fix
// graduates each def to definitions/sber/Sbe_*.cpp and wires the typed
// kFzIas<Type>Alarm converter + matching expose:
//
//   * SBDV-00029 — Smart motion sensor.  z2m zoneType "occupancy"
//       -> kFzIasMotionAlarm, expose `occupancy`.
//   * SBDV-00030 — Smart opening sensor. z2m zoneType "contact"
//       -> kFzIasContactAlarm, expose `contact`.
//   * SBDV-00154 — Smart water leak sensor. z2m zoneType "water_leak"
//       (zoneAttributes drop tamper) -> kFzIasWaterLeakAlarm, expose
//       `water_leak`.
//
// Each test pins: the semantic key is a declared expose, an alarm_1 report
// decodes to that key (and the bare "alarm" key is gone), and a clear
// report drops it.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::sber {
extern const PreparedDefinition kDef_SBDV_00029;  // motion    -> occupancy
extern const PreparedDefinition kDef_SBDV_00030;  // contact   -> contact
extern const PreparedDefinition kDef_SBDV_00154;  // waterleak -> water_leak
}  // namespace zhc::devices::sber

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

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// 9-byte IAS Zone Status Change Notification (ssIasZone cmd 0x00):
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

// Shared assertion: the semantic key `sem` is exposed, alarm_1 (bit 0)
// decodes to it (and the legacy bare "alarm" key is absent), and a
// clear report drops it. `expect_tamper_expose` distinguishes the two
// sensors that keep tamper (motion/contact) from water_leak, which z2m
// publishes without it.
//
// `expect_on_bit0`: most zone types publish the semantic key raw on bit 0
// (bit0 set -> key true). zoneType "contact" inverts in z2m (contact =
// !bit0: bit0 set -> contact:false), so the contact caller passes false.
// tamper/battery_low (bits 2/3) are unaffected.
void check_alarm1(const PreparedDefinition& def, const char* sem,
                  bool expect_tamper_expose, bool expect_on_bit0 = true) {
    // semantic key must be a declared expose (regression guard against
    // generic kFzIasZone, whose "alarm" key never matched the expose).
    assert(def_exposes(def, sem));
    assert(!def_exposes(def, "alarm"));
    assert(def_exposes(def, "tamper") == expect_tamper_expose);

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only (bit0 set)
    assert(on.any_matched);
    if (expect_on_bit0) assert(b_true(on.merged.find(sem)));
    else                assert(b_false(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);       // bare key must be gone
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // all clear (bit0 clear)
    assert(off.any_matched);
    if (expect_on_bit0) assert(b_false(off.merged.find(sem)));
    else                assert(b_true(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3), bit0 clear
    assert(tb.any_matched);
    if (expect_on_bit0) assert(b_false(tb.merged.find(sem)));
    else                assert(b_true(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

static void test_motion()  { check_alarm1(devices::sber::kDef_SBDV_00029, "occupancy",  true);  }
static void test_contact() { check_alarm1(devices::sber::kDef_SBDV_00030, "contact",    true,  /*expect_on_bit0=*/false); }
static void test_water()   { check_alarm1(devices::sber::kDef_SBDV_00154, "water_leak", false); }

}  // namespace

int main() {
    test_motion();
    test_contact();
    test_water();
    return 0;
}
