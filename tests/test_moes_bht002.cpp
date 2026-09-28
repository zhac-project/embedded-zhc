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
//
// Family parity (z2m v26.105.0, second half of this file): the nine
// manufacturer IDs land on three definitions by scaling group; running_state
// is inverted (DP36 true = idle), preset reads DP2 and DP3 and writes both,
// calibration counts negatives down from 4096, the sensor enum lists its
// values, the setpoint range is 5-90, local_temperature follows z2m per ID,
// every ID has the DP101 program, and pairing sends the magic packet and
// answers the MCU clock in the 1970 epoch.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "definitions/tuya/_shared.hpp"
#include "zhc/devices/moes_registry.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::moes {
extern const PreparedDefinition kDef_BHT_002;           // aoclfnxz, u9bfwha0
extern const PreparedDefinition kDef_BHT_002_5toc8efa;  // DP16-26 in tenths
extern const PreparedDefinition kDef_BHT_002_rawtemp;   // ztvwu4nk, ye5jkfsb
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

// ── Family parity (z2m v26.105.0) ────────────────────────────────────

namespace {

using devices::moes::kDef_BHT_002;
using devices::moes::kDef_BHT_002_5toc8efa;
using devices::moes::kDef_BHT_002_rawtemp;
using Bytes = std::vector<std::uint8_t>;

const PreparedDefinition* const kFamily[] = {
    &kDef_BHT_002, &kDef_BHT_002_5toc8efa, &kDef_BHT_002_rawtemp,
};

Bytes be32(std::int32_t v) {
    const auto u = static_cast<std::uint32_t>(v);
    return {static_cast<std::uint8_t>(u >> 24), static_cast<std::uint8_t>(u >> 16),
            static_cast<std::uint8_t>(u >> 8), static_cast<std::uint8_t>(u)};
}

// setData frame carrying one datapoint, as tz_tuya_datapoints builds it.
Bytes dp_frame(std::uint8_t dp, std::uint8_t type, const Bytes& value) {
    Bytes f = {0x01, 0x00, 0x00, 0x00, 0x01, dp, type, 0x00,
               static_cast<std::uint8_t>(value.size())};
    f.insert(f.end(), value.begin(), value.end());
    return f;
}

// Decodes one datapoint; `found` is false when `key` was not emitted.
struct Decoded { bool found; Value v; };
Decoded decode_dp(const PreparedDefinition& def, std::uint8_t dp, std::uint8_t type,
                  const Bytes& value, const char* key) {
    const TuyaDpRecord recs[] = {{dp, type, std::span<const std::uint8_t>(value)}};
    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>(recs, 1), def, raw, ctx);
    const Value* v = r.merged.find(key);
    return {v != nullptr, v ? *v : Value{}};
}

bool is_str(const Decoded& d, const char* want) {
    return d.found && d.v.type == ValueType::StringRef && d.v.str && std::strcmp(d.v.str, want) == 0;
}

bool is_float(const Decoded& d, float want) {
    return d.found && d.v.type == ValueType::Float && approx(d.v.f, want, 0.001f);
}

// Encoded setData frame for `key` = `v`; empty when no converter takes it.
Bytes encode(const PreparedDefinition& def, const char* key, const Value& v) {
    RuntimeContext ctx{};
    std::uint8_t frame[64]{};
    const auto r = dispatch_to_zigbee(def, key, v, ctx, frame);
    if (!r.ok) return {};
    assert(r.cluster_id == 0xEF00 && r.command_id == 0x00);
    return Bytes(frame, frame + r.frame_size);
}

Value str_v(const char* s) { Value v{}; v.type = ValueType::StringRef; v.str = s; return v; }
Value int_v(std::int64_t i) { Value v{}; v.type = ValueType::Int; v.i = i; return v; }
Value float_v(float f)      { Value v{}; v.type = ValueType::Float; v.f = f; return v; }

const Expose* find_expose(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (std::strcmp(def.exposes[i].name, name) == 0) return &def.exposes[i];
    return nullptr;
}

}  // namespace

// DP36 moesValve: z2m `running_state: value ? "idle" : "heat"` — true means idle.
static void test_running_state_inverted() {
    for (const auto* def : kFamily) {
        assert(is_str(decode_dp(*def, 36, 0x01, {0x01}, "running_state"), "idle"));
        assert(is_str(decode_dp(*def, 36, 0x01, {0x00}, "running_state"), "heat"));
        const Expose* x = find_expose(*def, "running_state");
        assert(x && x->type == ExposeType::Enum && x->access == Access::State);
        assert(x->enum_count == 2 && std::strcmp(x->enum_values[0], "idle") == 0 &&
               std::strcmp(x->enum_values[1], "heat") == 0);
    }
}

