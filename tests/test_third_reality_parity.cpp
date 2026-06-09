// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Third Reality IAS-Zone sensor family.
// z2m-source: third_reality.ts — the per-zone-type `fz.ias_<type>_alarm_1`
// decoders and the `m.iasZoneAlarm({zoneType})` modernExtend.
//
// Bug fixed: nine generated Third Reality IAS defs lowered the generic
// kFzIasZone converter — which emits the bare key "alarm" — while each
// device's primary state is semantic (occupancy / contact / water_leak /
// vibration). With no rename layer the sensor state never reached the
// shadow. Each def was graduated to a Tier-2 parent override that swaps in
// the typed kFzIas<Type>Alarm converter (emitting the semantic key + tamper
// + battery_low from zoneStatus bits 0/2/3). The light/buzzer on/off paths
// on the night light (3RSNL02043Z) and water sensor (3RWS18BZ) are left
// intact.
//
// These tests pin, on a real Zone-Status-Change-Notification wire shape,
// that each representative device now decodes its semantic key (and NOT
// the bare "alarm" key), plus the shared tamper/battery_low bits.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::third_reality {
extern const PreparedDefinition kDef_D3RMS16BZ;     // occupancy (motion)
extern const PreparedDefinition kDef_D3RSMR01067Z;  // occupancy (motion R1)
extern const PreparedDefinition kDef_D3RPS01083Z;   // occupancy (presence R2)
extern const PreparedDefinition kDef_D3RSNL02043Z;  // occupancy (night light)
extern const PreparedDefinition kDef_D3RDS17BZ;     // contact (door)
extern const PreparedDefinition kDef_D3RDTS01056Z;  // contact (garage tilt)
extern const PreparedDefinition kDef_D3RWS18BZ;     // water_leak
extern const PreparedDefinition kDef_D3RWS0218Z;    // water_leak (WL2)
extern const PreparedDefinition kDef_D3RVS01031Z;   // vibration
}  // namespace zhc::devices::third_reality

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

// Assert: alarm_1 (bit 0) asserted -> semantic key true, "alarm" absent,
// tamper/battery_low reflect bits 2/3.
void check_alarm1(const PreparedDefinition& def, const char* sem) {
    // semantic key must be a declared expose (regression guard against the
    // generic kFzIasZone, whose "alarm" key never matched the expose).
    assert(def_exposes(def, sem));
    assert(!def_exposes(def, "alarm"));

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);       // bare key must be gone
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

}  // namespace

// ── occupancy (motion / presence) — zoneStatus bit 0 ─────────────────
static void test_ms16bz()    { check_alarm1(devices::third_reality::kDef_D3RMS16BZ,    "occupancy"); }
static void test_smr01067z() { check_alarm1(devices::third_reality::kDef_D3RSMR01067Z, "occupancy"); }
static void test_ps01083z()  { check_alarm1(devices::third_reality::kDef_D3RPS01083Z,  "occupancy"); }
static void test_snl02043z() { check_alarm1(devices::third_reality::kDef_D3RSNL02043Z, "occupancy"); }

// ── contact (door / tilt) — zoneStatus bit 0 ─────────────────────────
static void test_ds17bz()    { check_alarm1(devices::third_reality::kDef_D3RDS17BZ,    "contact"); }
static void test_dts01056z() { check_alarm1(devices::third_reality::kDef_D3RDTS01056Z, "contact"); }

// ── water_leak — zoneStatus bit 0 ────────────────────────────────────
static void test_ws18bz()  { check_alarm1(devices::third_reality::kDef_D3RWS18BZ,  "water_leak"); }
static void test_ws0218z() { check_alarm1(devices::third_reality::kDef_D3RWS0218Z, "water_leak"); }

// ── vibration — zoneStatus bit 0 ─────────────────────────────────────
static void test_vs01031z() { check_alarm1(devices::third_reality::kDef_D3RVS01031Z, "vibration"); }

// The night light + water sensor keep their light/buzzer genOnOff path; the
// IAS swap must not have dropped the "state" / "brightness" exposes.
static void test_nightlight_keeps_light() {
    const auto& def = devices::third_reality::kDef_D3RSNL02043Z;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
}
static void test_water_keeps_buzzer() {
    assert(def_exposes(devices::third_reality::kDef_D3RWS18BZ, "state"));
}

int main() {
    test_ms16bz();
    test_smr01067z();
    test_ps01083z();
    test_snl02043z();
    test_ds17bz();
    test_dts01056z();
    test_ws18bz();
    test_ws0218z();
    test_vs01031z();
    test_nightlight_keeps_light();
    test_water_keeps_buzzer();
    return 0;
}
