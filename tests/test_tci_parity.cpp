// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the TCI family. The real z2m<->ez gap caught here:
//
//   * "TCI - Mini ZLL I" (Dali 1-10V driver) is BOTH a controllable light
//     AND a controller. z2m wires m.light() + m.commandsOnOff() +
//     m.commandsLevelCtrl(), so it exposes settable `state`/`brightness`
//     AND a composite `action`, with fz converters on the genOnOff /
//     genLevelCtrl command frames. The auto-generated port kept only the
//     light half (on/off + brightness) and silently dropped the `action`
//     expose + every command converter, so the controller role was dead.
//     Fixed by additively wiring the generic command converters.
//
//   * The plain drivers (122576, 151570, 676-00301024955Z) are bare
//     m.light() — on/off + brightness only, NO color / color_temp axis
//     and NO action. The SUSPECT "dropped color/colorTemp on RGBW/CCT"
//     was FALSE-by-category (z2m light() only pushes fz.color_colortemp +
//     the color/CT exposes when color/colorTemp args are set; these defs
//     set neither). Verified by the regression guard below.
//
// z2m-source: zigbee-herdsman-converters/src/devices/tci.ts.

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

namespace zhc::devices::tci {
extern const PreparedDefinition kDef_D122576;
extern const PreparedDefinition kDef_D151570;
extern const PreparedDefinition kDef_D676_00301024955Z;
extern const PreparedDefinition kDef_TCI___Mini_ZLL_I;
}  // namespace zhc::devices::tci

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

bool action_is(const DispatchResult& r, const char* expect) {
    const Value* v = r.merged.find("action");
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, expect) == 0;
}

constexpr std::uint16_t GEN_ON_OFF = 0x0006;
constexpr std::uint16_t GEN_LEVEL  = 0x0008;

// ── Mini ZLL I: light half + controller action stream ────────────────
void test_mini_zll() {
    const auto& def = devices::tci::kDef_TCI___Mini_ZLL_I;

    // Light half — settable state + brightness, plus the controller action.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "action"));
    // Light keeps its write path (it is a settable target, not a pure remote).
    assert(def.to_zigbee != nullptr && def.to_zigbee_count == 2);
    // No phantom color / color_temp axis.
    assert(!def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color"));

    // genOnOff On (cmd 0x01) → action "on"; Off (0x00) → "off"; Toggle (0x02).
    auto ron = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x01, {}));
    assert(ron.any_matched);
    assert(action_is(ron, "on"));
    auto roff = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x00, {}));
    assert(action_is(roff, "off"));
    auto rtog = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x02, {}));
    assert(action_is(rtog, "toggle"));

    // genLevelCtrl Step (cmd 0x02): step_mode u8 (0=up), size, time u16.
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

    // genLevelCtrl MoveToLevel (cmd 0x00): level u8, time u16.
    const std::uint8_t mtl[] = {0x80, 0x0A, 0x00};
    auto rmtl = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x00, mtl));
    assert(action_is(rmtl, "brightness_move_to_level"));

    // genLevelCtrl Stop (cmd 0x03) → brightness_stop.
    auto rst = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x03, {}));
    assert(action_is(rst, "brightness_stop"));
}

// ── plain drivers: dimmable only, no color/CT axis, no action ─────────
void check_plain(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    // FALSE-by-category: no tunable color / color_temp axis on these.
    assert(!def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color"));
    assert(!def_exposes(def, "color_xy"));
    assert(!def_exposes(def, "action"));
}

void test_plain_drivers() {
    check_plain(devices::tci::kDef_D122576);
    check_plain(devices::tci::kDef_D151570);
    check_plain(devices::tci::kDef_D676_00301024955Z);
}

}  // namespace

int main() {
    test_mini_zll();
    test_plain_drivers();
    return 0;
}
