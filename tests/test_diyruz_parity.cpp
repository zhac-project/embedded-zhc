// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for DIYRuZ sensors.
// z2m-source: diyruz.ts — #DIYRuZ_Flower, #DIYRuZ_AirSense, #DIYRuZ_magnet.
//
// Bugs fixed (all headline channels silently dropped by the generated
// Tier-1 ports because the matching generic converter did not yet exist):
//
//   * DIYRuZ_Flower exposes `soil_moisture` from z2m fz.soil_moisture
//     (msSoilMoisture 0x0408 measuredValue / 100) but wired no decoder for
//     it. Wired generic kFzSoilMoisture + added the 0x0408 binding.
//   * DIYRuZ_AirSense exposes `co2` from z2m fz.co2 (msCO2 0x040D
//     measuredValue * 1e6 → ppm) but wired no decoder. Wired generic
//     kFzCO2.
//   * DIYRuZ_magnet exposes `contact` from z2m fz.diyruz_contact
//     (genOnOff onOff 0x0000 != 0 — NOT IAS) but wired no decoder. Added
//     vendor kFzDiyruzContact (genOnOff onOff → bool contact).
//
// These tests pin, on real attribute-report wire shapes, that each restored
// channel decodes to the z2m-equivalent value, and regression-check the
// sibling channels (temperature/humidity/pressure/illuminance) still decode
// alongside.

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

namespace zhc::devices::diyruz {
extern const PreparedDefinition kDef_DIYRuZ_Flower;
extern const PreparedDefinition kDef_DIYRuZ_AirSense;
extern const PreparedDefinition kDef_DIYRuZ_magnet;
}  // namespace zhc::devices::diyruz

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t endpoint,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = endpoint;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

bool near(float a, float b) { return std::fabs(a - b) < 0.01f; }

// Profile-wide attribute report (S->C):
//   fc(1)=0x18 | tsn(1) | cmd(1)=0x0A | attrId(2,LE) | type(1) | value(LE).

