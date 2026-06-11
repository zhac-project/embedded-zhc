// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Novo C10-3E-1.2 (TS0601 / _TZE200_swhwv3k3 curtain switch).
// Found by a z2m<->embedded-zhc parity pass over the single novo def.
//
// REAL BUG (Tuya-MCU misroute): the auto-port wired the GENERIC windowCovering
// converters (cluster 0x0102: kFzCoverPosition / kTzCoverPosition) and bound
// endpoint 1 to 0x0102. But z2m decodes this device via legacy.fz.tuya_cover —
// the 0xEF00 (manuSpecificTuya) datapoint stream. The TS0601 Tuya MCU never
// emits on 0x0102, so position / state / set were all dead. Fixed by porting
// the DP map to fz_tuya_datapoints / tz_tuya_datapoints:
//
//   DP 1   state        Enum {0:OPEN, 1:STOP, 2:CLOSE}
//   DP 2   position     Numeric 0..100 % (started moving)
//   DP 3   position     Numeric 0..100 % (coverArrived — same key)
//   DP 105 motor_speed  Numeric
//
// These tests decode real 0xEF00 DP frames through the dispatcher and assert the
// position / state / motor_speed keys surface with the right type and values.
//
// z2m-source: novo.ts #C10-3E-1.2 ; lib/legacy.ts fz.tuya_cover
//   (dataPoints.state=1, coverPosition=2, coverArrived=3, coverSpeed=105;
//    getCoverStateEnums default {open:0, stop:1, close:2}).

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/novo_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> novo_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::novo::kNovoRegistry,
        devices::novo::kNovoRegistryCount);
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

const PreparedDefinition& resolve_novo() {
    const auto* d = find_definition("TS0601", "_TZE200_swhwv3k3", novo_registry());
    assert(d && "novo _TZE200_swhwv3k3 must resolve to a def");
    assert(d->model && std::strcmp(d->model, "C10-3E-1.2") == 0);
    return *d;
}

// Decode one Numeric DP (dp_type 0x02, 4 big-endian bytes) and return the Int
// value emitted under out_key (asserts present + Int).
std::int64_t decode_numeric(const PreparedDefinition& def, std::uint8_t dp,
                            std::int32_t wire, const char* out_key) {
    const std::uint8_t b[] = {
        static_cast<std::uint8_t>((wire >> 24) & 0xFF),
        static_cast<std::uint8_t>((wire >> 16) & 0xFF),
        static_cast<std::uint8_t>((wire >> 8) & 0xFF),
        static_cast<std::uint8_t>(wire & 0xFF),
    };
    const TuyaDpRecord recs[] = {
        { dp, 0x02, std::span<const std::uint8_t>(b, 4) },
    };
    const auto result = dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* v = result.merged.find(out_key);
    assert(v && "numeric DP must surface out_key");
    assert(v->type == ValueType::Int);
    return v->i;
}

const char* decode_enum(const PreparedDefinition& def, std::uint8_t dp,
                        std::uint8_t wire, const char* out_key) {
    const std::uint8_t b[] = { wire };
    const TuyaDpRecord recs[] = {
        { dp, 0x04, std::span<const std::uint8_t>(b, 1) },
    };
    const auto result = dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* v = result.merged.find(out_key);
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

}  // namespace

// The def must route through the Tuya 0xEF00 DP family, NOT the dead generic
// windowCovering 0x0102 converters that the auto-port wired.
static void test_def_routes_tuya_dp() {
    const auto& d = resolve_novo();
    bool has_tuya_dp = false;
    for (std::size_t i = 0; i < d.from_zigbee_count; ++i) {
        if (d.from_zigbee[i]->family == FrameFamily::TuyaDp) has_tuya_dp = true;
    }
    assert(has_tuya_dp && "novo curtain must decode via the Tuya DP stream");
}

// DP2 (coverPosition) -> position. A 64% report decodes to 64 (not dead).
static void test_position_dp2_decodes() {
    const auto& d = resolve_novo();
    assert(decode_numeric(d, 2, 64, "position") == 64);
    assert(decode_numeric(d, 2, 0, "position") == 0);
    assert(decode_numeric(d, 2, 100, "position") == 100);
}

// DP3 (coverArrived) -> position, same key as DP2.
static void test_position_dp3_decodes() {
    const auto& d = resolve_novo();
    assert(decode_numeric(d, 3, 42, "position") == 42);
}

// DP105 (coverSpeed) -> motor_speed.
static void test_motor_speed_dp105_decodes() {
    const auto& d = resolve_novo();
    assert(decode_numeric(d, 105, 7, "motor_speed") == 7);
}

// DP1 (state) -> OPEN / STOP / CLOSE, default getCoverStateEnums order.
static void test_state_enum_decodes() {
    const auto& d = resolve_novo();
    const char* s0 = decode_enum(d, 1, 0, "state");
    assert(s0 && std::strcmp(s0, "OPEN") == 0);
    const char* s1 = decode_enum(d, 1, 1, "state");
    assert(s1 && std::strcmp(s1, "STOP") == 0);
    const char* s2 = decode_enum(d, 1, 2, "state");
    assert(s2 && std::strcmp(s2, "CLOSE") == 0);
}

int main() {
    test_def_routes_tuya_dp();
    test_position_dp2_decodes();
    test_position_dp3_decodes();
    test_motor_speed_dp105_decodes();
    test_state_enum_decodes();
    return 0;
}
