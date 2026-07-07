// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// REPORT.md §2.4 #1 — the decoder must stamp the ZCL cluster name so dispatch
// can filter converters by cluster. Manufacturer-specific frames are left
// UNLABELLED (nullptr → dispatch fail-open, unchanged) because the same numeric
// cluster id collides across vendors (0xFC01 = Niko/Legrand/Ubisys, 0x0000 =
// genBasic vs vsmart, …) — see CLUSTER_NAMES_AUDIT.md. A manufacturer-code-aware
// labeller is the documented follow-up.
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t src_ep,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = src_ep;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

constexpr std::uint16_t kGenOnOff = 0x0006;   // genOnOff

}  // namespace

// A standard (non-mfg) cluster-specific frame must get its cluster name stamped.
static void test_decode_labels_standard_cluster() {
    const std::uint8_t body[] = { 0x01, 0x42, 0x00 };  // FC=cluster-spec, seq, cmd=off
    auto raw = make_frame(kGenOnOff, 1, body);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    assert(!msg.manufacturer_specific);
    assert(msg.cluster != nullptr);
    assert(std::strcmp(msg.cluster, "genOnOff") == 0);
}

// A manufacturer-specific frame must stay unlabelled (fail-open preserved).
static void test_decode_leaves_mfg_specific_unlabelled() {
    // FC = cluster-specific(0x01) | manufacturer(0x04); mfg code 0x1234; seq; cmd.
    const std::uint8_t body[] = { 0x05, 0x34, 0x12, 0x42, 0x00 };
    auto raw = make_frame(kGenOnOff, 1, body);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    assert(msg.manufacturer_specific);
    assert(msg.manufacturer_code == 0x1234);
    assert(msg.cluster == nullptr);
}

// End-to-end: two converters identical except `.cluster`. A decoded genOnOff
// frame (NO manual stamp) must fire ONLY the genOnOff converter.
static bool fz_on(const DecodedMessage&, const FzConverter&,
                  const PreparedDefinition&, RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    out.put("on_hit", Value{.type = ValueType::Bool, .b = true});
    return true;
}
static bool fz_lvl(const DecodedMessage&, const FzConverter&,
                   const PreparedDefinition&, RuntimeContext&,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    out.put("lvl_hit", Value{.type = ValueType::Bool, .b = true});
    return true;
}

static void test_dispatch_filters_by_decoded_cluster() {
    FzConverter on{
        .family = FrameFamily::Zcl, .cluster = "genOnOff",
        .type_mask = type_bit(MessageType::Command),
        .command_id = WILDCARD_CMD_ID, .attr_id = WILDCARD_ATTR_ID,
        .endpoint = WILDCARD_ENDPOINT,
        .frame_flags_mask = 0, .frame_flags_value = 0,
        .direction = Direction::ClientToServer,
        .fn = { .zcl_fn = fz_on }, .user_config = nullptr,
    };
    FzConverter lvl = on;
    lvl.cluster   = "genLevelCtrl";
    lvl.fn.zcl_fn = fz_lvl;

    const FzConverter* arr[] = { &on, &lvl };
    PreparedDefinition def{};
    def.from_zigbee       = arr;
    def.from_zigbee_count = 2;

    const std::uint8_t body[] = { 0x01, 0x42, 0x00 };  // genOnOff cluster-spec cmd
    auto raw = make_frame(kGenOnOff, 1, body);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));   // NO manual cluster stamp

    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    assert(r.merged.find("on_hit")  != nullptr);   // genOnOff converter fired
    assert(r.merged.find("lvl_hit") == nullptr);   // genLevelCtrl filtered out
}

int main() {
    test_decode_labels_standard_cluster();
    test_decode_leaves_mfg_specific_unlabelled();
    test_dispatch_filters_by_decoded_cluster();
    return 0;
}
