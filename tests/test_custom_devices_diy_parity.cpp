// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Custom devices (DiY) flower / soil sensors.
// z2m-source: custom_devices_diy.ts — #ZeeFlora, #b-parasite.
//
// Bug fixed: both defs expose `soil_moisture`, but no `kFzSoilMoisture`
// converter existed in _generic, and 0x0408 (msSoilMoisture) was missing
// from cluster_names.hpp. So every soil report silently dropped — the
// sensors decoded battery/temperature/humidity but NEVER soil moisture
// (the headline measurement). The same gap affects bacchus/diyruz/efekta
// flower sensors that reuse the generic stack.
//
// ZeeFlora additionally carried z2m `extend:[m.illuminance()]`, which the
// generator dropped entirely (no illuminance expose, no kFzIlluminance,
// no 0x0400 bind). Graduated to Tier-2 to wire it.
//
// Fix: added generic kFzSoilMoisture (msSoilMoisture 0x0408 measuredValue,
// u16 / 100 -> %, mirrors kFzHumidity) + the 0x0408 cluster-name mapping;
// graduated ZeeFlora + b_parasite to wire it (and illuminance on ZeeFlora).
//
// These tests pin, on real measurement-cluster attribute-report wire
// shapes, that soil moisture decodes to the z2m-scaled percentage and
// that illuminance decodes on ZeeFlora.

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::custom_devices_diy {
extern const PreparedDefinition kDef_ZeeFlora;
extern const PreparedDefinition kDef_b_parasite;
}  // namespace zhc::devices::custom_devices_diy

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// Profile-wide attribute report (S->C):
//   fc(1)=0x18 | tsn(1) | cmd(1)=0x0A | attrId(2,LE) | type(1) | value(LE).
// All sensor measured values here are at attr 0x0000.
DispatchResult report_u16(const PreparedDefinition& def, std::uint16_t cluster,
                          std::uint16_t value) {
    const std::array<std::uint8_t, 8> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 measuredValue
        0x21,                  // type 0x21 = uint16
        static_cast<std::uint8_t>(value & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF)};
    auto raw = make_frame(cluster,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool near(float a, float b) { return std::fabs(a - b) < 0.01f; }

// msSoilMoisture (0x0408) raw 4523 -> 45.23 %.  (z2m scale = 100)
void assert_soil(const PreparedDefinition& def) {
    auto r = report_u16(def, 0x0408, 4523);
    assert(r.any_matched);
    const Value* s = r.merged.find("soil_moisture");
    assert(s && s->type == ValueType::Float && near(s->f, 45.23f));
}

}  // namespace

int main() {
    using namespace zhc::devices::custom_devices_diy;

    // --- soil moisture must decode on BOTH defs (was dropped entirely) ---
    assert_soil(kDef_ZeeFlora);
    assert_soil(kDef_b_parasite);

    // --- ZeeFlora illuminance (m.illuminance()) must decode (was missing) ---
    {
        auto r = report_u16(kDef_ZeeFlora, 0x0400 /* msIlluminanceMeasurement */,
                            12345);
        assert(r.any_matched);
        const Value* l = r.merged.find("illuminance");
        assert(l && l->type == ValueType::Uint && l->u == 12345);
    }

    // --- regression: temperature still decodes alongside (0x0402, /100) ---
    {
        auto r = report_u16(kDef_ZeeFlora, 0x0402 /* msTemperatureMeasurement */,
                            2150);
        assert(r.any_matched);
        const Value* t = r.merged.find("temperature");
        assert(t && t->type == ValueType::Float && near(t->f, 21.50f));
    }

    // --- b_parasite humidity still decodes (0x0405, /100) ---
    {
        auto r = report_u16(kDef_b_parasite, 0x0405 /* msRelativeHumidity */,
                            5567);
        assert(r.any_matched);
        const Value* h = r.merged.find("humidity");
        assert(h && h->type == ValueType::Float && near(h->f, 55.67f));
    }

    std::printf("custom_devices_diy parity tests passed\n");
    return 0;
}
