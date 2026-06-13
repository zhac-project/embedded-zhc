// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Write-path (tz) encode tests for the Ysrsai YSR-MINI-01_rgbcct
// tuyaLight control parity: color_power_on_behavior (0x0300/0xf9),
// do_not_disturb (0x0300/0xfa), and effect (genIdentify 0x0003/0x40 for
// the 6 standard effects + lightingColorCtrl 0x0300/0x44 colorLoopSet for
// colorloop / stop_colorloop). Exercises the REAL kDef_ converter wiring,
// including the two-converters-share-"effect" dispatcher fall-through.
//
// Frame layout (per the _generic write_header convention):
//   [0]=FC=0x11 (cluster-specific, default-response suppressed)
//   [1]=TSN=0x00 (patched by the platform adapter)
//   [2]=command id
//   [3..]=payload body

#include <cassert>
#include <cstdint>
#include <cstring>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace zhc::devices::ysrsai {
extern const PreparedDefinition kDef_YSR_MINI_01_rgbcct;
}

namespace {

Value str_val(const char* s) {
    Value v{};
    v.type = ValueType::StringRef;
    v.str  = s;
    return v;
}

// ── color_power_on_behavior → 0x0300 cmd 0xf9, mode = v*256 + 10 zeros ──
void test_color_power_on_initial() {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    auto r = dispatch_to_zigbee(devices::ysrsai::kDef_YSR_MINI_01_rgbcct,
                                "color_power_on_behavior", str_val("initial"),
                                ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0x0300);
    assert(r.command_id == 0xf9);
    // FC, TSN, cmd, mode(LE)=0*256=0x0000, then 10 zero bytes = 15 total.
    const std::uint8_t want[] = {
        0x11, 0x00, 0xf9, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    assert(r.frame_size == sizeof(want));
    assert(std::memcmp(frame, want, r.frame_size) == 0);
}

void test_color_power_on_previous() {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    auto r = dispatch_to_zigbee(devices::ysrsai::kDef_YSR_MINI_01_rgbcct,
                                "color_power_on_behavior", str_val("previous"),
                                ctx, frame);
    assert(r.ok);
    // mode = 1*256 = 0x0100 → LE bytes 0x00, 0x01.
    assert(r.frame_size == 15);
    assert(frame[3] == 0x00 && frame[4] == 0x01);
    for (std::size_t i = 5; i < 15; ++i) assert(frame[i] == 0x00);
}

void test_color_power_on_customized_uint_index() {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Uint; v.u = 2;     // "customized"
    auto r = dispatch_to_zigbee(devices::ysrsai::kDef_YSR_MINI_01_rgbcct,
                                "color_power_on_behavior", v, ctx, frame);
    assert(r.ok);
    // mode = 2*256 = 0x0200 → LE 0x00, 0x02.
    assert(frame[3] == 0x00 && frame[4] == 0x02);
}

void test_color_power_on_bad_label_rejected() {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    auto r = dispatch_to_zigbee(devices::ysrsai::kDef_YSR_MINI_01_rgbcct,
                                "color_power_on_behavior", str_val("bogus"),
                                ctx, frame);
    assert(!r.ok);
}

// ── do_not_disturb → 0x0300 cmd 0xfa, 1-byte enable ──────────────────
void test_do_not_disturb_on() {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Bool; v.b = true;
    auto r = dispatch_to_zigbee(devices::ysrsai::kDef_YSR_MINI_01_rgbcct,
                                "do_not_disturb", v, ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0x0300);
    assert(r.command_id == 0xfa);
    const std::uint8_t want[] = { 0x11, 0x00, 0xfa, 0x01 };
    assert(r.frame_size == sizeof(want));
    assert(std::memcmp(frame, want, r.frame_size) == 0);
}

void test_do_not_disturb_off() {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Bool; v.b = false;
    auto r = dispatch_to_zigbee(devices::ysrsai::kDef_YSR_MINI_01_rgbcct,
                                "do_not_disturb", v, ctx, frame);
    assert(r.ok);
    assert(r.frame_size == 4);
    assert(frame[3] == 0x00);
}

// ── effect (genIdentify 0x0003 cmd 0x40 triggerEffect) ───────────────
void test_effect_blink() {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    auto r = dispatch_to_zigbee(devices::ysrsai::kDef_YSR_MINI_01_rgbcct,
                                "effect", str_val("blink"), ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0x0003);     // genIdentify
    assert(r.command_id == 0x40);       // triggerEffect
    // effect_id=0x00 (blink), variant=0x00.
    const std::uint8_t want[] = { 0x11, 0x00, 0x40, 0x00, 0x00 };
    assert(r.frame_size == sizeof(want));
    assert(std::memcmp(frame, want, r.frame_size) == 0);
}

void test_effect_breathe_and_ids() {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    struct { const char* label; std::uint8_t id; } cases[] = {
        { "breathe",        0x01 },
        { "okay",           0x02 },
        { "channel_change", 0x0b },
        { "finish_effect",  0xfe },
        { "stop_effect",    0xff },
    };
    for (const auto& c : cases) {
        auto r = dispatch_to_zigbee(devices::ysrsai::kDef_YSR_MINI_01_rgbcct,
                                    "effect", str_val(c.label), ctx, frame);
        assert(r.ok);
        assert(r.cluster_id == 0x0003 && r.command_id == 0x40);
        assert(r.frame_size == 5);
        assert(frame[3] == c.id && frame[4] == 0x00);
    }
}

// ── effect colorloop / stop_colorloop (0x0300 cmd 0x44 colorLoopSet) ──
// Exercises the dispatcher fall-through: the genIdentify converter
// returns false for these labels, so the colorLoopSet converter claims.
void test_effect_colorloop() {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    auto r = dispatch_to_zigbee(devices::ysrsai::kDef_YSR_MINI_01_rgbcct,
                                "effect", str_val("colorloop"), ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0x0300);     // lightingColorCtrl, NOT genIdentify
    assert(r.command_id == 0x44);       // colorLoopSet
    // updateFlags=0x06, action=0x01, dir=0x01, time(LE)=0x0001, startHue=0x0000.
    const std::uint8_t want[] = {
        0x11, 0x00, 0x44, 0x06, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
    };
    assert(r.frame_size == sizeof(want));
    assert(std::memcmp(frame, want, r.frame_size) == 0);
}

void test_effect_stop_colorloop() {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    auto r = dispatch_to_zigbee(devices::ysrsai::kDef_YSR_MINI_01_rgbcct,
                                "effect", str_val("stop_colorloop"), ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0x0300 && r.command_id == 0x44);
    // updateFlags=0x01, action=0x00, dir=0x00, time=0x0000, startHue=0x0000.
    const std::uint8_t want[] = {
        0x11, 0x00, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    assert(r.frame_size == sizeof(want));
    assert(std::memcmp(frame, want, r.frame_size) == 0);
}

void test_effect_bad_label_rejected() {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    auto r = dispatch_to_zigbee(devices::ysrsai::kDef_YSR_MINI_01_rgbcct,
                                "effect", str_val("not_an_effect"), ctx, frame);
    assert(!r.ok);     // neither converter claims it
}

// ── the wrong power_on_behavior must be GONE ─────────────────────────
void test_power_on_behavior_removed() {
    const auto& d = devices::ysrsai::kDef_YSR_MINI_01_rgbcct;
    for (std::uint8_t i = 0; i < d.exposes_count; ++i) {
        assert(std::strcmp(d.exposes[i].name, "power_on_behavior") != 0);
    }
    // and there is no tz converter claiming it
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Uint; v.u = 1;
    auto r = dispatch_to_zigbee(d, "power_on_behavior", v, ctx, frame);
    assert(!r.ok);
}

}  // namespace

int main() {
    test_color_power_on_initial();
    test_color_power_on_previous();
    test_color_power_on_customized_uint_index();
    test_color_power_on_bad_label_rejected();
    test_do_not_disturb_on();
    test_do_not_disturb_off();
    test_effect_blink();
    test_effect_breathe_and_ids();
    test_effect_colorloop();
    test_effect_stop_colorloop();
    test_effect_bad_label_rejected();
    test_power_on_behavior_removed();
    return 0;
}
