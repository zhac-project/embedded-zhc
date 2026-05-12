// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// T2.4: tuyaOnOff + tuyaLight factories exercised via two devices.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTZE200_0nmiizme;
extern const PreparedDefinition kDefTZE200_cjbofhxw;
}

using namespace zhc;

namespace {

InboundApsFrame make_raw() {
    InboundApsFrame r{};
    r.cluster_id = 0xEF00; r.src_endpoint = 1; r.dst_endpoint = 1;
    r.linkquality = 0xC8;
    return r;
}
DecodedMessage make_msg() {
    DecodedMessage m{};
    m.family = FrameFamily::TuyaDp;
    m.type = MessageType::Command;
    m.cluster = "manuSpecificTuya";
    m.direction = Direction::ServerToClient;
    m.command_id = 0x02;
    m.src_endpoint = 1; m.dst_endpoint = 1;
    return m;
}
}  // namespace

static void test_tuyaonoff_0nmiizme() {
    const std::uint8_t b[] = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 1, 0x01, std::span<const std::uint8_t>(b, 1) },
    };
    auto raw = make_raw();
    auto msg = make_msg();
    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1),
        devices::tuya::kDefTZE200_0nmiizme, raw, ctx);
    assert(r.any_matched);
    const Value* s = r.merged.find("state");
    assert(s && s->type == ValueType::Bool && s->b == true);
}

static void test_tuyalight_cjbofhxw() {
    const std::uint8_t on[]    = { 0x01 };
    const std::uint8_t br[]    = { 0x00, 0x00, 0x00, 0xC8 };    // 200
    const TuyaDpRecord recs[] = {
        { 1, 0x01, std::span<const std::uint8_t>(on, 1) },
        { 2, 0x02, std::span<const std::uint8_t>(br, 4) },
    };
    auto raw = make_raw();
    auto msg = make_msg();
    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 2),
        devices::tuya::kDefTZE200_cjbofhxw, raw, ctx);
    assert(r.any_matched);
    const Value* s = r.merged.find("state");
    assert(s && s->b == true);
    const Value* b = r.merged.find("brightness");
    assert(b && b->type == ValueType::Int && b->i == 200);
}

int main() {
    test_tuyaonoff_0nmiizme();
    test_tuyalight_cjbofhxw();
    return 0;
}
