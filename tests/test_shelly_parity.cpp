// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Shelly S4SN-0071A "Flood Gen 4" leak sensor.
//
// Bug fixed (wrong IAS converter + wrong key): z2m declares
//   m.iasZoneAlarm({zoneType: "water_leak",
//                   zoneAttributes: ["alarm_1", "tamper", "battery_low"]})
// which decodes the IAS Zone-Status-Change-Notification command and
// renames bit 0 to the semantic key "water_leak". The auto-generated
// port instead wired the generic kFzIasZone, which (a) only decodes an
// *attribute report* of ZoneStatus (attr 0x0002) — not the standard
// commandStatusChangeNotification push this sensor emits — and
// (b) emits the bare key "alarm" while the expose declared "alarm"
// too, so the leak state never reached the shadow under z2m's
// "water_leak" key. The def was graduated to a Tier-2 parent override
// that swaps in the typed kFzIasWaterLeakAlarm converter (emitting
// "water_leak" + "tamper" + "battery_low") and renames the expose to
// "water_leak".
//
// This test pins, on a real Zone-Status-Change-Notification wire shape,
// that the device now decodes "water_leak" (and NOT the bare "alarm"
// key), plus tamper / battery_low.
//
// z2m-source: zigbee-herdsman-converters/src/devices/shelly.ts #S4SN-0071A.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::shelly {
extern const PreparedDefinition kDef_S4SN_0071A;   // Flood Gen 4 (water_leak)
}  // namespace zhc::devices::shelly

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

void check_flood() {
    const auto& def = zhc::devices::shelly::kDef_S4SN_0071A;

    // The semantic key must be a declared expose; the bare "alarm" key
    // (left behind by the old generic kFzIasZone) must be gone.
    assert(def_exposes(def, "water_leak"));
    assert(!def_exposes(def, "alarm"));

    // alarm_1 bit set → water_leak true; tamper/battery_low clear.
    auto on = dispatch_ias(def, ias_notif(0x0001));
    assert(on.any_matched);
    assert(b_true(on.merged.find("water_leak")));
    assert(on.merged.find("alarm") == nullptr);   // bare key must never appear
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    // Clear.
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("water_leak")));

    // tamper (bit2) + battery_low (bit3), no leak.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("water_leak")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

}  // namespace

int main() {
    check_flood();
    return 0;
}
