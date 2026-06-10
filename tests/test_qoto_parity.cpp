// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Qoto QT-05M solar-powered garden watering timer
// (TS0601 / _TZE200_arge1ptm, _TZE200_anv5ujhv, _TZE200_xlppj4f5) — a
// Tuya-MCU (0xEF00 manuSpecificTuya) DP-stream device.
//
// Bug fixed: the auto extract ported it as a bare kFzBattery + kFzOnOff
// stub binding ZCL clusters 0x0001/0x0006 the device never speaks, with
// phantom state/battery/voltage exposes, dropping all 7 of z2m's watering
// datapoints (water_flow, last/remaining duration, valve_state,
// valve_state_auto_shutdown, shutdown_timer, battery). z2m decodes
// everything through legacy.fromZigbee.watering_timer, a 0xEF00 DP map.
// Graduated to a Tier-2 override wiring fz_tuya_datapoints +
// tz_tuya_datapoints with the full DP map.
//
// SUSPECT (valve/battery/duration/countdown float divisor or per-DP scale)
// confirmed FALSE: z2m's legacy converter passes every value DP straight
// through with NO scaling, so each DP decodes raw (Int), divisor 1.
//
// z2m-source: devices/qoto.ts + lib/legacy.ts watering_timer.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/qoto_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> qoto_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::qoto::kQotoRegistry, devices::qoto::kQotoRegistryCount);
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

const PreparedDefinition* qt05m() {
    const auto* d = find_definition("TS0601", "_TZE200_arge1ptm", qoto_registry());
    assert(d && "_TZE200_arge1ptm must resolve to the Qoto QT-05M def");
    return d;
}

}  // namespace

// All three manufacturer fingerprints resolve to one full DP def carrying a
// Tuya DP from_zigbee + to_zigbee converter (not a battery-only / on-off
// stub).
static void test_manu_resolves_to_full_dp_def() {
    const auto* d = qt05m();
    assert(d->model && std::strcmp(d->model, "QT-05M") == 0);
    assert(d->vendor && std::strcmp(d->vendor, "Qoto") == 0);
    assert(d->from_zigbee && d->from_zigbee_count >= 2);  // sync-time + DP map
    assert(d->to_zigbee && d->to_zigbee_count >= 1);

    // All three manufacturer-name variants from the z2m fingerprint resolve.
    assert(find_definition("TS0601", "_TZE200_anv5ujhv", qoto_registry()) == d);
    assert(find_definition("TS0601", "_TZE200_xlppj4f5", qoto_registry()) == d);
}

// The restored watering channels are declared as exposes; the stub's phantom
// state/voltage are gone.
static void test_restored_exposes_declared() {
    const auto* d = qt05m();
    assert(def_exposes(*d, "water_flow"));
    assert(def_exposes(*d, "last_watering_duration"));
    assert(def_exposes(*d, "remaining_watering_time"));
    assert(def_exposes(*d, "valve_state"));
    assert(def_exposes(*d, "valve_state_auto_shutdown"));
    assert(def_exposes(*d, "shutdown_timer"));
    assert(def_exposes(*d, "battery"));
    // The auto stub's phantom genOnOff state + electrical voltage must be gone.
    assert(!def_exposes(*d, "state"));
    assert(!def_exposes(*d, "voltage"));
}

// Every watering DP is a RAW pass-through (divisor 1 -> Int). Confirms the
// SUSPECT divisor is FALSE: e.g. battery 87 % decodes to 87 (not 8 or 0.87),
// shutdown_timer 3600 s to 3600 (not 360 or 36).
static void test_numeric_dps_raw_passthrough() {
    const auto* d = qt05m();
    // valve_state_auto_shutdown (DP 2)  = 50  = 0x00000032.
    const std::uint8_t kAuto[] = { 0x00, 0x00, 0x00, 0x32 };
    // water_flow (DP 3)                 = 75  = 0x0000004B.
    const std::uint8_t kFlow[] = { 0x00, 0x00, 0x00, 0x4B };
    // shutdown_timer (DP 11)            = 3600= 0x00000E10.
    const std::uint8_t kShut[] = { 0x00, 0x00, 0x0E, 0x10 };
    // remaining_watering_time (DP 101)  = 120 = 0x00000078.
    const std::uint8_t kRem[]  = { 0x00, 0x00, 0x00, 0x78 };
    // valve_state (DP 102)              = 100 = 0x00000064.
    const std::uint8_t kValve[]= { 0x00, 0x00, 0x00, 0x64 };
    // last_watering_duration (DP 107)   = 300 = 0x0000012C.
    const std::uint8_t kLast[] = { 0x00, 0x00, 0x01, 0x2C };
    // battery (DP 110)                  = 87  = 0x00000057.
    const std::uint8_t kBatt[] = { 0x00, 0x00, 0x00, 0x57 };
    const TuyaDpRecord recs[] = {
        { 2,   0x02, std::span<const std::uint8_t>(kAuto, 4) },
        { 3,   0x02, std::span<const std::uint8_t>(kFlow, 4) },
        { 11,  0x02, std::span<const std::uint8_t>(kShut, 4) },
        { 101, 0x02, std::span<const std::uint8_t>(kRem, 4) },
        { 102, 0x02, std::span<const std::uint8_t>(kValve, 4) },
        { 107, 0x02, std::span<const std::uint8_t>(kLast, 4) },
        { 110, 0x02, std::span<const std::uint8_t>(kBatt, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 7));
    assert(result.any_matched);

    const Value* autov = result.merged.find("valve_state_auto_shutdown");
    assert(autov && autov->type == ValueType::Int && autov->i == 50);

    const Value* flow = result.merged.find("water_flow");
    assert(flow && flow->type == ValueType::Int && flow->i == 75);

    const Value* shut = result.merged.find("shutdown_timer");
    assert(shut && shut->type == ValueType::Int && shut->i == 3600);

    const Value* rem = result.merged.find("remaining_watering_time");
    assert(rem && rem->type == ValueType::Int && rem->i == 120);

    const Value* valve = result.merged.find("valve_state");
    assert(valve && valve->type == ValueType::Int && valve->i == 100);

    const Value* last = result.merged.find("last_watering_duration");
    assert(last && last->type == ValueType::Int && last->i == 300);

    const Value* batt = result.merged.find("battery");
    assert(batt && batt->type == ValueType::Int && batt->i == 87);
}

// DP 2 maps to valve_state_auto_shutdown only — it must NOT leak onto the
// stub's old phantom keys, and an unmapped DP yields no output.
static void test_unmapped_dp_ignored() {
    const auto* d = qt05m();
    // DP 99 is not in the map.
    const std::uint8_t kJunk[] = { 0x00, 0x00, 0x00, 0x01 };
    const TuyaDpRecord recs[] = {
        { 99, 0x02, std::span<const std::uint8_t>(kJunk, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.merged.find("water_flow") == nullptr);
    assert(result.merged.find("valve_state") == nullptr);
}

int main() {
    test_manu_resolves_to_full_dp_def();
    test_restored_exposes_declared();
    test_numeric_dps_raw_passthrough();
    test_unmapped_dp_ignored();
    return 0;
}
