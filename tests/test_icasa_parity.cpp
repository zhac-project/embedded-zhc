// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the iCasa family (dimmers / metering dimmer / keypads /
// remote). The one real z2m<->ez gap caught here:
//
//   * ICZB-IW21D — dimmer + electricity meter. z2m drives it with
//     extend=[m.light, m.electricityMeter()]. electricityMeter() defaults to
//     cluster:"both" (lib/modernExtend.ts genericMeter) → it decodes BOTH
//     seMetering (0x0702: energy 0x0000 / power 0x0400) AND
//     haElectricalMeasurement (0x0B04: power 0x050B / voltage 0x0505 /
//     current 0x0508), exposing power+voltage+current+energy. The auto-
//     generated port wired only kFzMetering (the 0x0702 half) and exposed
//     only energy+power, so voltage + current were dropped entirely. The fix
//     adds kFzElectricalMeasurement (0x0B04) + binding + voltage/current
//     exposes alongside the metering half.
//
// The keypad / remote action devices (KPD12 / KPD14S / KPD18S / RM11S) were
// already hand-ported as command-style action devices; this file asserts
// they still decode their genOnOff / genLevelCtrl / genScenes commands and
// battery (regression guard).
//
// z2m-source: zigbee-herdsman-converters/src/devices/icasa.ts +
//             lib/modernExtend.ts electricityMeter() (cluster:"both").

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

namespace zhc::devices::icasa {
extern const PreparedDefinition kDef_ICZB_IW21D;
extern const PreparedDefinition kDef_ICZB_KPD12;
extern const PreparedDefinition kDef_ICZB_KPD14S;
extern const PreparedDefinition kDef_ICZB_RM11S;
}  // namespace zhc::devices::icasa

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

constexpr std::uint16_t SE_METERING = 0x0702;
constexpr std::uint16_t HA_ELEC     = 0x0B04;
constexpr std::uint16_t GEN_ON_OFF  = 0x0006;
constexpr std::uint16_t GEN_LEVEL   = 0x0008;
constexpr std::uint16_t GEN_SCENES  = 0x0005;
constexpr std::uint16_t GEN_POWER   = 0x0001;

std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}
std::vector<std::uint8_t> u48(std::uint64_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8),
            static_cast<std::uint8_t>(x >> 16), static_cast<std::uint8_t>(x >> 24),
            static_cast<std::uint8_t>(x >> 32), static_cast<std::uint8_t>(x >> 40)};
}

void expect_uint(const PreparedDefinition& def, std::uint16_t cluster,
                 std::uint16_t attr, std::uint8_t type,
                 const std::vector<std::uint8_t>& val, const char* key,
                 std::uint64_t want) {
    assert(def_exposes(def, key));
    auto r = dispatch_zcl(def, cluster, 1, attr_report(attr, type, val));
    assert(r.any_matched);
    const Value* v = r.merged.find(key);
    assert(v && v->type == ValueType::Uint && v->u == want);
}

void expect_int(const PreparedDefinition& def, std::uint16_t cluster,
                std::uint16_t attr, std::uint8_t type,
                const std::vector<std::uint8_t>& val, const char* key,
                std::int64_t want) {
    assert(def_exposes(def, key));
    auto r = dispatch_zcl(def, cluster, 1, attr_report(attr, type, val));
    assert(r.any_matched);
    const Value* v = r.merged.find(key);
    assert(v && v->type == ValueType::Int && v->i == want);
}

// ── ICZB-IW21D: the metering-dimmer gap (cluster:"both") ─────────────
void check_iw21d() {
    const auto& def = devices::icasa::kDef_ICZB_IW21D;

    // Light half stays (regression).
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));

    // haElectricalMeasurement (0x0B04) — the half the auto-port dropped.
    expect_uint(def, HA_ELEC, 0x0505, 0x21, u16(2300), "voltage", 2300);   // RMSVoltage
    expect_uint(def, HA_ELEC, 0x0508, 0x21, u16(1500), "current", 1500);   // RMSCurrent
    expect_int (def, HA_ELEC, 0x050B, 0x29, u16(0xFFCE), "power", -50);     // ActivePower s16

    // seMetering (0x0702) — the half that was already wired.
    expect_uint(def, SE_METERING, 0x0000, 0x25, u48(987654), "energy", 987654);  // CurrentSummDelivered
    // InstantaneousDemand 0x0400 (s24) → power via generic kFzMetering.
    const std::uint8_t demand[] = {0xE8, 0x03, 0x00};  // 1000
    auto rp = dispatch_zcl(def, SE_METERING, 1, attr_report(0x0400, 0x2A, demand));
    const Value* pv = rp.merged.find("power");
    assert(pv && pv->type == ValueType::Int && pv->i == 1000);
}

// ── ICZB-KPD12: 2-button keypad — command-style action device ────────
void check_kpd12() {
    const auto& def = devices::icasa::kDef_ICZB_KPD12;
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "battery"));
    // Not a controllable on/off switch — no `state` expose.
    assert(!def_exposes(def, "state"));

    // genOnOff On (cmd 0x01) → action "on".
    auto ron = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x01, {}));
    assert(action_is(ron, "on"));
    // genOnOff Off (cmd 0x00) → action "off".
    auto roff = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x00, {}));
    assert(action_is(roff, "off"));
    // genLevelCtrl Move (cmd 0x01), mode=0 → "brightness_move_up".
    const std::uint8_t mv[] = {0x00, 0x32};  // mode=up, rate=50
    auto rmv = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x01, mv));
    assert(action_is(rmv, "brightness_move_up"));
    // genLevelCtrl Stop (cmd 0x03) → "brightness_stop".
    auto rst = dispatch_zcl(def, GEN_LEVEL, 1, cmd_frame(0x03, {}));
    assert(action_is(rst, "brightness_stop"));
    // genPowerCfg battery percentage (attr 0x0021, u8 → /2).
    expect_uint(def, GEN_POWER, 0x0021, 0x20,
                std::vector<std::uint8_t>{0xC8}, "battery", 100);  // 200/2 = 100%
}

// ── ICZB-KPD14S: 4-scene keypad — recall + store via genScenes ───────
void check_kpd14s() {
    const auto& def = devices::icasa::kDef_ICZB_KPD14S;
    assert(def_exposes(def, "action"));

    // genScenes Recall (cmd 0x05): group u16 LE + scene u8.
    const std::uint8_t recall3[] = {0x00, 0x00, 0x03};
    auto rr = dispatch_zcl(def, GEN_SCENES, 1, cmd_frame(0x05, recall3));
    assert(action_is(rr, "recall_3"));
    // genScenes Store (cmd 0x04): group u16 LE + scene u8.
    const std::uint8_t store2[] = {0x00, 0x00, 0x02};
    auto rs = dispatch_zcl(def, GEN_SCENES, 1, cmd_frame(0x04, store2));
    assert(action_is(rs, "store_2"));
}

// ── ICZB-RM11S: remote — recall + on/off/move ───────────────────────
void check_rm11s() {
    const auto& def = devices::icasa::kDef_ICZB_RM11S;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));

    auto ron = dispatch_zcl(def, GEN_ON_OFF, 1, cmd_frame(0x01, {}));
    assert(action_is(ron, "on"));
    const std::uint8_t recall1[] = {0x00, 0x00, 0x01};
    auto rr = dispatch_zcl(def, GEN_SCENES, 1, cmd_frame(0x05, recall1));
    assert(action_is(rr, "recall_1"));
}

}  // namespace

int main() {
    check_iw21d();
    check_kpd12();
    check_kpd14s();
    check_rm11s();
    return 0;
}
