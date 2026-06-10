// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Evanell EZ200 — a Tuya-MCU thermostatic radiator
// valve (TS0601 + _TZE200_dmfguuli / _TZE200_rxypyjkw) reporting on cluster
// 0xEF00 (manuSpecificTuya) via the 0xEF00 datapoint stream.
//
// Bug fixed: the auto-port wired EZ200 to the generic genThermostat
// converter (cluster 0x0201) with a phantom 0x0201 binding. EZ200 is a
// Tuya-MCU device — it never speaks genThermostat, so the device decoded
// NOTHING (no local_temperature, setpoint, system_mode, child_lock, or
// battery). The def now wires the Tuya DP map from z2m legacy.ts
// fz.evanell_thermostat:
//   dp2 evanellMode      enum  → system_mode {0:auto, 2:heat, 3:off}
//   dp4 evanellHeatingSetpoint Numeric /10 → current_heating_setpoint
//   dp5 evanellLocalTemp Numeric /10 → local_temperature
//   dp6 evanellBattery   Numeric raw → battery
//   dp8 evanellChildLock Bool        → child_lock
//
// These tests pin (a) the resolved def is a Tuya-DP def (real fz + tz
// converters, NOT the generic genThermostat), (b) it carries flat
// thermostat exposes including a proper enum system_mode, and (c) the
// device's core datapoints decode on the real 0xEF00 wire shape with the
// correct /10 scaling and mode-enum mapping.
//
// z2m-source: evanell.ts #EZ200, legacy.ts fz.evanell_thermostat.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/evanell_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> evanell_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::evanell::kEvanellRegistry, devices::evanell::kEvanellRegistryCount);
}

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) {
            return &def.exposes[i];
        }
    }
    return nullptr;
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

}  // namespace

// Both manufacturer names resolve to the EZ200 DP def — not a genThermostat
// stub. The def must wire a real Tuya DP from_zigbee converter AND a write
// converter (TuyaRw): setpoint / system_mode / child_lock are controllable.
static void test_resolves_to_tuya_dp_def() {
    const char* manus[] = { "_TZE200_dmfguuli", "_TZE200_rxypyjkw" };
    for (const char* manu : manus) {
        const auto* d = find_definition("TS0601", manu, evanell_registry());
        assert(d && "evanell manufacturer name must resolve to a def");
        assert(d->model && std::strcmp(d->model, "EZ200") == 0);
        assert(d->from_zigbee_count >= 1 && "must wire a Tuya DP fz converter");
        assert(d->to_zigbee && d->to_zigbee_count >= 1 &&
               "TuyaRw wires a DP write converter");
        // The phantom genThermostat 0x0201 binding must be gone.
        for (std::size_t i = 0; i < d->bindings_count; ++i) {
            assert(d->bindings[i].cluster_id != 0x0201 &&
                   "no phantom genThermostat binding on a Tuya-MCU TRV");
        }
    }
}

// Flat thermostat exposes (no ExposeType::Climate). system_mode is an enum
// {off,heat,auto}, not a bare Binary as the auto-port had it.
static void test_flat_thermostat_exposes() {
    const auto* d = find_definition("TS0601", "_TZE200_dmfguuli", evanell_registry());
    assert(d);

    const Expose* lt = find_expose(*d, "local_temperature");
    assert(lt && lt->type == ExposeType::Numeric);

    const Expose* sp = find_expose(*d, "current_heating_setpoint");
    assert(sp && sp->type == ExposeType::Numeric && sp->access == Access::StateSet);

    const Expose* sm = find_expose(*d, "system_mode");
    assert(sm && sm->type == ExposeType::Enum && sm->enum_count == 3);

    assert(find_expose(*d, "child_lock"));
    assert(find_expose(*d, "battery"));
}

// dp4 setpoint /10 and dp5 local_temperature /10 decode as Float on the real
// 0xEF00 wire shape. A genThermostat-bound def would surface neither.
static void test_setpoint_and_local_temp_decode() {
    const auto* d = find_definition("TS0601", "_TZE200_dmfguuli", evanell_registry());
    assert(d);

    // Big-endian 4-byte numeric. 215 = 0x000000D7 → 21.5; 208 → 20.8.
    const std::uint8_t kSp215[] = { 0x00, 0x00, 0x00, 0xD7 };
    const std::uint8_t kLt208[] = { 0x00, 0x00, 0x00, 0xD0 };
    const TuyaDpRecord recs[] = {
        { 4, 0x02, std::span<const std::uint8_t>(kSp215, 4) },
        { 5, 0x02, std::span<const std::uint8_t>(kLt208, 4) },
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

// dp2 mode enum → system_mode string with the z2m mapping (2 → "heat");
// dp8 child_lock Bool=true → child_lock; dp6 battery raw → battery.
static void test_mode_childlock_battery_decode() {
    const auto* d = find_definition("TS0601", "_TZE200_dmfguuli", evanell_registry());
    assert(d);

    const std::uint8_t kModeHeat[] = { 0x02 };  // → "heat"
    const std::uint8_t kLockOn[]   = { 0x01 };  // → child_lock true
    const std::uint8_t kBatt[]     = { 0x00, 0x00, 0x00, 0x5A };  // 90
    const TuyaDpRecord recs[] = {
        { 2, 0x04, std::span<const std::uint8_t>(kModeHeat, 1) },
        { 8, 0x01, std::span<const std::uint8_t>(kLockOn, 1) },
        { 6, 0x02, std::span<const std::uint8_t>(kBatt, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 3));
    assert(result.any_matched);

    const Value* mode = result.merged.find("system_mode");
    assert(mode && mode->type == ValueType::StringRef);
    assert(mode->str && std::strcmp(mode->str, "heat") == 0);

    const Value* lock = result.merged.find("child_lock");
    assert(lock && lock->type == ValueType::Bool && lock->b == true);

    const Value* batt = result.merged.find("battery");
    assert(batt);
}

int main() {
    test_resolves_to_tuya_dp_def();
    test_flat_thermostat_exposes();
    test_setpoint_and_local_temp_decode();
    test_mode_childlock_battery_decode();
    return 0;
}