// DP2 moesHold: truthy = program. DP3 moesScheduleEnable: inverted. The
// write sends both as enums (z2m moes_thermostat_mode), here in one frame.
static void test_preset() {
    const Bytes hold    = {0x01, 0x00, 0x00, 0x00, 0x01,
                           0x02, 0x04, 0x00, 0x01, 0x00,    // DP2 = 0
                           0x03, 0x04, 0x00, 0x01, 0x01};   // DP3 = 1
    const Bytes program = {0x01, 0x00, 0x00, 0x00, 0x01,
                           0x02, 0x04, 0x00, 0x01, 0x01,    // DP2 = 1
                           0x03, 0x04, 0x00, 0x01, 0x00};   // DP3 = 0
    for (const auto* def : kFamily) {
        assert(is_str(decode_dp(*def, 2, 0x04, {0x01}, "preset"), "program"));
        assert(is_str(decode_dp(*def, 2, 0x01, {0x00}, "preset"), "hold"));
        assert(is_str(decode_dp(*def, 3, 0x04, {0x01}, "preset"), "hold"));
        assert(is_str(decode_dp(*def, 3, 0x04, {0x00}, "preset"), "program"));
        const Expose* x = find_expose(*def, "preset");
        assert(x && x->type == ExposeType::Enum && x->access == Access::StateSet &&
               x->enum_count == 2);
        assert(encode(*def, "preset", str_v("hold")) == hold);
        assert(encode(*def, "preset", str_v("program")) == program);
        assert(encode(*def, "preset", str_v("auto")).empty());
    }
}

// DP27: z2m decodes `v > 4000 ? v - 4096 : v` and writes `v < 0 ? 4096 + v : v`.
static void test_calibration() {
    for (const auto* def : kFamily) {
        const auto neg = decode_dp(*def, 27, 0x02, be32(4094), "local_temperature_calibration");
        assert(neg.found && neg.v.type == ValueType::Int && neg.v.i == -2);
        const auto pos = decode_dp(*def, 27, 0x02, be32(3), "local_temperature_calibration");
        assert(pos.found && pos.v.type == ValueType::Int && pos.v.i == 3);
        assert(encode(*def, "local_temperature_calibration", int_v(-2)) == dp_frame(27, 0x02, be32(4094)));
        assert(encode(*def, "local_temperature_calibration", float_v(-3.0f)) == dp_frame(27, 0x02, be32(4093)));
        assert(encode(*def, "local_temperature_calibration", int_v(5)) == dp_frame(27, 0x02, be32(5)));
        const Expose* x = find_expose(*def, "local_temperature_calibration");
        assert(x && x->access == Access::StateSet && x->value_min == -30 && x->value_max == 30);
    }
}

// Sensor enum carries its values (the UI had no options), config category;
// setpoint 5-90 (26.105.0) and z2m's limit ranges.
static void test_exposes() {
    for (const auto* def : kFamily) {
        const Expose* s = find_expose(*def, "sensor");
        assert(s && s->type == ExposeType::Enum && s->access == Access::StateSet &&
               s->category == ExposeCategory::Config && s->enum_count == 3);
        assert(std::strcmp(s->enum_values[0], "IN") == 0 && std::strcmp(s->enum_values[1], "AL") == 0 &&
               std::strcmp(s->enum_values[2], "OU") == 0);
        assert(encode(*def, "sensor", str_v("AL")) == dp_frame(43, 0x04, {0x01}));

        const Expose* sp = find_expose(*def, "current_heating_setpoint");
        assert(sp && sp->access == Access::StateSet && sp->value_min == 5 && sp->value_max == 90);
        // The 5toc8efa pair carries tenths (z2m: 0.5 steps on _TZE204_), the rest whole degrees.
        assert(sp->value_step == (def == &kDef_BHT_002_5toc8efa ? 0 : 1));
        const Expose* mx = find_expose(*def, "max_temperature_limit");
        assert(mx && mx->value_min == 0 && mx->value_max == 80);
        const Expose* mn = find_expose(*def, "min_temperature_limit");
        assert(mn && mn->value_min == 1 && mn->value_max == 5);
        const Expose* dz = find_expose(*def, "deadzone_temperature");
        assert(dz && dz->value_min == 0 && dz->value_max == 5 && dz->value_step == 1);
        assert(find_expose(*def, "program") && find_expose(*def, "local_temperature"));
        assert(std::strcmp(def->model, "BHT-002") == 0 && std::strcmp(def->vendor, "Moes") == 0);
    }
}

// Setpoint and limits: tenths on 5toc8efa, whole degrees elsewhere.
static void test_setpoint_scaling() {
    assert(is_float(decode_dp(kDef_BHT_002_5toc8efa, 16, 0x02, be32(215), "current_heating_setpoint"), 21.5f));
    assert(encode(kDef_BHT_002_5toc8efa, "current_heating_setpoint", float_v(21.5f)) == dp_frame(16, 0x02, be32(215)));
    for (const auto* def : {&kDef_BHT_002, &kDef_BHT_002_rawtemp}) {
        const auto sp = decode_dp(*def, 16, 0x02, be32(21), "current_heating_setpoint");
        assert(sp.found && sp.v.type == ValueType::Int && sp.v.i == 21);
        assert(encode(*def, "current_heating_setpoint", int_v(21)) == dp_frame(16, 0x02, be32(21)));
    }
}

