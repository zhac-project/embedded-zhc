// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Siterwell GS361A-H04 TS0601 radiator-valve thermostat
// (z2m siterwell.ts, legacy.fz.tuya_thermostat) — a Tuya-MCU device reporting
// on cluster 0xEF00 (manuSpecificTuya) via the DP stream.
//
// Bug fixed: the auto-port MISROUTED the decoder. It wired the generic
// genThermostat pair (kFzThermostat / kTzThermostat) and bound ZCL 0x0201,
// but this device never speaks genThermostat — z2m decodes it entirely over
// the 0xEF00 datapoint stream. Every reading (setpoint, local_temperature,
// system_mode, child_lock, window/valve detection, battery, valve position)
// was therefore dead. The def now carries the Tuya DP map with z2m-matching
// per-DP scale and enum order.
//
// These tests pin (a) the def resolves through find_definition and declares
// the real thermostat surface (never a phantom genThermostat shape), and
// (b) the core DPs decode through the dispatcher on the real 0xEF00 wire
// shape with the correct keys/scale/enum mapping.
//
// z2m-source: siterwell.ts #GS361A-H04; lib/legacy.ts dataPoints +
// thermostatSystemModes4 {0:off,1:auto,2:heat}.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/siterwell_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> siterwell_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::siterwell::kSiterwellRegistry,
        devices::siterwell::kSiterwellRegistryCount);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) {
            return true;
        }
    }
    return false;
}

// Inbound Tuya DP report on cluster 0xEF00 cmd 0x02.
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

}  // namespace

// The Siterwell manufacturer names resolve to the GS361A-H04 def and it
// declares the real Tuya-MCU thermostat surface (not a genThermostat shape).
static void test_resolves_to_thermostat_dp_def() {
    const char* manus[] = {
        "_TZE200_zivfvd7h", "_TZE200_kfvq6avy", "_TZE200_lrznf59v",
        "_TZE204_woww89ip",
    };
    for (const char* m : manus) {
        const auto* d = find_definition("TS0601", m, siterwell_registry());
        assert(d && "Siterwell manufacturer name must resolve to a def");
        assert(d->model && std::strcmp(d->model, "GS361A-H04") == 0);
        // Real thermostat surface the genThermostat misroute never decoded.
        assert(def_exposes(*d, "current_heating_setpoint"));
        assert(def_exposes(*d, "local_temperature"));
        assert(def_exposes(*d, "system_mode"));
        assert(def_exposes(*d, "child_lock"));
        assert(def_exposes(*d, "window_detection"));
        assert(def_exposes(*d, "valve_detection"));
        assert(def_exposes(*d, "battery"));
        // A Tuya DP read + write converter pair (genThermostat misroute had
        // neither on the 0xEF00 stream).
        assert(d->from_zigbee_count >= 1);
        assert(d->to_zigbee && d->to_zigbee_count >= 1);
    }
    // The device is Tuya-MCU: it must NOT carry a phantom genThermostat
    // (0x0201) binding the auto-port wired.
    const auto* d = find_definition("TS0601", "_TZE200_zivfvd7h",
                                    siterwell_registry());
    assert(d);
    for (std::size_t i = 0; i < d->bindings_count; ++i) {
        assert(d->bindings[i].cluster_id != 0x0201 &&
               "Tuya-MCU TRV must not bind genThermostat 0x0201");
    }
}

// DP2 heatingSetpoint /10 + DP3 localTemp /10 → Float keys.
static void test_setpoint_and_local_temp_decode() {
    const auto* d = find_definition("TS0601", "_TZE200_zivfvd7h",
                                    siterwell_registry());
    assert(d);
    // 215 = 0x000000D7 → 21.5 °C ; 208 = 0x000000D0 → 20.8 °C.
    const std::uint8_t kSp215[] = { 0x00, 0x00, 0x00, 0xD7 };
    const std::uint8_t kLt208[] = { 0x00, 0x00, 0x00, 0xD0 };
    const TuyaDpRecord recs[] = {
        { 2, 0x02, std::span<const std::uint8_t>(kSp215, 4) },
        { 3, 0x02, std::span<const std::uint8_t>(kLt208, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 2));
    assert(result.any_matched);

    const Value* sp = result.merged.find("current_heating_setpoint");
    assert(sp && sp->type == ValueType::Float);
    assert(sp->f > 21.49 && sp->f < 21.51);

    const Value* lt = result.merged.find("local_temperature");
    assert(lt && lt->type == ValueType::Float);
    assert(lt->f > 20.79 && lt->f < 20.81);
}

// DP4 mode (Enum) → system_mode via thermostatSystemModes4: 2 → "heat".
static void test_system_mode_enum_decode() {
    const auto* d = find_definition("TS0601", "_TZE200_zivfvd7h",
                                    siterwell_registry());
    assert(d);
    const std::uint8_t kHeat[] = { 0x02 };
    const TuyaDpRecord recs[] = {
        { 4, 0x04, std::span<const std::uint8_t>(kHeat, 1) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* mode = result.merged.find("system_mode");
    assert(mode && mode->type == ValueType::StringRef);
    assert(mode->str && std::strcmp(mode->str, "heat") == 0);
}

// DP7 child_lock (Bool) + DP21 battery (raw) + DP109 valve position (raw).
static void test_child_lock_battery_position_decode() {
    const auto* d = find_definition("TS0601", "_TZE200_zivfvd7h",
                                    siterwell_registry());
    assert(d);
    const std::uint8_t kLock[] = { 0x01 };
    const std::uint8_t kBat[]  = { 0x00, 0x00, 0x00, 0x5A };  // 90 %
    const std::uint8_t kPos[]  = { 0x00, 0x00, 0x00, 0x32 };  // 50 %
    const TuyaDpRecord recs[] = {
        { 7,   0x01, std::span<const std::uint8_t>(kLock, 1) },
        { 21,  0x02, std::span<const std::uint8_t>(kBat, 4) },
        { 109, 0x02, std::span<const std::uint8_t>(kPos, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 3));
    assert(result.any_matched);

    const Value* cl = result.merged.find("child_lock");
    assert(cl && cl->type == ValueType::Bool && cl->b == true);

    const Value* bat = result.merged.find("battery");
    assert(bat && bat->type == ValueType::Int && bat->i == 90);

    const Value* pos = result.merged.find("position");
    assert(pos && pos->type == ValueType::Int && pos->i == 50);
}

int main() {
    test_resolves_to_thermostat_dp_def();
    test_setpoint_and_local_temp_decode();
    test_system_mode_enum_decode();
    test_child_lock_battery_position_decode();
    return 0;
}
