// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for Profalux cover motors + remotes.
// z2m-source: profalux.ts (#NB102, #MOT-C1ZxxC/F, #NSAV061).
//
// Bugs fixed (all auto-port hallucinations / dropped channels):
//   NB102        — z2m is fromZigbee:[]/toZigbee:[]/exposes:[] (an empty
//                  cover remote bound directly to the cover). The port
//                  hallucinated a phantom on/off `state` + battery bundle.
//                  Stripped to an empty definition.
//   MOT-C1ZxxC/F — z2m is a lift+tilt cover on EP2 (closuresWindowCovering).
//                  The port wired genOnOff behind a phantom `state` expose
//                  and dropped the tilt channel. Now decodes position
//                  (attr 0x0008) + tilt (attr 0x0009), exposes both, binds
//                  closuresWindowCovering on EP2 with default_endpoint=2,
//                  and carries no `state` expose.
//   NSAV061      — older cover driven via genLevelCtrl currentLevel
//                  (position) + genOnOff (OPEN/CLOSE). The port wired the
//                  generic kFzBrightness (raw `brightness` key, no 0..100
//                  scale) + kFzOnOff (boolean state), so `position` decoded
//                  to nothing. Now emits scaled `position` + OPEN/CLOSE
//                  `state` via the profalux/_shared cover-via-brightness
//                  converters.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::profalux {
extern const PreparedDefinition kDef_NB102;
extern const PreparedDefinition kDef_MOT_C1ZxxC_F;
extern const PreparedDefinition kDef_NSAV061;
}  // namespace zhc::devices::profalux

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("  FAIL: %s\n", what);
        ++g_failures;
    }
}

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

// ZCL attribute report (cluster-specific=false, Report=0x0A).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool uint_is(const DispatchResult& r, const char* key, std::uint64_t expect) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Uint && v->u == expect;
}
bool str_is(const DispatchResult& r, const char* key, const char* expect) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, expect) == 0;
}
bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
}
bool def_exposes(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (std::strcmp(def.exposes[i].name, name) == 0) return true;
    return false;
}

// ── NB102 — empty remote (no fz, no exposes) ─────────────────────────
void test_nb102_empty() {
    const auto& def = devices::profalux::kDef_NB102;
    check(def.exposes_count == 0,      "NB102 has no exposes");
    check(def.from_zigbee_count == 0,  "NB102 has no fz converters");
    check(def.to_zigbee_count == 0,    "NB102 has no tz converters");
    check(def.bindings_count == 0,     "NB102 has no bindings");
    // The phantom on/off must be gone: a genOnOff onOff report yields nothing.
    auto onbytes = attr_report(0x0000, 0x10,
                               std::span<const std::uint8_t>(
                                   std::array<std::uint8_t, 1>{0x01}.data(), 1));
    auto r = dispatch_zcl(def, 0x0006, 1,
                          std::span<const std::uint8_t>(onbytes.data(), onbytes.size()));
    check(!has_key(r, "state"), "NB102 genOnOff report emits no state");
}

// ── MOT-C1ZxxC/F — lift + tilt cover on EP2 ──────────────────────────
void test_mot_position_tilt() {
    const auto& def = devices::profalux::kDef_MOT_C1ZxxC_F;

    // currentPositionLiftPercentage (attr 0x0008, u8) → position.
    auto liftbytes = attr_report(0x0008, 0x20,
                                 std::span<const std::uint8_t>(
                                     std::array<std::uint8_t, 1>{0x2A}.data(), 1));  // 42 %
    auto rp = dispatch_zcl(def, 0x0102, 2,
                           std::span<const std::uint8_t>(liftbytes.data(), liftbytes.size()));
    check(uint_is(rp, "position", 42), "MOT decodes position 42");

    // currentPositionTiltPercentage (attr 0x0009, u8) → tilt.
    auto tiltbytes = attr_report(0x0009, 0x20,
                                 std::span<const std::uint8_t>(
                                     std::array<std::uint8_t, 1>{0x4B}.data(), 1));
    auto rt = dispatch_zcl(def, 0x0102, 2,
                           std::span<const std::uint8_t>(tiltbytes.data(), tiltbytes.size()));
    check(uint_is(rt, "tilt", 75), "MOT decodes tilt 75");

    // Exposes: position + tilt, no phantom on/off `state`.
    check(def_exposes(def, "position"), "MOT exposes position");
    check(def_exposes(def, "tilt"),     "MOT exposes tilt");
    check(!def_exposes(def, "state"),   "MOT has no phantom state expose");

    // Cover lives on EP2.
    check(def.default_endpoint == 2, "MOT default_endpoint == 2");
    bool ep2_cover = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == 2 && def.bindings[i].cluster_id == 0x0102)
            ep2_cover = true;
    check(ep2_cover, "MOT binds closuresWindowCovering on EP2");
    // genOnOff must NOT be bound (phantom removed).
    bool any_onoff = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == 0x0006) any_onoff = true;
    check(!any_onoff, "MOT no genOnOff binding");
}