// DP24 per z2m: 5toc8efa ÷10; the rest wrap 16-bit negatives the z2m way
// (`v - 65536 + 1`), then ÷10 except ztvwu4nk / ye5jkfsb (raw); ≥ 100 °C dropped.
static void test_local_temperature_groups() {
    auto temp = [](const PreparedDefinition& def, std::int32_t raw) {
        return decode_dp(def, 24, 0x02, be32(raw), "local_temperature");
    };
    assert(is_float(temp(kDef_BHT_002, 235), 23.5f));
    assert(is_float(temp(kDef_BHT_002, 0xFFF6), -0.9f));
    assert(!temp(kDef_BHT_002, 1000).found);
    assert(is_float(temp(kDef_BHT_002_5toc8efa, 235), 23.5f));
    assert(!temp(kDef_BHT_002_5toc8efa, 0xFFF6).found);         // 6552.6 → dropped, no wrap
    assert(is_float(temp(kDef_BHT_002_rawtemp, 23), 23.0f));     // was 2.3 (÷10)
    assert(is_float(temp(kDef_BHT_002_rawtemp, 0xFFFE), -1.0f));
    assert(!temp(kDef_BHT_002_rawtemp, 235).found);
}

// DP101 weekly program on every group (was aoclfnxz only).
static void test_program_everywhere() {
    for (const auto* def : kFamily) {
        const Bytes sched(kSchedule36, kSchedule36 + 36);
        const TuyaDpRecord recs[] = {{101, 0x00, std::span<const std::uint8_t>(sched)}};
        auto msg = make_msg();
        auto raw = make_raw();
        RuntimeContext ctx{};
        const auto r = dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>(recs, 1), *def, raw, ctx);
        const Value* p = r.merged.find("program");
        assert(p && p->type == ValueType::StringRef && std::strcmp(p->str, kScheduleStr) == 0);
        const Bytes f = encode(*def, "program", str_v(kScheduleStr));
        assert(f.size() == 45 && std::memcmp(f.data() + 9, kSchedule36, 36) == 0);
    }
}

// Pairing: tuyaBase magic packet; the MCU clock answered in the 1970 epoch.
static void test_configure() {
    const std::uint8_t kMagic[] = {0x04, 0x00, 0x00, 0x00, 0x01, 0x00,
                                   0x05, 0x00, 0x07, 0x00, 0xFE, 0xFF};
    for (const auto* def : kFamily) {
        assert(def->tuya_time_start == 1);
        assert(def->config_steps_count == 1);
        const ConfigStep& s = def->config_steps[0];
        assert(s.op == ConfigStepOp::Read && s.cluster_id == 0x0000);
        assert(s.payload_len == sizeof(kMagic) && std::memcmp(s.payload, kMagic, sizeof(kMagic)) == 0);
    }
}

// Every z2m fingerprint lands on its group over the tuya + moes registries in
// adapter order; `_TZE200_ztvwu4nk` is no longer taken by a Tuya fan-coil def.
static void test_matcher() {
    std::vector<const PreparedDefinition*> merged(devices::tuya::kTuyaRegistry,
        devices::tuya::kTuyaRegistry + devices::tuya::kTuyaRegistryCount);
    merged.insert(merged.end(), devices::moes::kMoesRegistry,
                  devices::moes::kMoesRegistry + devices::moes::kMoesRegistryCount);
    const std::span<const PreparedDefinition* const> reg(merged.data(), merged.size());
    const struct { const char* manu; const PreparedDefinition* def; } kIds[] = {
        {"_TZE200_aoclfnxz", &kDef_BHT_002},          {"_TZE204_aoclfnxz", &kDef_BHT_002},
        {"_TZE200_u9bfwha0", &kDef_BHT_002},          {"_TZE204_u9bfwha0", &kDef_BHT_002},
        {"_TZE200_5toc8efa", &kDef_BHT_002_5toc8efa}, {"_TZE204_5toc8efa", &kDef_BHT_002_5toc8efa},
        {"_TZE200_ztvwu4nk", &kDef_BHT_002_rawtemp},  {"_TZE200_ye5jkfsb", &kDef_BHT_002_rawtemp},
        {"_TZE284_ye5jkfsb", &kDef_BHT_002_rawtemp},
    };
    for (const auto& id : kIds) assert(find_definition("TS0601", id.manu, reg) == id.def);
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
    test_running_state_inverted();
    test_preset();
    test_calibration();
    test_exposes();
    test_setpoint_scaling();
    test_local_temperature_groups();
    test_program_everywhere();
    test_configure();
    test_matcher();
    return 0;
}
