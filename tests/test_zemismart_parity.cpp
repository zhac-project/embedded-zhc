// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Zemismart family — curtain/roller-blind tubular motors
// and wall switches, mostly pure Tuya-datapoint TS0601 devices reporting on
// cluster 0xEF00 (manuSpecificTuya).
//
// Bug fixed: the zemismart registry held SIX stale generic-cover stubs left
// over from an earlier generation pass —
//   kDef_ZM16B, kDef_ZMP1, kDef_ZN_USC1U_HT, kDef_ZM25R1,
//   kDef_ZM25RX_08_30, kDef_ZM25TQ
// — each wiring generic::kFzCoverPosition / kTzCoverPosition and binding the
// standard closuresWindowCovering cluster (0x0102). But every one is a
// TS0601 device that speaks ONLY Tuya datapoints on 0xEF00 — it never emits
// the 0x0102 cluster, so the stub's `position` could decode nothing on the
// real wire.
//
// Five of the six (ZM16B / ZMP1 / ZN-USC1U-HT / ZM25R1 / ZM25RX-08/30)
// duplicated the {zigbeeModel "TS0601" + manufacturerName "_TZE*"} fingerprint
// of an existing kDefZem__TZE* DP def that carries the full datapoint map
// (position via DP 2/3 or 8/9, state, motor_direction, battery). Two defs per
// fingerprint made the resolved behaviour depend purely on registry order
// (find_definition is first-match within the Tuya-discriminated pass), so a
// motor could resolve to the dead generic-cover stub. Those five stubs were
// removed; the kDefZem__TZE* DP defs cover their manufacturer names as the
// sole match each.
//
// The sixth (ZM25TQ, _TZE200_fzo2pocs) had NO competing DP def — it was just
// wired wrong. It was graduated to a proper Tuya-DP def (DP 1 state
// {OPEN/STOP/CLOSE}, DP 2 + DP 3 -> position, DP 103/104/105 stroke limits),
// mirroring its ZM25RX-08/30 / ZMP1 siblings (z2m legacy.fz.tuya_cover:
// dataPoints.coverPosition=2, coverArrived=3).
//
// These tests pin (a) that every ex-stub fingerprint resolves through
// find_definition to a Tuya-DP def (correct model label, a real datapoint
// surface, a Tuya write converter), never a generic-cover stub, and (b) that
// the surviving defs actually decode `position` (and `state`) on the real
// 0xEF00 wire — which the removed generic-cover stubs could never do.
//
// z2m-source: zigbee-herdsman-converters/src/devices/zemismart.ts.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/zemismart_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> zemismart_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::zemismart::kZemismartRegistry,
        devices::zemismart::kZemismartRegistryCount);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) {
            return true;
        }
    }
    return false;
}

// Signature of the removed stubs: a single `position` expose and NOTHING
// else (no state, no motor_direction). A real Tuya-DP cover always carries a
// richer surface (state and/or motor_direction alongside position).
bool is_generic_cover_stub(const PreparedDefinition& def) {
    return def.exposes_count == 1 && def_exposes(def, "position");
}

// Build the DecodedMessage the dispatcher expects for an inbound Tuya DP
// report on cluster 0xEF00 cmd 0x02.
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

// Each ex-stub fingerprint paired with the model label its full DP def must
// carry. If a generic-cover stub still shadowed the fingerprint,
// find_definition could resolve to it (which the guards below reject).
struct ManuModel {
    const char* manu;
    const char* model;
};
constexpr ManuModel kExStubManus[] = {
    { "_TZE284_3mzb0sdz", "ZM16B" },        // was kDef_ZM16B
    { "_TZE284_6hrnp30w", "ZMP1" },         // was kDef_ZMP1
    { "_TZE204_mpg22jc1", "ZN-USC1U-HT" },  // was kDef_ZN_USC1U_HT
    { "_TZE200_1vxgqfba", "ZM25R1" },       // was kDef_ZM25R1
    { "_TZE200_sq6affpe", "ZM25R1" },       // was kDef_ZM25R1
    { "_TZE200_wdfurkoa", "ZM25R1" },       // was kDef_ZM25R1
    { "_TZE284_wdfurkoa", "ZM25R1" },       // was kDef_ZM25R1
    { "_TZE284_6fopvb6v", "ZM25R1" },       // was kDef_ZM25R1
    { "_TZE200_7eue9vhc", "ZM25RX-08/30" }, // was kDef_ZM25RX_08_30
    { "_TZE200_bv1jcqqu", "ZM25RX-08/30" }, // was kDef_ZM25RX_08_30
    { "_TZE200_wehza30a", "ZM25RX-08/30" }, // was kDef_ZM25RX_08_30
    { "_TZE200_fzo2pocs", "ZM25TQ" },       // graduated kDef_ZM25TQ
};

}  // namespace

// Core fix: each ex-stub fingerprint resolves through find_definition to a
// Tuya-DP def — correct model label, a real datapoint-bearing surface, and a
// Tuya write converter (the generic-cover stub had a position-only surface).
static void test_ex_stub_fingerprints_resolve_to_dp_def() {
    for (const auto& mm : kExStubManus) {
        const auto* d = find_definition("TS0601", mm.manu, zemismart_registry());
        assert(d && "zemismart manufacturer name must resolve to a def");
        assert(d->model && std::strcmp(d->model, mm.model) == 0);
        assert(!is_generic_cover_stub(*d) &&
               "resolved def must not be the position-only generic-cover stub");
        // Tuya-DP defs wire a from_zigbee DP decoder + a to_zigbee DP writer.
        assert(d->from_zigbee_count >= 1);
        assert(d->to_zigbee && d->to_zigbee_count >= 1 &&
               "DP defs carry a Tuya write converter; the stub used generic cover");
    }
}

