// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Parity tests for the z2m tuya_air_quality (Smart Air Box / Housekeeper)
// family — devices whose DPs the legacy converter remaps per manufacturer.
// z2m-source: lib/legacy.ts tuya_air_quality.
//   _TZE200/_TZE204_dwcarsat: DP22 -> co2, DP2 -> pm25.
//   _TZE200_mja3fuja:         DP22 -> co2, DP2 -> formaldehyde.
//   default branch (3ejwxpmu/ogkdpgy2/blfcpsxz/c2fmom5z/yvx5lh6k):
//                             DP2 -> co2, DP22 -> formaldehyde.
// Pins those mappings at the dispatch boundary.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS0601_air_house_keeper;
extern const PreparedDefinition kDefTZE200_mja3fuja;
extern const PreparedDefinition kDefTS0601_air_quality;
}

using namespace zhc;

static bool approx(float a, float b, float eps) {
    return std::fabs(a - b) <= eps;
}

static void test_dwcarsat_air_quality_dps() {
    // BE u32 wire payloads (Tuya DP type 0x02 = Numeric).
    const std::uint8_t kTemp251[] = { 0x00, 0x00, 0x00, 0xFB }; // 251 -> 25.1 C
    const std::uint8_t kHum426[]  = { 0x00, 0x00, 0x01, 0xAA }; // 426 -> 42.6 %
    const std::uint8_t kVoc100[]  = { 0x00, 0x00, 0x00, 0x64 }; // 100 ppb
    const std::uint8_t kCo2800[]  = { 0x00, 0x00, 0x03, 0x20 }; // 800 ppm
    const std::uint8_t kPm25_35[] = { 0x00, 0x00, 0x00, 0x23 }; // 35 ug/m3

    const TuyaDpRecord recs[] = {
        { 18, 0x02, std::span<const std::uint8_t>(kTemp251, 4) },
        { 19, 0x02, std::span<const std::uint8_t>(kHum426,  4) },
        { 21, 0x02, std::span<const std::uint8_t>(kVoc100,  4) },
        { 22, 0x02, std::span<const std::uint8_t>(kCo2800,  4) },
        {  2, 0x02, std::span<const std::uint8_t>(kPm25_35, 4) },
    };

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
    const auto result = dispatch_from_zigbee(
        msg,
        std::span<const TuyaDpRecord>(recs, 5),
        devices::tuya::kDefTS0601_air_house_keeper,
        raw, ctx);
    assert(result.any_matched);

    // divisor > 1 -> Float
    const Value* t = result.merged.find("temperature");
    assert(t && t->type == ValueType::Float && approx(t->f, 25.1f, 0.001f));

    const Value* h = result.merged.find("humidity");
    assert(h && h->type == ValueType::Float && approx(h->f, 42.6f, 0.001f));

    // divisor == 1 -> Int (raw pass-through)
    const Value* voc = result.merged.find("voc");
    assert(voc && voc->type == ValueType::Int && voc->i == 100);

    const Value* co2 = result.merged.find("co2");   // DP22 remap
    assert(co2 && co2->type == ValueType::Int && co2->i == 800);

    const Value* pm25 = result.merged.find("pm25");  // DP2 remap
    assert(pm25 && pm25->type == ValueType::Int && pm25->i == 35);

    // No spurious formaldehyde key for this variant.
    assert(result.merged.find("formaldehyde") == nullptr);
}

static void test_dwcarsat_definition_shape() {
    const auto& def = devices::tuya::kDefTS0601_air_house_keeper;
    assert(def.configure != nullptr);          // tuyaBase configure wired
    assert(def.manufacturer_names_count == 2);  // _TZE200 + _TZE204 dwcarsat
}

