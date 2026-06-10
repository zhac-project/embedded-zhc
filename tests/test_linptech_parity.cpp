// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Linptech ES1ZZ(TY) (TS0225) mmWave Presence sensor
// (consolidated kDef_ES1ZZ_TY_ covering _TZ3218_awarhusb / _t9ynfz4x /
// _ewrxirng + Momax SL12S whitelabel).
//
// Bug fixed (parity pass 2026-06-10): fzLocal.TS0225_illuminance.
//   z2m reads the WHOLE raw ZCL frame: `buffer = msg.data`, then
//   `measuredValue = buffer[7]*256 + buffer[6]` and
//   `illuminance = measuredValue === 0 ? 0 : round(10^((raw-1)/10000))`.
//   The previous ZHC decoder read `raw_body[6]/[7]` (post-header body —
//   off by the 3-byte ZCL header) and emitted the non-z2m key
//   `illuminance_raw` (the raw u16, no lux curve). On a real attribute
//   report this read past the 2-byte value into garbage and never matched
//   z2m's `illuminance` key. Fixed to read `raw_data[6]/[7]` (== z2m's
//   full-frame offsets) + apply the lux curve + emit `illuminance`.
//
// Also pins the still-correct surfaces:
//   * fz.ias_occupancy_alarm_1 → occupancy(bit0)/tamper(bit2)/battery_low(bit3)
//   * the manuSpecificTuya2 attr decoder (distance/sensitivity/led/keep_time)
//   * DP 101 fading_time.
//
// z2m-source: zigbee-herdsman-converters/src/devices/linptech.ts #ES1ZZ(TY).

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::linptech {
extern const PreparedDefinition kDef_ES1ZZ_TY_;
}  // namespace zhc::devices::linptech

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("  FAIL: %s\n", what);
        ++g_failures;
    }
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

DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster_id,
                        std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

const Value* find(const DispatchResult& r, const char* k) { return r.merged.find(k); }

// ── illuminance raw frame: full-frame offsets 6/7 carry the measuredValue ──
// profile-wide Report Attributes (server->client):
//   fc(1)=0x18  tsn(1)  cmd(1)=0x0A  attrId_lo  attrId_hi  dataType
//   value_lo  value_hi
//                ^idx6     ^idx7  (z2m's buffer[6]/buffer[7])
struct LuxFrame { std::array<std::uint8_t, 8> bytes; };
LuxFrame lux_frame(std::uint16_t raw, std::uint8_t tsn = 0x42) {
    LuxFrame f{};
    f.bytes = { 0x18, tsn, 0x0A, 0x00, 0x00, 0x21,
                static_cast<std::uint8_t>(raw & 0xFF),
                static_cast<std::uint8_t>(raw >> 8) };
    return f;
}

std::uint32_t expected_lux(std::uint16_t raw) {
    if (raw == 0) return 0;
    const double v = std::pow(10.0, (static_cast<double>(raw) - 1.0) / 10000.0);
    long r = std::lround(v);
    if (r < 0) r = 0;
    return static_cast<std::uint32_t>(r);
}

void check_lux(const PreparedDefinition& def, std::uint16_t raw) {
    auto r = dispatch(def, 0x0400 /* msIlluminanceMeasurement */,
                      std::span<const std::uint8_t>(lux_frame(raw).bytes.data(), 8));
    check(r.any_matched, "lux frame matched");
    const Value* il = find(r, "illuminance");
    check(il && il->type == ValueType::Uint, "illuminance key present (not illuminance_raw)");
    if (il && il->type == ValueType::Uint)
        check(il->u == expected_lux(raw), "illuminance lux value matches z2m curve");
    // The dropped non-z2m key must be gone.
    check(find(r, "illuminance_raw") == nullptr, "no illuminance_raw key");
}

// ── IAS occupancy notification (9-byte status change) ────────────────
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)  ext(1) zoneId(1) delay(2)
struct IasFrame { std::array<std::uint8_t, 9> bytes; };
IasFrame ias_notif(std::uint16_t zs, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = { 0x09, tsn, 0x00,
                static_cast<std::uint8_t>(zs & 0xFF),
                static_cast<std::uint8_t>(zs >> 8),
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

}  // namespace

static void test_exposes() {
    const auto& def = devices::linptech::kDef_ES1ZZ_TY_;
    // z2m exposes the standard `illuminance` (lx) key, not a `*_raw` variant.
    check(def_exposes(def, "illuminance"),      "exposes illuminance");
    check(!def_exposes(def, "illuminance_raw"), "no illuminance_raw expose");
    check(def_exposes(def, "occupancy"),                    "exposes occupancy");
    check(def_exposes(def, "target_distance"),              "exposes target_distance");
    check(def_exposes(def, "motion_detection_distance"),    "exposes motion_detection_distance");
    check(def_exposes(def, "motion_detection_sensitivity"), "exposes motion_detection_sensitivity");
    check(def_exposes(def, "static_detection_sensitivity"), "exposes static_detection_sensitivity");
    check(def_exposes(def, "presence_keep_time"),           "exposes presence_keep_time");
    check(def_exposes(def, "fading_time"),                  "exposes fading_time");
    check(def_exposes(def, "led_indicator"),                "exposes led_indicator");
}

static void test_illuminance() {
    const auto& def = devices::linptech::kDef_ES1ZZ_TY_;
    check_lux(def, 0);       // z2m: 0 -> 0
    check_lux(def, 1);       // 10^0 = 1
    check_lux(def, 10001);   // 10^1 = 10
    check_lux(def, 30000);   // mid-range bright reading
}

static void test_ias_occupancy() {
    const auto& def = devices::linptech::kDef_ES1ZZ_TY_;
    auto on = dispatch_ias(def, ias_notif(0x0001));   // bit0
    check(on.any_matched, "ias occupancy matched");
    check(b_true(find(on, "occupancy")), "occupancy true on bit0");

    auto off = dispatch_ias(def, ias_notif(0x0000));
    check(b_false(find(off, "occupancy")), "occupancy false when clear");

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    check(b_true(find(tb, "tamper")),      "tamper bit2");
    check(b_true(find(tb, "battery_low")), "battery_low bit3");
    check(b_false(find(tb, "occupancy")),  "occupancy false on tamper/batt frame");
}

int main() {
    test_exposes();
    test_illuminance();
    test_ias_occupancy();
    if (g_failures == 0) {
        std::printf("linptech parity: all checks passed\n");
        return 0;
    }
    std::printf("linptech parity: %d FAILURE(S)\n", g_failures);
    return 1;
}
