// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Bseed BSEED_TS0601_cover Tuya curtain switch.
// z2m-source: bseed.ts #BSEED_TS0601_cover —
//   fromZigbee: [legacy.fz.tuya_cover], toZigbee: [legacy.tz.tuya_cover_control]
//   (cluster manuSpecificTuya / 0xEF00).
//
// Bug fixed (Tuya-DP MISROUTE): the generated Tier-1 file wired this TS0601
// _TZE200/_TZE204 device to the generic ZCL windowCovering cluster
// (kFzCoverPosition on 0x0102) with a 0x0102 binding. A TS0601 Tuya curtain
// switch never speaks ZCL 0x0102 — it reports over the manuSpecificTuya
// (0xEF00) DP stream, so the generic cover converter matched nothing and
// `position` was a dead key. The def was graduated to a Tier-2 override that
// wires the Tuya-DP map (dp 2/3 → position, dp 105 → motor_speed) and binds
// 0xEF00.
//
// These tests pin, on real 0xEF00 DP wire shapes, that position decodes from
// both the "started moving" DP (2) and the "arrived" DP (3), that motor_speed
// decodes from DP 105, and that the def declares the right exposes + binding.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/bseed_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> bseed_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::bseed::kBseedRegistry, devices::bseed::kBseedRegistryCount);
}

const PreparedDefinition& resolve(const char* manu) {
    const auto* d = find_definition("TS0601", manu, bseed_registry());
    assert(d && "bseed manufacturer name must resolve to a def");
    assert(d->model && std::strcmp(d->model, "BSEED_TS0601_cover") == 0);
    return *d;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

// ── Tuya-DP frame harness (0xEF00, command 0x02 dataReport) ───────────
DecodedMessage make_dp_msg() {
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

InboundApsFrame make_dp_raw() {
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    return raw;
}

// Decode one 4-byte big-endian Value DP (dp_type 0x02).
DispatchResult decode_value(const PreparedDefinition& def, std::uint8_t dp,
                            std::uint32_t value) {
    const std::uint8_t b[] = {
        static_cast<std::uint8_t>((value >> 24) & 0xFF),
        static_cast<std::uint8_t>((value >> 16) & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF),
        static_cast<std::uint8_t>(value & 0xFF),
    };
    const TuyaDpRecord recs[] = {
        { dp, 0x02, std::span<const std::uint8_t>(b, 4) },
    };
    auto msg = make_dp_msg();
    auto raw = make_dp_raw();
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>(recs, 1),
                                def, raw, ctx);
}

double as_num(const Value* v) {
    assert(v);
    return (v->type == ValueType::Int)  ? double(v->i)
         : (v->type == ValueType::Uint) ? double(v->u)
                                        : double(v->f);
}

}  // namespace

// The def must wire the Tuya-DP cluster, NOT the generic ZCL cover cluster.
static void test_bseed_exposes_and_binding() {
    const auto& def = resolve("_TZE200_yenbr4om");
    assert(def_exposes(def, "position"));
    assert(def_exposes(def, "motor_speed"));
    assert(def_binds(def, 0xEF00) && "must bind the Tuya 0xEF00 cluster");
    assert(!def_binds(def, 0x0102) && "must NOT bind the dead ZCL cover cluster");
}

// dp 2 coverPosition → position (z2m: started moving / running).
static void test_bseed_position_moving() {
    const auto& def = resolve("_TZE204_bdblidq3");
    auto r = decode_value(def, 2, 42);
    assert(r.any_matched);
    const Value* p = r.merged.find("position");
    assert(p && std::fabs(as_num(p) - 42.0) < 0.01);
}

// dp 3 coverArrived → position (z2m: arrived at target).
static void test_bseed_position_arrived() {
    const auto& def = resolve("_TZE200_bdblidq3");
    auto r = decode_value(def, 3, 100);
    assert(r.any_matched);
    const Value* p = r.merged.find("position");
    assert(p && std::fabs(as_num(p) - 100.0) < 0.01);
}

// dp 105 coverSpeed → motor_speed.
static void test_bseed_motor_speed() {
    const auto& def = resolve("_TZE200_yenbr4om");
    auto r = decode_value(def, 105, 7);
    assert(r.any_matched);
    const Value* s = r.merged.find("motor_speed");
    assert(s && std::fabs(as_num(s) - 7.0) < 0.01);
}

int main() {
    test_bseed_exposes_and_binding();
    test_bseed_position_moving();
    test_bseed_position_arrived();
    test_bseed_motor_speed();
    return 0;
}
