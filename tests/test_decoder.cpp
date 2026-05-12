// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include <cassert>
#include <cstdint>
#include <cstring>

#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"
#include "zhc/zcl/foundation.hpp"

using namespace zhc;

// Helper: build an InboundApsFrame pointing at a stack buffer.
static InboundApsFrame make_frame(std::uint16_t cluster,
                                   std::uint8_t src_ep,
                                   std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_addr     = 0xABCD;
    f.group_id     = 0;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC0;
    f.data         = bytes;
    return f;
}

// ────────────────────────────────────────────────────────────────────────────
// Integration — an Aqara-style `Report Attributes` on cluster 0x0012
// (genMultistateInput) reporting the multistate gesture code on attr 0x0055.
// Wire bytes mirror what a real znp_wire capture of WXKG01LM / MFKZQ01LM emits:
//   ZCL header:  fc=0x18 (global, s→c, disable default)
//                tsn=0x42
//                cmd=0x0A (Report Attributes)
//   record:      attr_id=0x0055, type=0x21 (u16), value=0x005A
static void test_decode_multistate_report() {
    const std::uint8_t frame_bytes[] = {
        // header
        0x18, 0x42, 0x0A,
        // one record
        0x55, 0x00, 0x21, 0x5A, 0x00,
    };
    const AttrKeyEntry known[] = {
        {0x0055, "presentValue"},
    };

    DecodedMessage msg{};
    const bool ok = decode_frame(make_frame(0x0012, 2, frame_bytes), known, msg);
    assert(ok);
    assert(msg.family == FrameFamily::Zcl);
    assert(msg.type == MessageType::AttributeReport);
    assert(msg.direction == Direction::ServerToClient);
    assert(msg.manufacturer_specific == false);
    assert(msg.command_id == CMD_REPORT_ATTR);
    assert(msg.src_endpoint == 2);
    assert(msg.payload.count == 1);

    const Value* v = msg.payload.find("presentValue");
    assert(v && v->type == ValueType::Uint && v->u == 0x005A);
}

// ────────────────────────────────────────────────────────────────────────────
// Manufacturer-specific frame — Aqara LUMI (0x115F) reporting a single u16
// attribute at 0x0142. Header carries the manu code; decoder surfaces it.
static void test_decode_manufacturer_specific_report() {
    const std::uint8_t frame_bytes[] = {
        // header with manu bit set, manu=0x115F (LUMI)
        0x1C, 0x5F, 0x11, 0x01, 0x0A,
        // record: attr_id=0x0142, type=0x20 (u8), value=0x03
        0x42, 0x01, 0x20, 0x03,
    };
    DecodedMessage msg{};
    const bool ok = decode_frame(make_frame(0xFCC0, 1, frame_bytes), {}, msg);
    assert(ok);
    assert(msg.manufacturer_specific == true);
    assert(msg.manufacturer_code == 0x115F);
    assert(msg.type == MessageType::AttributeReport);
    assert(msg.payload.count == 1);
    // Unknown attr → decimal-string key
    assert(std::strcmp(msg.payload.items[0].key, "322") == 0);
    assert(msg.payload.items[0].value.type == ValueType::Uint);
    assert(msg.payload.items[0].value.u == 0x03);
}

// ────────────────────────────────────────────────────────────────────────────
// Tuya cluster 0xEF00 + cluster-specific bit → FrameFamily::TuyaDp.
// Body is the Tuya DP stream, left in raw_data for the dispatch layer.
static void test_decode_tuya_family_classification() {
    const std::uint8_t frame_bytes[] = {
        // header: cluster-specific, s→c, tsn=0x01, cmd=0x02 (Tuya DP report)
        0x09, 0x01, 0x02,
        // Tuya prefix would follow (seq/etc.) — not parsed at this layer.
        0xAA, 0xBB, 0xCC,
    };
    DecodedMessage msg{};
    const bool ok = decode_frame(make_frame(0xEF00, 1, frame_bytes), {}, msg);
    assert(ok);
    assert(msg.family == FrameFamily::TuyaDp);
    assert(msg.type == MessageType::Command);
    assert(msg.command_id == 0x02);
    // Dispatch layer walks raw_data (Tuya DPs); decoder leaves payload empty.
    assert(msg.payload.count == 0);
    assert(msg.raw_data.size() == sizeof(frame_bytes));
}

// ────────────────────────────────────────────────────────────────────────────
// Cluster-specific command on a regular ZCL cluster — type=Command, raw body.
static void test_decode_cluster_specific_onoff_toggle() {
    const std::uint8_t frame_bytes[] = {
        // fc=0x01 (cluster-specific, c→s), tsn=0x07, cmd=0x02 (Toggle)
        0x01, 0x07, 0x02,
    };
    DecodedMessage msg{};
    const bool ok = decode_frame(make_frame(0x0006, 1, frame_bytes), {}, msg);
    assert(ok);
    assert(msg.family == FrameFamily::Zcl);
    assert(msg.type == MessageType::Command);
    assert(msg.direction == Direction::ClientToServer);
    assert(msg.command_id == 0x02);
    assert(msg.payload.count == 0);
}

int main() {
    test_decode_multistate_report();
    test_decode_manufacturer_specific_report();
    test_decode_tuya_family_classification();
    test_decode_cluster_specific_onoff_toggle();
    return 0;
}
