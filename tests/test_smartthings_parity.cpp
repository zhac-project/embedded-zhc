// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Samsung SmartThings sensor family.
// z2m-source: zigbee-herdsman-converters/src/devices/smartthings.ts +
//             converters/fromZigbee.ts.
//
// Bugs fixed (each device graduated generated/ -> Tier-2 parent):
//
//  * IAS wrong-bit: 3305-S and 3325-S are motion sensors whose z2m defs
//    read occupancy from `fz.ias_occupancy_alarm_2` (zoneStatus bit 1)
//    ONLY. The generated ports wired `kFzIasMotionAlarm` (bit 0), so the
//    PIR never fired. Both now use `kFzIasMotionAlarm2` (bit 1). The
//    temperature channel (msTemperatureMeasurement) was already correct.
//
//  * Dead action enum: IM6001-BTP01 is a button. z2m decodes it with
//    `fz.command_status_change_notification_action`, which maps the whole
//    ssIasZone commandStatusChangeNotification `zoneStatus` value through
//    {0:"off",1:"single",2:"double",3:"hold"} -> action. The port wired
//    the generic `kFzIasZoneStatusChange` (alarm-bit booleans), so the
//    advertised `action` enum was never populated. Now wired to the new
//    vendor converter `smartthings::kFzStButtonAction`.
//
// These tests pin the corrected wire-shape decode for each fix.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::smartthings {
extern const PreparedDefinition kDef_D3305_S;       // occupancy (alarm_2) + temp
extern const PreparedDefinition kDef_D3325_S;       // occupancy (alarm_2) + temp
extern const PreparedDefinition kDef_IM6001_BTP01;  // button action
}  // namespace zhc::devices::smartthings

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool f_near(const Value* v, float expect) {
    if (!v || v->type != ValueType::Float) return false;
    const float d = v->f - expect;
    return (d < 0.01f) && (d > -0.01f);
}

bool str_eq(const Value* v, const char* expect) {
    return v && v->type == ValueType::StringRef && v->str
           && std::strcmp(v->str, expect) == 0;
}

// ── 9-byte IAS Zone Status Change Notification (ssIasZone cmd 0x00) ───
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
struct IasFrame { std::array<std::uint8_t, 9> bytes; };
IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = {0x09, tsn, 0x00,
               static_cast<std::uint8_t>(zone_status & 0xFF),
               static_cast<std::uint8_t>(zone_status >> 8),
               0x00, 0x00, 0x00, 0x00};
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

// ── ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then records ─
std::array<std::uint8_t, 8> temp_report(std::int16_t centidegrees) {
    return {0x18, 0x42, 0x0A,
            0x00, 0x00,            // attr 0x0000
            0x29,                  // type s16
            static_cast<std::uint8_t>(centidegrees & 0xFF),
            static_cast<std::uint8_t>(centidegrees >> 8)};
}
DispatchResult dispatch_temp(const PreparedDefinition& def, std::int16_t centidegrees) {
    auto bytes = temp_report(centidegrees);
    auto raw = make_frame(0x0402, 1,
                          std::span<const std::uint8_t>(bytes.data(), bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msTemperatureMeasurement";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Motion sensor reading occupancy from zoneStatus bit 1 (alarm_2). Bit 0
// must NOT drive occupancy (that was the bug). Temperature still decodes.
void check_motion_alarm2(const PreparedDefinition& def) {
    assert(def_exposes(def, "occupancy"));
    assert(!def_exposes(def, "alarm"));      // no bare key
    assert(def_binds(def, 0x0500));
    assert(def_binds(def, 0x0402));

    // bit 1 set -> occupancy true
    auto on = dispatch_ias(def, ias_notif(0x0002));
    assert(on.any_matched);
    assert(b_true(on.merged.find("occupancy")));

    // bit 1 clear -> occupancy false
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("occupancy")));

    // bit 0 set but bit 1 clear -> occupancy MUST stay false (regression
    // guard for the wrong-bit bug).
    auto b0 = dispatch_ias(def, ias_notif(0x0001));
    assert(b0.any_matched);
    assert(b_false(b0.merged.find("occupancy")));

    // tamper (bit 2) + battery_low (bit 3) still surface.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
    assert(b_false(tb.merged.find("occupancy")));

    // temperature channel intact: 0x08F4 = 2292 -> 22.92 °C
    auto t = dispatch_temp(def, 0x08F4);
    assert(t.any_matched);
    assert(f_near(t.merged.find("temperature"), 22.92f));
}

}  // namespace

// ── IAS wrong-bit fixes (occupancy on alarm_2 / bit 1) ───────────────
static void test_d3305_motion_alarm2() {
    check_motion_alarm2(devices::smartthings::kDef_D3305_S);
}
static void test_d3325_motion_alarm2() {
    check_motion_alarm2(devices::smartthings::kDef_D3325_S);
}

// ── IM6001-BTP01 — button action via zoneStatus value lookup ─────────
static void test_btp01_button_action() {
    const auto& def = devices::smartthings::kDef_IM6001_BTP01;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));      // not a controllable switch
    assert(def.to_zigbee_count == 0);        // no TZ

    // zoneStatus value (whole field) maps to the action label.
    auto off    = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(str_eq(off.merged.find("action"), "off"));

    auto single = dispatch_ias(def, ias_notif(0x0001));
    assert(single.any_matched);
    assert(str_eq(single.merged.find("action"), "single"));

    auto dbl    = dispatch_ias(def, ias_notif(0x0002));
    assert(dbl.any_matched);
    assert(str_eq(dbl.merged.find("action"), "double"));

    auto hold   = dispatch_ias(def, ias_notif(0x0003));
    assert(hold.any_matched);
    assert(str_eq(hold.merged.find("action"), "hold"));

    // The button must NOT publish IAS alarm-bit booleans.
    assert(off.merged.find("alarm_1") == nullptr);
    assert(off.merged.find("occupancy") == nullptr);
    assert(off.merged.find("contact") == nullptr);

    // Unknown code -> converter declines (no action emitted). Temperature
    // (a separate converter) still runs, so the frame may match overall;
    // assert only that no bogus action label leaks.
    auto unknown = dispatch_ias(def, ias_notif(0x00FF));
    assert(unknown.merged.find("action") == nullptr);
}

int main() {
    test_d3305_motion_alarm2();
    test_d3325_motion_alarm2();
    test_btp01_button_action();
    return 0;
}
