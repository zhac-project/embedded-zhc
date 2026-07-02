// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Decode regression for the Namron Simplify 6-button remote (4512793 / 4512794).
// Button presses arrive as a raw frame on the private cluster namronPrivateE004
// (0xE004). The last two payload bytes are (button, action): button 1..6 maps to
// a 3-column up/down layout (col = (btn-1)/2, sub = odd->up / even->down) and
// action 0=press, 1=release, 2=hold. The converter publishes
// `action` = "button_<1-3>_<up|down>_<press|release|hold>".
//
// z2m-source: namron.ts #4512793 + fzLocal.namronSimplifyRemote.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::namron {
extern const PreparedDefinition kDef_D4512793;
}  // namespace zhc::devices::namron

using namespace zhc;

namespace {

DecodedMessage make_raw_msg() {
    DecodedMessage msg{};
    msg.family       = FrameFamily::Zcl;
    msg.type         = MessageType::Command;
    msg.direction    = Direction::ServerToClient;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    return msg;
}

InboundApsFrame make_raw_frame() {
    InboundApsFrame raw{};
    raw.cluster_id   = 0xE004;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    return raw;
}

const char* decode_action(std::span<const std::uint8_t> body) {
    auto msg = make_raw_msg();
    msg.raw_body = body;
    auto raw = make_raw_frame();
    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(), devices::namron::kDef_D4512793, raw, ctx);
    const Value* a = r.merged.find("action");
    if (!a || a->type != ValueType::StringRef) return nullptr;
    return a->str;
}

void expect(std::span<const std::uint8_t> body, const char* want) {
    const char* got = decode_action(body);
    assert(got && std::strcmp(got, want) == 0);
}

void expect_none(std::span<const std::uint8_t> body) {
    assert(decode_action(body) == nullptr);
}

}  // namespace

int main() {
    // Valid frames — a leading header prefix is ignored (only the last two
    // bytes matter). Covers all three columns + up/down + each verb.
    const std::uint8_t f_1up_press[]   = {0xAA, 0xBB, 1, 0};
    expect(std::span<const std::uint8_t>(f_1up_press, 4), "button_1_up_press");
    const std::uint8_t f_1dn_hold[]    = {0xAA, 0xBB, 2, 2};
    expect(std::span<const std::uint8_t>(f_1dn_hold, 4), "button_1_down_hold");
    const std::uint8_t f_3up_release[] = {0x00, 5, 1};
    expect(std::span<const std::uint8_t>(f_3up_release, 3), "button_3_up_release");
    const std::uint8_t f_3dn_press[]   = {6, 0};
    expect(std::span<const std::uint8_t>(f_3dn_press, 2), "button_3_down_press");
    const std::uint8_t f_2up_press[]   = {3, 0};
    expect(std::span<const std::uint8_t>(f_2up_press, 2), "button_2_up_press");

    // Invalid — out-of-range button, unknown action byte, too-short frame:
    // no `action` published (frame left unclaimed).
    const std::uint8_t bad_btn[]   = {7, 0};
    expect_none(std::span<const std::uint8_t>(bad_btn, 2));
    const std::uint8_t bad_action[] = {1, 3};
    expect_none(std::span<const std::uint8_t>(bad_action, 2));
    const std::uint8_t too_short[] = {1};
    expect_none(std::span<const std::uint8_t>(too_short, 1));

    return 0;
}
