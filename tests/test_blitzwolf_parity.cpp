// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for two BlitzWolf sensors whose auto-ports misrouted the
// decode path.
//
// BW-IS3 (PIR motion sensor): z2m decodes it via
// legacy.fz.blitzwolf_occupancy_with_timeout — a manuSpecificTuya (0xEF00)
// DP stream where DP3 (`dataPoints.occupancy`) carries presence and the
// expose is e.occupancy(). The auto-port wired the generic kFzIasZone
// (ssIasZone 0x0500) and exposed phantom alarm/tamper/battery_low with a dead
// 0x0500 binding. The IAS-vs-DP misroute left occupancy dead. Graduated to a
// Tier-2 Tuya-DP override (DP3 -> occupancy, bind 0xEF00).
//
// BW-IS9 (water-leak sensor): z2m wires
//   m.iasZoneAlarm({zoneType:"water_leak",
//                   zoneAttributes:["alarm_1","alarm_2","tamper","battery_low"]})
// which (bothAlarms) exposes `water_leak_alarm_1` (zoneStatus bit 0) and
// `water_leak_alarm_2` (bit 1). The auto-port used the generic kFzIasZone
// (bare `alarm`) against an `alarm` expose — the declared semantic keys never
// populated. Repointed to the typed kFzIasWaterLeakAlarm1 (bit 0) +
// kFzIasWaterLeakAlarm2 (bit 1) converters with matching exposes.
//
// z2m-source: blitzwolf.ts + lib/legacy.ts fz.blitzwolf_occupancy_with_timeout
//             + lib/modernExtend.ts iasZoneAlarm.

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::blitzwolf {
extern const PreparedDefinition kDef_BW_IS3;  // PIR motion (Tuya DP3 occupancy)
extern const PreparedDefinition kDef_BW_IS9;  // water leak (IAS bit0 + bit1)
}  // namespace zhc::devices::blitzwolf

using namespace zhc;

namespace {

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

// ── BW-IS3: Tuya DP harness ────────────────────────────────────────────
InboundApsFrame make_dp_raw() {
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    return raw;
}

DecodedMessage make_dp_msg() {
    DecodedMessage msg{};
    msg.family       = FrameFamily::TuyaDp;
    msg.type         = MessageType::Command;
    msg.cluster      = "manuSpecificTuya";
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    return msg;
}

DispatchResult dispatch_dp(const PreparedDefinition& def,
                           std::span<const TuyaDpRecord> recs) {
    auto msg = make_dp_msg();
    auto raw = make_dp_raw();
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, recs, def, raw, ctx);
}

// ── BW-IS9: IAS zone harness ───────────────────────────────────────────
InboundApsFrame make_ias_raw(std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = 0x0500;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// 9-byte IAS Zone Status Change Notification:
//   fc(1)=0x09 tsn(1) cmd(1)=0x00 zoneStatus_le(2) extStatus(1) zoneId(1) delay_le(2)
struct IasFrame { std::array<std::uint8_t, 9> bytes; };

IasFrame ias_notif(std::uint16_t zone_status) {
    IasFrame f{};
    f.bytes = { 0x09, 0x42, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

DispatchResult dispatch_ias(const PreparedDefinition& def, const IasFrame& f) {
    auto raw = make_ias_raw(std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── BW-IS3 tests ───────────────────────────────────────────────────────

// The phantom IAS surface is gone; the real occupancy channel + 0xEF00 bind
// are in place.
void test_is3_exposes_and_bindings() {
    const auto& d = devices::blitzwolf::kDef_BW_IS3;
    assert(std::strcmp(d.model, "BW-IS3") == 0);
    assert(def_exposes(d, "occupancy"));
    assert(!def_exposes(d, "alarm"));
    assert(!def_exposes(d, "tamper"));
    assert(!def_exposes(d, "battery_low"));
    // Binds the Tuya MCU cluster, not the phantom ssIasZone 0x0500.
    assert(def_binds(d, 0xEF00));
    assert(!def_binds(d, 0x0500));
    // fz path is the Tuya DP bundle (sync-time + DP map), not IAS.
    assert(d.from_zigbee && d.from_zigbee_count >= 2);
}

// DP3 (bool) decodes to occupancy:true; presence reaches the shadow.
void test_is3_dp3_occupancy() {
    const auto& d = devices::blitzwolf::kDef_BW_IS3;
    const std::uint8_t kOn[] = { 0x01 };
    const TuyaDpRecord recs[] = { { 3, 0x01, std::span<const std::uint8_t>(kOn, 1) } };
    const auto r = dispatch_dp(d, std::span<const TuyaDpRecord>(recs, 1));
    assert(r.any_matched);
    assert(b_true(r.merged.find("occupancy")));

    const std::uint8_t kOff[] = { 0x00 };
    const TuyaDpRecord recsOff[] = { { 3, 0x01, std::span<const std::uint8_t>(kOff, 1) } };
    const auto rOff = dispatch_dp(d, std::span<const TuyaDpRecord>(recsOff, 1));
    assert(rOff.any_matched);
    assert(b_false(rOff.merged.find("occupancy")));
}

// ── BW-IS9 tests ───────────────────────────────────────────────────────

// The bare `alarm` expose is gone; the bothAlarms semantic keys are declared.
void test_is9_exposes() {
    const auto& d = devices::blitzwolf::kDef_BW_IS9;
    assert(std::strcmp(d.model, "BW-IS9") == 0);
    assert(def_exposes(d, "water_leak_alarm_1"));
    assert(def_exposes(d, "water_leak_alarm_2"));
    assert(def_exposes(d, "tamper"));
    assert(def_exposes(d, "battery_low"));
    assert(def_exposes(d, "battery"));
    assert(!def_exposes(d, "alarm"));
    assert(def_binds(d, 0x0500));
}

// bit 0 -> water_leak_alarm_1, bit 1 -> water_leak_alarm_2, independently.
void test_is9_alarm_bits() {
    const auto& d = devices::blitzwolf::kDef_BW_IS9;

    // bit 0 only -> alarm_1 true, alarm_2 false.
    auto a1 = dispatch_ias(d, ias_notif(0x0001));
    assert(a1.any_matched);
    assert(b_true(a1.merged.find("water_leak_alarm_1")));
    assert(b_false(a1.merged.find("water_leak_alarm_2")));
    assert(a1.merged.find("alarm") == nullptr);

    // bit 1 only -> alarm_2 true, alarm_1 false.
    auto a2 = dispatch_ias(d, ias_notif(0x0002));
    assert(a2.any_matched);
    assert(b_false(a2.merged.find("water_leak_alarm_1")));
    assert(b_true(a2.merged.find("water_leak_alarm_2")));

    // both bits set.
    auto both = dispatch_ias(d, ias_notif(0x0003));
    assert(both.any_matched);
    assert(b_true(both.merged.find("water_leak_alarm_1")));
    assert(b_true(both.merged.find("water_leak_alarm_2")));

    // tamper(bit2) + battery_low(bit3), no alarm bits.
    auto tb = dispatch_ias(d, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("water_leak_alarm_1")));
    assert(b_false(tb.merged.find("water_leak_alarm_2")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

}  // namespace

int main() {
    test_is3_exposes_and_bindings();
    test_is3_dp3_occupancy();
    test_is9_exposes();
    test_is9_alarm_bits();
    return 0;
}
