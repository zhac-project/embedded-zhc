// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"
#include "zhc/types.hpp"

using namespace zhc;

// ───────────────────────────────────────────────────────────────────
// Test plumbing — direct DecodedMessage + InboundApsFrame builders.
// We don't run the parser here; dispatch is exercised in isolation.
namespace {

DecodedMessage make_msg(FrameFamily fam, MessageType type,
                         const char* cluster, Direction dir,
                         std::uint16_t cmd) {
    DecodedMessage m{};
    m.family      = fam;
    m.cluster     = cluster;
    m.type        = type;
    m.direction   = dir;
    m.command_id  = cmd;
    m.src_endpoint = 1;
    return m;
}

InboundApsFrame make_frame(std::uint8_t ep) {
    InboundApsFrame f{};
    f.src_endpoint = ep;
    return f;
}

}  // namespace

// ───────────────────────────────────────────────────────────────────
// Fixed env pointer shared with the unhandled hook.
static bool* s_unhandled_flag = nullptr;
static void unhandled_cb(const InboundApsFrame&, const PreparedDefinition*,
                          RuntimeContext&) {
    if (s_unhandled_flag) *s_unhandled_flag = true;
}

// ───────────────────────────────────────────────────────────────────
// Two converters match. Generic writes state=on; specific runs next
// and overwrites with state=toggle. Last-writer-wins assertion.
static bool fz_generic(const DecodedMessage&, const FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value v{}; v.type = ValueType::StringRef; v.str = "on";
    out.put("state", v);
    return true;
}
static bool fz_specific(const DecodedMessage&, const FzConverter&,
                         const PreparedDefinition&,
                         RuntimeContext&,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value v{}; v.type = ValueType::StringRef; v.str = "toggle";
    out.put("state", v);
    out.put("extra", Value{.type = ValueType::Bool, .b = true});
    return true;
}

static void test_array_order_merge_last_wins() {
    FzConverter cvt_generic{
        .family = FrameFamily::Zcl, .cluster = "genOnOff",
        .type_mask = type_bit(MessageType::AttributeReport),
        .command_id = WILDCARD_CMD_ID, .attr_id = WILDCARD_ATTR_ID,
        .endpoint = WILDCARD_ENDPOINT,
        .frame_flags_mask = 0, .frame_flags_value = 0,
        .direction = Direction::ServerToClient,
        .fn = { .zcl_fn = fz_generic },
        .user_config = nullptr,
    };
    FzConverter cvt_specific = cvt_generic;
    cvt_specific.fn.zcl_fn = fz_specific;

    const FzConverter* arr[] = { &cvt_generic, &cvt_specific };
    PreparedDefinition def{};
    def.from_zigbee       = arr;
    def.from_zigbee_count = 2;

    auto msg = make_msg(FrameFamily::Zcl, MessageType::AttributeReport,
                         "genOnOff", Direction::ServerToClient, 0x0A);
    auto raw = make_frame(1);
    RuntimeContext ctx{};

    auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched);
    assert(result.merged.count == 2);
    const Value* state = result.merged.find("state");
    assert(state && state->type == ValueType::StringRef);
    assert(std::strcmp(state->str, "toggle") == 0);   // specific won
    const Value* extra = result.merged.find("extra");
    assert(extra && extra->b == true);
}

// ───────────────────────────────────────────────────────────────────
// Zero matches → unhandled hook fires; any_matched == false.
static void test_zero_match_fires_unhandled() {
    FzConverter only{
        .family = FrameFamily::Zcl, .cluster = "genOnOff",
        .type_mask = type_bit(MessageType::AttributeReport),
        .command_id = WILDCARD_CMD_ID, .attr_id = WILDCARD_ATTR_ID,
        .endpoint = WILDCARD_ENDPOINT,
        .frame_flags_mask = 0, .frame_flags_value = 0,
        .direction = Direction::ServerToClient,
        .fn = { .zcl_fn = fz_generic },
        .user_config = nullptr,
    };
    const FzConverter* arr[] = { &only };
    PreparedDefinition def{};
    def.from_zigbee       = arr;
    def.from_zigbee_count = 1;

    // Frame on a different cluster — no match.
    auto msg = make_msg(FrameFamily::Zcl, MessageType::AttributeReport,
                         "genLevelCtrl", Direction::ServerToClient, 0x0A);
    auto raw = make_frame(1);

    bool fired = false;
    s_unhandled_flag = &fired;
    RuntimeContext ctx{};
    ctx.unhandled = unhandled_cb;

    auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched == false);
    assert(fired == true);
    s_unhandled_flag = nullptr;
}