// Registry hygiene: none of the SIX removed/graduated fingerprints resolve to
// a position-only generic-cover stub any more, and the registry holds no def
// that both is position-only AND claims a TS0601 fingerprint (TS0601 = pure
// Tuya DP, so a position-only TS0601 def is necessarily a dead generic-cover
// stub). Two position-only defs intentionally survive and are NOT stubs:
//   * ZM-CSW032-D — zigbeeModel TS0302, a genuine standard-cluster cover
//     (z2m fz.ZMCSW032D_cover_position), not a Tuya-DP device.
//   * ZM-AM02_cover — TS0601 but driven by the complex per-model legacy
//     converter legacy.fz.ZMAM02_cover (AM02 lookups), deferred as INFRA;
//     it is the sole match for its _TZE200_iossyxra/_cxu0jkjk fingerprint
//     (no competing DP def), so registry order cannot mis-resolve it.
static void test_registry_has_no_ts0601_cover_stub() {
    for (const auto* d : zemismart_registry()) {
        assert(d != nullptr);
        if (!is_generic_cover_stub(*d)) continue;
        // The only acceptable position-only survivors are the two documented
        // above. Any other (and in particular any TS0601 position-only def)
        // would be a resurrected dead stub.
        const bool is_csw032 = d->model && std::strcmp(d->model, "ZM-CSW032-D") == 0;
        const bool is_am02   = d->model && std::strcmp(d->model, "ZM-AM02_cover") == 0;
        assert((is_csw032 || is_am02) &&
               "unexpected position-only zemismart def — a dead generic-cover "
               "stub crept back");
    }
}

// The graduated ZM25TQ must declare the cover surface a generic stub showed
// (position) PLUS the state/stroke-limit DP keys it never could.
static void test_zm25tq_declares_dp_surface() {
    const auto* d = find_definition("TS0601", "_TZE200_fzo2pocs", zemismart_registry());
    assert(d && std::strcmp(d->model, "ZM25TQ") == 0);
    assert(def_exposes(*d, "position"));
    assert(def_exposes(*d, "state"));
    assert(def_exposes(*d, "upper_stroke_limit"));
    assert(def_exposes(*d, "lower_stroke_limit"));
}

// Real-wire decode through the dispatcher against the graduated ZM25TQ def.
// DP 3 (coverArrived) Numeric -> position; DP 1 Enum {0:OPEN,1:STOP,2:CLOSE}
// -> state="CLOSE". The removed generic-cover stub bound 0x0102 and decoded
// nothing on this 0xEF00 frame.
static void test_zm25tq_position_and_state_decode() {
    const auto* d = find_definition("TS0601", "_TZE200_fzo2pocs", zemismart_registry());
    assert(d);

    // DP 3 numeric (4-byte BE): 60 -> position 60.
    const std::uint8_t kPos60[]   = { 0x00, 0x00, 0x00, 0x3C };
    const std::uint8_t kStateCl[] = { 0x02 };  // enum 2 -> CLOSE
    const TuyaDpRecord recs[] = {
        { 3, 0x02, std::span<const std::uint8_t>(kPos60, 4) },
        { 1, 0x04, std::span<const std::uint8_t>(kStateCl, 1) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 2));
    assert(result.any_matched);

    const Value* pos = result.merged.find("position");
    assert(pos && (pos->type == ValueType::Int || pos->type == ValueType::Float));
    const double pv = (pos->type == ValueType::Int) ? double(pos->i) : pos->f;
    assert(pv > 59.5 && pv < 60.5);

    const Value* st = result.merged.find("state");
    assert(st && st->type == ValueType::StringRef);
    assert(st->str && std::strcmp(st->str, "CLOSE") == 0);
}

// Decode through the resolved ZM16B def (ex kDef_ZM16B fingerprint): DP 8
// (percent state) Numeric -> position. Pins that the surviving DP def — the
// one find_definition now returns for _TZE284_3mzb0sdz — really produces the
// cover position the generic-cover stub advertised but could never decode.
static void test_zm16b_position_decode() {
    const auto* d = find_definition("TS0601", "_TZE284_3mzb0sdz", zemismart_registry());
    assert(d && std::strcmp(d->model, "ZM16B") == 0);

    const std::uint8_t kPos25[] = { 0x00, 0x00, 0x00, 0x19 };  // 25
    const TuyaDpRecord recs[] = {
        { 8, 0x02, std::span<const std::uint8_t>(kPos25, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);

    const Value* pos = result.merged.find("position");
    assert(pos && (pos->type == ValueType::Int || pos->type == ValueType::Float));
    const double pv = (pos->type == ValueType::Int) ? double(pos->i) : pos->f;
    assert(pv > 24.5 && pv < 25.5);
}

int main() {
    test_ex_stub_fingerprints_resolve_to_dp_def();
    test_registry_has_no_ts0601_cover_stub();
    test_zm25tq_declares_dp_surface();
    test_zm25tq_position_and_state_decode();
    test_zm16b_position_decode();
    return 0;
}
