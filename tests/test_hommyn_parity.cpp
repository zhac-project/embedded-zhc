// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Hommyn family. Both auto-generated ports carried the
// IAS dead-key bug: they wired the generic kFzIasZone (which emits the raw
// "alarm"/"tamper"/"battery_low" bits) against a semantic expose key the
// converter never produced. z2m decodes each via a typed fz.ias_*_alarm_1
// that publishes a semantic key (zoneStatus bit 0):
//   * MS-20-Z  fz.ias_occupancy_alarm_1_with_timeout (bit0) -> kFzIasMotionAlarm    -> occupancy
//   * WS-20-Z  fz.ias_water_leak_alarm_1             (bit0) -> kFzIasWaterLeakAlarm -> water_leak
// In both, tamper (bit2) + battery_low (bit3) co-decode. MS-20-Z also wires
// fz.battery; WS-20-Z does not (battery_low rides the zoneStatus frame).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::hommyn {
extern const PreparedDefinition kDef_MS_20_Z;  // occupancy (motion) + battery
extern const PreparedDefinition kDef_WS_20_Z;  // water_leak
}  // namespace zhc::devices::hommyn

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

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// IAS Zone Status Change Notification (ssIasZone cmd 0x00): fc=0x09 S->C,
// tsn, cmd, zonestatus(u16 LE), extstatus, zoneid, delay(u16).
struct IasFrame { std::array<std::uint8_t, 9> bytes; };

IasFrame ias_notif(std::uint16_t zone_status) {
    IasFrame f{};
    f.bytes = { 0x09, 0x42, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

DispatchResult dispatch_ias(RuntimeContext& ctx, const PreparedDefinition& def,
                            const IasFrame& f) {
    return dispatch_zcl_ep(ctx, def, 0x0500, 1, "ssIasZone",
                           std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
}

// Typed IAS sensor: the semantic key is exposed (and bare "alarm" is gone), an
// alarm on bit 0 decodes to it, a clear report drops it, and tamper(bit2) +
// battery_low(bit3) always co-decode. All hommyn sensors are raw-bit polarity.
void check_ias_bit0(const PreparedDefinition& def, const char* sem) {
    assert(def_exposes(def, sem));
    assert(!def_exposes(def, "alarm"));
    assert(def_binds(def, 1, 0x0500));

    RuntimeContext c1{};
    auto on = dispatch_ias(c1, def, ias_notif(0x0001));   // bit0 set
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);           // legacy bare key gone

    RuntimeContext c2{};
    auto off = dispatch_ias(c2, def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    RuntimeContext c3{};
    auto tb = dispatch_ias(c3, def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));                 // bit0 clear → sensor false
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

void test_ms_20_z() {
    using namespace zhc::devices::hommyn;
    // z2m fz.ias_occupancy_alarm_1_with_timeout -> "occupancy" (bit 0).
    check_ias_bit0(kDef_MS_20_Z, "occupancy");

    // fz.battery is wired: a powerCfg battery-percent report decodes to battery.
    assert(def_exposes(kDef_MS_20_Z, "battery"));
    assert(def_binds(kDef_MS_20_Z, 1, 0x0001));
    RuntimeContext c{};
    const std::array<std::uint8_t, 7> rpt = {
        0x18, 0x42, 0x0A, 0x21, 0x00, 0x20, 0xC8 };  // attr 0x0021 u8 = 200 (=100%)
    auto br = dispatch_zcl_ep(c, kDef_MS_20_Z, 0x0001, 1, "genPowerCfg",
                              std::span<const std::uint8_t>(rpt.data(), rpt.size()));
    assert(br.any_matched);
    const Value* bv = br.merged.find("battery");
    assert(bv && bv->type == ValueType::Uint && bv->u == 100);  // 0xC8=200 half-% → 100%
}

void test_ws_20_z() {
    using namespace zhc::devices::hommyn;
    // z2m fz.ias_water_leak_alarm_1 -> "water_leak" (bit 0).
    check_ias_bit0(kDef_WS_20_Z, "water_leak");
    // z2m WS-20-Z has no fz.battery / no battery% expose — only battery_low.
    assert(!def_exposes(kDef_WS_20_Z, "battery"));
}

}  // namespace

int main() {
    test_ms_20_z();
    test_ws_20_z();
    return 0;
}