// ───────────────────────────────────────────────────────────────────
// Tuya family must select `tuya_fn` not `zcl_fn`.
static bool fz_tuya_dps(std::span<const TuyaDpRecord> dps,
                         const DecodedMessage&, const FzConverter&,
                         const PreparedDefinition&,
                         RuntimeContext&,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (dps.size() != 1) return false;
    Value v{}; v.type = ValueType::Uint; v.u = dps[0].dp_id;
    out.put("dp_seen", v);
    return true;
}

static void test_tuya_family_routes_to_tuya_fn() {
    FzConverter cvt{
        .family = FrameFamily::TuyaDp, .cluster = nullptr,
        .type_mask = type_bit(MessageType::Command),
        .command_id = WILDCARD_CMD_ID, .attr_id = WILDCARD_ATTR_ID,
        .endpoint = WILDCARD_ENDPOINT,
        .frame_flags_mask = 0, .frame_flags_value = 0,
        .direction = Direction::ServerToClient,
        .fn = { .tuya_fn = fz_tuya_dps },
        .user_config = nullptr,
    };
    const FzConverter* arr[] = { &cvt };
    PreparedDefinition def{};
    def.from_zigbee       = arr;
    def.from_zigbee_count = 1;

    auto msg = make_msg(FrameFamily::TuyaDp, MessageType::Command,
                         nullptr, Direction::ServerToClient, 0x02);
    auto raw = make_frame(1);
    RuntimeContext ctx{};

    static const std::uint8_t payload[] = { 0xDE, 0xAD };
    TuyaDpRecord dps[1] = {
        { /*dp_id*/ 0x07, /*dp_type*/ 0x01,
          std::span<const std::uint8_t>(payload, sizeof(payload)) },
    };

    auto result = dispatch_from_zigbee(msg, dps, def, raw, ctx);
    assert(result.any_matched);
    const Value* v = result.merged.find("dp_seen");
    assert(v && v->type == ValueType::Uint && v->u == 0x07);
}

// ───────────────────────────────────────────────────────────────────
// Stateful converter — first call bumps counter, second call reads it
// back and emits it. Proves RuntimeStore is reachable through ctx.
static bool fz_stateful(const DecodedMessage&, const FzConverter&,
                         const PreparedDefinition&,
                         RuntimeContext& ctx,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    auto* st = ctx.device_state();
    if (!st) return false;
    st->counter += 1;
    Value v{}; v.type = ValueType::Uint; v.u = st->counter;
    out.put("hits", v);
    return true;
}

static void test_stateful_store_roundtrip() {
    FzConverter cvt{
        .family = FrameFamily::Zcl, .cluster = nullptr,
        .type_mask = type_bit(MessageType::AttributeReport),
        .command_id = WILDCARD_CMD_ID, .attr_id = WILDCARD_ATTR_ID,
        .endpoint = WILDCARD_ENDPOINT,
        .frame_flags_mask = 0, .frame_flags_value = 0,
        .direction = Direction::ServerToClient,
        .fn = { .zcl_fn = fz_stateful },
        .user_config = nullptr,
    };
    const FzConverter* arr[] = { &cvt };
    PreparedDefinition def{};
    def.from_zigbee       = arr;
    def.from_zigbee_count = 1;

    RuntimeStore<4> store{};
    RuntimeContext  ctx{};
    ctx.store        = &store;
    ctx.store_get    = &RuntimeStore<4>::get;
    ctx.device_index = 2;

    auto msg = make_msg(FrameFamily::Zcl, MessageType::AttributeReport,
                         nullptr, Direction::ServerToClient, 0x0A);
    auto raw = make_frame(1);

    auto first = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(first.any_matched);
    const Value* hits1 = first.merged.find("hits");
    assert(hits1 && hits1->u == 1);

    auto second = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(second.any_matched);
    const Value* hits2 = second.merged.find("hits");
    assert(hits2 && hits2->u == 2);

    assert(store.devices[2].counter == 2);
    assert(store.devices[0].counter == 0);  // other slots untouched
}

