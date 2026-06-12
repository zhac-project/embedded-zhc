// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Behavioural-parity fixture tests for Moes BHT-002 thermostat.
//
// Goal: prove ZHC's decode output matches z2m for the wire frames a real
// BHT-002 emits via the Tuya DP stream (cluster 0xEF00 cmd 0x02). Three
// representative datapoints are exercised:
//
//   * dp_id 24  localTemperature  Numeric, divisor 10 → "local_temperature"
//   * dp_id 40  childLock         Bool                → "child_lock"
//   * dp_id 43  sensor            Enum 0/1/2          → "sensor" = "OU"
//
// z2m-source: zigbee-herdsman-converters/src/lib/legacy.ts
//             `fz.moes_thermostat` switch table (lines 1876-2010, commit
//             2025-Q1) — see Moe_BHT_002.cpp header comment.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::moes {
extern const PreparedDefinition kDef_BHT_002;
}

using namespace zhc;

namespace {

bool approx(float a, float b, float eps) {
    return std::fabs(a - b) <= eps;
}

// Build the DecodedMessage + InboundApsFrame pair the dispatcher expects
// for an inbound Tuya DP report on cluster 0xEF00 cmd 0x02.
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

}  // namespace

// dp_id=24 (localTemperature) Numeric=235 with divisor 10 → 23.5 °C.
// z2m emits `local_temperature: 23.5`.
static void test_local_temperature_decode() {
    // Big-endian 4-byte numeric: 235 = 0x000000EB.
    const std::uint8_t kRaw235[] = { 0x00, 0x00, 0x00, 0xEB };
    const TuyaDpRecord recs[] = {
        { 24, 0x02, std::span<const std::uint8_t>(kRaw235, 4) },
    };

    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1),
        devices::moes::kDef_BHT_002, raw, ctx);
    assert(result.any_matched);

    const Value* lt = result.merged.find("local_temperature");
    assert(lt && lt->type == ValueType::Float);
    assert(approx(lt->f, 23.5f, 0.001f));
}

// dp_id=40 (childLock) Bool=true → child_lock=true.
// z2m emits `child_lock: 'LOCK'`; ZHC surfaces as Bool — adapter does the
// LOCK/UNLOCK string mapping when shadow-formatting.
static void test_child_lock_decode() {
    const std::uint8_t kBoolOn[] = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 40, 0x01, std::span<const std::uint8_t>(kBoolOn, 1) },
    };

    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1),
        devices::moes::kDef_BHT_002, raw, ctx);
    assert(result.any_matched);

    const Value* cl = result.merged.find("child_lock");
    assert(cl && cl->type == ValueType::Bool);
    assert(cl->b == true);
}

// dp_id=43 (sensor) Enum=2 → sensor="OU".
// z2m's lookup: { IN: 0, AL: 1, OU: 2 } — see legacy.ts moes_thermostat.
static void test_sensor_enum_decode() {
    const std::uint8_t kEnum2[] = { 0x02 };
    const TuyaDpRecord recs[] = {
        { 43, 0x04, std::span<const std::uint8_t>(kEnum2, 1) },
    };

    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1),
        devices::moes::kDef_BHT_002, raw, ctx);
    assert(result.any_matched);

    const Value* s = result.merged.find("sensor");
    assert(s && s->type == ValueType::StringRef);
    assert(s->str && std::strcmp(s->str, "OU") == 0);
}

// Bonus: multi-DP frame (state+sensor) decodes both keys in one pass.
// Mirrors the wire shape z2m sees when the device batches several DPs.
static void test_multi_dp_frame_decodes_all_keys() {
    const std::uint8_t kBoolOff[] = { 0x00 };
    const std::uint8_t kEnum0[]   = { 0x00 };
    const TuyaDpRecord recs[] = {
        { 1,  0x01, std::span<const std::uint8_t>(kBoolOff, 1) },  // state=false
        { 43, 0x04, std::span<const std::uint8_t>(kEnum0,   1) },  // sensor=IN
    };

    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 2),
        devices::moes::kDef_BHT_002, raw, ctx);
    assert(result.any_matched);

    const Value* st = result.merged.find("state");
    assert(st && st->type == ValueType::Bool && st->b == false);

    const Value* s  = result.merged.find("sensor");
    assert(s && s->type == ValueType::StringRef);
    assert(std::strcmp(s->str, "IN") == 0);
}

// ── DP101 moesSchedule weekly program codec ──────────────────────────
//
// DP101 is a 36-byte Raw payload = 3 day-groups [weekdays, saturday,
// sunday], each 4 periods × [hour, minute, temp*2]. ZHC surfaces ONE
// round-trippable `program` string: three 4-period groups joined by
// " | ", each period "HH:MM/T.t" (temp = byte/2). The codec lives in
// tuya/_shared.cpp behind kTuyaDpFlagMoesSchedule and is symmetric:
// decode (fz) and encode (tz) must round-trip byte-identically. The
// definition (kDef_BHT_002) carries both the DP101 map entry and a
// read+write `program` expose, so the same def drives both directions.