// ── NSAV061 — genLevelCtrl-driven cover ──────────────────────────────
void test_nsav061_via_brightness() {
    const auto& def = devices::profalux::kDef_NSAV061;

    // genLevelCtrl currentLevel (attr 0x0000, u8) → scaled position + state.
    auto lvlbytes = attr_report(0x0000, 0x20,
                                std::span<const std::uint8_t>(
                                    std::array<std::uint8_t, 1>{0xFF}.data(), 1));  // 255 → 100 %
    auto rl = dispatch_zcl(def, 0x0008, 1,
                           std::span<const std::uint8_t>(lvlbytes.data(), lvlbytes.size()));
    check(uint_is(rl, "position", 100), "NSAV061 level 255 → position 100");
    check(str_is(rl, "state", "OPEN"),  "NSAV061 level 255 → state OPEN");
    // The phantom `brightness` key must never appear.
    check(!has_key(rl, "brightness"),   "NSAV061 no brightness key");

    // Half-open level → ~50 %.
    auto halfbytes = attr_report(0x0000, 0x20,
                                 std::span<const std::uint8_t>(
                                     std::array<std::uint8_t, 1>{0x80}.data(), 1));  // 128 → 50 %
    auto rh = dispatch_zcl(def, 0x0008, 1,
                           std::span<const std::uint8_t>(halfbytes.data(), halfbytes.size()));
    check(uint_is(rh, "position", 50),  "NSAV061 level 128 → position 50");

    // Zero level → position 0 + state CLOSE.
    auto zerobytes = attr_report(0x0000, 0x20,
                                 std::span<const std::uint8_t>(
                                     std::array<std::uint8_t, 1>{0x00}.data(), 1));
    auto rz = dispatch_zcl(def, 0x0008, 1,
                           std::span<const std::uint8_t>(zerobytes.data(), zerobytes.size()));
    check(uint_is(rz, "position", 0),   "NSAV061 level 0 → position 0");
    check(str_is(rz, "state", "CLOSE"), "NSAV061 level 0 → state CLOSE");

    // genOnOff onOff (attr 0x0000, bool) → state OPEN/CLOSE strings.
    auto onbytes = attr_report(0x0000, 0x10,
                               std::span<const std::uint8_t>(
                                   std::array<std::uint8_t, 1>{0x01}.data(), 1));
    auto ron = dispatch_zcl(def, 0x0006, 1,
                            std::span<const std::uint8_t>(onbytes.data(), onbytes.size()));
    check(str_is(ron, "state", "OPEN"), "NSAV061 onOff=1 → state OPEN");

    auto offbytes = attr_report(0x0000, 0x10,
                                std::span<const std::uint8_t>(
                                    std::array<std::uint8_t, 1>{0x00}.data(), 1));
    auto roff = dispatch_zcl(def, 0x0006, 1,
                             std::span<const std::uint8_t>(offbytes.data(), offbytes.size()));
    check(str_is(roff, "state", "CLOSE"), "NSAV061 onOff=0 → state CLOSE");

    check(def_exposes(def, "position"), "NSAV061 exposes position");
    check(def_exposes(def, "state"),    "NSAV061 exposes state");
}

}  // namespace

int main() {
    std::printf("test_profalux_parity\n");
    test_nb102_empty();
    test_mot_position_tilt();
    test_nsav061_via_brightness();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
