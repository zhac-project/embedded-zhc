// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the GiEX water-irrigation valves QT06_1 (TS0601 /
// _TZE200_sh1btabb) and QT06_2 (TS0601 / 5 _TZE2xx variants) — Tuya-MCU
// (0xEF00 manuSpecificTuya) DP-stream devices.
//
// Bug fixed: the auto extract ported both as bare kFzBattery + kFzOnOff
// stubs binding ZCL clusters 0x0001/0x0006 the devices never speak, and
// dropping 8 of z2m's irrigation datapoints (mode, irrigation_target,
// cycle counts/interval, start/end time, last duration, water_consumed) —
// leaving only a dead state/battery/voltage surface. z2m decodes everything
// through legacy.fromZigbee.giexWaterValve, a 0xEF00 DP map. These were
// graduated to Tier-2 overrides wiring fz_tuya_datapoints + tz_tuya_datapoints
// with the full DP map.
//
// SUSPECT (water_consumed float divisor / per-DP scale) confirmed FALSE:
// z2m's legacy converter passes every value DP straight through with NO
// scaling, so water_consumed / battery / irrigation_target decode raw (Int).
// DP 106 (currentTemperature) is intentionally NOT mapped — z2m ignores it.
//
// z2m-source: devices/giex.ts + lib/legacy.ts giexWaterValve.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/giex_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> giex_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::giex::kGiexRegistry, devices::giex::kGiexRegistryCount);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) {
            return true;
        }
    }
    return false;
}

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

DispatchResult dispatch_dp(const PreparedDefinition& def,
                           std::span<const TuyaDpRecord> recs) {
    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, recs, def, raw, ctx);
}

const PreparedDefinition* qt06_1() {
    const auto* d = find_definition("TS0601", "_TZE200_sh1btabb", giex_registry());
    assert(d && "_TZE200_sh1btabb must resolve to the GiEX QT06_1 def");
    return d;
}

const PreparedDefinition* qt06_2() {
    const auto* d = find_definition("TS0601", "_TZE204_7ytb3h8u", giex_registry());
    assert(d && "_TZE204_7ytb3h8u must resolve to the GiEX QT06_2 def");
    return d;
}

}  // namespace

// Both manufacturer fingerprints resolve to full DP defs carrying a Tuya DP
// from_zigbee + to_zigbee converter (not a battery-only / on-off stub).
static void test_manu_resolves_to_full_dp_def() {
    const auto* d1 = qt06_1();
    assert(d1->model && std::strcmp(d1->model, "QT06_1") == 0);
    assert(d1->vendor && std::strcmp(d1->vendor, "Giex") == 0);
    assert(d1->from_zigbee && d1->from_zigbee_count >= 2);  // sync-time + DP map
    assert(d1->to_zigbee && d1->to_zigbee_count >= 1);

    const auto* d2 = qt06_2();
    assert(d2->model && std::strcmp(d2->model, "QT06_2") == 0);
    assert(d2->from_zigbee && d2->from_zigbee_count >= 2);
    assert(d2->to_zigbee && d2->to_zigbee_count >= 1);

    // QT06_2 keeps all five manufacturer-name variants in its fingerprint.
    assert(find_definition("TS0601", "_TZE200_a7sghmms", giex_registry()) == d2);
    assert(find_definition("TS0601", "_TZE284_7ytb3h8u", giex_registry()) == d2);
}

// The restored irrigation channels are declared as exposes; the phantom
// `voltage` of the stub is gone.
static void test_restored_exposes_declared() {
    const auto* d = qt06_1();
    assert(def_exposes(*d, "state"));
    assert(def_exposes(*d, "mode"));
    assert(def_exposes(*d, "cycle_irrigation_num_times"));
    assert(def_exposes(*d, "irrigation_start_time"));
    assert(def_exposes(*d, "irrigation_end_time"));
    assert(def_exposes(*d, "last_irrigation_duration"));
    assert(def_exposes(*d, "water_consumed"));
    assert(def_exposes(*d, "battery"));
    assert(def_exposes(*d, "irrigation_target"));
    assert(def_exposes(*d, "cycle_irrigation_interval"));
    // The auto stub's phantom electrical channel must be gone.
    assert(!def_exposes(*d, "voltage"));
}

// DP 2 state = bool -> "ON"/"OFF" (z2m e.binary publishes the string).
static void test_state_dp2_bool_enum() {
    const auto* d = qt06_1();

    const std::uint8_t kOn[] = { 0x01 };
    const TuyaDpRecord recsOn[] = { { 2, 0x01, std::span<const std::uint8_t>(kOn, 1) } };
    const auto rOn = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsOn, 1));
    assert(rOn.any_matched);
    const Value* sOn = rOn.merged.find("state");
    assert(sOn && sOn->type == ValueType::StringRef);
    assert(sOn->str && std::strcmp(sOn->str, "ON") == 0);

    const std::uint8_t kOff[] = { 0x00 };
    const TuyaDpRecord recsOff[] = { { 2, 0x01, std::span<const std::uint8_t>(kOff, 1) } };
    const auto rOff = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsOff, 1));
    assert(rOff.any_matched);
    const Value* sOff = rOff.merged.find("state");
    assert(sOff && sOff->type == ValueType::StringRef);
    assert(sOff->str && std::strcmp(sOff->str, "OFF") == 0);
}