// Canonical fixture: 12 periods. byte triples [h, m, temp*2].
static const std::uint8_t kSchedule36[36] = {
    // weekdays
    6, 0, 40,   8, 0, 30,   11, 30, 30,   12, 30, 30,
    // saturday
    6, 0, 40,   8, 0, 30,   11, 30, 30,   12, 30, 30,
    // sunday (period 2 temp = 15.5 °C → 31, exercises the .5 half-step)
    6, 0, 40,   8, 0, 31,   11, 30, 30,   12, 30, 30,
};
// Decode of kSchedule36 (temp = byte/2, %.1f):
static const char* const kScheduleStr =
    "06:00/20.0 08:00/15.0 11:30/15.0 12:30/15.0 | "
    "06:00/20.0 08:00/15.0 11:30/15.0 12:30/15.0 | "
    "06:00/20.0 08:00/15.5 11:30/15.0 12:30/15.0";

// (1) DECODE: a DP101 Raw frame (dp_type 0x00) → expected program string.
static void test_program_decode() {
    const TuyaDpRecord recs[] = {
        { 101, 0x00, std::span<const std::uint8_t>(kSchedule36, 36) },
    };
    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1),
        devices::moes::kDef_BHT_002, raw, ctx);
    assert(result.any_matched);

    const Value* prog = result.merged.find("program");
    assert(prog && prog->type == ValueType::StringRef);
    assert(prog->str && std::strcmp(prog->str, kScheduleStr) == 0);
}

// (2) ENCODE: the program string → the exact 45-byte DP101 Raw frame.
//   fc=01 tsn=00 cmd=00 seq=0001 dp=101(0x65) type=00(Raw) len=0x0024(36)
//   then the 36-byte packed schedule (== kSchedule36).
static void test_program_encode() {
    RuntimeContext ctx{};
    std::uint8_t frame[64]{};
    Value v{}; v.type = ValueType::StringRef; v.str = kScheduleStr;
    auto r = dispatch_to_zigbee(devices::moes::kDef_BHT_002, "program", v,
                                ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0xEF00);
    assert(r.command_id == 0x00);
    assert(r.frame_size == 45);

    const std::uint8_t want_hdr[9] = {
        0x01, 0x00, 0x00, 0x00, 0x01, 0x65, 0x00, 0x00, 0x24,
    };
    assert(std::memcmp(frame, want_hdr, 9) == 0);
    assert(std::memcmp(frame + 9, kSchedule36, 36) == 0);
}

// (3) ROUND-TRIP: decode(bytes) → string, encode(string) → identical bytes.
static void test_program_round_trip() {
    // decode
    const TuyaDpRecord recs[] = {
        { 101, 0x00, std::span<const std::uint8_t>(kSchedule36, 36) },
    };
    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext dctx{};
    const auto dec = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1),
        devices::moes::kDef_BHT_002, raw, dctx);
    const Value* prog = dec.merged.find("program");
    assert(prog && prog->type == ValueType::StringRef && prog->str);

    // encode the decoded string back
    RuntimeContext ectx{};
    std::uint8_t frame[64]{};
    Value v{}; v.type = ValueType::StringRef; v.str = prog->str;
    auto r = dispatch_to_zigbee(devices::moes::kDef_BHT_002, "program", v,
                                ectx, frame);
    assert(r.ok && r.frame_size == 45);
    // payload bytes [9..44] must equal the original 36-byte buffer.
    assert(std::memcmp(frame + 9, kSchedule36, 36) == 0);
}

// (4) MALFORMED: bad program strings must abstain (ok=false, nothing written).
static void test_program_encode_rejects_malformed() {
    const char* const bad[] = {
        "",                                                       // empty
        "06:00/20.0 08:00/15.0 11:30/15.0",                       // 3 periods
        "06:00/20.0 08:00/15.0 11:30/15.0 12:30/15.0",            // 1 group only
        // wrong separator (pipe without surrounding spaces):
        "06:00/20.0 08:00/15.0 11:30/15.0 12:30/15.0|"
        "06:00/20.0 08:00/15.0 11:30/15.0 12:30/15.0|"
        "06:00/20.0 08:00/15.5 11:30/15.0 12:30/15.0",
        // hour out of range (25):
        "25:00/20.0 08:00/15.0 11:30/15.0 12:30/15.0 | "
        "06:00/20.0 08:00/15.0 11:30/15.0 12:30/15.0 | "
        "06:00/20.0 08:00/15.5 11:30/15.0 12:30/15.0",
        // temp*2 overflow (200.0 → 400 > 255):
        "06:00/200.0 08:00/15.0 11:30/15.0 12:30/15.0 | "
        "06:00/20.0 08:00/15.0 11:30/15.0 12:30/15.0 | "
        "06:00/20.0 08:00/15.5 11:30/15.0 12:30/15.0",
    };
    for (const char* s : bad) {
        RuntimeContext ctx{};
        std::uint8_t frame[64]{};
        Value v{}; v.type = ValueType::StringRef; v.str = s;
        auto r = dispatch_to_zigbee(devices::moes::kDef_BHT_002, "program", v,
                                    ctx, frame);
        assert(!r.ok);
        assert(r.frame_size == 0);
    }

    // Non-string input on the program key must also abstain.
    RuntimeContext ctx{};
    std::uint8_t frame[64]{};
    Value vi{}; vi.type = ValueType::Int; vi.i = 42;
    auto r = dispatch_to_zigbee(devices::moes::kDef_BHT_002, "program", vi,
                                ctx, frame);
    assert(!r.ok);
}

int main() {
    test_local_temperature_decode();
    test_child_lock_decode();
    test_sensor_enum_decode();
    test_multi_dp_frame_decodes_all_keys();
    test_program_decode();
    test_program_encode();
    test_program_round_trip();
    test_program_encode_rejects_malformed();
    return 0;
}