// ───────────────────────────────────────────────────────────────────
// Tz path — dispatch_to_zigbee routes a value by key and gets back
// a wire-ready APS frame from the converter.
static bool tz_state_encoder(std::string_view key, const Value& input,
                              const TzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              std::span<std::uint8_t> out, std::size_t& n) {
    if (key != "state") return false;
    if (input.type != ValueType::Bool) return false;
    if (out.size() < 3) return false;
    out[0] = 0x01;                          // fc: cluster-specific
    out[1] = 0xAA;                          // seq
    out[2] = input.b ? 0x01 : 0x00;         // on / off
    n = 3;
    return true;
}

static void test_dispatch_to_zigbee_encodes_on_off() {
    TzConverter tz{
        .key         = "state",
        .cluster     = "genOnOff",
        .cluster_id  = 0x0006,
        .command_id  = 0xFFFF,      // encoder picks per-request
        .fn          = tz_state_encoder,
        .user_config = nullptr,
    };
    const TzConverter* arr[] = { &tz };
    PreparedDefinition def{};
    def.to_zigbee       = arr;
    def.to_zigbee_count = 1;

    RuntimeContext ctx{};
    std::uint8_t frame[8] = {};
    Value v{}; v.type = ValueType::Bool; v.b = true;

    auto r = dispatch_to_zigbee(def, "state", v, ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0x0006);
    assert(r.frame_size == 3);
    assert(frame[0] == 0x01 && frame[1] == 0xAA && frame[2] == 0x01);

    Value miss{}; miss.type = ValueType::Int; miss.i = 0;
    auto rmiss = dispatch_to_zigbee(def, "not_a_key", miss, ctx, frame);
    assert(!rmiss.ok);
}

// ───────────────────────────────────────────────────────────────────
// run_configure walks def.bindings[] + def.reports[] and fires the
// registered hooks with the right arguments.
struct CfgTrace {
    int binds = 0;
    int reports = 0;
    std::uint16_t last_cluster = 0;
    std::uint16_t last_attr = 0;
};
static CfgTrace* s_cfg = nullptr;

static bool cb_bind(std::uint16_t, std::uint8_t, std::uint16_t cluster) {
    if (s_cfg) { s_cfg->binds++; s_cfg->last_cluster = cluster; }
    return true;
}
static bool cb_report(std::uint16_t, std::uint8_t, std::uint16_t cluster,
                      std::uint16_t attr, std::uint8_t, std::uint16_t,
                      std::uint16_t, std::uint32_t, std::uint16_t) {
    if (s_cfg) { s_cfg->reports++;
                  s_cfg->last_cluster = cluster;
                  s_cfg->last_attr = attr; }
    return true;
}

static void test_run_configure_walks_specs() {
    static constexpr BindingSpec kBinds[] = {
        {1, 0x0006},
        {1, 0x0001},
    };
    static constexpr ReportingSpec kReps[] = {
        {1, 0x0006, 0x0000, 0x10, 0, 3600, 0, 0},
        {1, 0x0001, 0x0021, 0x20, 10, 3600, 1, 0},
    };
    PreparedDefinition def{};
    def.bindings       = kBinds;
    def.bindings_count = 2;
    def.reports        = kReps;
    def.reports_count  = 2;

    CfgTrace t{};
    s_cfg = &t;

    RuntimeContext ctx{};
    ctx.configure_bind   = cb_bind;
    ctx.configure_report = cb_report;

    assert(run_configure(def, ctx));
    assert(t.binds == 2);
    assert(t.reports == 2);
    assert(t.last_cluster == 0x0001);
    assert(t.last_attr    == 0x0021);
    s_cfg = nullptr;
}

// ───────────────────────────────────────────────────────────────────
// dispatch_from_zigbee fires `def.on_event(idx, EventId::Checkin)` at
// the tail of every successful dispatch. Sleep-aware battery devices
// rely on this hook to push pending writes (time sync, reporting
// reapply, etc.) within the device's narrow wake window.

static int s_on_event_count = 0;
static EventId s_on_event_last = EventId::FirstSeen;
static std::uint16_t s_on_event_idx = 0xFFFF;
static void on_event_cb(std::uint16_t idx, EventId id, RuntimeContext&) {
    ++s_on_event_count;
    s_on_event_last = id;
    s_on_event_idx  = idx;
}