// DP 1 mode = bool -> "duration"/"capacity" (z2m value ? CAPACITY : DURATION).
static void test_mode_dp1_bool_enum() {
    const auto* d = qt06_1();

    const std::uint8_t kCap[] = { 0x01 };
    const TuyaDpRecord recsCap[] = { { 1, 0x01, std::span<const std::uint8_t>(kCap, 1) } };
    const auto rCap = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsCap, 1));
    assert(rCap.any_matched);
    const Value* mCap = rCap.merged.find("mode");
    assert(mCap && mCap->type == ValueType::StringRef);
    assert(mCap->str && std::strcmp(mCap->str, "capacity") == 0);

    const std::uint8_t kDur[] = { 0x00 };
    const TuyaDpRecord recsDur[] = { { 1, 0x01, std::span<const std::uint8_t>(kDur, 1) } };
    const auto rDur = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsDur, 1));
    assert(rDur.any_matched);
    const Value* mDur = rDur.merged.find("mode");
    assert(mDur && mDur->type == ValueType::StringRef);
    assert(mDur->str && std::strcmp(mDur->str, "duration") == 0);
}

// Numeric DPs are RAW pass-through (divisor 1 -> Int) — water_consumed,
// battery, irrigation_target. Confirms the SUSPECT divisor is FALSE: 1234 L
// decodes to 1234 (not 12.34 or 1.234).
static void test_numeric_dps_raw_passthrough() {
    const auto* d = qt06_1();
    // water_consumed (DP 111) = 1234 = 0x000004D2.
    const std::uint8_t kWater[] = { 0x00, 0x00, 0x04, 0xD2 };
    // battery (DP 108) = 87 % = 0x00000057.
    const std::uint8_t kBatt[] = { 0x00, 0x00, 0x00, 0x57 };
    // irrigation_target (DP 104) = 600 = 0x00000258.
    const std::uint8_t kTarget[] = { 0x00, 0x00, 0x02, 0x58 };
    // cycle_irrigation_num_times (DP 103) = 3 = 0x00000003.
    const std::uint8_t kNum[] = { 0x00, 0x00, 0x00, 0x03 };
    // cycle_irrigation_interval (DP 105) = 5 = 0x00000005.
    const std::uint8_t kIvl[] = { 0x00, 0x00, 0x00, 0x05 };
    const TuyaDpRecord recs[] = {
        { 111, 0x02, std::span<const std::uint8_t>(kWater, 4) },
        { 108, 0x02, std::span<const std::uint8_t>(kBatt, 4) },
        { 104, 0x02, std::span<const std::uint8_t>(kTarget, 4) },
        { 103, 0x02, std::span<const std::uint8_t>(kNum, 4) },
        { 105, 0x02, std::span<const std::uint8_t>(kIvl, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 5));
    assert(result.any_matched);

    const Value* water = result.merged.find("water_consumed");
    assert(water && water->type == ValueType::Int && water->i == 1234);

    const Value* batt = result.merged.find("battery");
    assert(batt && batt->type == ValueType::Int && batt->i == 87);

    const Value* tgt = result.merged.find("irrigation_target");
    assert(tgt && tgt->type == ValueType::Int && tgt->i == 600);

    const Value* num = result.merged.find("cycle_irrigation_num_times");
    assert(num && num->type == ValueType::Int && num->i == 3);

    const Value* ivl = result.merged.find("cycle_irrigation_interval");
    assert(ivl && ivl->type == ValueType::Int && ivl->i == 5);
}

// String DPs (101 irrigation_start_time, 102 irrigation_end_time, 114
// last_irrigation_duration) surface as BytesRef borrowing the wire bytes
// (Tuya strings carry no NUL).
static void test_string_dps_surface() {
    const auto* d = qt06_2();
    // "06:30:00" -> 8 ASCII bytes.
    const std::uint8_t kStart[] = { '0','6',':','3','0',':','0','0' };
    const TuyaDpRecord recs[] = {
        { 101, 0x03, std::span<const std::uint8_t>(kStart, 8) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* st = result.merged.find("irrigation_start_time");
    assert(st && st->type == ValueType::BytesRef);
    assert(st->bytes.size() == 8);
    assert(std::memcmp(st->bytes.data(), kStart, 8) == 0);
}

// DP 106 (currentTemperature) is intentionally unmapped — a frame carrying
// only DP 106 produces no output (z2m ignores it).
static void test_dp106_ignored() {
    const auto* d = qt06_1();
    const std::uint8_t kTemp[] = { 0x00, 0x00, 0x00, 0x19 };
    const TuyaDpRecord recs[] = {
        { 106, 0x02, std::span<const std::uint8_t>(kTemp, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.merged.find("current_temperature") == nullptr);
    assert(result.merged.find("temperature") == nullptr);
}

int main() {
    test_manu_resolves_to_full_dp_def();
    test_restored_exposes_declared();
    test_state_dp2_bool_enum();
    test_mode_dp1_bool_enum();
    test_numeric_dps_raw_passthrough();
    test_string_dps_surface();
    test_dp106_ignored();
    return 0;
}
