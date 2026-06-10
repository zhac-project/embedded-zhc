// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Fantem ZB003-X "4 in 1 multi sensor" (TS0202).
// z2m-source: fantem.ts — fz.ZB003X_occupancy + legacy.fz.ZB003X.
//
// Bug fixed (IAS dead-key): the ZB003-X def lowered the generic
// kFzIasZoneStatusChange converter, which emits the bare key "alarm_1"
// for zoneStatus bit 0. But the expose declares the semantic key
// `occupancy`, and z2m's fz.ZB003X_occupancy maps zoneStatus bit 0 ->
// occupancy (bit 2 -> tamper). With the generic converter the primary
// motion state never reached the shadow (the `occupancy` expose was
// dead). The def was re-pointed at the typed kFzIasMotionAlarm
// converter, which emits `occupancy` from bit 0 directly.
//
// These tests pin, on a real Zone-Status-Change-Notification wire shape,
// that the ZB003-X decodes `occupancy` (and NOT bare alarm_1) plus
// tamper/battery_low, and that the Tuya-DP multi-sensor path still
// surfaces temperature (DP107 /10) and humidity (DP108).

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

namespace zhc::devices::fantem {
extern const PreparedDefinition kDef_ZB003_X;  // 4-in-1 multi sensor
}  // namespace zhc::devices::fantem

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

// ── IAS Zone Status Change Notification ──────────────────────────────
// fc(1)=0x09 tsn(1) cmd(1)=0x00 zoneStatus_le(2) extStatus(1) zoneId(1)
// delay_le(2).
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
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── Tuya 0xEF00 datapoint frame ──────────────────────────────────────
DispatchResult dispatch_dp(const PreparedDefinition& def,
                           std::span<const TuyaDpRecord> recs) {
    DecodedMessage msg{};
    msg.family       = FrameFamily::TuyaDp;
    msg.type         = MessageType::Command;
    msg.cluster      = "manuSpecificTuya";
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;

    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;

    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, recs, def, raw, ctx);
}

}  // namespace

// ── IAS occupancy / tamper / battery_low ─────────────────────────────
static void test_ias_occupancy() {
    const auto& def = devices::fantem::kDef_ZB003_X;

    // Regression guard: occupancy must be the declared semantic key.
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "tamper"));

    // bit 0 set -> occupancy true; the bare generic key alarm_1 must be gone.
    auto on = dispatch_ias(def, ias_notif(0x0001));
    assert(on.any_matched);
    assert(b_true(on.merged.find("occupancy")));
    assert(on.merged.find("alarm_1") == nullptr);
    assert(on.merged.find("alarm") == nullptr);
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    // clear.
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("occupancy")));

    // tamper(bit2)+battery_low(bit3) -> occupancy stays false.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("occupancy")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// ── Tuya DP multi-sensor: temperature (DP107 /10) + humidity (DP108) ─
static void test_dp_temperature_humidity() {
    const auto& def = devices::fantem::kDef_ZB003_X;
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));

    // DP107 fantemTemp: 235 wire (0x000000EB) /10 -> 23.5 C.
    const std::uint8_t kTemp[] = { 0x00, 0x00, 0x00, 0xEB };
    const TuyaDpRecord temp_recs[] = {
        { 107, 0x02, std::span<const std::uint8_t>(kTemp, 4) },
    };
    auto t = dispatch_dp(def, std::span<const TuyaDpRecord>(temp_recs, 1));
    assert(t.any_matched);
    const Value* tv = t.merged.find("temperature");
    assert(tv && "DP107 must surface temperature");
    const double tval = (tv->type == ValueType::Int) ? double(tv->i) : tv->f;
    assert(tval > 23.0 && tval < 24.0);

    // DP108 fantemHumidity: 55 wire -> 55 % (identity).
    const std::uint8_t kHum[] = { 0x00, 0x00, 0x00, 0x37 };
    const TuyaDpRecord hum_recs[] = {
        { 108, 0x02, std::span<const std::uint8_t>(kHum, 4) },
    };
    auto h = dispatch_dp(def, std::span<const TuyaDpRecord>(hum_recs, 1));
    assert(h.any_matched);
    const Value* hv = h.merged.find("humidity");
    assert(hv && "DP108 must surface humidity");
    const double hval = (hv->type == ValueType::Int) ? double(hv->i) : hv->f;
    assert(hval > 54.0 && hval < 56.0);
}

int main() {
    test_ias_occupancy();
    test_dp_temperature_humidity();
    return 0;
}
