// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the MindY Leleka air-quality monitor.
// z2m-source: mindy.ts #Leleka.
//
// Bug fixed: the def declares a `co2` expose and binds the standard
// msCO2 cluster (0x040D), but its from-zigbee list omitted the decoder
// — so every CO2 report was silently dropped. The generic kFzCO2
// (msCO2 attr 0x0000 single-precision float, z2m fz.co2 =
// floor(measuredValue * 1e6) → ppm) already existed; the def's header
// comment claiming "Tier-1 has no kFzCO2" was stale. Fix: graduated the
// def generated/ -> parent and wired &generic::kFzCO2.
//
// This test pins, on the real msCO2 attribute-report wire shape, that
// CO2 decodes to the z2m-equivalent ppm, and regression-checks that the
// sibling standard-cluster channels (temperature / humidity / pressure /
// illuminance) still decode alongside it. The Leleka has no
// endpoint_map, so the runtime keys are bare.
//
// The manuf-specific config knobs (read_interval, night_*, co2_*, lux_*,
// offset_*, temperature_sensor) and the per-device last_boot / wifi
// surfaces have no generic decoder and are intentionally not asserted
// (deferred infra).

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

namespace zhc::devices::mindy {
extern const PreparedDefinition kDef_Leleka;
}  // namespace zhc::devices::mindy

using namespace zhc;

namespace {

bool near(float a, float b) { return std::fabs(a - b) < 0.01f; }

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

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// Profile-wide attribute report (S->C):
//   fc(1)=0x18 | tsn(1) | cmd(1)=0x0A | attrId(2,LE) | type(1) | value(LE).
DispatchResult dispatch_report(std::uint16_t cluster,
                               std::span<const std::uint8_t> rep) {
    auto raw = make_frame(cluster, rep);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster);  // dispatch filters by name
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, devices::mindy::kDef_Leleka, raw, ctx);
}

// uint16 measured value at attr 0x0000 (humidity / illuminance).
DispatchResult report_u16(std::uint16_t cluster, std::uint16_t value) {
    const std::array<std::uint8_t, 8> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 measuredValue
        0x21,                  // type 0x21 = uint16
        static_cast<std::uint8_t>(value & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF)};
    return dispatch_report(cluster, std::span<const std::uint8_t>(rep.data(), rep.size()));
}

// signed-int16 measured value at attr 0x0000 (temperature / pressure).
DispatchResult report_s16(std::uint16_t cluster, std::int16_t value) {
    const std::array<std::uint8_t, 8> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 measuredValue
        0x29,                  // type 0x29 = int16
        static_cast<std::uint8_t>(static_cast<std::uint16_t>(value) & 0xFF),
        static_cast<std::uint8_t>((static_cast<std::uint16_t>(value) >> 8) & 0xFF)};
    return dispatch_report(cluster, std::span<const std::uint8_t>(rep.data(), rep.size()));
}

// msCO2 measuredValue is a single-precision float (type 0x39) fraction;
// z2m does Math.floor(measuredValue * 1e6) → ppm.
DispatchResult report_co2_float(float frac) {
    std::array<std::uint8_t, 10> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 measuredValue
        0x39,                  // type 0x39 = single float
        0, 0, 0, 0};
    static_assert(sizeof(float) == 4, "float must be 4 bytes");
    std::uint8_t bytes[4];
    std::memcpy(bytes, &frac, 4);   // host is little-endian on the test target
    rep[6] = bytes[0];
    rep[7] = bytes[1];
    rep[8] = bytes[2];
    rep[9] = bytes[3];
    return dispatch_report(0x040D /* msCO2 */,
                           std::span<const std::uint8_t>(rep.data(), rep.size()));
}

}  // namespace

int main() {
    using namespace zhc::devices::mindy;
    const auto& def = kDef_Leleka;

    // Exposes carry the headline sensor channels.
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "pressure"));
    assert(def_exposes(def, "co2"));
    assert(def_exposes(def, "illuminance"));

    // ── Parity fix: co2 restored (was dropped). msCO2 (0x040D)
    //    measuredValue 0.000800 -> floor(0.0008 * 1e6) = 800 ppm. ──
    {
        auto r = report_co2_float(0.0008f);
        assert(r.any_matched);
        const Value* c = r.merged.find("co2");
        assert(c && c->type == ValueType::Uint && c->u == 800u);
        // exclusivity: a co2 report must not masquerade as another channel.
        assert(r.merged.find("temperature") == nullptr);
        assert(r.merged.find("humidity") == nullptr);
        assert(r.merged.find("pressure") == nullptr);
        assert(r.merged.find("illuminance") == nullptr);
    }

    // ── Regression: temperature (msTemperatureMeasurement 0x0402, s16 /100). ──
    {
        auto r = report_s16(0x0402, 2375);
        assert(r.any_matched);
        const Value* t = r.merged.find("temperature");
        assert(t && t->type == ValueType::Float && near(t->f, 23.75f));
    }

    // ── Regression: humidity (msRelativeHumidity 0x0405, u16 /100). ──
    {
        auto r = report_u16(0x0405, 5512);
        assert(r.any_matched);
        const Value* h = r.merged.find("humidity");
        assert(h && h->type == ValueType::Float && near(h->f, 55.12f));
    }

    // ── Regression: pressure (msPressureMeasurement 0x0403, s16 hPa direct). ──
    {
        auto r = report_s16(0x0403, 1013);
        assert(r.any_matched);
        const Value* p = r.merged.find("pressure");
        assert(p != nullptr);
    }

    // ── Regression: illuminance (msIlluminanceMeasurement 0x0400, raw lx). ──
    {
        auto r = report_u16(0x0400, 12345);
        assert(r.any_matched);
        const Value* l = r.merged.find("illuminance");
        assert(l && l->type == ValueType::Uint && l->u == 12345u);
    }

    std::printf("mindy parity tests passed\n");
    return 0;
}
