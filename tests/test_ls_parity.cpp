// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the L&S Lighting (ls) family. Pins the real gap the
// auto-generated SEMOTE port carried against the z2m wire shape:
//
//   * SEMOTE (756200643) is an action remote whose z2m
//     `m.commandsColorCtrl({commands:[..., "stop_move_step"]})` decodes a
//     lightingColorCtrl `commandStopMoveStep` (cmd 0x47) to
//     action="stop_move_step". The generic catalogue has no decoder for
//     0x47 (only 0x4C step / 0x4B move), so the port declared the
//     `stop_move_step` action in its enum but emitted it from NO converter
//     — a dead action key. Fixed with the bespoke `kFzLsCommandStopMoveStep`.
//
// The remaining SEMOTE actions (on/off, on_double/off_double, brightness
// step/stop, color-temp step) are regression-guarded here, and the three
// L&S lights are spot-checked for correct sink shape (state/brightness/
// color_temp, controllable, no phantom `action`).
//
// z2m-source: zigbee-herdsman-converters/src/devices/ls.ts.

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

namespace zhc::devices::ls {
extern const PreparedDefinition kDef_D756200643;   // SEMOTE remote (action)
extern const PreparedDefinition kDef_A319463;      // Emotion home base (light)
extern const PreparedDefinition kDef_D756200027;   // Mec Driver 1ch (light)
extern const PreparedDefinition kDef_D756200030;   // Mec Driver 4ch (light)
}  // namespace zhc::devices::ls

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

// command frames share low cmd ids across clusters (genOnOff Off cmd 0x00 vs
// genLevelCtrl MoveToLevel cmd 0x00), so the cluster name must be stamped
// AFTER decode for the dispatcher to discriminate. (See test_ctm_parity.)
DispatchResult dispatch_zcl(RuntimeContext& ctx, const PreparedDefinition& def,
                            std::uint16_t cluster_id, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn, cmd,
// then the command payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> payload = {}) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : payload) v.push_back(b);
    return v;
}

bool action_is(const DispatchResult& r, const char* expected) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

}  // namespace

// ── SEMOTE remote: every declared action must decode to a real key. ────────
static void test_semote() {
    const auto& def = devices::ls::kDef_D756200643;

    // Remote shape: action expose, battery, NO settable state, no write path.
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "state"));     // a remote is not a controllable relay
    assert(def.to_zigbee_count == 0);       // no Tz — pure outbound commands
    assert(def.endpoint_map == nullptr);    // single EP → no key suffixing
    assert(def_binds(def, 1, 0x0300));      // lightingColorCtrl bound (the 0x47 path)

    // THE FIX: lightingColorCtrl commandStopMoveStep (cmd 0x47) → stop_move_step.
    {
        RuntimeContext ctx{};
        auto r = dispatch_zcl(ctx, def, 0x0300, "lightingColorCtrl", cmd_frame(0x47));
        assert(r.any_matched);
        assert(action_is(r, "stop_move_step"));
    }

    // Regression guards for the rest of the action surface.
    {   // genOnOff On (0x01) → on.
        RuntimeContext ctx{};
        auto r = dispatch_zcl(ctx, def, 0x0006, "genOnOff", cmd_frame(0x01));
        assert(r.any_matched && action_is(r, "on"));
    }
    {   // genOnOff Off (0x00) → off.
        RuntimeContext ctx{};
        auto r = dispatch_zcl(ctx, def, 0x0006, "genOnOff", cmd_frame(0x00));
        assert(r.any_matched && action_is(r, "off"));
    }
    {   // genOnOff commandOnWithRecallGlobalScene (0x07) → on_double.
        RuntimeContext ctx{};
        auto r = dispatch_zcl(ctx, def, 0x0006, "genOnOff", cmd_frame(0x07));
        assert(r.any_matched && action_is(r, "on_double"));
    }
    {   // genOnOff commandOffWithEffect (0x40), 2-byte payload → off_double.
        RuntimeContext ctx{};
        const std::array<std::uint8_t, 2> eff = {0x00, 0x00};
        auto r = dispatch_zcl(ctx, def, 0x0006, "genOnOff",
                              cmd_frame(0x40, std::span<const std::uint8_t>(eff.data(), eff.size())));
        assert(r.any_matched && action_is(r, "off_double"));
    }
    {   // genLevelCtrl Step up (0x02): mode=0, size, tt → brightness_step_up.
        RuntimeContext ctx{};
        const std::array<std::uint8_t, 4> step = {0x00, 0x0A, 0x00, 0x00};
        auto r = dispatch_zcl(ctx, def, 0x0008, "genLevelCtrl",
                              cmd_frame(0x02, std::span<const std::uint8_t>(step.data(), step.size())));
        assert(r.any_matched && action_is(r, "brightness_step_up"));
    }
    {   // genLevelCtrl Stop (0x03) → brightness_stop.
        RuntimeContext ctx{};
        auto r = dispatch_zcl(ctx, def, 0x0008, "genLevelCtrl", cmd_frame(0x03));
        assert(r.any_matched && action_is(r, "brightness_stop"));
    }
    {   // lightingColorCtrl StepColorTemp (0x4C) up → color_temperature_step_up.
        RuntimeContext ctx{};
        const std::array<std::uint8_t, 5> sct = {0x01, 0x0A, 0x00, 0x00, 0x00};
        auto r = dispatch_zcl(ctx, def, 0x0300, "lightingColorCtrl",
                              cmd_frame(0x4C, std::span<const std::uint8_t>(sct.data(), sct.size())));
        assert(r.any_matched && action_is(r, "color_temperature_step_up"));
    }
}

// ── Lights: correct sink shape, NOT action devices. ────────────────────────
static void check_light(const PreparedDefinition& def, const char* state_key,
                        const char* brightness_key, const char* color_temp_key) {
    assert(def_exposes(def, state_key));
    assert(def_exposes(def, brightness_key));
    assert(def_exposes(def, color_temp_key));
    assert(!def_exposes(def, "action"));   // a light is not a remote
    assert(def.to_zigbee_count > 0);       // controllable → has a write path
}

static void test_lights() {
    using namespace zhc::devices::ls;
    // A319463 + 756200027 ship the flat single-EP shape.
    check_light(kDef_A319463, "state", "brightness", "color_temp");
    assert(def_exposes(kDef_A319463, "color"));   // Emotion is RGB+CT
    check_light(kDef_D756200027, "state", "brightness", "color_temp");

    // 756200030 is the 4-channel variant: keys are EP-suffixed and it
    // carries an endpoint_map so the dispatcher rewrites <key>_l<N>.
    assert(kDef_D756200030.endpoint_map != nullptr);
    check_light(kDef_D756200030, "state_l11", "brightness_l11", "color_temp_l11");
    assert(def_exposes(kDef_D756200030, "state_l14"));
}

int main() {
    test_semote();
    test_lights();
    return 0;
}
