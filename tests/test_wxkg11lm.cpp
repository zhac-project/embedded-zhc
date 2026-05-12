// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: WXKG11LM — verifies fz_lumi_action_multistate drives the
// per-device action map via user_config. All six mapped values
// round-trip; an unmapped value is silently dropped.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/lumi/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefWXKG11LM;
}

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = 0x0012;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

DispatchResult dispatch(std::uint16_t raw_value, RuntimeStore<4>& store) {
    // Encode as attr 0x0055 u16. Even though our values fit in one
    // byte, the ZCL type prescribes 2 value bytes — emit both.
    std::uint8_t buf[8] = {
        0x18, 0x01, 0x0A,
        0x55, 0x00, 0x21,
        static_cast<std::uint8_t>(raw_value & 0xFF),
        static_cast<std::uint8_t>((raw_value >> 8) & 0xFF),
    };
    auto raw = build_frame(std::span<const std::uint8_t>(buf, 8));

    DecodedMessage msg{};
    if (!decode_frame(raw, {}, msg)) return {};
    msg.cluster = "genMultistateInput";

    RuntimeContext ctx{};
    ctx.store        = &store;
    ctx.store_get    = &RuntimeStore<4>::get;
    ctx.device_index = 0;
    return dispatch_from_zigbee(msg, {}, devices::lumi::kDefWXKG11LM,
                                  raw, ctx);
}

}  // namespace

static void test_all_six_actions_round_trip() {
    struct Case { std::uint16_t raw; const char* want; };
    const Case cases[] = {
        {0, "hold"},
        {1, "single"},
        {2, "double"},
        {3, "triple"},
        {4, "quadruple"},
        {5, "release"},
    };
    for (const auto& c : cases) {
        RuntimeStore<4> store{};
        const auto r = dispatch(c.raw, store);
        assert(r.any_matched);
        const Value* action = r.merged.find("action");
        assert(action && action->type == ValueType::StringRef);
        assert(std::strcmp(action->str, c.want) == 0);
    }
}

// Value 99 isn't in the device map → converter emits nothing.
static void test_unmapped_value_dropped() {
    RuntimeStore<4> store{};
    const auto r = dispatch(99, store);
    assert(r.any_matched == false);
}

// Device map must not be shadowed by the default — value 4 doesn't
// exist in kDefaultLumiActionMap but does here, so it must resolve to
// "quadruple".
static void test_device_map_overrides_default() {
    RuntimeStore<4> store{};
    const auto r = dispatch(4, store);
    assert(r.any_matched);
    const Value* action = r.merged.find("action");
    assert(action && std::strcmp(action->str, "quadruple") == 0);
}

int main() {
    test_all_six_actions_round_trip();
    test_unmapped_value_dropped();
    test_device_map_overrides_default();
    return 0;
}
