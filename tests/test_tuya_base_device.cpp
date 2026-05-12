// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// T1.2: prove a real device using tuyaBase + a DP map decodes cleanly.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTZE200_bjawzodf;
}

using namespace zhc;

static bool approx(float a, float b, float eps) {
    return std::fabs(a - b) <= eps;
}

static void test_tze200_bjawzodf_dp_report() {
    // Two DPs: state on (dp1) + current 2000 mA (dp18).
    const std::uint8_t kBoolOn[]   = { 0x01 };
    const std::uint8_t kNum2000[]  = { 0x00, 0x00, 0x07, 0xD0 };

    const TuyaDpRecord recs[] = {
        { 1,  0x01, std::span<const std::uint8_t>(kBoolOn,  1) },
        { 18, 0x02, std::span<const std::uint8_t>(kNum2000, 4) },
    };

    // Build a minimal DecodedMessage + InboundApsFrame.
    DecodedMessage msg{};
    msg.family  = FrameFamily::TuyaDp;
    msg.type    = MessageType::Command;
    msg.cluster = "manuSpecificTuya";
    msg.direction = Direction::ServerToClient;
    msg.command_id = 0x02;
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
        std::span<const TuyaDpRecord>(recs, 2),
        devices::tuya::kDefTZE200_bjawzodf,
        raw, ctx);
    assert(result.any_matched);

    const Value* s = result.merged.find("state");
    assert(s && s->type == ValueType::Bool && s->b == true);

    const Value* cur = result.merged.find("current");
    assert(cur && cur->type == ValueType::Float);
    assert(approx(cur->f, 2.0f, 0.001f));
}

static void test_configure_hook_identity() {
    // tuyaBase-style ConfigureFn should be wired on the device.
    const auto& def = devices::tuya::kDefTZE200_bjawzodf;
    assert(def.configure != nullptr);
}

int main() {
    test_tze200_bjawzodf_dp_report();
    test_configure_hook_identity();
    return 0;
}
