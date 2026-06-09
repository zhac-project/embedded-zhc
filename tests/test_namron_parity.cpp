// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Namron 4512752/4512753 "Touch thermostat 16A 2.0"
// (Tuya TS0601 / _TZE204_p3lqqy2r), a pure Tuya-datapoint thermostat that
// reports on cluster 0xEF00 (manuSpecificTuya) cmd 0x02.
//
// Bug fixed: the namron registry held TWO defs claiming the identical
// fingerprint (zigbeeModel "TS0601" + manufacturer "_TZE204_p3lqqy2r"):
//   * kDefNam__TZE204_p3lqqy2r — the full port, wiring fz_tuya_datapoints
//     over the device's 17-DP map (setpoint / local_temperature / mode /
//     sensor / child_lock / metering / …).
//   * kDef_D4512752_4512753   — a second generated def that lowered ONLY
//     kFzMetering + kFzElectricalMeasurement and NO Tuya DP decoder, so it
//     dropped the thermostat's entire core surface.
// Both matched TS0601 + _TZE204_p3lqqy2r, so the device's behaviour hung on
// registry order (find_definition is first-match within the Tuya-styled
// pass). The metering-only stub was removed and the full-DP def graduated to
// definitions/namron/Nam__TZE204_p3lqqy2r.cpp as the sole match.
//
// These tests pin, on real Tuya DP wire shapes routed through the full
// dispatcher against the registered def, that the thermostat's core
// datapoints decode to their semantic keys — i.e. the surviving def is the
// full-DP port, not the metering-only stub (which would surface none of
// these keys).
//
// z2m-source: namron.ts #4512752/4512753 — tuya.modernExtend.tuyaBase({dp:true})
// with the per-DP map (dp 16 setpoint, 24 local_temperature, 30 child_lock,
// 102 sensor enum, 108 mode enum).

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::namron {
extern const PreparedDefinition kDefNam__TZE204_p3lqqy2r;
}  // namespace zhc::devices::namron

using namespace zhc;

namespace {

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

DispatchResult dispatch_dp(std::span<const TuyaDpRecord> recs) {
    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    return dispatch_from_zigbee(
        msg, recs, devices::namron::kDefNam__TZE204_p3lqqy2r, raw, ctx);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) {
            return true;
        }
    }
    return false;
}

}  // namespace

// dp 24 localTemperature, Numeric divisor 1 → passes through as Int (raw °C).
// The surviving def must surface "local_temperature"; the removed
// metering-only stub never decoded any DP.
static void test_local_temperature_decode() {
    // Big-endian 4-byte numeric: 22 = 0x00000016.
    const std::uint8_t kRaw22[] = { 0x00, 0x00, 0x00, 0x16 };
    const TuyaDpRecord recs[] = {
        { 24, 0x02, std::span<const std::uint8_t>(kRaw22, 4) },
    };
    const auto result = dispatch_dp(std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);

    const Value* lt = result.merged.find("local_temperature");
    assert(lt && lt->type == ValueType::Int && lt->i == 22);
}

// dp 16 currentHeatingSetpoint, Numeric divisor 1 → Int.
static void test_heating_setpoint_decode() {
    const std::uint8_t kRaw21[] = { 0x00, 0x00, 0x00, 0x15 };  // 21
    const TuyaDpRecord recs[] = {
        { 16, 0x02, std::span<const std::uint8_t>(kRaw21, 4) },
    };
    const auto result = dispatch_dp(std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);

    const Value* sp = result.merged.find("current_heating_setpoint");
    assert(sp && sp->type == ValueType::Int && sp->i == 21);
}

// dp 30 childLock, Bool=true → child_lock=true.
static void test_child_lock_decode() {
    const std::uint8_t kBoolOn[] = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 30, 0x01, std::span<const std::uint8_t>(kBoolOn, 1) },
    };
    const auto result = dispatch_dp(std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);

    const Value* cl = result.merged.find("child_lock");
    assert(cl && cl->type == ValueType::Bool && cl->b == true);
}

// dp 108 mode, Enum=1 → mode="thermostat" (lookup {regulator:0, thermostat:1}).
static void test_mode_enum_decode() {
    const std::uint8_t kEnum1[] = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 108, 0x04, std::span<const std::uint8_t>(kEnum1, 1) },
    };
    const auto result = dispatch_dp(std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);

    const Value* m = result.merged.find("mode");
    assert(m && m->type == ValueType::StringRef);
    assert(m->str && std::strcmp(m->str, "thermostat") == 0);
}

// dp 102 sensor, Enum=1 → sensor="floor_sensor"
// (lookup {air_sensor:0, floor_sensor:1, both:2}).
static void test_sensor_enum_decode() {
    const std::uint8_t kEnum1[] = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 102, 0x04, std::span<const std::uint8_t>(kEnum1, 1) },
    };
    const auto result = dispatch_dp(std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);

    const Value* s = result.merged.find("sensor");
    assert(s && s->type == ValueType::StringRef);
    assert(s->str && std::strcmp(s->str, "floor_sensor") == 0);
}

// Multi-DP frame: setpoint + local_temperature + mode in one report all
// decode — the wire shape z2m sees when the thermostat batches DPs.
static void test_multi_dp_frame_decodes_all_keys() {
    const std::uint8_t kSp[]   = { 0x00, 0x00, 0x00, 0x17 };  // setpoint 23
    const std::uint8_t kLt[]   = { 0x00, 0x00, 0x00, 0x14 };  // local_temp 20
    const std::uint8_t kEnum0[] = { 0x00 };                   // mode=regulator
    const TuyaDpRecord recs[] = {
        { 16,  0x02, std::span<const std::uint8_t>(kSp, 4) },
        { 24,  0x02, std::span<const std::uint8_t>(kLt, 4) },
        { 108, 0x04, std::span<const std::uint8_t>(kEnum0, 1) },
    };
    const auto result = dispatch_dp(std::span<const TuyaDpRecord>(recs, 3));
    assert(result.any_matched);

    const Value* sp = result.merged.find("current_heating_setpoint");
    assert(sp && sp->type == ValueType::Int && sp->i == 23);
    const Value* lt = result.merged.find("local_temperature");
    assert(lt && lt->type == ValueType::Int && lt->i == 20);
    const Value* m = result.merged.find("mode");
    assert(m && m->type == ValueType::StringRef &&
           std::strcmp(m->str, "regulator") == 0);
}

// Guard: the surviving def is the full-DP thermostat, declaring the core
// thermostat exposes. The removed metering-only stub exposed only
// energy/power/voltage/current.
static void test_def_declares_thermostat_exposes() {
    const auto& def = devices::namron::kDefNam__TZE204_p3lqqy2r;
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "mode"));
    assert(def_exposes(def, "sensor"));
    assert(def_exposes(def, "child_lock"));
    // model label preserved as the human-facing pair after graduation.
    assert(def.model && std::strcmp(def.model, "4512752/4512753") == 0);
}

int main() {
    test_local_temperature_decode();
    test_heating_setpoint_decode();
    test_child_lock_decode();
    test_mode_enum_decode();
    test_sensor_enum_decode();
    test_multi_dp_frame_decodes_all_keys();
    test_def_declares_thermostat_exposes();
    return 0;
}
