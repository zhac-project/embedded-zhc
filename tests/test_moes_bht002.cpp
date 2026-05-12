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

int main() {
    test_local_temperature_decode();
    test_child_lock_decode();
    test_sensor_enum_decode();
    test_multi_dp_frame_decodes_all_keys();
    return 0;
}
