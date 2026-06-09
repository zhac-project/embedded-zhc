// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Parity regression for the Hue motion sensors (SML00x).  The `occupancy`
// expose + the msOccupancySensing (0x0406) binding were always declared, but
// the shared fz array `kFzPhilipsMotionSensor` omitted the decoder, so the
// state never surfaced.  This wires `generic::kFzOccupancy` and proves the
// SML001 def (kDef_D9290012607) now decodes occupancy from cluster 0x0406.

#include <cassert>
#include <cstdint>
#include <span>

#include "definitions/philips/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::philips {
extern const PreparedDefinition kDef_D9290012607;  // SML001 — Hue motion sensor
}

using namespace zhc;

namespace {

// msOccupancySensing (0x0406), attr 0x0000, type 0x18 (bitmap8).
// Header 0x18 0x01 0x0A = ServerToClient, seq 1, ReadAttrResponse? no — this is
// an AttributeReport (cmd 0x0A), matching the lumi RTCGQ11LM fixture shape.
constexpr std::uint8_t kOccupiedFrame[] = {
    0x18, 0x01, 0x0A,            // ZCL header (cmd 0x0A = report attributes)
    0x00, 0x00, 0x18, 0x01,      // attr 0x0000, type 0x18 (bitmap8), value 0x01
};
constexpr std::uint8_t kVacantFrame[] = {
    0x18, 0x02, 0x0A,
    0x00, 0x00, 0x18, 0x00,      // value 0x00 — not occupied
};

InboundApsFrame build_frame(std::uint16_t cluster, std::uint8_t endpoint,
                            std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = endpoint;   // Hue motion sensors report on endpoint 2
    f.dst_endpoint = 1;
    f.linkquality  = 0xB8;
    f.data         = bytes;
    return f;
}

bool decode_occupancy(std::span<const std::uint8_t> bytes, bool& out) {
    auto raw = build_frame(0x0406, 2, bytes);
    DecodedMessage msg{};
    if (!decode_frame(raw, {}, msg)) return false;
    msg.cluster = "msOccupancySensing";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(msg, {},
                                             devices::philips::kDef_D9290012607,
                                             raw, ctx);
    if (!result.any_matched) return false;
    const Value* occ = result.merged.find("occupancy");
    if (!occ || occ->type != ValueType::Bool) return false;
    out = occ->b;
    return true;
}

}  // namespace

int main() {
    bool occ = false;

    // Occupied: bit 0 set → occupancy == true.
    assert(decode_occupancy(kOccupiedFrame, occ));
    assert(occ == true);

    // Vacant: bit 0 clear → occupancy == false (decoder still matches).
    assert(decode_occupancy(kVacantFrame, occ));
    assert(occ == false);

    return 0;
}
