// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Sonoff SNZB-02 temperature & humidity sensor.
// z2m-source: sonoff.ts #SNZB-02 —
//     exposes:    e.battery(), e.temperature(), e.humidity(),
//                 e.battery_voltage()
//     fromZigbee: [fz.SNZB02_temperature, fz.humidity, fz.battery]
//
// The auto-generated def lowered only `ewelinkBattery()` and dropped
// temperature + humidity entirely; the def was graduated to a Tier-2
// parent override (definitions/sonoff/SNZB_02.cpp) that adds the
// generic kFzTemperature / kFzHumidity converters and the matching
// msTemperatureMeasurement / msRelativeHumidity bindings. These tests
// pin that the override now decodes temp + humidity from real wire
// shapes and that its expose list / bindings match z2m.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::sonoff {
extern const PreparedDefinition kDef_SNZB_02;
}

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster,
                            std::uint8_t  src_endpoint,
                            std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_endpoint;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

bool nearly(float a, float b, float eps = 0.005f) {
    return std::fabs(a - b) < eps;
}

// msTemperatureMeasurement (0x0402) attr 0x0000 measuredValue, int16.
//   0x093A = 2362 raw -> 23.62 °C.
constexpr std::uint8_t kTempReport_2362[] = {
    0x08,              // FC: SRV->CLI, disable default rsp
    0x42,              // tsn
    0x0A,              // Report Attributes
    0x00, 0x00,        // attr id 0x0000
    0x29,              // type int16
    0x3A, 0x09,        // LE value 0x093A = 2362
};

// msRelativeHumidity (0x0405) attr 0x0000 measuredValue, u16.
//   0x1450 = 5200 raw -> 52.00 %.
constexpr std::uint8_t kHumReport_5200[] = {
    0x08,
    0x43,
    0x0A,
    0x00, 0x00,
    0x21,              // type u16
    0x50, 0x14,        // LE value 0x1450 = 5200
};

// Negative temperature path — int16 0xFF38 = -200 raw -> -2.00 °C.
constexpr std::uint8_t kTempReport_neg200[] = {
    0x08, 0x44, 0x0A,
    0x00, 0x00,
    0x29,
    0x38, 0xFF,        // LE value 0xFF38 = -200
};

}  // namespace

// ─────────────────────────────────────────────────────────────────────
// 1. Definition shape — exposes + bindings match z2m.
// ─────────────────────────────────────────────────────────────────────

static void test_snzb02_exposes_temperature_and_humidity() {
    const auto& def = devices::sonoff::kDef_SNZB_02;

    bool has_temp = false, has_hum = false, has_batt = false, has_volt = false;
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        const char* n = def.exposes[i].name;
        if (!n) continue;
        if (std::strcmp(n, "temperature") == 0) has_temp = true;
        if (std::strcmp(n, "humidity") == 0)    has_hum  = true;
        if (std::strcmp(n, "battery") == 0)      has_batt = true;
        if (std::strcmp(n, "voltage") == 0)      has_volt = true;
    }
    assert(has_temp);   // ← the gap this override fixes
    assert(has_hum);    // ← the gap this override fixes
    assert(has_batt);
    assert(has_volt);
}

static void test_snzb02_from_zigbee_and_bindings_cover_sensors() {
    const auto& def = devices::sonoff::kDef_SNZB_02;

    bool fz_temp = false, fz_hum = false, fz_batt = false;
    for (std::uint8_t i = 0; i < def.from_zigbee_count; ++i) {
        const FzConverter* fz = def.from_zigbee[i];
        if (!fz || !fz->cluster) continue;
        if (std::strcmp(fz->cluster, "msTemperatureMeasurement") == 0) fz_temp = true;
        if (std::strcmp(fz->cluster, "msRelativeHumidity") == 0)       fz_hum  = true;
        if (std::strcmp(fz->cluster, "genPowerCfg") == 0)              fz_batt = true;
    }
    assert(fz_temp && fz_hum && fz_batt);

    bool bind_temp = false, bind_hum = false, bind_batt = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i) {
        switch (def.bindings[i].cluster_id) {
            case 0x0402: bind_temp = true; break;
            case 0x0405: bind_hum  = true; break;
            case 0x0001: bind_batt = true; break;
            default: break;
        }
    }
    assert(bind_temp && bind_hum && bind_batt);
}

// ─────────────────────────────────────────────────────────────────────
// 2. Decode — dispatch real ZCL reports through the def.
// ─────────────────────────────────────────────────────────────────────

static void test_snzb02_decodes_temperature() {
    auto raw = build_frame(/*cluster=*/0x0402, /*src_ep=*/1, kTempReport_2362);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msTemperatureMeasurement";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{}, devices::sonoff::kDef_SNZB_02, raw, ctx);

    assert(result.any_matched);
    const Value* t = result.merged.find("temperature");
    assert(t && t->type == ValueType::Float);
    assert(nearly(t->f, 23.62f));
}

static void test_snzb02_decodes_negative_temperature() {
    auto raw = build_frame(0x0402, 1, kTempReport_neg200);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msTemperatureMeasurement";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{}, devices::sonoff::kDef_SNZB_02, raw, ctx);

    assert(result.any_matched);
    const Value* t = result.merged.find("temperature");
    assert(t && t->type == ValueType::Float);
    assert(nearly(t->f, -2.00f));
}

static void test_snzb02_decodes_humidity() {
    auto raw = build_frame(0x0405, 1, kHumReport_5200);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msRelativeHumidity";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{}, devices::sonoff::kDef_SNZB_02, raw, ctx);

    assert(result.any_matched);
    const Value* h = result.merged.find("humidity");
    assert(h && h->type == ValueType::Float);
    assert(nearly(h->f, 52.00f));
}

int main() {
    test_snzb02_exposes_temperature_and_humidity();
    test_snzb02_from_zigbee_and_bindings_cover_sensors();
    test_snzb02_decodes_temperature();
    test_snzb02_decodes_negative_temperature();
    test_snzb02_decodes_humidity();
    return 0;
}
