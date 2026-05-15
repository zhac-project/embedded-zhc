// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Fixture: HOBEIAN ZG-204Z (Tuya zigbeeModel "ZG-204Z") motion sensor.
// Pins the IAS-Zone wire format the device actually sends, captured
// from a real hardware run:
//
//     I (..) zhc_adapter: 0x...77a7cea6 Tuya/ZG-204Z (no match)
//     cluster=0x0500(ssIasZone) family=Zcl type=4 cmd=0x00
//     zcl[9]=09 fa 00 01 00 00 00 00 00
//
// Before the def was rewritten this dispatcher returned `no match`
// because the generator emitted Tuya MCU converters on cluster
// 0xEF00. After: kFzBattery + kFzIasMotionAlarm bound to clusters
// 0x0001 + 0x0500. These tests lock that pairing.

// Force assert() to remain enabled even when the rest of the tree is
// built with -DNDEBUG; without this Release builds elide every check
// and the test reports a false pass.
#undef NDEBUG
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/types.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefGen_ZG_204Z;
}  // namespace zhc::devices::tuya

namespace {

InboundApsFrame make_frame(std::uint16_t cluster,
                            std::uint8_t   src_ep,
                            std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// Build the 9-byte Zone Status Change Notification body. The frame
// matches HOBEIAN's wire format exactly: cluster-specific, S→C,
// disable-default-response, no manufacturer code.
//
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
struct IasFrame {
    std::array<std::uint8_t, 9> bytes;
};

IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = { 0x09, tsn, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

DispatchResult dispatch_ias(const IasFrame& f) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(),
                                                          f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, devices::tuya::kDefGen_ZG_204Z,
                                raw, ctx);
}

bool b(const Value* v) { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

}  // namespace

// Motion detected: zoneStatus = 0x0001 (alarm_1 set, all other bits clear).
// This is the canonical "movement seen" packet captured on the user's
// network. Expect occupancy=true, tamper=false, battery_low=false.
static void test_motion_present() {
    auto r = dispatch_ias(ias_notif(0x0001));
    assert(r.any_matched);
    assert(b(r.merged.find("occupancy")));
    assert(b_false(r.merged.find("tamper")));
    assert(b_false(r.merged.find("battery_low")));
}

// Motion cleared: zoneStatus = 0x0000.
static void test_motion_clear() {
    auto r = dispatch_ias(ias_notif(0x0000));
    assert(r.any_matched);
    assert(b_false(r.merged.find("occupancy")));
    assert(b_false(r.merged.find("tamper")));
    assert(b_false(r.merged.find("battery_low")));
}

// Tamper alarm: bit 2 set without bit 0 (motion).
static void test_tamper_only() {
    auto r = dispatch_ias(ias_notif(0x0004));
    assert(r.any_matched);
    assert(b_false(r.merged.find("occupancy")));
    assert(b(r.merged.find("tamper")));
    assert(b_false(r.merged.find("battery_low")));
}

// Battery low: bit 3 — sensors report this when cell drops below the
// internal threshold. Motion may or may not be co-asserted.
static void test_battery_low_with_motion() {
    auto r = dispatch_ias(ias_notif(0x0009));   // bit0 + bit3
    assert(r.any_matched);
    assert(b(r.merged.find("occupancy")));
    assert(b_false(r.merged.find("tamper")));
    assert(b(r.merged.find("battery_low")));
}

// Sanity: the converter only fires on cmd 0x00 (Zone Status Change
// Notification). A bogus cluster-specific cmd id should return no
// match — guards against regressions where someone widens the cmd
// filter and accidentally swallows Zone-Enroll-Request etc.
static void test_other_cmd_does_not_match() {
    const std::uint8_t kFrame[] = { 0x09, 0x01, 0x01,   // cmd 0x01
                                     0x00 };
    auto raw = make_frame(0x0500, 1, std::span<const std::uint8_t>(kFrame, 4));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, devices::tuya::kDefGen_ZG_204Z,
                                   raw, ctx);
    assert(!r.any_matched);
}

int main() {
    test_motion_present();
    test_motion_clear();
    test_tamper_only();
    test_battery_low_with_motion();
    test_other_cmd_does_not_match();
    return 0;
}
