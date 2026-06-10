// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Essentials "Thermostat Zigbee smart home"
// (TS0601 / _TZE200_i48qyn9s) — a pure Tuya-datapoint thermostat reporting
// on cluster 0xEF00 (manuSpecificTuya) cmd 0x02.
//
// Bug fixed: the auto extract kept only 7 of z2m's 18 active datapoints,
// dropping the PRIMARY setpoint (DP 16 current_heating_setpoint, divideBy 2)
// and the open-window detection channels (DP 107 window_open, DP 116
// open_window_temperature, DP 117 detect_window_time_minute). The thermostat
// therefore exposed a system_mode but had no settable/readable target
// temperature. These DPs are now mapped with z2m-matching scale; window_open
// fans the boolean DP to a "YES"/"NO" string via kTuyaDpFlagBoolEnum.
//
// These tests pin (a) the manufacturer name resolves through find_definition
// to the full DP def, (b) the restored exposes are declared, (c) the restored
// datapoints decode on the real wire with the correct z2m scale, and (d) the
// pre-existing local_temperature / signed-calibration channels still decode.
//
// z2m-source: devices/essentials.ts #_TZE200_i48qyn9s.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/essentials_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> essentials_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::essentials::kEssentialsRegistry,
        devices::essentials::kEssentialsRegistryCount);
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

const PreparedDefinition* ess_def() {
    const auto* d = find_definition("TS0601", "_TZE200_i48qyn9s",
                                    essentials_registry());
    assert(d && "_TZE200_i48qyn9s must resolve to the Essentials thermostat def");
    return d;
}

}  // namespace

// The manufacturer name resolves to the full DP def, carrying both a Tuya DP
// from_zigbee + to_zigbee converter (not a battery-only / on-off stub).
static void test_manu_resolves_to_full_dp_def() {
    const auto* d = ess_def();
    assert(d->model && std::strcmp(d->model, "TS0601__TZE200_i48qyn9s") == 0);
    assert(d->vendor && std::strcmp(d->vendor, "Essentials") == 0);
    assert(d->from_zigbee && d->from_zigbee_count >= 1);
    assert(d->to_zigbee && d->to_zigbee_count >= 1);
}

// The restored channels are declared as exposes alongside the pre-existing
// climate surface.
static void test_restored_exposes_declared() {
    const auto* d = ess_def();
    assert(def_exposes(*d, "current_heating_setpoint"));
    assert(def_exposes(*d, "window_open"));
    assert(def_exposes(*d, "open_window_temperature"));
    assert(def_exposes(*d, "detect_window_time_minute"));
    // Pre-existing surface still present.
    assert(def_exposes(*d, "system_mode"));
    assert(def_exposes(*d, "local_temperature"));
    assert(def_exposes(*d, "local_temperature_calibration"));
}

// Core restored fix: DP 16 current_heating_setpoint decodes /2 (z2m
// valueConverterBasic.divideBy(2)). Raw 41 -> 20.5 °C.
static void test_setpoint_dp16_divide_by_2() {
    const auto* d = ess_def();
    // 41 = 0x00000029 -> /2 -> 20.5 °C.
    const std::uint8_t kSp41[] = { 0x00, 0x00, 0x00, 0x29 };
    const TuyaDpRecord recs[] = {
        { 16, 0x02, std::span<const std::uint8_t>(kSp41, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* sp = result.merged.find("current_heating_setpoint");
    assert(sp && sp->type == ValueType::Float);
    assert(sp->f > 20.49f && sp->f < 20.51f);
}

// DP 24 local_temperature /10 and DP 104 local_temperature_calibration /10 with
// signed int32 sign-handling (z2m localTempCalibration1; ez reads the 4-byte BE
// numeric as a signed int32 so a negative calibration arrives directly).
static void test_local_temp_and_signed_calibration() {
    const auto* d = ess_def();
    // 235 = 0x000000EB -> /10 -> 23.5 °C local temperature.
    const std::uint8_t kLt235[] = { 0x00, 0x00, 0x00, 0xEB };
    // -25 as signed BE int32 = 0xFFFFFFE7 -> /10 -> -2.5 °C calibration.
    const std::uint8_t kCalNeg25[] = { 0xFF, 0xFF, 0xFF, 0xE7 };
    const TuyaDpRecord recs[] = {
        { 24,  0x02, std::span<const std::uint8_t>(kLt235, 4) },
        { 104, 0x02, std::span<const std::uint8_t>(kCalNeg25, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 2));
    assert(result.any_matched);

    const Value* lt = result.merged.find("local_temperature");
    assert(lt && lt->type == ValueType::Float);
    assert(lt->f > 23.49f && lt->f < 23.51f);

    const Value* cal = result.merged.find("local_temperature_calibration");
    assert(cal && cal->type == ValueType::Float);
    assert(cal->f > -2.51f && cal->f < -2.49f);
}

// DP 107 window_open fans a boolean DP to a "YES"/"NO" string label
// (z2m lookup{YES: true, NO: false}).
static void test_window_open_bool_enum() {
    const auto* d = ess_def();

    const std::uint8_t kBoolOn[] = { 0x01 };
    const TuyaDpRecord recsYes[] = {
        { 107, 0x01, std::span<const std::uint8_t>(kBoolOn, 1) },
    };
    const auto rYes = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsYes, 1));
    assert(rYes.any_matched);
    const Value* wYes = rYes.merged.find("window_open");
    assert(wYes && wYes->type == ValueType::StringRef);
    assert(wYes->str && std::strcmp(wYes->str, "YES") == 0);

    const std::uint8_t kBoolOff[] = { 0x00 };
    const TuyaDpRecord recsNo[] = {
        { 107, 0x01, std::span<const std::uint8_t>(kBoolOff, 1) },
    };
    const auto rNo = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsNo, 1));
    assert(rNo.any_matched);
    const Value* wNo = rNo.merged.find("window_open");
    assert(wNo && wNo->type == ValueType::StringRef);
    assert(wNo->str && std::strcmp(wNo->str, "NO") == 0);
}

// DP 116 open_window_temperature /2 and DP 117 detect_window_time_minute raw.
static void test_open_window_channels() {
    const auto* d = ess_def();
    // 30 = 0x0000001E -> /2 -> 15.0 °C open-window temperature.
    const std::uint8_t kOwt30[] = { 0x00, 0x00, 0x00, 0x1E };
    // 12 minutes, raw pass-through (divisor 1 -> Int).
    const std::uint8_t kMin12[] = { 0x00, 0x00, 0x00, 0x0C };
    const TuyaDpRecord recs[] = {
        { 116, 0x02, std::span<const std::uint8_t>(kOwt30, 4) },
        { 117, 0x02, std::span<const std::uint8_t>(kMin12, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 2));
    assert(result.any_matched);

    const Value* owt = result.merged.find("open_window_temperature");
    assert(owt && owt->type == ValueType::Float);
    assert(owt->f > 14.99f && owt->f < 15.01f);

    const Value* mins = result.merged.find("detect_window_time_minute");
    assert(mins && mins->type == ValueType::Int);
    assert(mins->i == 12);
}

int main() {
    test_manu_resolves_to_full_dp_def();
    test_restored_exposes_declared();
    test_setpoint_dp16_divide_by_2();
    test_local_temp_and_signed_calibration();
    test_window_open_bool_enum();
    test_open_window_channels();
    return 0;
}
