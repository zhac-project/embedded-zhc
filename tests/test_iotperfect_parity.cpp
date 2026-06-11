// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the IOTPerfect PF-PM02D-TYZ smart water/gas valve (TS0601 /
// 6× _TZE2xx) — a Tuya-MCU (0xEF00 manuSpecificTuya) DP-stream device.
//
// Bug fixed: the auto extract mis-routed the valve as a bare kFzOnOff / kTzOnOff
// stub bound to ZCL genOnOff cluster 0x0006 — a cluster a TS0601/_TZE* device
// never speaks — leaving the single `state` expose DEAD. z2m decodes via
// legacy.fromZigbee.tuya_switch (DP 1 state, bool -> "ON"/"OFF") and writes via
// legacy.toZigbee.tuya_switch_state. Graduated to a Tier-2 override wiring
// fz_tuya_datapoints + tz_tuya_datapoints over the 0xEF00 DP map.
//
// z2m-source: iotperfect.ts #PF-PM02D-TYZ + lib/legacy.ts tuya_switch /
//   tuya_switch_state (dataPoints.state == 1).

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/iotperfect_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> iotperfect_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::iotperfect::kIotperfectRegistry,
        devices::iotperfect::kIotperfectRegistryCount);
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

const PreparedDefinition* valve() {
    const auto* d = find_definition("TS0601", "_TZE200_vrjkcam9",
                                    iotperfect_registry());
    assert(d && "_TZE200_vrjkcam9 must resolve to the PF-PM02D-TYZ def");
    return d;
}

}  // namespace

// Every manufacturer fingerprint resolves to one full DP def carrying a Tuya DP
// from_zigbee + to_zigbee converter (not an on-off / genOnOff stub).
static void test_manu_resolves_to_full_dp_def() {
    const auto* d = valve();
    assert(d->model && std::strcmp(d->model, "PF-PM02D-TYZ") == 0);
    assert(d->vendor && std::strcmp(d->vendor, "Iotperfect") == 0);
    // sync-time + DP map fz; wildcard DP tz.
    assert(d->from_zigbee && d->from_zigbee_count >= 2);
    assert(d->to_zigbee && d->to_zigbee_count >= 1);

    // All six manufacturer-name variants keep resolving to the same def.
    assert(find_definition("TS0601", "_TZE200_d0ypnbvn", iotperfect_registry()) == d);
    assert(find_definition("TS0601", "_TZE204_v5xjyphj", iotperfect_registry()) == d);
    assert(find_definition("TS0601", "_TZE204_d0ypnbvn", iotperfect_registry()) == d);
    assert(find_definition("TS0601", "_TZE284_v5xjyphj", iotperfect_registry()) == d);
    assert(find_definition("TS0601", "_TZE284_d0ypnbvn", iotperfect_registry()) == d);
}

// The `state` expose is declared; no phantom Light/electrical channels remain.
static void test_state_expose_declared() {
    const auto* d = valve();
    assert(def_exposes(*d, "state"));
    assert(!def_exposes(*d, "brightness"));
    assert(!def_exposes(*d, "color_temp"));
    assert(!def_exposes(*d, "voltage"));
    // Exactly one expose (the switch).
    assert(d->exposes_count == 1);
}

// DP 1 state = bool -> "ON"/"OFF" (z2m tuya_switch: value ? "ON" : "OFF").
static void test_state_dp1_bool_enum() {
    const auto* d = valve();

    const std::uint8_t kOn[] = { 0x01 };
    const TuyaDpRecord recsOn[] = { { 1, 0x01, std::span<const std::uint8_t>(kOn, 1) } };
    const auto rOn = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsOn, 1));
    assert(rOn.any_matched);
    const Value* sOn = rOn.merged.find("state");
    assert(sOn && sOn->type == ValueType::StringRef);
    assert(sOn->str && std::strcmp(sOn->str, "ON") == 0);

    const std::uint8_t kOff[] = { 0x00 };
    const TuyaDpRecord recsOff[] = { { 1, 0x01, std::span<const std::uint8_t>(kOff, 1) } };
    const auto rOff = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsOff, 1));
    assert(rOff.any_matched);
    const Value* sOff = rOff.merged.find("state");
    assert(sOff && sOff->type == ValueType::StringRef);
    assert(sOff->str && std::strcmp(sOff->str, "OFF") == 0);
}

// An unmapped DP produces no output and is silently skipped (no genOnOff key
// leaks through; the device speaks only DP 1).
static void test_unmapped_dp_ignored() {
    const auto* d = valve();
    const std::uint8_t kJunk[] = { 0x00, 0x00, 0x00, 0x05 };
    const TuyaDpRecord recs[] = {
        { 9, 0x02, std::span<const std::uint8_t>(kJunk, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.merged.find("state") == nullptr);
}

int main() {
    test_manu_resolves_to_full_dp_def();
    test_state_expose_declared();
    test_state_dp1_bool_enum();
    test_unmapped_dp_ignored();
    return 0;
}
