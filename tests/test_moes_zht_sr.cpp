// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Behavioural-parity fixture tests for Moes ZHT-SR smart-ring thermostat.
//
// Goal: prove ZHC's decode output matches z2m for the wire frames a real
// ZHT-SR emits via the Tuya DP stream. Three representative datapoints
// are exercised:
//
//   * dp_id 2    preset                          Enum  → "Eco" (raw=3)
//   * dp_id 101  local_temperature_calibration   Numeric, divisor 256
//                  raw=256                       → 1.0 (Float)
//   * dp_id 115  rgb_light                       Bool  → true
//
// z2m-source: zigbee-herdsman-converters/src/devices/moes.ts #ZHT-SR
//             (line ~1539, fingerprint TS0601 / _TZE204_lpedvtvr,
//             meta.tuyaDatapoints lines ~1605-1635).

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::moes {
extern const PreparedDefinition kDef_ZHT_SR;
}

using namespace zhc;

namespace {

bool approx(float a, float b, float eps) {
    return std::fabs(a - b) <= eps;
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

}  // namespace

// dp_id=2 (preset) Enum raw=3 → "Eco" per z2m's lookup
//   { Manual:0, "Temporary manual":1, Program:2, Eco:3 }.
static void test_preset_enum_eco() {
    const std::uint8_t kEnum3[] = { 0x03 };
    const TuyaDpRecord recs[] = {
        { 2, 0x04, std::span<const std::uint8_t>(kEnum3, 1) },
    };

    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1),
        devices::moes::kDef_ZHT_SR, raw, ctx);
    assert(result.any_matched);

    const Value* p = result.merged.find("preset");
    assert(p && p->type == ValueType::StringRef);
    assert(p->str && std::strcmp(p->str, "Eco") == 0);
}

// dp_id=101 (local_temperature_calibration) Numeric raw=256 with
// divisor=256 → 1.0 °C. z2m's special-case scaling for ZHT-SR
// (divisor 256 instead of the usual 10) is the load-bearing detail.
static void test_local_temperature_calibration_div256() {
    // Big-endian 4-byte numeric: 256 = 0x00000100.
    const std::uint8_t kRaw256[] = { 0x00, 0x00, 0x01, 0x00 };
    const TuyaDpRecord recs[] = {
        { 101, 0x02, std::span<const std::uint8_t>(kRaw256, 4) },
    };

    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1),
        devices::moes::kDef_ZHT_SR, raw, ctx);
    assert(result.any_matched);

    const Value* cal = result.merged.find("local_temperature_calibration");
    assert(cal && cal->type == ValueType::Float);
    assert(approx(cal->f, 1.0f, 0.001f));
}

// dp_id=115 (rgb_light) Bool=true → rgb_light=true.
// The DP map declares this as Bool (no enum lookup) so ZHC emits the
// raw boolean — same wire shape z2m's `tuya.valueConverterBasic.lookup
// ({ON:true, OFF:false})` produces before the friendly-name pass.
static void test_rgb_light_bool_true() {
    const std::uint8_t kBoolOn[] = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 115, 0x01, std::span<const std::uint8_t>(kBoolOn, 1) },
    };

    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1),
        devices::moes::kDef_ZHT_SR, raw, ctx);
    assert(result.any_matched);

    const Value* rgb = result.merged.find("rgb_light");
    assert(rgb && rgb->type == ValueType::Bool);
    assert(rgb->b == true);
}

// Bonus: dp_id=16 (local_temperature) divisor 10 → 21.5 from raw=215.
// Catches the regression that originally motivated the Phase-2 cleanup
// (the previous port emitted bogus `state=true` for any frame).
static void test_local_temperature_div10() {
    const std::uint8_t kRaw215[] = { 0x00, 0x00, 0x00, 0xD7 };
    const TuyaDpRecord recs[] = {
        { 16, 0x02, std::span<const std::uint8_t>(kRaw215, 4) },
    };

    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1),
        devices::moes::kDef_ZHT_SR, raw, ctx);
    assert(result.any_matched);

    const Value* lt = result.merged.find("local_temperature");
    assert(lt && lt->type == ValueType::Float);
    assert(approx(lt->f, 21.5f, 0.001f));

    // Make sure no ghost `state` key leaked through (the bug fix).
    const Value* s = result.merged.find("state");
    assert(s == nullptr);
}

int main() {
    test_preset_enum_eco();
    test_local_temperature_calibration_div256();
    test_rgb_light_bool_true();
    test_local_temperature_div10();
    return 0;
}
