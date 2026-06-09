// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Bacchus flower / soil sensors.
// z2m-source: bacchus.ts — #Flower_Sensor_v2, #Flower_Sensor_v4.
//
// Bug fixed: both defs expose `soil_moisture` (z2m `m.soilMoisture()` on
// msSoilMoisture 0x0408 — the headline measurement), but the generated
// Tier-1 defs only wired kFzTemperature/kFzIlluminance/kFzBattery. The
// generic kFzSoilMoisture (added for the sibling custom_devices_diy /
// diyruz / efekta flower sensors) was never wired here, so every soil
// report silently dropped — the sensors decoded temperature/illuminance/
// battery but NEVER soil moisture.
//
// Fix: graduated both defs generated/ -> parent and added
// &generic::kFzSoilMoisture to their from-zigbee lists.
//
// These tests pin, on real msSoilMoisture attribute-report wire shapes,
// that soil moisture decodes to the z2m-scaled percentage (u16 / 100),
// and regression-check that temperature (and illuminance on v4) still
// decode alongside it.

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

namespace zhc::devices::bacchus {
extern const PreparedDefinition kDef_Flower_Sensor_v2;
extern const PreparedDefinition kDef_Flower_Sensor_v4;
}  // namespace zhc::devices::bacchus

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
    using namespace zhc::devices::bacchus;

    // --- soil moisture must decode on BOTH defs (was dropped entirely) ---
    assert_soil(kDef_Flower_Sensor_v2);
    assert_soil(kDef_Flower_Sensor_v4);

    // --- regression: temperature still decodes on both (0x0402, /100) ---
    {
        auto r = report_u16(kDef_Flower_Sensor_v2,
                            0x0402 /* msTemperatureMeasurement */, 2150);
        assert(r.any_matched);
        const Value* t = r.merged.find("temperature");
        assert(t && t->type == ValueType::Float && near(t->f, 21.50f));
    }
    {
        auto r = report_u16(kDef_Flower_Sensor_v4,
                            0x0402 /* msTemperatureMeasurement */, 1875);
        assert(r.any_matched);
        const Value* t = r.merged.find("temperature");
        assert(t && t->type == ValueType::Float && near(t->f, 18.75f));
    }

    // --- v4 illuminance (m.illuminance()) must still decode (0x0400) ---
    {
        auto r = report_u16(kDef_Flower_Sensor_v4,
                            0x0400 /* msIlluminanceMeasurement */, 12345);
        assert(r.any_matched);
        const Value* l = r.merged.find("illuminance");
        assert(l && l->type == ValueType::Uint && l->u == 12345);
    }

    std::printf("bacchus parity tests passed\n");
    return 0;
}
