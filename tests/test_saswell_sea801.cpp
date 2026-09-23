// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Saswell SEA801/SEA802 weekly schedule codec (kTuyaDpFlagSaswellSchedule).
//
// The TRV reports each day's program on its own Raw datapoint, 123..129 =
// Sunday..Saturday: [mode:1] + 4 periods × [minutes:2 BE][temp×10:2 BE]
// (mode 4 = "7 day"). It takes a day's program on ONE datapoint, 109:
// [day bitmap:1, bit0 = Sunday][mode = 4] + the same 4 periods.
// ZHC surfaces one round-trippable string per day, `schedule_<day>` =
// four "HH:MM/T.t" periods, and writes it back as the DP109 frame.
//
// z2m-source: zigbee-herdsman-converters src/lib/legacy.ts —
//   dataPoints.saswellScheduleSet (109) / saswellScheduleSunday..Saturday
//   (123..129), tz.tuya_thermostat_weekly_schedule (conversion "saswell"),
//   fz.tuya_thermostat_weekly_schedule_2 ("mode in the first byte").
// Bench: a real SEA801 (_TYST11_KGbxAXL2) answered DATA_QUERY with DP123 =
// 04 01 68 00 c8 01 e0 … (the log shows the first 7 bytes); the fixture
// below keeps that prefix and fills the rest with a plausible program.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::saswell {
extern const PreparedDefinition kDef_SEA801;
}

using namespace zhc;

namespace {

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

// mode 4, then 06:00/20.0 08:00/15.0 17:00/21.0 22:00/15.0
const std::uint8_t kDay17[17] = {
    0x04,
    0x01, 0x68, 0x00, 0xC8,   // 360 min = 06:00, 200 = 20.0 °C
    0x01, 0xE0, 0x00, 0x96,   // 480 min = 08:00, 150 = 15.0 °C
    0x03, 0xFC, 0x00, 0xD2,   // 1020 min = 17:00, 210 = 21.0 °C
    0x05, 0x28, 0x00, 0x96,   // 1320 min = 22:00, 150 = 15.0 °C
};
const char* const kDayStr = "06:00/20.0 08:00/15.0 17:00/21.0 22:00/15.0";

const char* decode_day(std::uint8_t dp, const std::uint8_t* bytes, const char* key,
                       RuntimeContext& ctx) {
    const TuyaDpRecord recs[] = { { dp, 0x00, std::span<const std::uint8_t>(bytes, 17) } };
    auto msg = make_msg();
    auto raw = make_raw();
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1), devices::saswell::kDef_SEA801, raw, ctx);
    if (!result.any_matched) return nullptr;
    const Value* v = result.merged.find(key);
    return (v && v->type == ValueType::StringRef) ? v->str : nullptr;
}

TzDispatchResult encode_day(const char* key, const char* program, std::uint8_t (&frame)[64]) {
    RuntimeContext ctx{};
    Value v{}; v.type = ValueType::StringRef; v.str = program;
    return dispatch_to_zigbee(devices::saswell::kDef_SEA801, key, v, ctx, frame);
}

}  // namespace

// (1) DP123 = Sunday, DP129 = Saturday decode to their day's string.
static void test_day_decode() {
    RuntimeContext ctx{};
    const char* sun = decode_day(123, kDay17, "schedule_sunday", ctx);
    assert(sun && std::strcmp(sun, kDayStr) == 0);
    const char* sat = decode_day(129, kDay17, "schedule_saturday", ctx);
    assert(sat && std::strcmp(sat, kDayStr) == 0);
}

// (2) Half degrees survive: 205 → "20.5".
static void test_half_degree_decode() {
    std::uint8_t day[17];
    std::memcpy(day, kDay17, sizeof(day));
    day[3] = 0x00; day[4] = 0xCD;   // period 1 = 20.5 °C
    RuntimeContext ctx{};
    const char* s = decode_day(124, day, "schedule_monday", ctx);
    assert(s && std::strcmp(s, "06:00/20.5 08:00/15.0 17:00/21.0 22:00/15.0") == 0);
}

// (3) A day string encodes to the DP109 frame: 9-byte Tuya header, then
//     [day bitmap][mode 4][4 × minutes BE, temp×10 BE] = 18 bytes.
static void test_encode_monday() {
    std::uint8_t frame[64]{};
    const auto r = encode_day("schedule_monday", kDayStr, frame);
    assert(r.ok);
    assert(r.cluster_id == 0xEF00);
    assert(r.command_id == 0x00);
    assert(r.frame_size == 27);
    const std::uint8_t want_hdr[9] = { 0x01, 0x00, 0x00, 0x00, 0x01, 109, 0x00, 0x00, 18 };
    assert(std::memcmp(frame, want_hdr, 9) == 0);
    assert(frame[9] == 0x02);                               // bit1 = Monday
    assert(frame[10] == 0x04);                              // mode "7 day"
    assert(std::memcmp(frame + 11, kDay17 + 1, 16) == 0);   // the 4 periods
}

// (4) Day bitmap follows the day: Sunday bit0, Saturday bit6.
static void test_day_bitmap() {
    std::uint8_t frame[64]{};
    assert(encode_day("schedule_sunday", kDayStr, frame).ok && frame[9] == 0x01);
    std::uint8_t frame2[64]{};
    assert(encode_day("schedule_saturday", kDayStr, frame2).ok && frame2[9] == 0x40);
}

