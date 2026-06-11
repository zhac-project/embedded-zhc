// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the SOMA SmartShades3 smart shade. The auto-generated
// port carried one real gap pinned here after the fix:
//
//   windowCovering tilt 0x0009 dropped —
//     z2m m.windowCovering({controls:["lift","tilt"]}) exposes BOTH position
//     (lift, attr 0x0008) AND tilt (currentPositionTiltPercentage, attr
//     0x0009), and decodes/writes both via fz/tz.cover_position_tilt. The
//     port wired only kFzCoverPosition / kTzCoverPosition (lift) and exposed
//     only `position`, dropping the tilt channel. Added kFzCoverTilt +
//     kTzCoverPositionTilt + a `tilt` expose. Both halves report on the same
//     closuresWindowCovering (0x0102) cluster, so the existing {1, 0x0102}
//     bind already covers tilt.
//
//   Battery (battery + voltage via m.battery() / kFzBattery) was already
//   correctly ported — asserted here as a non-regression.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::soma {
extern const PreparedDefinition kDef_SmartShades3;  // battery + cover lift + tilt
}  // namespace zhc::devices::soma

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

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool def_uses_fz(const PreparedDefinition& def, const FzConverter* conv) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i)
        if (def.from_zigbee[i] == conv) return true;
    return false;
}

bool def_uses_tz(const PreparedDefinition& def, const TzConverter* conv) {
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i)
        if (def.to_zigbee[i] == conv) return true;
    return false;
}

DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── SmartShades3: cover lift + tilt both live; battery half kept. ───────────
void test_smartshades3_tilt_and_battery() {
    using namespace zhc::devices::soma;
    const auto& def = kDef_SmartShades3;

    // Exposes: lift + tilt + battery half.
    assert(def_exposes(def, "position"));   // lift kept
    assert(def_exposes(def, "tilt"));       // tilt added
    assert(def_exposes(def, "battery"));    // battery half kept
    assert(def_exposes(def, "voltage"));

    // Converters: both cover decoders + both setters present.
    assert(def_uses_fz(def, &::zhc::generic::kFzCoverPosition));
    assert(def_uses_fz(def, &::zhc::generic::kFzCoverTilt));
    assert(def_uses_fz(def, &::zhc::generic::kFzBattery));
    assert(def_uses_tz(def, &::zhc::generic::kTzCoverPosition));
    assert(def_uses_tz(def, &::zhc::generic::kTzCoverPositionTilt));

    // Binds: window-covering (covers both lift+tilt) and power-config (battery).
    assert(def_binds(def, 1, 0x0102));      // closuresWindowCovering
    assert(def_binds(def, 1, 0x0001));      // genPowerCfg (battery)

    // currentPositionTiltPercentage (0x0009, u8) on closuresWindowCovering -> "tilt".
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> t = {0x32};   // 50 %
    auto tr = dispatch_zcl_ep(c1, def, 0x0102, 1, "closuresWindowCovering",
                              attr_report(0x0009, 0x20, std::span<const std::uint8_t>(t.data(), t.size())));
    assert(tr.any_matched);
    assert(tr.merged.find("tilt") != nullptr);

    // currentPositionLiftPercentage (0x0008, u8) -> "position" still works.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> p = {0x14};   // 20 %
    auto pr = dispatch_zcl_ep(c2, def, 0x0102, 1, "closuresWindowCovering",
                              attr_report(0x0008, 0x20, std::span<const std::uint8_t>(p.data(), p.size())));
    assert(pr.any_matched);
    assert(pr.merged.find("position") != nullptr);

    // batteryPercentageRemaining (0x0021, u8) on genPowerCfg -> "battery".
    RuntimeContext c3{};
    const std::array<std::uint8_t, 1> b = {0xC8};   // 200 raw -> 100 %
    auto br = dispatch_zcl_ep(c3, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20, std::span<const std::uint8_t>(b.data(), b.size())));
    assert(br.any_matched);
    assert(br.merged.find("battery") != nullptr);
}

}  // namespace

int main() {
    test_smartshades3_tilt_and_battery();
    return 0;
}
