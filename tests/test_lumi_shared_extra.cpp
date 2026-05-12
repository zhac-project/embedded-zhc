// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: one scenario per Task-2.4 shared converter. Each exercises
// a synthetic minimal PreparedDefinition carrying just that converter
// + its (optional) user_config. MCCGQ01LM gets full treatment via its
// own definition since it's a real device from the port plan.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/lumi/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefMCCGQ01LM;
}

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster,
                             std::uint8_t endpoint,
                             std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = endpoint;
    f.dst_endpoint = endpoint;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

PreparedDefinition make_def(const FzConverter* const* list, std::uint8_t n) {
    PreparedDefinition def{};
    static const char* models[] = { "SYNTH" };
    def.zigbee_models = models; def.zigbee_models_count = 1;
    def.model = "SYNTH"; def.vendor = "Test";
    def.from_zigbee = list; def.from_zigbee_count = n;
    return def;
}

bool approx(float a, float b, float eps) { return std::fabs(a - b) <= eps; }

}  // namespace

// ── fz_lumi_contact via MCCGQ01LM (exit-criterion device) ──────────

static void test_mccgq01lm_contact_open() {
    constexpr std::uint8_t kFrame[] = {
        0x18, 0x01, 0x0A,
        0x00, 0x00, 0x10, 0x00,   // genOnOff attr 0 bool = 0 (open)
    };
    auto raw = build_frame(0x0006, 1, kFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";

    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, {},
                                          devices::lumi::kDefMCCGQ01LM,
                                          raw, ctx);
    assert(r.any_matched);
    const Value* c = r.merged.find("contact");
    assert(c && c->type == ValueType::Bool && c->b == false);
}

static void test_mccgq01lm_contact_closed() {
    constexpr std::uint8_t kFrame[] = {
        0x18, 0x02, 0x0A,
        0x00, 0x00, 0x10, 0x01,
    };
    auto raw = build_frame(0x0006, 1, kFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";

    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, {},
                                          devices::lumi::kDefMCCGQ01LM,
                                          raw, ctx);
    assert(r.any_matched);
    const Value* c = r.merged.find("contact");
    assert(c && c->type == ValueType::Bool && c->b == true);
}

// ── fz_lumi_on_off endpoint-aware ──────────────────────────────────

static void test_lumi_on_off_without_endpoint_map_emits_state() {
    const FzConverter* const list[] = { &lumi::kFzLumiOnOff };
    auto def = make_def(list, 1);

    constexpr std::uint8_t kFrame[] = {
        0x18, 0x01, 0x0A, 0x00, 0x00, 0x10, 0x01,
    };
    auto raw = build_frame(0x0006, 1, kFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";

    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    assert(r.merged.find("state") != nullptr);
}

static void test_lumi_on_off_with_endpoint_map_emits_state_label() {
    static constexpr lumi::DeviceEndpointLabel kEntries[] = {
        {1, "state_left"},
        {2, "state_right"},
    };
    static constexpr lumi::DeviceEndpointLabels kMap{ kEntries, 2 };

    static constexpr FzConverter kFz{
        .family            = FrameFamily::Zcl,
        .cluster           = "genOnOff",
        .type_mask         = type_bit(MessageType::AttributeReport) |
                             type_bit(MessageType::ReadResponse),
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .zcl_fn = &lumi::fz_lumi_on_off },
        .user_config       = &kMap,
    };
    static const FzConverter* const list[] = { &kFz };
    auto def = make_def(list, 1);

    constexpr std::uint8_t kFrame[] = {
        0x18, 0x01, 0x0A, 0x00, 0x00, 0x10, 0x01,
    };
    auto raw = build_frame(0x0006, 2, kFrame);    // endpoint 2 → "right"
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";

    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    assert(r.merged.find("state") == nullptr);
    const Value* v = r.merged.find("state_right");
    assert(v && v->type == ValueType::Bool && v->b == true);
}

