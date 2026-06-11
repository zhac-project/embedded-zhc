// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Bubendorff roller-shutter family.
//
//   Missing tilt — MONO iD4 is a lift+tilt roller shutter:
//     z2m m.windowCovering({controls:["lift","tilt"]}) exposes both position
//     AND tilt and decodes both via fz.cover_position_tilt
//     (currentPositionLiftPercentage 0x0008 -> position,
//      currentPositionTiltPercentage 0x0009 -> tilt). The auto-port wired only
//     kFzCoverPosition / kTzCoverPosition (lift), dropping the tilt channel.
//     Added the generic kFzCoverTilt decoder + kTzCoverPositionTilt encoder +
//     a `tilt` expose. There is no battery in the z2m def, so the auto-port's
//     absence of battery is correct (not a gap).
//   z2m-source: bubendorff.ts #MONO iD4.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::bubendorff {
extern const PreparedDefinition kDef_MONO_iD4;   // cover lift + tilt
}  // namespace zhc::devices::bubendorff

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

// Keep `ctx` alive while the result is read: suffixing / scratch allocations
// alias RuntimeContext storage.
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

// ZCL attribute report builder (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── MONO iD4: position (0x0008) + tilt (0x0009) both decode. ───────────────
void test_mono_id4_cover() {
    using namespace zhc::devices::bubendorff;
    const auto& def = kDef_MONO_iD4;

    // Both halves exposed; lift + tilt write converters both wired
    // (kTzCoverPosition claims position/state, kTzCoverPositionTilt the tilt).
    assert(def_exposes(def, "position"));
    assert(def_exposes(def, "tilt"));
    assert(def.to_zigbee_count == 2);
    assert(def_binds(def, 1, 0x0102));

    // currentPositionLiftPercentage (0x0008, u8) -> position.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> p = {40};
    auto pr = dispatch_zcl_ep(c1, def, 0x0102, 1, "closuresWindowCovering",
                              attr_report(0x0008, 0x20, std::span<const std::uint8_t>(p.data(), p.size())));
    assert(pr.any_matched);
    const Value* pv = pr.merged.find("position");
    assert(pv && pv->type == ValueType::Uint && pv->u == 40);

    // currentPositionTiltPercentage (0x0009, u8) -> tilt.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> t = {70};
    auto tr = dispatch_zcl_ep(c2, def, 0x0102, 1, "closuresWindowCovering",
                              attr_report(0x0009, 0x20, std::span<const std::uint8_t>(t.data(), t.size())));
    assert(tr.any_matched);
    const Value* tv = tr.merged.find("tilt");
    assert(tv && tv->type == ValueType::Uint && tv->u == 70);
}

}  // namespace

int main() {
    test_mono_id4_cover();
    return 0;
}