static void test_dispatch_fires_on_event_checkin() {
    s_on_event_count = 0;
    s_on_event_last  = EventId::FirstSeen;
    s_on_event_idx   = 0xFFFF;

    FzConverter cvt{
        .family = FrameFamily::Zcl, .cluster = "genOnOff",
        .type_mask = type_bit(MessageType::AttributeReport),
        .command_id = WILDCARD_CMD_ID, .attr_id = WILDCARD_ATTR_ID,
        .endpoint = WILDCARD_ENDPOINT,
        .frame_flags_mask = 0, .frame_flags_value = 0,
        .direction = Direction::ServerToClient,
        .fn = { .zcl_fn = &fz_generic },
        .user_config = nullptr,
    };
    const FzConverter* arr[] = { &cvt };

    PreparedDefinition def{};
    def.from_zigbee       = arr;
    def.from_zigbee_count = 1;
    def.on_event          = &on_event_cb;

    auto msg   = make_msg(FrameFamily::Zcl, MessageType::AttributeReport,
                          "genOnOff", Direction::ServerToClient, 0x0a);
    auto frame = make_frame(1);

    RuntimeContext ctx{};
    ctx.device_index = 42;
    auto r = dispatch_from_zigbee(msg, {}, def, frame, ctx);
    assert(r.any_matched);
    assert(s_on_event_count == 1);
    assert(s_on_event_last == EventId::Checkin);
    assert(s_on_event_idx  == 42);

    // Second dispatch fires again — every inbound is a checkin.
    auto r2 = dispatch_from_zigbee(msg, {}, def, frame, ctx);
    (void)r2;
    assert(s_on_event_count == 2);
}

// dispatch_from_zigbee with on_event=nullptr is the common case (most
// devices don't define the hook). Must not crash, must not skip the
// unhandled-frame fallback for unmatched messages.
static void test_dispatch_no_on_event_is_safe() {
    s_on_event_count = 0;

    PreparedDefinition def{};
    def.from_zigbee       = nullptr;
    def.from_zigbee_count = 0;
    def.on_event          = nullptr;

    bool unhandled = false;
    s_unhandled_flag = &unhandled;

    auto msg   = make_msg(FrameFamily::Zcl, MessageType::AttributeReport,
                          "genOnOff", Direction::ServerToClient, 0x0a);
    auto frame = make_frame(1);

    RuntimeContext ctx{};
    ctx.unhandled    = &unhandled_cb;
    ctx.device_index = 7;
    auto r = dispatch_from_zigbee(msg, {}, def, frame, ctx);
    assert(!r.any_matched);
    assert(unhandled);
    assert(s_on_event_count == 0);
    s_unhandled_flag = nullptr;
}

// ───────────────────────────────────────────────────────────────────
// Multi-endpoint suffix rewrite — when a device opts in via
// `endpoint_map`, dispatcher rewrites emitted keys with `_<label>` for
// keys not in the global blocklist. Three cases verified:
//   1) opted-in device, mapped EP, non-blocklisted key → suffixed.
//   2) opted-in device, mapped EP, blocklisted key (battery) → not suffixed.
//   3) opted-in device, unmapped EP → no rewrite.

static bool fz_emit_state_and_battery(const DecodedMessage&,
                                       const FzConverter&,
                                       const PreparedDefinition&,
                                       RuntimeContext&,
                                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value sv{}; sv.type = ValueType::Bool; sv.b = true;
    out.put("state", sv);
    Value bv{}; bv.type = ValueType::Uint; bv.u = 85;
    out.put("battery", bv);
    return true;
}

static FzConverter make_state_battery_cvt() {
    return FzConverter{
        .family = FrameFamily::Zcl, .cluster = "genOnOff",
        .type_mask = type_bit(MessageType::AttributeReport),
        .command_id = WILDCARD_CMD_ID, .attr_id = WILDCARD_ATTR_ID,
        .endpoint = WILDCARD_ENDPOINT,
        .frame_flags_mask = 0, .frame_flags_value = 0,
        .direction = Direction::ServerToClient,
        .fn = { .zcl_fn = &fz_emit_state_and_battery },
        .user_config = nullptr,
    };
}

