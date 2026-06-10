// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the two Smartenit definitions.
// z2m-source: smartenit.ts.
//
// Bugs fixed:
//  * ZBHT-1 (temperature & humidity sensor) — the Tier-1 auto-port
//    dropped fz.temperature (0x0402) and fz.humidity (0x0405), reducing
//    a temp/humidity sensor to battery-only. Re-wired the generic
//    msTemperatureMeasurement / msRelativeHumidity decoders + exposes.
//  * 4040B (wireless metering 30A dual-load switch) — the auto-port
//    omitted the z2m endpoint map {l1:1, l2:2}, so genOnOff reports
//    from both loads collided on the bare "state" key. Added the
//    endpoint_map so the per-load switch surfaces as state_l1 / state_l2,
//    while the device-global power / energy (z2m's untagged presets)
//    stay un-suffixed via the dispatch global-key blocklist.
//
// These tests pin the decoders on real ZCL attribute-report wire shapes.

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

namespace zhc::devices::smartenit {
extern const PreparedDefinition kDef_ZBHT_1;  // temp/humidity sensor
extern const PreparedDefinition kDef_D4040B;  // dual-load metering switch
}  // namespace zhc::devices::smartenit

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

// ── ZBHT-1: temperature + humidity + battery channels ───────────────
void test_zbht1() {
    using namespace zhc::devices::smartenit;

    // The dropped channels must be back in the expose table.
    bool has_temp = false, has_hum = false, has_batt = false;
    for (std::size_t i = 0; i < kDef_ZBHT_1.exposes_count; ++i) {
        const char* n = kDef_ZBHT_1.exposes[i].name;
        if (std::strcmp(n, "temperature") == 0) has_temp = true;
        if (std::strcmp(n, "humidity") == 0) has_hum = true;
        if (std::strcmp(n, "battery") == 0) has_batt = true;
    }
    assert(has_temp && has_hum && has_batt);

    // msTemperatureMeasurement (0x0402) attr 0x0000 measuredValue = 2150
    // (s16, type 0x29) → 21.5 °C.
    //   fc=0x18 (profile-wide, S->C) | tsn | cmd=0x0A (report) |
    //   attr 0x0000 LE | type 0x29 | value 0x0866 LE (2150).
    {
        // s16 value 2150 = 0x0866 → little-endian bytes 0x66 0x08.
        const std::array<std::uint8_t, 8> full = {0x18, 0x01, 0x0A, 0x00, 0x00,
                                                  0x29, 0x66, 0x08};
        auto r = run(kDef_ZBHT_1, 0x0402, 1,
                     std::span<const std::uint8_t>(full.data(), full.size()));
        assert(r.any_matched);
        const Value* t = r.merged.find("temperature");
        assert(t && t->type == ValueType::Float);
        assert(t->f > 21.4f && t->f < 21.6f);
    }

    // msRelativeHumidity (0x0405) attr 0x0000 = 5000 (u16, type 0x21)
    // → 50.0 %.  value 0x1388 → bytes 0x88 0x13.
    {
        const std::array<std::uint8_t, 8> rep = {0x18, 0x02, 0x0A, 0x00, 0x00,
                                                 0x21, 0x88, 0x13};
        auto r = run(kDef_ZBHT_1, 0x0405, 1,
                     std::span<const std::uint8_t>(rep.data(), rep.size()));
        assert(r.any_matched);
        const Value* h = r.merged.find("humidity");
        assert(h && h->type == ValueType::Float);
        assert(h->f > 49.9f && h->f < 50.1f);
    }

    // genPowerCfg (0x0001) batteryPercentageRemaining (attr 0x0021) = 200
    // (u8) → 100 %.
    {
        const std::array<std::uint8_t, 7> rep = {0x18, 0x03, 0x0A, 0x21, 0x00,
                                                 0x20, 0xC8};
        auto r = run(kDef_ZBHT_1, 0x0001, 1,
                     std::span<const std::uint8_t>(rep.data(), rep.size()));
        assert(r.any_matched);
        const Value* b = r.merged.find("battery");
        assert(b && b->type == ValueType::Uint && b->u == 100);
    }
}

// ── 4040B: per-load state_l1/state_l2 + device-global power/energy ───
void test_4040b() {
    using namespace zhc::devices::smartenit;

    // endpoint_map must be present with l1->1, l2->2.
    assert(kDef_D4040B.endpoint_map != nullptr);
    assert(kDef_D4040B.endpoint_map_count == 2);

    // genOnOff (0x0006) attr 0x0000 onOff = 1 (bool, type 0x10).
    //   fc=0x18 | tsn | cmd=0x0A | attr 0x0000 LE | type 0x10 | val 0x01.
    auto onoff_report = [&](std::uint8_t src_ep) {
        const std::array<std::uint8_t, 7> rep = {0x18, 0x01, 0x0A, 0x00, 0x00,
                                                 0x10, 0x01};
        return run(kDef_D4040B, 0x0006, src_ep,
                   std::span<const std::uint8_t>(rep.data(), rep.size()));
    };

    // ep1 → suffixed to state_l1; bare "state" must NOT appear.
    {
        auto r = onoff_report(1);
        assert(r.any_matched);
        const Value* s = r.merged.find("state_l1");
        assert(s && s->type == ValueType::Bool && s->b == true);
        assert(r.merged.find("state") == nullptr);
        assert(r.merged.find("state_l2") == nullptr);
    }
    // ep2 → suffixed to state_l2 (the two loads no longer collide).
    {
        auto r = onoff_report(2);
        assert(r.any_matched);
        const Value* s = r.merged.find("state_l2");
        assert(s && s->type == ValueType::Bool && s->b == true);
        assert(r.merged.find("state") == nullptr);
        assert(r.merged.find("state_l1") == nullptr);
    }

    // seMetering (0x0702): currentSummDelivered (attr 0x0000, u48 type 0x25)
    // + instantaneousDemand (attr 0x0400, s24 type 0x2A) — divisor set on
    // ep2 in z2m, so the meaningful report arrives on ep2. power/energy are
    // device-global → must stay UN-suffixed even with the endpoint_map.
    {
        // Two attribute records back-to-back in one report frame:
        //   attr 0x0000 LE | type 0x25 | u48 value (6 bytes, =1000)
        //   attr 0x0400 LE | type 0x2A | s24 value (3 bytes, =50)
        const std::array<std::uint8_t, 18> rep = {
            0x18, 0x01, 0x0A,
            0x00, 0x00, 0x25, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00,  // 1000
            0x00, 0x04, 0x2A, 0x32, 0x00, 0x00,                    // 50
        };
        auto r = run(kDef_D4040B, 0x0702, 2,
                     std::span<const std::uint8_t>(rep.data(), rep.size()));
        assert(r.any_matched);
        // Un-suffixed device-global metering keys.
        const Value* e = r.merged.find("energy");
        assert(e && e->type == ValueType::Uint && e->u == 1000);
        const Value* p = r.merged.find("power");
        assert(p && p->type == ValueType::Int && p->i == 50);
        // Must NOT have been suffixed to the endpoint.
        assert(r.merged.find("energy_l2") == nullptr);
        assert(r.merged.find("power_l2") == nullptr);
    }
}

}  // namespace

int main() {
    test_zbht1();
    test_4040b();
    std::printf("smartenit parity tests passed\n");
    return 0;
}