static void test_mja3fuja_air_quality_dps() {
    // Real frames captured from a _TZE200_mja3fuja (BE u32, Tuya DP type 0x02).
    const std::uint8_t kTemp262[] = { 0x00, 0x00, 0x01, 0x06 }; // 262 -> 26.2 C
    const std::uint8_t kHum460[]  = { 0x00, 0x00, 0x01, 0xCC }; // 460 -> 46.0 %
    const std::uint8_t kVoc1[]    = { 0x00, 0x00, 0x00, 0x01 }; // 1
    const std::uint8_t kCo2_358[] = { 0x00, 0x00, 0x01, 0x66 }; // 358 ppm
    const std::uint8_t kCh2o2[]   = { 0x00, 0x00, 0x00, 0x02 }; // 2

    const TuyaDpRecord recs[] = {
        { 18, 0x02, std::span<const std::uint8_t>(kTemp262, 4) },
        { 19, 0x02, std::span<const std::uint8_t>(kHum460,  4) },
        { 21, 0x02, std::span<const std::uint8_t>(kVoc1,    4) },
        { 22, 0x02, std::span<const std::uint8_t>(kCo2_358, 4) },
        {  2, 0x02, std::span<const std::uint8_t>(kCh2o2,   4) },
    };

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
    const auto result = dispatch_from_zigbee(
        msg,
        std::span<const TuyaDpRecord>(recs, 5),
        devices::tuya::kDefTZE200_mja3fuja,
        raw, ctx);
    assert(result.any_matched);

    const Value* t = result.merged.find("temperature");
    assert(t && t->type == ValueType::Float && approx(t->f, 26.2f, 0.001f));

    const Value* h = result.merged.find("humidity");
    assert(h && h->type == ValueType::Float && approx(h->f, 46.0f, 0.001f));

    const Value* voc = result.merged.find("voc");
    assert(voc && voc->type == ValueType::Int && voc->i == 1);

    const Value* co2 = result.merged.find("co2");           // DP22 remap
    assert(co2 && co2->type == ValueType::Int && co2->i == 358);

    const Value* ch2o = result.merged.find("formaldehyde");  // DP2 remap
    assert(ch2o && ch2o->type == ValueType::Int && ch2o->i == 2);

    // This variant carries no pm25 (that is the dwcarsat DP2 mapping).
    assert(result.merged.find("pm25") == nullptr);
}

static void test_air_quality_default_dps() {
    // Default tuya_air_quality branch (e.g. _TZE200_ogkdpgy2 / c2fmom5z):
    // DP2 -> co2, DP22 -> formaldehyde (the opposite of the dwcarsat remap).
    const std::uint8_t kTemp265[] = { 0x00, 0x00, 0x01, 0x09 }; // 265 -> 26.5 C
    const std::uint8_t kHum500[]  = { 0x00, 0x00, 0x01, 0xF4 }; // 500 -> 50.0 %
    const std::uint8_t kVoc5[]    = { 0x00, 0x00, 0x00, 0x05 }; // 5
    const std::uint8_t kCo2_800[] = { 0x00, 0x00, 0x03, 0x20 }; // 800 ppm
    const std::uint8_t kCh2o10[]  = { 0x00, 0x00, 0x00, 0x0A }; // 10

    const TuyaDpRecord recs[] = {
        { 18, 0x02, std::span<const std::uint8_t>(kTemp265, 4) },
        { 19, 0x02, std::span<const std::uint8_t>(kHum500,  4) },
        { 21, 0x02, std::span<const std::uint8_t>(kVoc5,    4) },
        {  2, 0x02, std::span<const std::uint8_t>(kCo2_800, 4) },
        { 22, 0x02, std::span<const std::uint8_t>(kCh2o10,  4) },
    };

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
    const auto result = dispatch_from_zigbee(
        msg,
        std::span<const TuyaDpRecord>(recs, 5),
        devices::tuya::kDefTS0601_air_quality,
        raw, ctx);
    assert(result.any_matched);

    const Value* t = result.merged.find("temperature");
    assert(t && t->type == ValueType::Float && approx(t->f, 26.5f, 0.001f));

    const Value* h = result.merged.find("humidity");
    assert(h && h->type == ValueType::Float && approx(h->f, 50.0f, 0.001f));

    const Value* voc = result.merged.find("voc");
    assert(voc && voc->type == ValueType::Int && voc->i == 5);

    const Value* co2 = result.merged.find("co2");           // DP2 (default)
    assert(co2 && co2->type == ValueType::Int && co2->i == 800);

    const Value* ch2o = result.merged.find("formaldehyde");  // DP22 (default)
    assert(ch2o && ch2o->type == ValueType::Int && ch2o->i == 10);

    assert(result.merged.find("pm25") == nullptr);
    // one shared def claims all 9 default-branch fingerprints
    assert(devices::tuya::kDefTS0601_air_quality.manufacturer_names_count == 9);
}

int main() {
    test_dwcarsat_air_quality_dps();
    test_dwcarsat_definition_shape();
    test_mja3fuja_air_quality_dps();
    test_air_quality_default_dps();
    return 0;
}