static void test_multi_ep_dispatch_suffixes_keys() {
    constexpr EndpointLabel kMap[] = { {"l1", 1}, {"l2", 2}, {"l3", 3} };
    auto cvt = make_state_battery_cvt();
    const FzConverter* arr[] = { &cvt };

    PreparedDefinition def{};
    def.from_zigbee         = arr;
    def.from_zigbee_count   = 1;
    def.endpoint_map        = kMap;
    def.endpoint_map_count  = 3;

    auto msg   = make_msg(FrameFamily::Zcl, MessageType::AttributeReport,
                          "genOnOff", Direction::ServerToClient, 0x0a);
    msg.src_endpoint = 2;       // matches kMap[1] → label "l2"
    auto frame = make_frame(2);

    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, frame, ctx);
    assert(r.any_matched);

    // `state` got suffixed with the label for EP 2 → "state_l2"
    bool found_state_l2 = false;
    bool found_state_unsuffixed = false;
    bool found_battery_unsuffixed = false;
    for (std::uint8_t i = 0; i < r.merged.count; ++i) {
        const char* k = r.merged.items[i].key;
        if (std::strcmp(k, "state_l2") == 0) found_state_l2 = true;
        if (std::strcmp(k, "state")    == 0) found_state_unsuffixed = true;
        if (std::strcmp(k, "battery")  == 0) found_battery_unsuffixed = true;
    }
    assert(found_state_l2);
    assert(!found_state_unsuffixed);
    // Blocklist: battery stays unsuffixed.
    assert(found_battery_unsuffixed);
}

static void test_multi_ep_dispatch_unmapped_endpoint_no_rewrite() {
    constexpr EndpointLabel kMap[] = { {"l1", 1}, {"l2", 2} };
    auto cvt = make_state_battery_cvt();
    const FzConverter* arr[] = { &cvt };

    PreparedDefinition def{};
    def.from_zigbee         = arr;
    def.from_zigbee_count   = 1;
    def.endpoint_map        = kMap;
    def.endpoint_map_count  = 2;

    auto msg   = make_msg(FrameFamily::Zcl, MessageType::AttributeReport,
                          "genOnOff", Direction::ServerToClient, 0x0a);
    msg.src_endpoint = 5;       // NOT in kMap — no rewrite expected
    auto frame = make_frame(5);

    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, frame, ctx);
    assert(r.any_matched);

    bool found_state_unsuffixed = false;
    bool found_state_l5 = false;
    for (std::uint8_t i = 0; i < r.merged.count; ++i) {
        const char* k = r.merged.items[i].key;
        if (std::strcmp(k, "state")    == 0) found_state_unsuffixed = true;
        if (std::strcmp(k, "state_l5") == 0) found_state_l5 = true;
    }
    assert(found_state_unsuffixed);
    assert(!found_state_l5);
}

static void test_multi_ep_dispatch_no_endpoint_map_unchanged() {
    auto cvt = make_state_battery_cvt();
    const FzConverter* arr[] = { &cvt };

    PreparedDefinition def{};
    def.from_zigbee       = arr;
    def.from_zigbee_count = 1;
    // endpoint_map left null — single-EP behaviour preserved.

    auto msg   = make_msg(FrameFamily::Zcl, MessageType::AttributeReport,
                          "genOnOff", Direction::ServerToClient, 0x0a);
    msg.src_endpoint = 2;
    auto frame = make_frame(2);

    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, frame, ctx);
    assert(r.any_matched);

    bool found_state_unsuffixed = false;
    for (std::uint8_t i = 0; i < r.merged.count; ++i) {
        const char* k = r.merged.items[i].key;
        if (std::strcmp(k, "state") == 0) found_state_unsuffixed = true;
        // No `state_l*` should appear when no map is registered.
        assert(std::strncmp(k, "state_l", 7) != 0);
    }
    assert(found_state_unsuffixed);
}

int main() {
    test_array_order_merge_last_wins();
    test_zero_match_fires_unhandled();
    test_tuya_family_routes_to_tuya_fn();
    test_stateful_store_roundtrip();
    test_dispatch_to_zigbee_encodes_on_off();
    test_run_configure_walks_specs();
    test_dispatch_fires_on_event_checkin();
    test_dispatch_no_on_event_is_safe();
    test_multi_ep_dispatch_suffixes_keys();
    test_multi_ep_dispatch_unmapped_endpoint_no_rewrite();
    test_multi_ep_dispatch_no_endpoint_map_unchanged();
    return 0;
}
