// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Airam family. The real z2m<->ez gap caught here:
//
//   * AIRAM-CTR.U + CTR.UBX are command-style dimmer remotes. z2m wires
//     fz.command_on / command_off / command_step / command_move /
//     command_stop / command_recall and exposes a single composite
//     `action`. The auto-generated ports mis-ported them as controllable
//     on/off switches (kFzOnOff + kTzOnOff + a dead `state` Binary expose
//     + a genOnOff write path), so every button press was dropped. Fixed
//     by wiring the generic genOnOff / genLevelCtrl / genScenes command
//     converters and publishing `action`.
//
//   * The LED bulbs (4713406 GU10, 4713407 A60) are plain dimmable
//     m.light() — on/off + brightness only, no color/CT axis. Verified
//     FALSE-by-category (regression guard below).
//
// z2m-source: zigbee-herdsman-converters/src/devices/airam.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::airam {
extern const PreparedDefinition kDef_AIRAM_CTR_U;
extern const PreparedDefinition kDef_CTR_UBX;
extern const PreparedDefinition kDef_D4713406;
extern const PreparedDefinition kDef_D4713407;
}  // namespace zhc::devices::airam

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

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Cluster-specific command frame: fc=0x01, tsn, cmd, payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> body) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : body) v.push_back(b);
    return v;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool action_is(const DispatchResult& r, const char* expect) {
    const Value* v = r.merged.find("action");
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, expect) == 0;
}

constexpr std::uint16_t GEN_ON_OFF = 0x0006;
constexpr std::uint16_t GEN_LEVEL  = 0x0008;
constexpr std::uint16_t GEN_SCENES = 0x0005;

// ── command remotes: action stream, not a controllable on/off ────────
void check_remote(const PreparedDefinition& def) {
    // Must publish action; must NOT carry a phantom controllable state.
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    // No write path — a remote is a client, never a settable target.
    assert(def.to_zigbee == nullptr && def.to_zigbee_count == 0);
    // Action source clusters bound.
    assert(def_binds(def, GEN_ON_OFF));
    assert(def_binds(def, GEN_LEVEL));
    assert(def_binds(def, GEN_SCENES));

    // genOnOff On (cmd 0x01) → action "on".
    auto ron = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x01, {}));
    assert(ron.any_matched);
    assert(action_is(ron, "on"));
    // genOnOff Off (cmd 0x00) → action "off".
    auto roff = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x00, {}));
    assert(action_is(roff, "off"));
    // No bare on/off `state` leaks onto the shadow.
    assert(ron.merged.find("state") == nullptr);

    // genLevelCtrl Step (cmd 0x02): step_mode u8 (0=up), size u8, time u16.
    const std::uint8_t step_up[] = {0x00, 0x40, 0x0A, 0x00};
    auto rsu = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x02, step_up));
    assert(action_is(rsu, "brightness_step_up"));
    const std::uint8_t step_dn[] = {0x01, 0x40, 0x0A, 0x00};
    auto rsd = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x02, step_dn));
    assert(action_is(rsd, "brightness_step_down"));

    // genLevelCtrl Move (cmd 0x01): move_mode u8 (0=up), rate u8.
    const std::uint8_t mv_up[] = {0x00, 0x32};
    auto rmu = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x01, mv_up));
    assert(action_is(rmu, "brightness_move_up"));
    const std::uint8_t mv_dn[] = {0x01, 0x32};
    auto rmd = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x01, mv_dn));
    assert(action_is(rmd, "brightness_move_down"));

    // genLevelCtrl Stop (cmd 0x03) → brightness_stop.
    auto rst = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x03, {}));
    assert(action_is(rst, "brightness_stop"));

    // genScenes Recall (cmd 0x05): group u16 LE + scene u8 → recall_<n>.
    const std::uint8_t recall0[] = {0x00, 0x00, 0x00};
    auto rr = dispatch_zcl(def, GEN_SCENES, 1, cmd_frame(0x05, recall0));
    assert(action_is(rr, "recall_0"));
}

void test_ctr_u()   { check_remote(devices::airam::kDef_AIRAM_CTR_U); }
void test_ctr_ubx() { check_remote(devices::airam::kDef_CTR_UBX); }

// ── bulbs: plain dimmable, no color/CT axis (regression guard) ───────
void check_bulb(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // FALSE-by-category: no tunable color / color_temp axis on these.
    assert(!def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color"));
    assert(!def_exposes(def, "color_xy"));
    assert(!def_exposes(def, "action"));
}

void test_bulb_gu10() { check_bulb(devices::airam::kDef_D4713406); }
void test_bulb_a60()  { check_bulb(devices::airam::kDef_D4713407); }

}  // namespace

int main() {
    test_ctr_u();
    test_ctr_ubx();
    test_bulb_gu10();
    test_bulb_a60();
    return 0;
}
