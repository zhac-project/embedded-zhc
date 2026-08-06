// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Regression test for the Tuya local_temperature_calibration divisor class.
//
// Seven thermostat definitions carried divisor 1 on this datapoint while z2m
// has always applied /10 (via valueConverter.localTempCalibration3, whose body
// is `(v > 0x7fffffff ? v - 0x100000000 : v) / 10`). Every reported calibration
// was therefore 10x too large. z2m v26.92.0 deleted the localTempCalibrationN
// helpers and the triples now read plainly as `divideBy10`, which settled it.
//
// The two things worth pinning are the two that were actually wrong or
// fragile: the divisor, and the sign. A -3.0 C offset arrives as the 32-bit
// two's complement 0xFFFFFFE2 (-30), so a decoder that forgets either the
// sign extension or the divisor produces a wildly different number and this
// test fails loudly.
//
// BOT-R15W (_TZE284_agcxaw3f, DP19) stands in for the class; all seven
// graduated definitions share the identical entry shape.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDef__TZE284_agcxaw3f;
}

using namespace zhc;

namespace {

bool approx(float a, float b, float eps) { return std::fabs(a - b) <= eps; }

DecodedMessage make_msg() {
    DecodedMessage msg{};
    msg.family       = FrameFamily::TuyaDp;
    msg.type         = MessageType::Command;
    msg.cluster      = "manuSpecificTuya";
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    return msg;
}

InboundApsFrame make_raw() {
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    return raw;
}

const Value* decode_dp19(std::span<const std::uint8_t> payload,
                          DispatchResult& keep_alive) {
    const TuyaDpRecord recs[] = { { 19, 0x02, payload } };
    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    keep_alive = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1),
        devices::tuya::kDef__TZE284_agcxaw3f, raw, ctx);
    assert(keep_alive.any_matched);
    return keep_alive.merged.find("local_temperature_calibration");
}

}  // namespace

// +2.5 C -> raw 25 -> 0x00000019.
static void test_positive_calibration_divided_by_ten() {
    const std::uint8_t kRaw25[] = { 0x00, 0x00, 0x00, 0x19 };
    DispatchResult r{};
    const Value* v = decode_dp19(kRaw25, r);
    assert(v);
    assert(v->type == ValueType::Float);
    assert(approx(v->f, 2.5f, 0.001f));
}

// -3.0 C -> raw -30 -> two's complement 0xFFFFFFE2.
// Catches BOTH failure modes: divisor 1 would give -30.0, and a missing sign
// extension would give 4294967266.
static void test_negative_calibration_sign_and_divisor() {
    const std::uint8_t kRawNeg30[] = { 0xFF, 0xFF, 0xFF, 0xE2 };
    DispatchResult r{};
    const Value* v = decode_dp19(kRawNeg30, r);
    assert(v);
    assert(v->type == ValueType::Float);
    assert(approx(v->f, -3.0f, 0.001f));
}

// Zero must stay zero under both transforms.
static void test_zero_calibration() {
    const std::uint8_t kRaw0[] = { 0x00, 0x00, 0x00, 0x00 };
    DispatchResult r{};
    const Value* v = decode_dp19(kRaw0, r);
    assert(v);
    assert(approx(v->f, 0.0f, 0.001f));
}

int main() {
    test_positive_calibration_divided_by_ten();
    test_negative_calibration_sign_and_divisor();
    test_zero_calibration();
    return 0;
}
