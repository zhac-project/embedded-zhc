// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Halemeier family. Most of the catalogue is plain
// m.light() drivers (1-channel + 4-channel deviceEndpoints) which the
// auto-generator ports faithfully. The real z2m<->ez gap caught here is the
// three S-Mitter / X-Mitter REMOTES:
//
//   * HA-ZSM-MW2, HA-ZBM-MW2, HA-ZX1 — battery remotes. In z2m they decode
//     via fz.command_* (genOnOff / genLevelCtrl / lightingColorCtrl /
//     genScenes cluster-specific commands) and expose `action`. The auto-
//     generator instead emitted kFzOnOff + kTzOnOff — i.e. a controllable
//     on/off switch with a dead `state` attribute that the hardware never
//     reports. The fix rewires command_* converters + an `action` expose and
//     drops the phantom state / toZigbee.
//
// The 4-channel drivers (HA-ZM12/24-4K + HA-ZM12mw2-4K) carry an
// endpoint_map (l1..l4) already — asserted here as a regression guard.
//
// z2m-source: zigbee-herdsman-converters/src/devices/halemeier.ts.

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

namespace zhc::devices::halemeier {
extern const PreparedDefinition kDef_HA_ZSM_MW2;
extern const PreparedDefinition kDef_HA_ZBM_MW2;
extern const PreparedDefinition kDef_HA_ZX1;
extern const PreparedDefinition kDef_HA_ZM12_24_4K;
extern const PreparedDefinition kDef_HA_ZM12mw2_4K;
}  // namespace zhc::devices::halemeier

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

// ZCL attribute report (cluster-specific=false, Report=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
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

bool has_endpoint(const PreparedDefinition& def, const char* label, std::uint8_t ep) {
    for (std::size_t i = 0; i < def.endpoint_map_count; ++i)
        if (def.endpoint_map[i].label && std::strcmp(def.endpoint_map[i].label, label) == 0 &&
            def.endpoint_map[i].endpoint == ep)
            return true;
    return false;
}

constexpr std::uint16_t GEN_ON_OFF = 0x0006;
constexpr std::uint16_t GEN_LEVEL  = 0x0008;
constexpr std::uint16_t COLOR_CTRL = 0x0300;
constexpr std::uint16_t GEN_SCENES = 0x0005;
constexpr std::uint16_t GEN_POWER  = 0x0001;

// ── HA-ZSM-MW2: remote — on/off + step + step color temp + recall ────
void check_zsm_mw2() {
    const auto& def = devices::halemeier::kDef_HA_ZSM_MW2;
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "battery"));
    // Not a controllable on/off switch — no `state` expose, no toZigbee.
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);

    // genOnOff On (cmd 0x01) / Off (cmd 0x00).
    assert(action_is(dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x01, {})), "on"));
    assert(action_is(dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x00, {})), "off"));

    // genLevelCtrl Step (cmd 0x02): mode u8, size u8, tt u16 LE.
    const std::uint8_t step_up[] = {0x00, 0x4B, 0x0A, 0x00};
    assert(action_is(dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x02, step_up)),
                     "brightness_step_up"));
    const std::uint8_t step_dn[] = {0x01, 0x4B, 0x0A, 0x00};
    assert(action_is(dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x02, step_dn)),
                     "brightness_step_down"));

    // lightingColorCtrl StepColorTemperature (cmd 0x4C):
    //   mode u8, size u16 LE, tt u16 LE.
    const std::uint8_t ct_up[] = {0x01, 0x32, 0x00, 0x0A, 0x00};
    assert(action_is(dispatch_zcl(def, COLOR_CTRL, 1, cmd_frame(0x4C, ct_up)),
                     "color_temperature_step_up"));
    const std::uint8_t ct_dn[] = {0x03, 0x32, 0x00, 0x0A, 0x00};
    assert(action_is(dispatch_zcl(def, COLOR_CTRL, 1, cmd_frame(0x4C, ct_dn)),
                     "color_temperature_step_down"));

    // genScenes Recall (cmd 0x05): group u16 LE + scene u8.
    const std::uint8_t recall2[] = {0x00, 0x00, 0x02};
    assert(action_is(dispatch_zcl(def, GEN_SCENES, 1, cmd_frame(0x05, recall2)),
                     "recall_2"));

    // genPowerCfg battery percentage (attr 0x0021, u8 → /2).
    const std::uint8_t bat[] = {0xC8};  // 200/2 = 100%
    auto rb = dispatch_zcl(def, GEN_POWER, 1, attr_report(0x0021, 0x20, bat));
    const Value* bv = rb.merged.find("battery");
    assert(bv && bv->type == ValueType::Uint && bv->u == 100);
}

// ── HA-ZBM-MW2: basic remote — recall + on/off + step (+CT) ──────────
void check_zbm_mw2() {
    const auto& def = devices::halemeier::kDef_HA_ZBM_MW2;
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);

    assert(action_is(dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x01, {})), "on"));
    assert(action_is(dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x00, {})), "off"));

    const std::uint8_t recall4[] = {0x00, 0x00, 0x04};
    assert(action_is(dispatch_zcl(def, GEN_SCENES, 1, cmd_frame(0x05, recall4)),
                     "recall_4"));

    const std::uint8_t step_dn[] = {0x01, 0x4B, 0x0A, 0x00};
    assert(action_is(dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x02, step_dn)),
                     "brightness_step_down"));
}

// ── HA-ZX1: X-Mitter remote — on/off + move + stop ───────────────────
void check_zx1() {
    const auto& def = devices::halemeier::kDef_HA_ZX1;
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);

    assert(action_is(dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x01, {})), "on"));
    assert(action_is(dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x00, {})), "off"));

    // genLevelCtrl Move (cmd 0x01): mode u8 (0=up), rate u8.
    const std::uint8_t move_up[] = {0x00, 0x32};
    assert(action_is(dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x01, move_up)),
                     "brightness_move_up"));
    const std::uint8_t move_dn[] = {0x01, 0x32};
    assert(action_is(dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x01, move_dn)),
                     "brightness_move_down"));
    // genLevelCtrl Stop (cmd 0x03) → "brightness_stop".
    assert(action_is(dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x03, {})),
                     "brightness_stop"));
}

// ── 4-channel drivers: endpoint_map regression ───────────────────────
void check_4ch() {
    const auto& d4k = devices::halemeier::kDef_HA_ZM12_24_4K;
    assert(def_exposes(d4k, "state"));
    assert(def_exposes(d4k, "brightness"));
    assert(d4k.endpoint_map_count == 4);
    assert(has_endpoint(d4k, "l1", 1) && has_endpoint(d4k, "l2", 2) &&
           has_endpoint(d4k, "l3", 3) && has_endpoint(d4k, "l4", 4));

    const auto& dws = devices::halemeier::kDef_HA_ZM12mw2_4K;
    assert(dws.endpoint_map_count == 4);
    assert(has_endpoint(dws, "l4", 4));
}

}  // namespace

int main() {
    check_zsm_mw2();
    check_zbm_mw2();
    check_zx1();
    check_4ch();
    return 0;
}
