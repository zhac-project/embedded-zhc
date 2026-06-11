// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Philio PAT04-A (whitelabel Evology PAT04-A) water leak
// detector.
// z2m-source: philio.ts #PAT04-A —
//   m.iasZoneAlarm({zoneType: "water_leak",
//                   zoneAttributes: ["alarm_1", "tamper", "battery_low"]})
//   + m.battery()
//
// Bug fixed (found by a z2m<->embedded-zhc parity pass):
//   The auto-port wired the generic kFzIasZone (bare `alarm` from AttributeReport
//   attr 0x0002) and exposed a bare `alarm` binary. But this device reports via
//   z2m m.iasZoneAlarm(zoneType:"water_leak") — an ssIasZone
//   `commandStatusChangeNotification` (Command frame, cmd 0x00) whose
//   ZoneStatus rides the command body, publishing the semantic `water_leak`
//   key (zoneStatus bit 0). The old generic path matched nothing on the real
//   wire AND exposed the wrong key. Now wires kFzIasWaterLeakAlarm and exposes
//   `water_leak`: bit0 -> water_leak, bit2 -> tamper, bit3 -> battery_low.
//
// This test pins, on a real ZCL ZoneStatusChangeNotification wire shape, that
// the leak detector decodes its three semantic keys (and not the bare
// alarm/alarm_1/alarm_2 dead keys produced by the old generic path).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::philio {
extern const PreparedDefinition kDef_PAT04_A;  // water leak -> typed water_leak
}  // namespace zhc::devices::philio

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

// ── IAS Zone Status Change Notification (cluster 0x0500, cmd 0x00) ────
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

}  // namespace

// ── leak: ssIasZone commandStatusChangeNotification -> water_leak/tamper/battery_low
static void test_leak_decodes_command_notification() {
    const auto& def = devices::philio::kDef_PAT04_A;
    assert(def_exposes(def, "water_leak"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_exposes(def, "battery"));
    assert(def_binds(def, 0x0500));
    assert(def_binds(def, 0x0001));

    // alarm_1 (bit 0) asserted -> water_leak true, tamper/battery_low clear.
    auto on = dispatch_ias(def, ias_notif(0x0001));
    assert(on.any_matched);
    assert(b_true(on.merged.find("water_leak")));
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    // clear -> water_leak false (no invert for water_leak zoneType).
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("water_leak")));

    // tamper(bit2) + battery_low(bit3), alarm bit0 clear.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("water_leak")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// Regression: the device reports via the command notification and publishes the
// semantic `water_leak` key — never the bare `alarm` (old generic kFzIasZone)
// nor the alarm_1/alarm_2 dead keys.
static void test_no_dead_alarm_keys() {
    auto r = dispatch_ias(devices::philio::kDef_PAT04_A, ias_notif(0x0001));
    assert(r.merged.find("alarm") == nullptr);
    assert(r.merged.find("alarm_1") == nullptr);
    assert(r.merged.find("alarm_2") == nullptr);
}

int main() {
    test_leak_decodes_command_notification();
    test_no_dead_alarm_keys();
    return 0;
}
