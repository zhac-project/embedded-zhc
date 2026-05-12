// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: LGZCDD01LM — validates the `factory::LumiLight` bundle for
// {level, color_temp} options. On/off + level + color-temp round-trip
// end-to-end; outbound encoders are queried via their keys.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/lumi/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefLGZCDD01LM;
}

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster,
                             std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

DispatchResult dispatch(std::uint16_t cluster, const char* cluster_name,
                         std::span<const std::uint8_t> bytes) {
    auto raw = build_frame(cluster, bytes);
    DecodedMessage msg{};
    if (!decode_frame(raw, {}, msg)) return {};
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, devices::lumi::kDefLGZCDD01LM,
                                  raw, ctx);
}

// genOnOff attr 0x0000 bool = 1.
constexpr std::uint8_t kOnFrame[] = {
    0x18, 0x01, 0x0A, 0x00, 0x00, 0x10, 0x01,
};

// genLevelCtrl attr 0x0000 u8 = 200.
constexpr std::uint8_t kLevelFrame[] = {
    0x18, 0x02, 0x0A, 0x00, 0x00, 0x20, 0xC8,
};

// lightingColorCtrl attr 0x0007 u16 = 370 (mireds). LE 72 01.
constexpr std::uint8_t kColorTempFrame[] = {
    0x18, 0x03, 0x0A, 0x07, 0x00, 0x21, 0x72, 0x01,
};

}  // namespace

static void test_on_off_routes_through_factory() {
    const auto r = dispatch(0x0006, "genOnOff", kOnFrame);
    assert(r.any_matched);
    const Value* s = r.merged.find("state");
    assert(s && s->type == ValueType::Bool && s->b == true);
}

static void test_brightness_routes_through_factory() {
    const auto r = dispatch(0x0008, "genLevelCtrl", kLevelFrame);
    assert(r.any_matched);
    const Value* b = r.merged.find("brightness");
    assert(b && b->type == ValueType::Uint && b->u == 200);
}

static void test_color_temp_routes_through_factory() {
    const auto r = dispatch(0x0300, "lightingColorCtrl", kColorTempFrame);
    assert(r.any_matched);
    const Value* ct = r.merged.find("color_temp");
    assert(ct && ct->type == ValueType::Uint && ct->u == 370);
}

// Factory wires all three tz encoders — prove they're reachable by key.
static void test_tz_encoders_present() {
    const auto& def = devices::lumi::kDefLGZCDD01LM;
    assert(def.to_zigbee != nullptr);
    assert(def.to_zigbee_count == 3);

    bool has_state = false, has_bright = false, has_ct = false;
    for (std::uint8_t i = 0; i < def.to_zigbee_count; ++i) {
        const auto* tz = def.to_zigbee[i];
        if (!tz || !tz->key) continue;
        if (std::strcmp(tz->key, "state")      == 0) has_state  = true;
        if (std::strcmp(tz->key, "brightness") == 0) has_bright = true;
        if (std::strcmp(tz->key, "color_temp") == 0) has_ct     = true;
    }
    assert(has_state && has_bright && has_ct);
}

int main() {
    test_on_off_routes_through_factory();
    test_brightness_routes_through_factory();
    test_color_temp_routes_through_factory();
    test_tz_encoders_present();
    return 0;
}
