// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Futurehome TS0601 thermostat (Co020 / FH-clones,
// _TZE204_e5hpkc6d / _TZE200_4hbx5cvx / _TZE200_e5hpkc6d) — a pure
// Tuya-datapoint thermostat reporting on cluster 0xEF00 (manuSpecificTuya).
//
// Bug fixed: the hand-ported Tier-2 DP map aggregated z2m's datapoints but
// dropped DP 104 running_state (z2m lookup{idle: false, heat: true}, exposed
// via withRunningState(["idle","heat"])). The thermostat therefore declared a
// running-state surface that never decoded. DP 104 is now mapped as a boolean
// DP fanned to an "idle"/"heat" string via kTuyaDpFlagBoolEnum, matching z2m.
//
// These tests pin (a) every Futurehome manufacturer name resolves through
// find_definition to the aggregated DP def, (b) running_state is declared as
// an expose, (c) DP 104 decodes idle/heat on the real wire, and (d) the
// pre-existing local_temperature / setpoint channels still decode.
//
// z2m-source: devices/futurehome.ts #TS0601_futurehome_thermostat.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/futurehome_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> futurehome_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::futurehome::kFuturehomeRegistry,
        devices::futurehome::kFuturehomeRegistryCount);
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

const PreparedDefinition* fut_def(const char* manu) {
    const auto* d = find_definition("TS0601", manu, futurehome_registry());
    assert(d && "Futurehome manufacturer name must resolve to the thermostat def");
    return d;
}

}  // namespace

// All three z2m manufacturer names resolve to the aggregated DP def — carrying
// a Tuya DP from_zigbee converter (not a battery-only / on-off stub) — and the
// model-only neutered stubs never win.
static void test_all_manus_resolve_to_full_dp_def() {
    const char* manus[] = {
        "_TZE204_e5hpkc6d", "_TZE200_4hbx5cvx", "_TZE200_e5hpkc6d",
    };
    for (const char* m : manus) {
        const auto* d = fut_def(m);
        assert(d->model &&
               std::strcmp(d->model, "TS0601_futurehome_thermostat") == 0);
        assert(d->vendor && std::strcmp(d->vendor, "Futurehome") == 0);
        assert(d->from_zigbee && d->from_zigbee_count >= 1);
    }
}

// The restored running_state channel is declared as an expose alongside the
// pre-existing climate surface.
static void test_running_state_expose_declared() {
    const auto* d = fut_def("_TZE204_e5hpkc6d");
    assert(def_exposes(*d, "running_state"));
    // Pre-existing surface still present.
    assert(def_exposes(*d, "system_mode"));
    assert(def_exposes(*d, "local_temperature"));
    assert(def_exposes(*d, "current_heating_setpoint"));
}

// Core restored fix: DP 104 running_state fans the boolean DP to an
// "idle"/"heat" string (z2m lookup{idle: false, heat: true}).
static void test_running_state_dp104_bool_enum() {
    const auto* d = fut_def("_TZE204_e5hpkc6d");

    const std::uint8_t kBoolHeat[] = { 0x01 };
    const TuyaDpRecord recsHeat[] = {
        { 104, 0x01, std::span<const std::uint8_t>(kBoolHeat, 1) },
    };
    const auto rHeat = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsHeat, 1));
    assert(rHeat.any_matched);
    const Value* rs = rHeat.merged.find("running_state");
    assert(rs && rs->type == ValueType::StringRef);
    assert(rs->str && std::strcmp(rs->str, "heat") == 0);

    const std::uint8_t kBoolIdle[] = { 0x00 };
    const TuyaDpRecord recsIdle[] = {
        { 104, 0x01, std::span<const std::uint8_t>(kBoolIdle, 1) },
    };
    const auto rIdle = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsIdle, 1));
    assert(rIdle.any_matched);
    const Value* ri = rIdle.merged.find("running_state");
    assert(ri && ri->type == ValueType::StringRef);
    assert(ri->str && std::strcmp(ri->str, "idle") == 0);
}

// Regression guard: the pre-existing channels still decode. DP 24
// local_temperature (raw, /1 → Int) and DP 16 current_heating_setpoint
// (raw, /1 → Int) per z2m valueConverter.raw.
static void test_pre_existing_channels_still_decode() {
    const auto* d = fut_def("_TZE204_e5hpkc6d");
    // 215 = 0x000000D7 local temperature; 22 = 0x00000016 setpoint.
    const std::uint8_t kLt215[] = { 0x00, 0x00, 0x00, 0xD7 };
    const std::uint8_t kSp22[]  = { 0x00, 0x00, 0x00, 0x16 };
    const TuyaDpRecord recs[] = {
        { 24, 0x02, std::span<const std::uint8_t>(kLt215, 4) },
        { 16, 0x02, std::span<const std::uint8_t>(kSp22, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 2));
    assert(result.any_matched);

    const Value* lt = result.merged.find("local_temperature");
    assert(lt && lt->type == ValueType::Int && lt->i == 215);

    const Value* sp = result.merged.find("current_heating_setpoint");
    assert(sp && sp->type == ValueType::Int && sp->i == 22);
}

int main() {
    test_all_manus_resolve_to_full_dp_def();
    test_running_state_expose_declared();
    test_running_state_dp104_bool_enum();
    test_pre_existing_channels_still_decode();
    return 0;
}