// ── fz_lumi_action (attr 0x8000) ──────────────────────────────────

static void test_lumi_action_via_click_count_attr() {
    const FzConverter* const list[] = { &lumi::kFzLumiAction };
    auto def = make_def(list, 1);

    constexpr std::uint8_t kFrame[] = {
        0x18, 0x01, 0x0A,
        0x00, 0x80, 0x20, 0x02,    // attr 0x8000 type 0x20 u8 = 2
    };
    auto raw = build_frame(0x0006, 1, kFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";

    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* action = r.merged.find("action");
    assert(action && action->type == ValueType::StringRef);
    assert(std::strcmp(action->str, "double") == 0);
}

// ── fz_lumi_operation_mode_basic ──────────────────────────────────

static void test_lumi_operation_mode_basic_maps_via_user_config() {
    static constexpr lumi::LumiActionEntry kEntries[] = {
        {0, "disabled"},
        {1, "on_off"},
        {2, "toggle"},
    };
    static constexpr lumi::LumiActionMap kMap{ kEntries, 3 };

    static constexpr FzConverter kFz{
        .family            = FrameFamily::Zcl,
        .cluster           = "genBasic",
        .type_mask         = type_bit(MessageType::AttributeReport) |
                             type_bit(MessageType::ReadResponse),
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .zcl_fn = &lumi::fz_lumi_operation_mode_basic },
        .user_config       = &kMap,
    };
    static const FzConverter* const list[] = { &kFz };
    auto def = make_def(list, 1);

    // genBasic attr 0xFF22 type 0x20 u8 = 2 → "toggle".
    constexpr std::uint8_t kFrame[] = {
        0x18, 0x01, 0x0A,
        0x22, 0xFF, 0x20, 0x02,
    };
    auto raw = build_frame(0x0000, 1, kFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genBasic";

    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* mode = r.merged.find("operation_mode");
    assert(mode && std::strcmp(mode->str, "toggle") == 0);
}

// ── fz_lumi_curtain_position ──────────────────────────────────────

static void test_lumi_curtain_position_emits_float() {
    const FzConverter* const list[] = { &lumi::kFzLumiCurtainPosition };
    auto def = make_def(list, 1);

    // genAnalogOutput attr 0x0055 type 0x39 (float32) = 42.5
    // 42.5 = 0x42 2A 00 00 → LE 00 00 2A 42
    constexpr std::uint8_t kFrame[] = {
        0x18, 0x01, 0x0A,
        0x55, 0x00, 0x39, 0x00, 0x00, 0x2A, 0x42,
    };
    auto raw = build_frame(0x000D, 1, kFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genAnalogOutput";

    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* pos = r.merged.find("position");
    assert(pos && pos->type == ValueType::Float);
    assert(approx(pos->f, 42.5f, 0.01f));
}

// ── fz_lumi_door_lock_report ──────────────────────────────────────

static void test_lumi_door_lock_report_maps_to_locked() {
    const FzConverter* const list[] = { &lumi::kFzLumiDoorLockReport };
    auto def = make_def(list, 1);

    constexpr std::uint8_t kFrame[] = {
        0x18, 0x01, 0x0A,
        0x00, 0x00, 0x30, 0x02,    // closuresDoorLock attr 0 enum8 = 2 (locked)
    };
    auto raw = build_frame(0x0101, 1, kFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "closuresDoorLock";

    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* s = r.merged.find("lock_state");
    assert(s && std::strcmp(s->str, "locked") == 0);
}

int main() {
    test_mccgq01lm_contact_open();
    test_mccgq01lm_contact_closed();
    test_lumi_on_off_without_endpoint_map_emits_state();
    test_lumi_on_off_with_endpoint_map_emits_state_label();
    test_lumi_action_via_click_count_attr();
    test_lumi_operation_mode_basic_maps_via_user_config();
    test_lumi_curtain_position_emits_float();
    test_lumi_door_lock_report_maps_to_locked();
    return 0;
}
