// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Current Products Corp CP180335E-01 ("E-Wand"), a Gen. 2
// hybrid tilt blind. The auto-generated port wired the LIFT cover channel
// (kFzCoverPosition / kTzCoverPosition → "position") but z2m decodes this
// device via the TILT channel:
//
//   current_products_corp.ts #CP180335E-01:
//     fromZigbee: [fz.cover_position_tilt, fz.battery]
//     toZigbee:   [tz.cover_state, tz.cover_position_tilt]
//     meta: {coverStateFromTilt: true}
//     exposes: [e.battery(), e.cover_tilt()]   // = state + tilt
//
// The graduated Tier-2 override (definitions/current_products_corp/
// Cur_CP180335E_01.cpp) swaps to kFzCoverTilt (attr 0x0009 → "tilt") and
// wires kTzCoverState + kTzCoverPositionTilt, replacing the phantom
// "position" expose with "state" + "tilt".
//
// This test pins the expose list / fz cluster / tz keys and that a real
// closuresWindowCovering tilt report decodes to "tilt" at the
// dispatch_from_zigbee boundary.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::current_products_corp {
extern const PreparedDefinition kDef_CP180335E_01;
}  // namespace zhc::devices::current_products_corp

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster,
                            std::uint8_t  src_endpoint,
                            std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_endpoint;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

bool has_expose(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        const char* n = def.exposes[i].name;
        if (n && std::strcmp(n, name) == 0) return true;
    }
    return false;
}

bool has_binding(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool has_fz_cluster(const PreparedDefinition& def, const char* cluster) {
    for (std::uint8_t i = 0; i < def.from_zigbee_count; ++i) {
        const FzConverter* fz = def.from_zigbee[i];
        if (fz && fz->cluster && std::strcmp(fz->cluster, cluster) == 0) return true;
    }
    return false;
}

bool has_tz_key(const PreparedDefinition& def, const char* key) {
    for (std::uint8_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* tz = def.to_zigbee[i];
        if (tz && tz->key && std::strcmp(tz->key, key) == 0) return true;
    }
    return false;
}

DispatchResult dispatch_report(const PreparedDefinition& def,
                               std::uint16_t cluster_id,
                               const char* cluster_name,
                               std::uint8_t src_ep,
                               std::span<const std::uint8_t> bytes) {
    auto raw = build_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>{}, def, raw, ctx);
}

// closuresWindowCovering (0x0102):
//   attr 0x0009 CurrentPositionTiltPercentage (u8) = 25 -> "tilt"
constexpr std::uint8_t kTiltReport[] = {
    0x08, 0x10, 0x0A,
    0x09, 0x00, 0x20, 0x19,   // 0x0009 u8 = 25
};

// genPowerCfg (0x0001):
//   attr 0x0021 batteryPercentageRemaining (u8) = 0x96 = 150 -> 75 % (raw/2)
constexpr std::uint8_t kBatteryReport[] = {
    0x08, 0x11, 0x0A,
    0x21, 0x00, 0x20, 0x96,   // 0x0021 u8 = 150
};

}  // namespace

// ─────────────────────────────────────────────────────────────────────
// CP180335E-01 — tilt blind whose auto-port wired the wrong (lift) channel.
// ─────────────────────────────────────────────────────────────────────

void test_ewand_exposes_and_bindings() {
    const auto& def = devices::current_products_corp::kDef_CP180335E_01;
    // z2m e.battery() -> battery + voltage; e.cover_tilt() -> state + tilt.
    assert(has_expose(def, "battery"));
    assert(has_expose(def, "voltage"));
    assert(has_expose(def, "state"));     // ← cover_tilt() includes state
    assert(has_expose(def, "tilt"));      // ← gap fixed (real channel)
    assert(!has_expose(def, "position")); // ← phantom lift channel removed

    // fz: battery (genPowerCfg) + cover tilt (closuresWindowCovering 0x0009).
    assert(has_fz_cluster(def, "genPowerCfg"));
    assert(has_fz_cluster(def, "closuresWindowCovering"));

    // tz: cover_state (open/close/stop) + tilt set (goToTiltPercentage).
    assert(has_tz_key(def, "state"));     // ← kTzCoverState
    assert(has_tz_key(def, "tilt"));      // ← kTzCoverPositionTilt (gap fixed)

    // bindings: genPowerCfg 0x0001 + closuresWindowCovering 0x0102.
    assert(has_binding(def, 0x0001));
    assert(has_binding(def, 0x0102));

    // single-endpoint device → no endpoint_map, bare keys.
    assert(def.endpoint_map_count == 0);
}

void test_ewand_decodes_tilt() {
    const auto& def = devices::current_products_corp::kDef_CP180335E_01;
    auto r = dispatch_report(def, 0x0102, "closuresWindowCovering", 1,
                             {kTiltReport, sizeof(kTiltReport)});
    assert(r.any_matched);
    const Value* tilt = r.merged.find("tilt");   // ← gap fixed
    assert(tilt && tilt->type == ValueType::Uint && tilt->u == 25);
    // The (wrong) lift channel must NOT produce a phantom position key.
    assert(r.merged.find("position") == nullptr);
}

void test_ewand_decodes_battery() {
    const auto& def = devices::current_products_corp::kDef_CP180335E_01;
    auto r = dispatch_report(def, 0x0001, "genPowerCfg", 1,
                             {kBatteryReport, sizeof(kBatteryReport)});
    assert(r.any_matched);
    const Value* bat = r.merged.find("battery");
    assert(bat && bat->type == ValueType::Uint && bat->u == 75);  // 150/2
}

int main() {
    test_ewand_exposes_and_bindings();
    test_ewand_decodes_tilt();
    test_ewand_decodes_battery();
    return 0;
}