// uint16 measured value at attr 0x0000 (soil/humidity/illuminance/co2).
DispatchResult report_u16(const PreparedDefinition& def, std::uint16_t cluster,
                          std::uint8_t endpoint, std::uint16_t value) {
    const std::array<std::uint8_t, 8> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 measuredValue
        0x21,                  // type 0x21 = uint16
        static_cast<std::uint8_t>(value & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF)};
    auto raw = make_frame(cluster, endpoint,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster);  // dispatch filters by name
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// signed-int16 measured value at attr 0x0000 (sub-zero temperature).
DispatchResult report_s16(const PreparedDefinition& def, std::uint16_t cluster,
                          std::uint8_t endpoint, std::int16_t value) {
    const std::array<std::uint8_t, 8> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 measuredValue
        0x29,                  // type 0x29 = int16
        static_cast<std::uint8_t>(static_cast<std::uint16_t>(value) & 0xFF),
        static_cast<std::uint8_t>((static_cast<std::uint16_t>(value) >> 8) & 0xFF)};
    auto raw = make_frame(cluster, endpoint,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster);  // dispatch filters by name
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// msCO2 measuredValue is a single-precision float (type 0x39) fraction;
// z2m does Math.floor(measuredValue * 1e6) → ppm.
DispatchResult report_co2_float(const PreparedDefinition& def, float frac) {
    std::array<std::uint8_t, 11> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 measuredValue
        0x39,                  // type 0x39 = single float
        0, 0, 0, 0};
    std::uint8_t bytes[4];
    static_assert(sizeof(float) == 4, "float must be 4 bytes");
    std::memcpy(bytes, &frac, 4);   // host is little-endian on the test target
    rep[6] = bytes[0];
    rep[7] = bytes[1];
    rep[8] = bytes[2];
    rep[9] = bytes[3];
    auto raw = make_frame(0x040D /* msCO2 */, 1,
                          std::span<const std::uint8_t>(rep.data(), 10));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(0x040D);  // dispatch filters by name
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// genOnOff onOff (attr 0x0000) as a bool (type 0x10).
DispatchResult report_onoff(const PreparedDefinition& def, std::uint8_t value) {
    const std::array<std::uint8_t, 7> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 onOff
        0x10,                  // type 0x10 = boolean
        value};
    auto raw = make_frame(0x0006 /* genOnOff */, 1,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(0x0006);  // dispatch filters by name
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

int main() {
    using namespace zhc::devices::diyruz;

    // ===== DIYRuZ_Flower: soil_moisture restored (was dropped) ============
    // DIYRuZ_Flower opts into multi-endpoint (endpoint_map {bme:1, ds:2}),
    // so the runtime suffixes every non-global key on EP1 with "_bme" and
    // EP2 with "_ds". soil_moisture/humidity/illuminance therefore land as
    // *_bme. The headline soil channel previously had no decoder at all.
    //
    // msSoilMoisture (0x0408) raw 4523 -> 45.23 %.  (z2m scale = 100)
    {
        auto r = report_u16(kDef_DIYRuZ_Flower, 0x0408, 1, 4523);
        assert(r.any_matched);
        const Value* s = r.merged.find("soil_moisture_bme");
        assert(s && s->type == ValueType::Float && near(s->f, 45.23f));
        // exclusivity: a soil report must not masquerade as another channel.
        assert(r.merged.find("temperature_bme") == nullptr);
        assert(r.merged.find("humidity_bme") == nullptr);
    }
    // regression: BME temperature on EP1 -> "temperature_bme" (0x0402, /100)
    {
        auto r = report_s16(kDef_DIYRuZ_Flower, 0x0402, 1, 2150);
        assert(r.any_matched);
        const Value* t = r.merged.find("temperature_bme");
        assert(t && t->type == ValueType::Float && near(t->f, 21.50f));
        assert(r.merged.find("soil_moisture_bme") == nullptr);
    }
    // regression: DS18B20 temperature on EP2 -> "temperature_ds"
    {
        auto r = report_s16(kDef_DIYRuZ_Flower, 0x0402, 2, 1875);
        assert(r.any_matched);
        const Value* t = r.merged.find("temperature_ds");
        assert(t && t->type == ValueType::Float && near(t->f, 18.75f));
    }
    // regression: humidity (0x0405, /100) still decodes -> "humidity_bme"
    {
        auto r = report_u16(kDef_DIYRuZ_Flower, 0x0405, 1, 6789);
        assert(r.any_matched);
        const Value* h = r.merged.find("humidity_bme");
        assert(h && h->type == ValueType::Float && near(h->f, 67.89f));
    }
    // regression: illuminance (m.illuminance(), 0x0400) -> "illuminance_bme"
    {
        auto r = report_u16(kDef_DIYRuZ_Flower, 0x0400, 1, 12345);
        assert(r.any_matched);
        const Value* l = r.merged.find("illuminance_bme");
        assert(l && l->type == ValueType::Uint && l->u == 12345);
    }

    // ===== DIYRuZ_AirSense: co2 restored (was dropped) ====================
    // msCO2 (0x040D) measuredValue 0.000800 -> floor(0.0008 * 1e6) = 800 ppm.
    {
        auto r = report_co2_float(kDef_DIYRuZ_AirSense, 0.0008f);
        assert(r.any_matched);
        const Value* c = r.merged.find("co2");
        assert(c && c->type == ValueType::Uint && c->u == 800);
        // exclusivity: a co2 report must not also emit temp/humidity/pressure.
        assert(r.merged.find("temperature") == nullptr);
        assert(r.merged.find("humidity") == nullptr);
        assert(r.merged.find("pressure") == nullptr);
    }
    // regression: temperature (0x0402, /100)
    {
        auto r = report_s16(kDef_DIYRuZ_AirSense, 0x0402, 1, 2375);
        assert(r.any_matched);
        const Value* t = r.merged.find("temperature");
        assert(t && t->type == ValueType::Float && near(t->f, 23.75f));
    }
    // regression: humidity (0x0405, /100)
    {
        auto r = report_u16(kDef_DIYRuZ_AirSense, 0x0405, 1, 5512);
        assert(r.any_matched);
        const Value* h = r.merged.find("humidity");
        assert(h && h->type == ValueType::Float && near(h->f, 55.12f));
    }
    // regression: pressure (0x0403). Unscaled measuredValue path -> hPa as-is.
    {
        auto r = report_u16(kDef_DIYRuZ_AirSense, 0x0403, 1, 1013);
        assert(r.any_matched);
        const Value* p = r.merged.find("pressure");
        assert(p != nullptr);
    }

    // ===== DIYRuZ_magnet: contact restored (was dropped) ==================
    // genOnOff onOff 0x01 -> contact closed (true)
    {
        auto r = report_onoff(kDef_DIYRuZ_magnet, 0x01);
        assert(r.any_matched);
        const Value* c = r.merged.find("contact");
        assert(c && c->type == ValueType::Bool && c->b == true);
    }
    // genOnOff onOff 0x00 -> contact open (false)
    {
        auto r = report_onoff(kDef_DIYRuZ_magnet, 0x00);
        assert(r.any_matched);
        const Value* c = r.merged.find("contact");
        assert(c && c->type == ValueType::Bool && c->b == false);
    }

    std::printf("diyruz parity tests passed\n");
    return 0;
}
