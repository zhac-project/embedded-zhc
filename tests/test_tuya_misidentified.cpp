// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Two Tuya fingerprints were ported as the wrong KIND of device (Tuya DP
// coverage audit, 2026-08-31). z2m has carried both unchanged since it first
// listed them:
//   _TZE200_vzekyi4c  TS0601_smoke_4 smoke sensor (z2m 2022-12-11, #15483).
//                     Ported as a PIR: a fire raised `occupancy`, no smoke.
//   _TZE200_nw1r9hp6  TS0601_cover_3 curtain motor, Zemismart ZM85EL-2Z
//                     (z2m 2023-06-03, #11251). Ported as a smoke detector:
//                     the curtain moving raised `smoke`.
// Pins what the matcher returns for each and how its datapoints decode.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"

using namespace zhc;

namespace {

const PreparedDefinition* match(const char* manufacturer) {
    return find_definition("TS0601", manufacturer,
        std::span<const PreparedDefinition* const>(devices::tuya::kTuyaRegistry,
                                                    devices::tuya::kTuyaRegistryCount));
}

// One datapoint per frame, as these devices report.
DispatchResult decode(const PreparedDefinition& def, std::uint8_t dp, std::uint8_t wire_type,
                      std::span<const std::uint8_t> value, RuntimeContext& ctx) {
    const TuyaDpRecord rec{ dp, wire_type, value };
    DecodedMessage msg{};
    msg.family       = FrameFamily::TuyaDp;
    msg.type         = MessageType::Command;
    msg.cluster      = "manuSpecificTuya";
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    return dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>(&rec, 1), def, raw, ctx);
}

bool is_bool(const Value* v, bool b) { return v && v->type == ValueType::Bool && v->b == b; }
bool is_int(const Value* v, std::int64_t i) { return v && v->type == ValueType::Int && v->i == i; }
bool is_str(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}

constexpr std::uint8_t kBool = 0x01, kValue = 0x02, kEnum = 0x04;

}  // namespace

static void test_vzekyi4c_is_a_smoke_sensor() {
    const PreparedDefinition* d = match("_TZE200_vzekyi4c");
    assert(d && std::strcmp(d->model, "TS0601_smoke_4") == 0);

    const std::uint8_t k0[] = {0x00}, k1[] = {0x01};
    const std::uint8_t k87[] = {0x00, 0x00, 0x00, 0x57};
    {   // z2m trueFalse0: 0 is the alarm, as an enum ...
        RuntimeContext ctx{};
        const auto r = decode(*d, 1, kEnum, k0, ctx);
        assert(is_bool(r.merged.find("smoke"), true));
        assert(r.merged.find("occupancy") == nullptr);
    }
    {   // ... or wire-typed bool
        RuntimeContext ctx{};
        assert(is_bool(decode(*d, 1, kBool, k0, ctx).merged.find("smoke"), true));
    }
    {
        RuntimeContext ctx{};
        assert(is_bool(decode(*d, 1, kEnum, k1, ctx).merged.find("smoke"), false));
    }
    {
        RuntimeContext ctx{};
        assert(is_int(decode(*d, 15, kValue, k87, ctx).merged.find("battery"), 87));
    }
    {
        RuntimeContext ctx{};
        assert(is_str(decode(*d, 14, kEnum, k0, ctx).merged.find("battery_state"), "low"));
    }
}

static void test_nw1r9hp6_is_a_curtain_motor() {
    const PreparedDefinition* d = match("_TZE200_nw1r9hp6");
    assert(d && std::strcmp(d->model, "TS0601_cover_3") == 0);

    const std::uint8_t k0[] = {0x00}, k1[] = {0x01};
    const std::uint8_t k40[] = {0x00, 0x00, 0x00, 0x28};
    {   // DP1 is the motor command, never smoke
        RuntimeContext ctx{};
        const auto r = decode(*d, 1, kEnum, k0, ctx);
        assert(is_str(r.merged.find("state"), "OPEN"));
        assert(r.merged.find("smoke") == nullptr);
    }
    {
        RuntimeContext ctx{};
        assert(is_str(decode(*d, 1, kEnum, k1, ctx).merged.find("state"), "STOP"));
    }
    {
        RuntimeContext ctx{};
        assert(is_int(decode(*d, 2, kValue, k40, ctx).merged.find("position"), 40));
    }
    {
        RuntimeContext ctx{};
        assert(is_int(decode(*d, 13, kValue, k40, ctx).merged.find("battery"), 40));
    }
}

int main() {
    test_vzekyi4c_is_a_smoke_sensor();
    test_nw1r9hp6_is_a_curtain_motor();
    return 0;
}
