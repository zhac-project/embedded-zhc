// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Second-device sanity test for Task 6 — RTCGQ11LM (Aqara motion
// sensor). Exercises two shared converters at once (fz_occupancy on
// msOccupancySensing and fz_lumi_basic on genBasic) on the same
// prepared definition.

#include <cassert>
#include <cstdint>
#include <span>
#include <string_view>

#include "definitions/lumi/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefRTCGQ11LM;
}

using namespace zhc;

namespace {

// Occupancy report: cluster msOccupancySensing (0x0406), attr 0x0000
// type 0x18 (bitmap8), value 0x01 — occupied.
constexpr std::uint8_t kOccupancyFrame[] = {
    0x18, 0x01, 0x0A,           // header
    0x00, 0x00, 0x18, 0x01,      // attr 0x0000, type 0x18, value 1
};

// Illuminance report, sent with every motion (hub log 2026-10-23 read
// `18 71 0a 00 00 21 00 00`), here 0x0123 = 291 lx. z2m lumi_illuminance
// takes it as raw lux and as motion.
constexpr std::uint8_t kIlluminanceFrame[] = {
    0x18, 0x71, 0x0A,
    0x00, 0x00, 0x21, 0x23, 0x01,   // attr 0x0000, type 0x21 (u16)
};

InboundApsFrame build_frame(std::uint16_t cluster,
                             std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xB8;
    f.data         = bytes;
    return f;
}

}  // namespace

int main() {
    auto raw = build_frame(0x0406, kOccupancyFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msOccupancySensing";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(msg, {},
                                              devices::lumi::kDefRTCGQ11LM,
                                              raw, ctx);
    assert(result.any_matched);

    const Value* occ = result.merged.find("occupancy");
    assert(occ && occ->type == ValueType::Bool && occ->b == true);

    {
        auto ill = build_frame(0x0400, kIlluminanceFrame);
        DecodedMessage m{};
        assert(decode_frame(ill, {}, m));
        m.cluster = "msIlluminanceMeasurement";
        RuntimeContext c{};
        const auto r = dispatch_from_zigbee(m, {}, devices::lumi::kDefRTCGQ11LM,
                                            ill, c);
        const Value* lux = r.merged.find("illuminance");
        assert(lux && lux->type == ValueType::Uint && lux->u == 291);
        const Value* o = r.merged.find("occupancy");
        assert(o && o->type == ValueType::Bool && o->b);
    }

    const auto& def = devices::lumi::kDefRTCGQ11LM;
    bool exposes_lux = false;
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        exposes_lux |= std::string_view(def.exposes[i].name) == "illuminance";
    assert(exposes_lux);
    // z2m binds nothing here; the sensor sleeps through every bind request.
    assert(def.bindings_count == 0);
    return 0;
}
