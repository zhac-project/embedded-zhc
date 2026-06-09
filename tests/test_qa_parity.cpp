// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the QA QAT44Z4H (4-gang) and QAT44Z6H (6-gang) wall
// switches.
// z2m-source: qa.ts #QAT44Z4H (TS0601 / _TZE204_kyzjsjo3),
//             #QAT44Z6H (TS0601 / _TZE204_4cl0dzt4).
//
// Bug fixed: both generated stubs wired generic genOnOff (kFzOnOff/kTzOnOff,
// cluster 0x0006) and exposed a single bare `state`. But these are TS0601
// Tuya-MCU devices that z2m decodes with `legacy.fz.tuya_switch` /
// `legacy.tz.tuya_switch_state` on manuSpecificTuya (0xEF00): the
// multiEndpoint datapoints 1..N carry per-gang bool ON/OFF (DP 1=l1,
// 2=l2, … 6=l6). A paired switch therefore surfaced one `state` and
// decoded/controlled none of its gangs. The defs were graduated to Tier-2
// parent overrides that wire the Tuya-DP infra (fz_tuya_datapoints) with a
// {1..N → state_lN} map and declare per-gang state_l1..lN exposes.
//
// These tests pin, on inbound Tuya DP records (cluster 0xEF00, cmd 0x02),
// that each gang decodes independently to its own state_lN key, and that
// the exposes + 0xEF00 binding are declared while the wrong-bundle genOnOff
// (0x0006) binding is gone.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::qa {
extern const PreparedDefinition kDef_QAT44Z4H;   // 4-gang Tuya-DP wall switch
extern const PreparedDefinition kDef_QAT44Z6H;   // 6-gang Tuya-DP wall switch
}  // namespace zhc::devices::qa

using namespace zhc;

namespace {

// Build the DecodedMessage + InboundApsFrame pair the dispatcher expects for
// an inbound Tuya DP report on cluster 0xEF00 cmd 0x02 (mirrors the proven
// namron/zemismart parity harness).
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

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool bool_is(const Value* v, bool expect) {
    return v && v->type == ValueType::Bool && v->b == expect;
}

const std::uint8_t kOn[]  = { 0x01 };
const std::uint8_t kOff[] = { 0x00 };

}  // namespace

// Surface shape: per-gang exposes present, bare `state` gone, Tuya-DP bound
// on 0xEF00 and the wrong-bundle genOnOff (0x0006) no longer bound.
static void test_qat44z4h_surface() {
    const auto& def = devices::qa::kDef_QAT44Z4H;
    assert(def_exposes(def, "state_l1"));
    assert(def_exposes(def, "state_l2"));
    assert(def_exposes(def, "state_l3"));
    assert(def_exposes(def, "state_l4"));
    assert(!def_exposes(def, "state"));
    assert(def_binds(def, 0xEF00));
    assert(!def_binds(def, 0x0006));
}

// Each of the 4 gangs decodes from its own datapoint (1..4 → state_l1..l4).
static void test_qat44z4h_gangs_decode() {
    const auto& def = devices::qa::kDef_QAT44Z4H;

    // DP 3 ON -> state_l3 = true.
    const TuyaDpRecord r3[] = { { 3, 0x01, std::span<const std::uint8_t>(kOn, 1) } };
    auto d3 = dispatch_dp(def, std::span<const TuyaDpRecord>(r3, 1));
    assert(d3.any_matched);
    assert(bool_is(d3.merged.find("state_l3"), true));

    // DP 1 OFF -> state_l1 = false.
    const TuyaDpRecord r1[] = { { 1, 0x01, std::span<const std::uint8_t>(kOff, 1) } };
    auto d1 = dispatch_dp(def, std::span<const TuyaDpRecord>(r1, 1));
    assert(d1.any_matched);
    assert(bool_is(d1.merged.find("state_l1"), false));
}

// 6-gang variant: datapoints 1..6 -> state_l1..l6.
static void test_qat44z6h_surface_and_decode() {
    const auto& def = devices::qa::kDef_QAT44Z6H;
    for (const char* k : {"state_l1", "state_l2", "state_l3",
                          "state_l4", "state_l5", "state_l6"})
        assert(def_exposes(def, k));
    assert(!def_exposes(def, "state"));
    assert(def_binds(def, 0xEF00));
    assert(!def_binds(def, 0x0006));

    // DP 6 ON -> state_l6 (the gang the old single-state stub could never reach).
    const TuyaDpRecord r6[] = { { 6, 0x01, std::span<const std::uint8_t>(kOn, 1) } };
    auto d6 = dispatch_dp(def, std::span<const TuyaDpRecord>(r6, 1));
    assert(d6.any_matched);
    assert(bool_is(d6.merged.find("state_l6"), true));

    // DP 2 ON -> state_l2.
    const TuyaDpRecord r2[] = { { 2, 0x01, std::span<const std::uint8_t>(kOn, 1) } };
    auto d2 = dispatch_dp(def, std::span<const TuyaDpRecord>(r2, 1));
    assert(d2.any_matched);
    assert(bool_is(d2.merged.find("state_l2"), true));
}

int main() {
    test_qat44z4h_surface();
    test_qat44z4h_gangs_decode();
    test_qat44z6h_surface_and_decode();
    return 0;
}
