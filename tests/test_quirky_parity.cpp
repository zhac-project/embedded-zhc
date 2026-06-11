// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Quirky family (one device).
//
//   IAS dead-key — POFLW-WH02 ("Overflow") is a water-leak sensor whose z2m
//   def is m.iasZoneAlarm({zoneType:"water_leak", zoneAttributes:["alarm_1",
//   "battery_low"]}). zoneType is not "generic" and not bothAlarms, so z2m
//   publishes the alarm under the semantic key `water_leak` (zoneStatus bit 0),
//   never the bare "alarm". The auto-port wired the generic kFzIasZone (bare
//   "alarm") against an `alarm` expose the device never names. Swapped to the
//   typed kFzIasWaterLeakAlarm: bit 0 -> water_leak, bit 2 -> tamper, bit 3 ->
//   battery_low. z2m carries no battery-percentage extend here, so there is no
//   `battery` expose (the auto-port had none either — the "dropped battery"
//   suspect was false).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::quirky {
extern const PreparedDefinition kDef_POFLW_WH02;   // water_leak (bit0)
}  // namespace zhc::devices::quirky

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

// Keep `ctx` alive while the result is read: result pointers alias ctx scratch.
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

// A 9-byte Zone Status Change Notification (ssIasZone cmd 0x00).
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

void test_poflw_wh02() {
    using namespace zhc::devices::quirky;
    const auto& def = kDef_POFLW_WH02;

    // Semantic key present, legacy bare "alarm" gone.
    assert(def_exposes(def, "water_leak"));
    assert(!def_exposes(def, "alarm"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    // z2m has no battery-percentage extend for this device.
    assert(!def_exposes(def, "battery"));
    assert(def_binds(def, 1, 0x0500));

    // bit 0 set -> water_leak true; bare "alarm" never emitted.
    RuntimeContext c1{};
    auto on = dispatch_ias(c1, def, ias_notif(0x0001));
    assert(on.any_matched);
    assert(b_true(on.merged.find("water_leak")));
    assert(on.merged.find("alarm") == nullptr);

    // clear -> water_leak false.
    RuntimeContext c2{};
    auto off = dispatch_ias(c2, def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("water_leak")));

    // tamper (bit2) + battery_low (bit3) co-decode; water_leak clear here.
    RuntimeContext c3{};
    auto tb = dispatch_ias(c3, def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("water_leak")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

}  // namespace

int main() {
    test_poflw_wh02();
    return 0;
}
