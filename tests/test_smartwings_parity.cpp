// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Smartwings WM25L-Z roller shade. The auto-generated
// port wired the default kFzBattery, which always halves the genPowerCfg
// batteryPercentageRemaining (attr 0x0021) per the ZCL half-percent
// encoding. z2m, however, declares meta {battery:{dontDividePercentage:
// true}} for this model — the device already reports a whole 0..100 %
// value, so dividing by 2 read the battery level 2x low.
//
// The Tier-2 override (definitions/smartwings/Sma_WM25L_Z.cpp) swaps in the
// new generic kFzBatteryNoDivide converter, which mirrors fz_battery but
// emits batteryPercentageRemaining verbatim. Cover position (attr 0x0008)
// is unchanged: z2m's coverInverted:true path emits the lift percentage raw,
// which already matches kFzCoverPosition.
//
// z2m-source: smartwings.ts #WM25L-Z; converters/fromZigbee.ts `battery`
//             (dontDividePercentage path) + `cover_position_tilt`.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::smartwings {
extern const PreparedDefinition kDef_WM25L_Z;
}  // namespace zhc::devices::smartwings

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

// genPowerCfg (0x0001) Report Attributes carrying:
//   attr 0x0021 batteryPercentageRemaining (u8) = 90 -> "battery" 90 %
//   (dontDividePercentage: must NOT be halved to 45)
constexpr std::uint8_t kBatteryReport[] = {
    0x08, 0x10, 0x0A,
    0x21, 0x00, 0x20, 0x5A,   // 0x0021 u8 = 90
};

// closuresWindowCovering (0x0102):
//   attr 0x0008 CurrentPositionLiftPercentage (u8) = 60 -> "position"
constexpr std::uint8_t kLiftReport[] = {
    0x08, 0x53, 0x0A,
    0x08, 0x00, 0x20, 0x3C,   // 0x0008 u8 = 60
};

}  // namespace

// ─────────────────────────────────────────────────────────────────────
// WM25L-Z — roller shade: cover position + dontDividePercentage battery.
// ─────────────────────────────────────────────────────────────────────

void test_wm25l_exposes_and_bindings() {
    const auto& def = devices::smartwings::kDef_WM25L_Z;
    assert(has_expose(def, "position"));
    assert(has_expose(def, "battery"));
    assert(has_fz_cluster(def, "closuresWindowCovering"));
    assert(has_fz_cluster(def, "genPowerCfg"));   // battery (no-divide) wired
    assert(has_binding(def, 0x0102));
    assert(has_binding(def, 0x0001));
    assert(def.endpoint_map_count == 0);  // single-endpoint: bare keys
}

void test_wm25l_battery_not_halved() {
    const auto& def = devices::smartwings::kDef_WM25L_Z;
    auto r = dispatch_report(def, 0x0001, "genPowerCfg", 1,
                             {kBatteryReport, sizeof(kBatteryReport)});
    assert(r.any_matched);
    const Value* b = r.merged.find("battery");
    // dontDividePercentage: raw 90 stays 90 (default kFzBattery would emit 45).
    assert(b && b->type == ValueType::Uint && b->u == 90);
}

void test_wm25l_decodes_position() {
    const auto& def = devices::smartwings::kDef_WM25L_Z;
    auto r = dispatch_report(def, 0x0102, "closuresWindowCovering", 1,
                             {kLiftReport, sizeof(kLiftReport)});
    assert(r.any_matched);
    const Value* pos = r.merged.find("position");
    assert(pos && pos->type == ValueType::Uint && pos->u == 60);
}

int main() {
    test_wm25l_exposes_and_bindings();
    test_wm25l_battery_not_halved();
    test_wm25l_decodes_position();
    return 0;
}
