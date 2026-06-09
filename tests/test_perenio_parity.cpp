// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Perenio vendor port.
// z2m-source: zigbee-herdsman-converters/src/devices/perenio.ts.
//
// Bug fixed (PEHWE20 — two-channel single-wire mini-relay):
//   The def declared per-endpoint exposes `state_l1`/`state_l2`,
//   `power_on_behavior_l1`/`_l2`, `switch_type_l1`/`_l2` (mirroring
//   z2m's `endpoint: () => ({l1: 1, l2: 2})` + `withEndpoint(...)`),
//   but it was MISSING the `endpoint_map` field. The dispatcher only
//   rewrites emitted keys to `<key>_<label>` when `endpoint_map` is
//   populated (see src/runtime/dispatch.cpp). Without it, the bare
//   `state` / `power_on_behavior` / `switch_type` keys emitted by the
//   converters were never suffixed: both channels collided on the same
//   unsuffixed key (last-writer-wins in the merge) and the declared
//   `_l1`/`_l2` exposes never populated. Outbound writes to `state_l2`
//   likewise had no endpoint to route to.
//
//   Fix: graduate to Tier-2 parent + add `endpoint_map = {{l1,1},{l2,2}}`.
//
// These tests pin, on real ZCL wire shapes, that:
//   * a genOnOff onOff report from ep1 → `state_l1`, from ep2 → `state_l2`
//     (and the bare `state` key never survives the rewrite);
//   * power_on_behavior (genOnOff startUpOnOff 0x4003) suffixes per-EP;
//   * haDiagnostic reports (ep10, NOT in the endpoint_map) stay
//     unsuffixed — `last_message_lqi` / `last_message_rssi` are
//     device-global;
//   * PEHPL0X decodes the perenioSpecific smart-plug attributes
//     (rms_voltage / active_power / consumed_energy / default_on_off_state);
//   * the three IAS sensors decode the z2m semantic keys
//     (occupancy / contact / water_leak) — regression guard.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::perenio {
extern const PreparedDefinition kDef_PEHWE20;  // 2-channel relay
extern const PreparedDefinition kDef_PEHPL0X;  // smart plug + metering
extern const PreparedDefinition kDef_PECMS01;  // motion (IAS occupancy)
extern const PreparedDefinition kDef_PECWS01;  // contact (IAS)
extern const PreparedDefinition kDef_PECLS01;  // leak (IAS water_leak)
}  // namespace zhc::devices::perenio

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

DispatchResult run(const PreparedDefinition& def, std::uint16_t cluster,
                   std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool has_str(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

bool has_uint(const DispatchResult& r, const char* key, std::uint64_t expected) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Uint && v->u == expected;
}

bool has_bool(const DispatchResult& r, const char* key, bool expected) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Bool && v->b == expected;
}

// ── PEHWE20 — multi-endpoint key suffixing ──────────────────────────

// genOnOff attribute report: fc=0x18 (profile-wide, S->C) | tsn | cmd=0x0A
// (report) | attr 0x0000 LE | type 0x10 (bool) | value.
std::array<std::uint8_t, 7> on_off_report(std::uint8_t val) {
    return {0x18, 0x01, 0x0A, 0x00, 0x00, 0x10, val};
}

// genOnOff startUpOnOff (attr 0x4003, enum8 — 1 value byte) report.
std::array<std::uint8_t, 7> startup_report(std::uint8_t val) {
    return {0x18, 0x01, 0x0A, 0x03, 0x40, 0x30, val};
}

void test_pehwe20_endpoint_suffix() {
    using zhc::devices::perenio::kDef_PEHWE20;

    // Sanity: the def actually carries an endpoint_map (the fix).
    assert(kDef_PEHWE20.endpoint_map != nullptr);
    assert(kDef_PEHWE20.endpoint_map_count == 2);

    // onOff report from ep1 → state_l1 = true; bare `state` must NOT exist.
    {
        auto rep = on_off_report(0x01);
        auto r = run(kDef_PEHWE20, 0x0006, 1, rep);
        assert(r.any_matched);
        assert(has_bool(r, "state_l1", true));
        assert(r.merged.find("state") == nullptr);     // never unsuffixed
        assert(r.merged.find("state_l2") == nullptr);   // wrong channel absent
    }
    // onOff report from ep2 → state_l2 = false.
    {
        auto rep = on_off_report(0x00);
        auto r = run(kDef_PEHWE20, 0x0006, 2, rep);
        assert(r.any_matched);
        assert(has_bool(r, "state_l2", false));
        assert(r.merged.find("state") == nullptr);
        assert(r.merged.find("state_l1") == nullptr);
    }
    // power_on_behavior suffixes per endpoint (0x01 = "on").
    {
        auto rep = startup_report(0x01);
        auto r1 = run(kDef_PEHWE20, 0x0006, 1, rep);
        assert(r1.any_matched);
        assert(has_str(r1, "power_on_behavior_l1", "on"));
        assert(r1.merged.find("power_on_behavior") == nullptr);

        auto r2 = run(kDef_PEHWE20, 0x0006, 2, rep);
        assert(has_str(r2, "power_on_behavior_l2", "on"));
    }
    // haDiagnostic (ep10 — NOT in endpoint_map) → keys stay UNsuffixed.
    // attr 0x011C lastMessageLqi (u8) + 0x011D lastMessageRssi (s8).
    {
        const std::array<std::uint8_t, 11> rep = {
            0x18, 0x01, 0x0A,
            0x1C, 0x01, 0x20, 0xB4,         // 0x011C u8 = 180
            0x1D, 0x01, 0x28, 0xCE,         // 0x011D s8 = -50
        };
        auto r = run(kDef_PEHWE20, 0x0B05, 10, rep);
        assert(r.any_matched);
        assert(r.merged.find("last_message_lqi") != nullptr);
        // Must NOT be suffixed to a phantom _l1/_l2 (ep10 unmapped).
        assert(r.merged.find("last_message_lqi_l1") == nullptr);
        assert(r.merged.find("last_message_lqi_l2") == nullptr);
        assert(has_uint(r, "last_message_lqi", 180));
    }
}

