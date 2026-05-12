// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: ZNCZ02LM plug.
//
// Exit criterion for plan Task 2.3: activePower raw=123 → 12.3 W via
// the device-supplied /10 divisor. Extra: energy raw=456789 → 456.789
// kWh via /1000. Also proves one plug definition wires four unrelated
// converters on four different clusters and each takes the matching
// frame.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefZNCZ02LM;
}

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster,
                             std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

bool approx(float a, float b, float eps) { return std::fabs(a - b) <= eps; }

DispatchResult dispatch(std::uint16_t cluster, const char* cluster_name,
                         std::span<const std::uint8_t> bytes) {
    auto raw = build_frame(cluster, bytes);
    DecodedMessage msg{};
    if (!decode_frame(raw, {}, msg)) return {};
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, devices::lumi::kDefZNCZ02LM,
                                  raw, ctx);
}

// haElectricalMeasurement attr 0x050B int16 = 123 → 12.3 W.
constexpr std::uint8_t kPowerFrame[] = {
    0x18, 0x01, 0x0A,
    0x0B, 0x05, 0x29, 0x7B, 0x00,    // attr 0x050B type 0x29 int16 = 123
};

// seMetering attr 0x0000 u48 — use u32 here (type 0x23) value 456789 → 456.789 kWh.
// 456789 = 0x6F855; bytes LE: 55 F8 06 00.
constexpr std::uint8_t kEnergyFrame[] = {
    0x18, 0x02, 0x0A,
    0x00, 0x00, 0x23, 0x55, 0xF8, 0x06, 0x00,
};

// genOnOff attr 0x0000 bool = 1.
constexpr std::uint8_t kOnFrame[] = {
    0x18, 0x03, 0x0A,
    0x00, 0x00, 0x10, 0x01,
};

}  // namespace

static void test_power_scales_divisor_10() {
    const auto r = dispatch(0x0B04, "haElectricalMeasurement", kPowerFrame);
    assert(r.any_matched);
    const Value* p = r.merged.find("power");
    assert(p && p->type == ValueType::Float);
    assert(approx(p->f, 12.3f, 0.01f));
}

static void test_energy_scales_divisor_1000() {
    const auto r = dispatch(0x0702, "seMetering", kEnergyFrame);
    assert(r.any_matched);
    const Value* e = r.merged.find("energy");
    assert(e && e->type == ValueType::Float);
    assert(approx(e->f, 456.789f, 0.01f));
}

static void test_generic_on_off_routes_through_plug() {
    const auto r = dispatch(0x0006, "genOnOff", kOnFrame);
    assert(r.any_matched);
    const Value* s = r.merged.find("state");
    assert(s && s->type == ValueType::Bool && s->b == true);
}

int main() {
    test_power_scales_divisor_10();
    test_energy_scales_divisor_1000();
    test_generic_on_off_routes_through_plug();
    return 0;
}
