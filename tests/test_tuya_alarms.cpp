// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Safety parity test: inverted-bool alarm decode (dp::binary_inv) for Tuya
// gas/smoke. z2m trueFalse0 => wire 0 means ALARM (true). A polarity bug here
// would silence a smoke/gas alarm, so both polarities are pinned.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS0601_smoke_3;
extern const PreparedDefinition kDefZG_225Z;
extern const PreparedDefinition kDefDCR_CO;
extern const PreparedDefinition kDefTS0601_switch_2g;
extern const PreparedDefinition kDefTS0601_cover_inv;
extern const PreparedDefinition kDefHochDin;
}

using namespace zhc;

static auto dispatch(const PreparedDefinition& def,
                     const TuyaDpRecord* recs, std::size_t n) {
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
    static RuntimeContext ctx;   // fresh per call
    ctx = RuntimeContext{};
    return dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>(recs, n), def, raw, ctx);
}

static bool approx(float a, float b, float eps) {
    const float d = a - b;
    return d < eps && d > -eps;
}

static void test_smoke_inverted() {
    const std::uint8_t kBool0[] = { 0x00 };  // wire 0 -> ALARM
    const std::uint8_t kEnum2[] = { 0x02 };  // battery_state -> "high"
    const TuyaDpRecord recs[] = {
        { 1,  0x01, std::span<const std::uint8_t>(kBool0, 1) },
        { 14, 0x04, std::span<const std::uint8_t>(kEnum2, 1) },
    };
    const auto r = dispatch(devices::tuya::kDefTS0601_smoke_3, recs, 2);
    assert(r.any_matched);
    const Value* s = r.merged.find("smoke");
    assert(s && s->type == ValueType::Bool && s->b == true);   // 0 => alarm
    const Value* b = r.merged.find("battery_state");
    assert(b && b->type == ValueType::StringRef && std::strcmp(b->str, "high") == 0);

    const std::uint8_t kBool1[] = { 0x01 };  // wire 1 -> clear
    const TuyaDpRecord recs2[] = { { 1, 0x01, std::span<const std::uint8_t>(kBool1, 1) } };
    const auto r2 = dispatch(devices::tuya::kDefTS0601_smoke_3, recs2, 1);
    const Value* s2 = r2.merged.find("smoke");
    assert(s2 && s2->type == ValueType::Bool && s2->b == false);
}

static void test_gas_inverted() {
    const std::uint8_t kBool0[] = { 0x00 };                 // gas detected
    const std::uint8_t kGv100[] = { 0x00, 0x00, 0x00, 0x64 }; // 100 ppm
    const std::uint8_t kSens1[] = { 0x01 };                 // medium
    const std::uint8_t kRing0[] = { 0x00 };                 // ring1
    const TuyaDpRecord recs[] = {
        { 1,   0x01, std::span<const std::uint8_t>(kBool0, 1) },
        { 2,   0x02, std::span<const std::uint8_t>(kGv100, 4) },
        { 101, 0x04, std::span<const std::uint8_t>(kSens1, 1) },
        { 6,   0x04, std::span<const std::uint8_t>(kRing0, 1) },
    };
    const auto r = dispatch(devices::tuya::kDefZG_225Z, recs, 4);
    assert(r.any_matched);
    const Value* g = r.merged.find("gas");
    assert(g && g->type == ValueType::Bool && g->b == true);   // 0 => detected
    const Value* gv = r.merged.find("gas_value");
    assert(gv && gv->type == ValueType::Int && gv->i == 100);
    const Value* sens = r.merged.find("sensitivity");
    assert(sens && sens->type == ValueType::StringRef && std::strcmp(sens->str, "medium") == 0);
    const Value* ring = r.merged.find("ring");
    assert(ring && ring->type == ValueType::StringRef && std::strcmp(ring->str, "ring1") == 0);
}

static void test_co_inverted() {
    const std::uint8_t kBool0[] = { 0x00 };                  // CO detected -> ON
    const std::uint8_t kCo350[] = { 0x00, 0x00, 0x01, 0x5E }; // 350 -> 3.5 ppm (/100)
    const TuyaDpRecord recs[] = {
        { 1, 0x01, std::span<const std::uint8_t>(kBool0, 1) },
        { 2, 0x02, std::span<const std::uint8_t>(kCo350, 4) },
    };
    const auto r = dispatch(devices::tuya::kDefDCR_CO, recs, 2);
    assert(r.any_matched);
    const Value* co = r.merged.find("carbon_monoxide");
    assert(co && co->type == ValueType::Bool && co->b == true);   // wire 0 => alarm
    const Value* v = r.merged.find("co");
    assert(v && v->type == ValueType::Float);
}

static void test_switch_2g_control() {
    // factory::TuyaRw write path: state_l1 = ON -> DP1 bool ON frame.
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Bool; v.b = true;
    auto r = dispatch_to_zigbee(devices::tuya::kDefTS0601_switch_2g, "state_l1", v, ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0xEF00);
    assert(r.command_id == 0x00);
    // fc=01 tsn=00 cmd=00 seq=0001 dp=1 type=1(Bool) len=0001 val=01
    const std::uint8_t want[] = { 0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01 };
    assert(r.frame_size == sizeof(want));
    assert(std::memcmp(frame, want, sizeof(want)) == 0);

    // OFF on l2 -> DP2 bool 0
    Value off{}; off.type = ValueType::Bool; off.b = false;
    std::uint8_t frame2[32]{};
    auto r2 = dispatch_to_zigbee(devices::tuya::kDefTS0601_switch_2g, "state_l2", off, ctx, frame2);
    assert(r2.ok && r2.frame_size >= 10);
    assert(frame2[5] == 0x02 && frame2[9] == 0x00);   // dp=2, val=0
}

