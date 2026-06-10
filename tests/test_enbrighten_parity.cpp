// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Enbrighten (GE / Jasco) in-wall switch / outlet /
// dimmer family. The one real z2m<->ez gap caught here:
//
//   * Every device pairs its controllable surface with z2m's
//     m.commandsOnOff({commands:["on","off"], bind:true}) and (dimmers)
//     m.commandsLevelCtrl({commands:["brightness_move_up",
//     "brightness_move_down","brightness_stop"], bind:true}). Those bind
//     genOnOff / genLevelCtrl OUTPUT and decode the device's physical
//     button presses (genOnOff commandOn/commandOff, genLevelCtrl
//     Move/Stop) into an `action` enum. The auto-generated ports kept only
//     the controllable half (kFzOnOff / kFzBrightness / kFzMetering) and
//     dropped the command/action channel entirely. Fixed by wiring
//     kFzCommandOn / kFzCommandOff (all four) + kFzCommandMove /
//     kFzCommandStop (dimmers) and adding an `action` expose, while keeping
//     the controllable state / brightness / energy surfaces intact.
//
//   * Regression guard: electricityMeter({cluster:"metering"}) is seMetering
//     0x0702 only (energy + power); the metering devices must NOT carry a
//     0x0B04 voltage/current half (that would be cluster:"both"/"electrical").
//
// z2m-source: zigbee-herdsman-converters/src/devices/enbrighten.ts +
//             lib/modernExtend.ts commandsOnOff() / commandsLevelCtrl().

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

namespace zhc::devices::enbrighten {
extern const PreparedDefinition kDef_D43076;  // in-wall switch
extern const PreparedDefinition kDef_D43078;  // switch + energy
extern const PreparedDefinition kDef_D43080;  // dimmer
extern const PreparedDefinition kDef_D43082;  // dimmer + energy
}  // namespace zhc::devices::enbrighten

using namespace zhc;

namespace {

constexpr std::uint16_t GEN_ON_OFF = 0x0006;
constexpr std::uint16_t GEN_LEVEL  = 0x0008;
constexpr std::uint16_t SE_METERING = 0x0702;

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
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);  // command frames need a stamped cluster
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

bool binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

// genOnOff commandOn (0x01) / commandOff (0x00) → action "on" / "off".
void check_onoff_action(const PreparedDefinition& def) {
    assert(def_exposes(def, "action"));
    auto ron = dispatch_zcl(def, GEN_ON_OFF, cmd_frame(0x01, {}));
    assert(action_is(ron, "on"));
    auto roff = dispatch_zcl(def, GEN_ON_OFF, cmd_frame(0x00, {}));
    assert(action_is(roff, "off"));
}

// genLevelCtrl Move (0x01, mode/rate) / Stop (0x03) → brightness_move_*/stop.
void check_level_action(const PreparedDefinition& def) {
    const std::uint8_t up[]   = {0x00, 0x32};  // mode=up, rate=50
    const std::uint8_t down[] = {0x01, 0x32};  // mode=down, rate=50
    assert(action_is(dispatch_zcl(def, GEN_LEVEL, cmd_frame(0x01, up)),
                     "brightness_move_up"));
    assert(action_is(dispatch_zcl(def, GEN_LEVEL, cmd_frame(0x01, down)),
                     "brightness_move_down"));
    assert(action_is(dispatch_zcl(def, GEN_LEVEL, cmd_frame(0x03, {})),
                     "brightness_stop"));
}

// ── 43076: in-wall switch — controllable state + on/off action ───────
void check_d43076() {
    const auto& def = devices::enbrighten::kDef_D43076;
    assert(def_exposes(def, "state"));   // controllable half kept
    check_onoff_action(def);
}

// ── 43078: switch + energy monitoring (metering 0x0702 only) ─────────
void check_d43078() {
    const auto& def = devices::enbrighten::kDef_D43078;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    // metering cluster is 0x0702 only — no 0x0B04 voltage/current half.
    assert(!def_exposes(def, "voltage"));
    assert(!def_exposes(def, "current"));
    assert(binds(def, SE_METERING));
    check_onoff_action(def);
}

// ── 43080: dimmer — controllable light + on/off + level action ───────
void check_d43080() {
    const auto& def = devices::enbrighten::kDef_D43080;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    check_onoff_action(def);
    check_level_action(def);
}

// ── 43082: dimmer + energy monitoring (metering 0x0702 only) ─────────
void check_d43082() {
    const auto& def = devices::enbrighten::kDef_D43082;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(!def_exposes(def, "voltage"));
    assert(!def_exposes(def, "current"));
    check_onoff_action(def);
    check_level_action(def);
}

}  // namespace

int main() {
    check_d43076();
    check_d43078();
    check_d43080();
    check_d43082();
    return 0;
}