// ── PEHPL0X — perenioSpecific smart-plug decode ─────────────────────

void test_pehpl0x_smart_plug() {
    using zhc::devices::perenio::kDef_PEHPL0X;

    // perenioSpecific manuf-specific attribute report (cluster 0xFC7B).
    // fc=0x1C (profile-wide, S->C, MANUF-specific) | manuf LE 0x007B |
    // tsn | cmd=0x0A (report) | <attr records>.
    //   attr 0x0003 rms_voltage (u16) = 230
    //   attr 0x000A active_power (u16) = 1500
    //   attr 0x000E consumed_energy (u16) = 4200
    //   attr 0x0000 default_on_off_state (enum8) = 2 -> "previous"
    const std::array<std::uint8_t, 24> rep = {
        0x1C, 0x7B, 0x00, 0x01, 0x0A,
        0x03, 0x00, 0x21, 0xE6, 0x00,        // 0x0003 u16 = 230
        0x0A, 0x00, 0x21, 0xDC, 0x05,        // 0x000A u16 = 1500
        0x0E, 0x00, 0x21, 0x68, 0x10,        // 0x000E u16 = 4200
        0x00, 0x00, 0x30, 0x02,              // 0x0000 enum8 = 2 (previous)
    };

    auto r = run(kDef_PEHPL0X, 0xFC7B, 1,
                 std::span<const std::uint8_t>(rep.data(), rep.size()));
    assert(r.any_matched);
    assert(has_uint(r, "rms_voltage", 230));
    assert(has_uint(r, "active_power", 1500));
    assert(has_uint(r, "consumed_energy", 4200));
    assert(has_str(r, "default_on_off_state", "previous"));
}

// ── IAS sensors — semantic-key regression guard ─────────────────────

// ssIasZone zoneStatusChangeNotification (cmd 0x00): fc=0x09
// (cluster-specific, S->C) | tsn | cmd=0x00 | zonestatus LE (u16) |
// extendedstatus(1) | zoneid(1) | delay LE(2).
std::array<std::uint8_t, 9> ias_status(std::uint16_t status) {
    return {0x09, 0x01, 0x00,
            static_cast<std::uint8_t>(status & 0xFF),
            static_cast<std::uint8_t>((status >> 8) & 0xFF),
            0x00, 0x00, 0x00, 0x00};
}

void test_ias_sensors() {
    using namespace zhc::devices::perenio;

    // PECMS01 motion: bit0 set → occupancy true.
    {
        auto rep = ias_status(0x0001);
        auto r = run(kDef_PECMS01, 0x0500, 1, rep);
        assert(r.any_matched);
        assert(has_bool(r, "occupancy", true));
    }
    // PECWS01 contact: bit0 set → contact true.
    {
        auto rep = ias_status(0x0001);
        auto r = run(kDef_PECWS01, 0x0500, 1, rep);
        assert(r.any_matched);
        assert(has_bool(r, "contact", true));
    }
    // PECLS01 leak: bit0 clear → water_leak false; bit2 → tamper true.
    {
        auto rep = ias_status(0x0004);
        auto r = run(kDef_PECLS01, 0x0500, 1, rep);
        assert(r.any_matched);
        assert(has_bool(r, "water_leak", false));
        assert(has_bool(r, "tamper", true));
    }
}

}  // namespace

int main() {
    test_pehwe20_endpoint_suffix();
    test_pehpl0x_smart_plug();
    test_ias_sensors();
    std::printf("perenio parity tests passed\n");
    return 0;
}