static void test_cover_inverted() {
    // fz: DP2 raw 30 -> position 70 (kTuyaDpFlagInvertPosition: 100-raw)
    const std::uint8_t kPos30[] = { 0x00, 0x00, 0x00, 0x1E };
    const TuyaDpRecord recs[] = { { 2, 0x02, std::span<const std::uint8_t>(kPos30, 4) } };
    DecodedMessage msg{};
    msg.family = FrameFamily::TuyaDp; msg.type = MessageType::Command;
    msg.cluster = "manuSpecificTuya"; msg.direction = Direction::ServerToClient;
    msg.command_id = 0x02; msg.src_endpoint = 1; msg.dst_endpoint = 1;
    InboundApsFrame raw{};
    raw.cluster_id = 0xEF00; raw.src_endpoint = 1; raw.dst_endpoint = 1; raw.linkquality = 0xC8;
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>(recs, 1),
                                  devices::tuya::kDefTS0601_cover_inv, raw, ctx);
    assert(r.any_matched);
    const Value* p = r.merged.find("position");
    assert(p && p->type == ValueType::Int && p->i == 70);   // 100 - 30

    // tz: set position 70 -> DP2 raw 30 (encode 100-value)
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Int; v.i = 70;
    auto w = dispatch_to_zigbee(devices::tuya::kDefTS0601_cover_inv, "position", v, ctx, frame);
    assert(w.ok);
    assert(frame[5] == 0x02);          // dp = 2
    assert(frame[6] == 0x02);          // type Numeric
    assert(frame[12] == 0x1E);         // value low byte = 30
}

static void test_hoch_din() {
    // custom byte-slice fz: DP101 raw {09,1A} -> voltage (0x091A)/10 = 233.0,
    // DP113 numeric 10000 -> energy /100 = 100.0, DP1 -> state ON.
    const std::uint8_t kV[]  = { 0x09, 0x1A };
    const std::uint8_t kE[]  = { 0x00, 0x00, 0x27, 0x10 };
    const std::uint8_t kOn[] = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 101, 0x00, std::span<const std::uint8_t>(kV,  2) },
        { 113, 0x02, std::span<const std::uint8_t>(kE,  4) },
        {   1, 0x01, std::span<const std::uint8_t>(kOn, 1) },
    };
    DecodedMessage msg{};
    msg.family = FrameFamily::TuyaDp; msg.type = MessageType::Command;
    msg.cluster = "manuSpecificTuya"; msg.direction = Direction::ServerToClient;
    msg.command_id = 0x02; msg.src_endpoint = 1; msg.dst_endpoint = 1;
    InboundApsFrame raw{};
    raw.cluster_id = 0xEF00; raw.src_endpoint = 1; raw.dst_endpoint = 1; raw.linkquality = 0xC8;
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>(recs, 3),
                                  devices::tuya::kDefHochDin, raw, ctx);
    assert(r.any_matched);
    const Value* v = r.merged.find("voltage");
    assert(v && v->type == ValueType::Float && approx(v->f, 233.0f, 0.01f));
    const Value* e = r.merged.find("energy");
    assert(e && e->type == ValueType::Float && approx(e->f, 100.0f, 0.01f));
    const Value* s = r.merged.find("state");
    assert(s && s->type == ValueType::Bool && s->b == true);
}

static void test_dp_passthrough() {
    // A def with NO converters/DP map: a Tuya DP frame must still surface as
    // dp_<id> (the dispatch fallback), not vanish.
    PreparedDefinition def{};   // value-init: from_zigbee_count = 0
    const std::uint8_t kNum265[] = { 0x00, 0x00, 0x01, 0x09 };  // 265
    const std::uint8_t kBool1[]  = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 18, 0x02, std::span<const std::uint8_t>(kNum265, 4) },
        {  2, 0x01, std::span<const std::uint8_t>(kBool1,  1) },
    };
    DecodedMessage msg{};
    msg.family = FrameFamily::TuyaDp; msg.type = MessageType::Command;
    msg.cluster = "manuSpecificTuya"; msg.direction = Direction::ServerToClient;
    msg.command_id = 0x02; msg.src_endpoint = 1; msg.dst_endpoint = 1;
    InboundApsFrame raw{};
    raw.cluster_id = 0xEF00; raw.src_endpoint = 1; raw.dst_endpoint = 1; raw.linkquality = 0xC8;
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>(recs, 2), def, raw, ctx);
    assert(r.any_matched);                       // fallback claimed it
    const Value* a = r.merged.find("dp_18");
    assert(a && a->type == ValueType::Int && a->i == 265);
    const Value* b = r.merged.find("dp_2");
    assert(b && b->type == ValueType::Bool && b->b == true);
}

int main() {
    test_smoke_inverted();
    test_gas_inverted();
    test_co_inverted();
    test_switch_2g_control();
    test_cover_inverted();
    test_hoch_din();
    test_dp_passthrough();
    return 0;
}
