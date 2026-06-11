// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Shyugj S901D-ZG door sensor (IAS zone + battery).
//
// Bug fixed:
//   * z2m wires m.iasZoneAlarm({zoneType:"generic",
//       zoneAttributes:["alarm_1","alarm_2","tamper","battery_low"]}). The
//     "generic" zoneType keeps the bare zone keys (no semantic rename) and,
//     with BOTH alarm_1 + alarm_2 requested, exposes BOTH alarms:
//       alarm_1 = zoneStatus bit0, alarm_2 = bit1, tamper = bit2,
//       battery_low = bit3.
//   * The auto-ported def wired the generic kFzIasZone, which emits a single
//     `alarm` (bit0) + tamper + battery_low — the WRONG key (`alarm` vs
//     `alarm_1`) AND dropped alarm_2 (bit1) entirely.
//   * Fix: swap to kFzIasZoneStatusChange (emits alarm_1/alarm_2/tamper/
//     battery_low) and fix the exposes to declare alarm_1 + alarm_2.
//
// zoneType:"generic" is the z2m-correct bare-key form here, so this is NOT a
// dead-key rename to a typed semantic alarm — it is a missing-channel (alarm_2)
// + wrong-key (alarm -> alarm_1) repair.
//
// z2m-source: shyugj.ts + lib/modernExtend.ts iasZoneAlarm (generic branch).

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/shyugj_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> shyugj_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::shyugj::kShyugjRegistry, devices::shyugj::kShyugjRegistryCount);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

const PreparedDefinition* s901d() {
    const auto* d = find_definition("DoorSensor-ZB3.0", "Shyugj", shyugj_registry());
    assert(d && "DoorSensor-ZB3.0 / Shyugj must resolve to the S901D-ZG def");
    return d;
}

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

// The def exposes BOTH bare alarm keys + the diagnostics, and the single
// phantom `alarm` key the auto stub carried is gone.
void test_exposes_dual_alarm() {
    const auto* d = s901d();
    assert(std::strcmp(d->model, "S901D-ZG") == 0);
    assert(std::strcmp(d->vendor, "Shyugj") == 0);
    assert(def_exposes(*d, "alarm_1"));
    assert(def_exposes(*d, "alarm_2"));
    assert(def_exposes(*d, "tamper"));
    assert(def_exposes(*d, "battery_low"));
    assert(def_exposes(*d, "battery"));
    assert(def_exposes(*d, "voltage"));
    // Wrong-key phantom from the auto stub must be gone.
    assert(!def_exposes(*d, "alarm"));
}

// alarm_1 = zoneStatus bit0, independent of alarm_2 (bit1).
void test_alarm_1_bit0() {
    const auto& d = *s901d();
    auto on = dispatch_ias(d, ias_notif(0x0001));  // bit0 only
    assert(on.any_matched);
    assert(b_true(on.merged.find("alarm_1")));
    assert(b_false(on.merged.find("alarm_2")));
    // Never the wrong key.
    assert(on.merged.find("alarm") == nullptr);

    auto off = dispatch_ias(d, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("alarm_1")));
}

// alarm_2 = zoneStatus bit1 — the channel the auto stub dropped entirely.
void test_alarm_2_bit1() {
    const auto& d = *s901d();
    auto on = dispatch_ias(d, ias_notif(0x0002));  // bit1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find("alarm_2")));
    assert(b_false(on.merged.find("alarm_1")));  // bit0 clear -> alarm_1 false
    assert(on.merged.find("alarm") == nullptr);
}

// tamper (bit2) + battery_low (bit3) decode without tripping either alarm.
void test_tamper_and_battery_low() {
    const auto& d = *s901d();
    auto tb = dispatch_ias(d, ias_notif(0x000C));  // bit2 + bit3
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
    assert(b_false(tb.merged.find("alarm_1")));
    assert(b_false(tb.merged.find("alarm_2")));
}

}  // namespace

int main() {
    test_exposes_dual_alarm();
    test_alarm_1_bit0();
    test_alarm_2_bit1();
    test_tamper_and_battery_low();
    return 0;
}
