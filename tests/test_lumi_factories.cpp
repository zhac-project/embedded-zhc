// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: smoke tests for the LumiOnOff / LumiAction / LumiPower
// factories through their ported representative devices.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/lumi/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefQBKG11LM;
extern const PreparedDefinition kDefWXKG03LM;
extern const PreparedDefinition kDefZNCZ15LM;
}

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster, std::uint8_t ep,
                             std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = ep;
    f.dst_endpoint = ep;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

DispatchResult dispatch(const PreparedDefinition& def,
                         std::uint16_t cluster, const char* cluster_name,
                         std::uint8_t ep,
                         std::span<const std::uint8_t> bytes) {
    auto raw = build_frame(cluster, ep, bytes);
    DecodedMessage msg{};
    if (!decode_frame(raw, {}, msg)) return {};
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool approx(float a, float b, float eps) { return std::fabs(a - b) <= eps; }

}  // namespace

// ── LumiOnOff via QBKG11LM ────────────────────────────────────────

static void test_qbkg11lm_endpoint_routing() {
    constexpr std::uint8_t kOnFrame[] = {
        0x18, 0x01, 0x0A, 0x00, 0x00, 0x10, 0x01,
    };
    // Endpoint 1 → "state_left".
    {
        const auto r = dispatch(devices::lumi::kDefQBKG11LM,
                                 0x0006, "genOnOff", 1, kOnFrame);
        assert(r.any_matched);
        const Value* v = r.merged.find("state_left");
        assert(v && v->type == ValueType::Bool && v->b == true);
        assert(r.merged.find("state") == nullptr);
    }
    // Endpoint 2 → "state_right".
    {
        const auto r = dispatch(devices::lumi::kDefQBKG11LM,
                                 0x0006, "genOnOff", 2, kOnFrame);
        assert(r.any_matched);
        const Value* v = r.merged.find("state_right");
        assert(v && v->type == ValueType::Bool && v->b == true);
    }
}

// ── LumiAction via WXKG03LM ───────────────────────────────────────

static void test_wxkg03lm_action_map() {
    // genMultistateInput attr 0x0055 u16 = N.
    auto make = [](std::uint16_t val, std::uint8_t* buf) {
        buf[0] = 0x18; buf[1] = 0x01; buf[2] = 0x0A;
        buf[3] = 0x55; buf[4] = 0x00;
        buf[5] = 0x21;
        buf[6] = static_cast<std::uint8_t>(val & 0xFF);
        buf[7] = static_cast<std::uint8_t>((val >> 8) & 0xFF);
    };
    struct Case { std::uint16_t raw; const char* want; };
    const Case cases[] = {
        {0, "hold"}, {1, "single"}, {2, "double"},
    };
    for (const auto& c : cases) {
        std::uint8_t buf[8]{};
        make(c.raw, buf);
        const auto r = dispatch(devices::lumi::kDefWXKG03LM,
                                 0x0012, "genMultistateInput", 1,
                                 std::span<const std::uint8_t>(buf, 8));
        assert(r.any_matched);
        const Value* a = r.merged.find("action");
        assert(a && std::strcmp(a->str, c.want) == 0);
    }
}

// ── LumiPower via ZNCZ15LM ────────────────────────────────────────

static void test_zncz15lm_power_scales_divisor_10() {
    constexpr std::uint8_t kPowerFrame[] = {
        0x18, 0x01, 0x0A,
        0x0B, 0x05, 0x29, 0x7B, 0x00,     // 0x050B int16 = 123 → 12.3 W
    };
    const auto r = dispatch(devices::lumi::kDefZNCZ15LM,
                             0x0B04, "haElectricalMeasurement", 1,
                             kPowerFrame);
    assert(r.any_matched);
    const Value* p = r.merged.find("power");
    assert(p && p->type == ValueType::Float);
    assert(approx(p->f, 12.3f, 0.01f));
}

static void test_zncz15lm_energy_scales_divisor_1000() {
    // 456789 = 0x6F855; LE bytes: 55 F8 06 00.
    constexpr std::uint8_t kEnergyFrame[] = {
        0x18, 0x02, 0x0A,
        0x00, 0x00, 0x23, 0x55, 0xF8, 0x06, 0x00,
    };
    const auto r = dispatch(devices::lumi::kDefZNCZ15LM,
                             0x0702, "seMetering", 1, kEnergyFrame);
    assert(r.any_matched);
    const Value* e = r.merged.find("energy");
    assert(e && e->type == ValueType::Float);
    assert(approx(e->f, 456.789f, 0.01f));
}

static void test_zncz15lm_relay_state_via_on_off() {
    constexpr std::uint8_t kOnFrame[] = {
        0x18, 0x03, 0x0A, 0x00, 0x00, 0x10, 0x01,
    };
    const auto r = dispatch(devices::lumi::kDefZNCZ15LM,
                             0x0006, "genOnOff", 1, kOnFrame);
    assert(r.any_matched);
    const Value* s = r.merged.find("state");
    assert(s && s->type == ValueType::Bool && s->b == true);
}

int main() {
    test_qbkg11lm_endpoint_routing();
    test_wxkg03lm_action_map();
    test_zncz15lm_power_scales_divisor_10();
    test_zncz15lm_energy_scales_divisor_1000();
    test_zncz15lm_relay_state_via_on_off();
    return 0;
}