// (5) Round trip: decode(bytes) → string → encode → the same period bytes.
static void test_round_trip() {
    RuntimeContext ctx{};
    const char* s = decode_day(126, kDay17, "schedule_wednesday", ctx);
    assert(s);
    std::uint8_t frame[64]{};
    const auto r = encode_day("schedule_wednesday", s, frame);
    assert(r.ok && frame[9] == 0x08);
    assert(std::memcmp(frame + 10, kDay17, 17) == 0);   // mode + periods, byte-identical
}

// (6) Fewer than 4 periods pad with the last one (as z2m does); whole
//     degrees need no decimal.
static void test_pad_short_program() {
    std::uint8_t frame[64]{};
    const auto r = encode_day("schedule_friday", "06:00/21 22:00/17", frame);
    assert(r.ok && frame[9] == 0x20);
    const std::uint8_t want[16] = {
        0x01, 0x68, 0x00, 0xD2,   // 06:00 21.0
        0x05, 0x28, 0x00, 0xAA,   // 22:00 17.0
        0x05, 0x28, 0x00, 0xAA,   // padded
        0x05, 0x28, 0x00, 0xAA,   // padded
    };
    assert(std::memcmp(frame + 11, want, 16) == 0);
}

// (6b) The TRV stores a short day padded with its last period (bench echo
//      2026-09-23 after writing "11:00/22.0 19:00/16.0"). Trailing repeats
//      print once, so the text passes the encoder's ascending check and
//      encodes back to the same bytes.
static void test_padded_echo_round_trip() {
    const std::uint8_t echo[17] = {
        0x04,
        0x02, 0x94, 0x00, 0xDC,   // 660 min = 11:00, 22.0 °C
        0x04, 0x74, 0x00, 0xA0,   // 1140 min = 19:00, 16.0 °C
        0x04, 0x74, 0x00, 0xA0,   // padded
        0x04, 0x74, 0x00, 0xA0,   // padded
    };
    RuntimeContext ctx{};
    const char* s = decode_day(124, echo, "schedule_monday", ctx);
    assert(s && std::strcmp(s, "11:00/22.0 19:00/16.0") == 0);
    std::uint8_t frame[64]{};
    assert(encode_day("schedule_monday", s, frame).ok);
    assert(std::memcmp(frame + 11, echo + 1, 16) == 0);

    const std::uint8_t same[17] = { 0x04, 0x02, 0x94, 0x00, 0xDC, 0x02, 0x94, 0x00, 0xDC,
                                    0x02, 0x94, 0x00, 0xDC, 0x02, 0x94, 0x00, 0xDC };
    const char* one = decode_day(125, same, "schedule_tuesday", ctx);
    assert(one && std::strcmp(one, "11:00/22.0") == 0);
}

// (7) Anything the TRV cannot take is refused and nothing is written.
static void test_rejects() {
    const char* const bad[] = {
        "",                                                   // no period
        "25:00/20.0",                                         // hour
        "06:60/20.0",                                         // minute
        "06:00/4.5",                                          // below 5 °C
        "06:00/30.5",                                         // above 30 °C
        "08:00/20.0 06:00/18.0",                              // not ascending
        "06:00/20.0 06:00/18.0",                              // same time twice
        "06:00/20 08:00/15 11:00/15 17:00/21 22:00/15",       // 5 periods
        "06:00/20.0 ",                                        // trailing space
        "06:00/20.0  08:00/15.0",                             // double space
        "06:00 20.0",                                         // no slash
        "06:00/20.0x",                                        // trailing garbage
    };
    for (const char* s : bad) {
        std::uint8_t frame[64]{};
        const auto r = encode_day("schedule_tuesday", s, frame);
        assert(!r.ok);
        assert(r.frame_size == 0);
    }
    // A number is not a program.
    RuntimeContext ctx{};
    std::uint8_t frame[64]{};
    Value v{}; v.type = ValueType::Int; v.i = 5;
    assert(!dispatch_to_zigbee(devices::saswell::kDef_SEA801, "schedule_tuesday", v, ctx, frame).ok);
}

// (8) The seven days are exposed as writable strings, so a UI can edit them.
static void test_exposes() {
    const char* const days[] = { "schedule_sunday", "schedule_monday", "schedule_tuesday",
        "schedule_wednesday", "schedule_thursday", "schedule_friday", "schedule_saturday" };
    const auto& d = devices::saswell::kDef_SEA801;
    for (const char* day : days) {
        bool found = false;
        for (std::uint8_t i = 0; i < d.exposes_count; ++i) {
            if (std::strcmp(d.exposes[i].name, day) == 0) {
                assert(d.exposes[i].type == ExposeType::String);
                assert(d.exposes[i].access == Access::StateSet);
                found = true;
            }
        }
        assert(found);
    }
}

int main() {
    test_day_decode();
    test_half_degree_decode();
    test_encode_monday();
    test_day_bitmap();
    test_round_trip();
    test_pad_short_program();
    test_padded_echo_round_trip();
    test_rejects();
    test_exposes();
    return 0;
}
