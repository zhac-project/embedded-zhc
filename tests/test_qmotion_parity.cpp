// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the two Qmotion definitions.
// z2m-source: qmotion.ts.
//
// Bugs fixed:
//  * QZR-ZIG2400 ("5 channel remote") — the Tier-1 auto-port mis-modelled
//    this input device as a controllable on/off cover. It wired kFzOnOff
//    (z2m never decodes genOnOff), a writable `state` Binary expose, and
//    phantom kTzOnOff + kTzCoverPosition write paths, whereas z2m's
//    toZigbee is EMPTY. Graduated to a Tier-2 override matching z2m:
//    fz.identify (genIdentify → action="identify") + fz.cover_position_tilt
//    (position, read-only), no toZigbee, exposes action + read-only
//    position, genOnOff binding dropped.
//
//  * HDM40PV620 (motorized roller blind) — verified already correct: the
//    generic kFzCoverPosition + kFzBattery decoders match z2m's fz, and the
//    single kTzCoverPosition converter already claims BOTH "position"
//    (goToLiftPercentage) AND "state" (OPEN/CLOSE/STOP), so it fully covers
//    z2m's [tz.cover_state, tz.cover_position_tilt]. No change.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::qmotion {
extern const PreparedDefinition kDef_QZR_ZIG2400;  // 5 channel remote
extern const PreparedDefinition kDef_HDM40PV620;   // motorized roller blind
}  // namespace zhc::devices::qmotion

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t src_ep,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

DispatchResult run(const PreparedDefinition& def, std::uint16_t cluster,
                   std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    // decode_frame leaves msg.cluster null (the dispatch layer maps id→name
    // in production). Stamp it so cluster-scoped converters self-filter, as
    // they do on-device.
    msg.cluster = cluster_id_to_name(cluster);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool has_expose(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (std::strcmp(def.exposes[i].name, name) == 0) return true;
    }
    return false;
}

const Expose* find_expose(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (std::strcmp(def.exposes[i].name, name) == 0) return &def.exposes[i];
    }
    return nullptr;
}

// ── QZR-ZIG2400: remote, not a controllable cover ──────────────────
void test_qzr_zig2400() {
    using namespace zhc::devices::qmotion;

    // It is a remote: no toZigbee, no phantom on/off state expose.
    assert(kDef_QZR_ZIG2400.to_zigbee == nullptr);
    assert(kDef_QZR_ZIG2400.to_zigbee_count == 0);
    assert(!has_expose(kDef_QZR_ZIG2400, "state"));

    // Exposes action + read-only position (z2m e.numeric("position", STATE)).
    const Expose* action = find_expose(kDef_QZR_ZIG2400, "action");
    assert(action && action->type == ExposeType::Enum);
    const Expose* pos = find_expose(kDef_QZR_ZIG2400, "position");
    assert(pos && pos->type == ExposeType::Numeric);
    // STATE = read-only: not StateSet (must NOT have the Set bit).
    assert(pos->access == Access::State);

    // fz.identify: genIdentify (0x0003) attributeReport → action="identify".
    //   fc=0x18 | tsn | cmd=0x0A (report) | attr 0x0000 LE | type 0x21 | u16.
    {
        const std::array<std::uint8_t, 8> rep = {0x18, 0x01, 0x0A, 0x00, 0x00,
                                                 0x21, 0x00, 0x00};
        auto r = run(kDef_QZR_ZIG2400, 0x0003, 1,
                     std::span<const std::uint8_t>(rep.data(), rep.size()));
        assert(r.any_matched);
        const Value* a = r.merged.find("action");
        assert(a && a->type == ValueType::StringRef);
        assert(std::strcmp(a->str, "identify") == 0);
    }

    // fz.cover_position_tilt (lift half): closuresWindowCovering (0x0102)
    // attr 0x0008 currentPositionLiftPercentage = 42 (u8, type 0x20) →
    // position = 42. The remote must NOT emit a phantom on/off `state`.
    {
        const std::array<std::uint8_t, 7> rep = {0x18, 0x02, 0x0A, 0x08, 0x00,
                                                 0x20, 0x2A};
        auto r = run(kDef_QZR_ZIG2400, 0x0102, 1,
                     std::span<const std::uint8_t>(rep.data(), rep.size()));
        assert(r.any_matched);
        const Value* p = r.merged.find("position");
        assert(p && p->type == ValueType::Uint && p->u == 42);
        // No on/off decode path → no boolean `state`.
        assert(r.merged.find("state") == nullptr);
    }

    // genOnOff (0x0006) must NOT decode anything (kFzOnOff was removed).
    {
        const std::array<std::uint8_t, 7> rep = {0x18, 0x03, 0x0A, 0x00, 0x00,
                                                 0x10, 0x01};
        auto r = run(kDef_QZR_ZIG2400, 0x0006, 1,
                     std::span<const std::uint8_t>(rep.data(), rep.size()));
        assert(!r.any_matched);
        assert(r.merged.find("state") == nullptr);
    }
}

// ── HDM40PV620: roller blind — already correct, pin the contract ───
void test_hdm40pv620() {
    using namespace zhc::devices::qmotion;

    // A controllable cover: keeps its write path (position + state).
    assert(kDef_HDM40PV620.to_zigbee != nullptr);
    assert(kDef_HDM40PV620.to_zigbee_count == 1);
    // The single tz converter claims both "position" and "state" (its key is
    // nullptr → matches both), covering z2m's [tz.cover_state,
    // tz.cover_position_tilt].
    assert(kDef_HDM40PV620.to_zigbee[0]->key == nullptr);

    assert(has_expose(kDef_HDM40PV620, "position"));
    assert(has_expose(kDef_HDM40PV620, "battery"));

    // fz.cover_position_tilt: attr 0x0008 = 75 (u8) → position = 75.
    {
        const std::array<std::uint8_t, 7> rep = {0x18, 0x01, 0x0A, 0x08, 0x00,
                                                 0x20, 0x4B};
        auto r = run(kDef_HDM40PV620, 0x0102, 1,
                     std::span<const std::uint8_t>(rep.data(), rep.size()));
        assert(r.any_matched);
        const Value* p = r.merged.find("position");
        assert(p && p->type == ValueType::Uint && p->u == 75);
    }

    // fz.battery: genPowerCfg (0x0001) batteryPercentageRemaining
    // (attr 0x0021) = 200 (u8) → battery = 100 %.
    {
        const std::array<std::uint8_t, 7> rep = {0x18, 0x02, 0x0A, 0x21, 0x00,
                                                 0x20, 0xC8};
        auto r = run(kDef_HDM40PV620, 0x0001, 1,
                     std::span<const std::uint8_t>(rep.data(), rep.size()));
        assert(r.any_matched);
        const Value* b = r.merged.find("battery");
        assert(b && b->type == ValueType::Uint && b->u == 100);
    }
}

}  // namespace

int main() {
    test_qzr_zig2400();
    test_hdm40pv620();
    std::printf("qmotion parity tests passed\n");
    return 0;
}
