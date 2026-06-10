// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Light Solutions family. The real z2m<->ez gaps caught
// here are dropped lighting axes on the two non-plain bulbs:
//
//   * 42-032 (LED driver CCT) — z2m m.light({colorTemp:{range:[160,450]}}).
//     The auto-port wired only OnOff + Brightness, dropping the colorTemp
//     axis (lightingColorCtrl 0x0300 colorTemperature). Fixed by wiring the
//     generic kFzColorTemperature / kTzColorTemp, a `color_temp` expose and
//     the 0x0300 binding.
//
//   * 91-943 (LED Controller, RGBW+CCT) — z2m
//     m.light({color:true, colorTemp:{range:[160,450]}}). The auto-port
//     dropped BOTH the color (hue/sat/xy) and the colorTemp axes. Fixed by
//     wiring kFzColor + kFzColorTemperature (and tz), `color_temp` + `color_xy`
//     exposes and the 0x0300 binding.
//
// The remaining Light Solutions devices are FALSE-by-category (plain
// m.light() / m.onOff()) and are guarded below so a future regen that grows a
// phantom color axis is caught.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lightsolutions.ts.

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

namespace zhc::devices::lightsolutions {
extern const PreparedDefinition kDef_D42_032;
extern const PreparedDefinition kDef_D91_943;
extern const PreparedDefinition kDef_D91_943_PRO;
extern const PreparedDefinition kDef_D200403V2_B;
extern const PreparedDefinition kDef_D91_947_micro;
extern const PreparedDefinition kDef_D200106V3;
extern const PreparedDefinition kDef_D3004482_3137308_3137309;
}  // namespace zhc::devices::lightsolutions

using namespace zhc;

namespace {

constexpr std::uint16_t GEN_LEVEL  = 0x0008;
constexpr std::uint16_t COLOR_CTRL = 0x0300;

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
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report builder (fc=0x18, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
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

bool has_uint(const DispatchResult& r, const char* key, std::uint64_t expect) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Uint && v->u == expect;
}

bool has_float_key(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Float;
}

// ── 42-032: CCT driver — colorTemp axis restored ─────────────────────
void test_d42_032() {
    const auto& def = devices::lightsolutions::kDef_D42_032;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    // CCT only — no full color axis.
    assert(!def_exposes(def, "color_xy"));
    assert(def_binds(def, COLOR_CTRL));

    // lightingColorCtrl attr 0x0007 colorTemperature (u16) = 370 mireds.
    const std::uint8_t ct[] = {0x72, 0x01};
    auto rct = dispatch_zcl(def, COLOR_CTRL, attr_report(0x0007, 0x21, ct));
    assert(rct.any_matched);
    assert(has_uint(rct, "color_temp", 370));

    // genLevelCtrl attr 0x0000 currentLevel (u8) = 200 → brightness.
    const std::uint8_t lvl[] = {0xC8};
    auto rlv = dispatch_zcl(def, GEN_LEVEL, attr_report(0x0000, 0x20, lvl));
    assert(has_uint(rlv, "brightness", 200));
}

// ── 91-943: RGBW+CCT controller — color AND colorTemp restored ───────
void test_d91_943() {
    const auto& def = devices::lightsolutions::kDef_D91_943;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_xy"));
    assert(def_binds(def, COLOR_CTRL));

    // colorTemp axis: attr 0x0007 (u16) = 280 mireds.
    const std::uint8_t ct[] = {0x18, 0x01};
    auto rct = dispatch_zcl(def, COLOR_CTRL, attr_report(0x0007, 0x21, ct));
    assert(rct.any_matched);
    assert(has_uint(rct, "color_temp", 280));

    // color axis: attr 0x0003 currentX (u16) → color_x float.
    const std::uint8_t cx[] = {0x00, 0x80};
    auto rcx = dispatch_zcl(def, COLOR_CTRL, attr_report(0x0003, 0x21, cx));
    assert(rcx.any_matched);
    assert(has_float_key(rcx, "color_x"));

    // color axis: attr 0x0001 currentSaturation (u8) → saturation.
    const std::uint8_t sat[] = {0xFE};
    auto rsat = dispatch_zcl(def, COLOR_CTRL, attr_report(0x0001, 0x20, sat));
    assert(rsat.any_matched);
    assert(has_uint(rsat, "saturation", 254));
}

// ── FALSE-by-category guards: plain dimmers / switch ─────────────────
void check_plain_light(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(!def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color_xy"));
    assert(!def_binds(def, COLOR_CTRL));
}

void test_plain_lights() {
    check_plain_light(devices::lightsolutions::kDef_D91_943_PRO);
    check_plain_light(devices::lightsolutions::kDef_D200403V2_B);
    check_plain_light(devices::lightsolutions::kDef_D91_947_micro);
    check_plain_light(devices::lightsolutions::kDef_D3004482_3137308_3137309);
}

void test_switch() {
    const auto& def = devices::lightsolutions::kDef_D200106V3;
    assert(def_exposes(def, "state"));
    assert(!def_exposes(def, "brightness"));
    assert(!def_exposes(def, "color_temp"));
    assert(!def_binds(def, COLOR_CTRL));
}

}  // namespace

int main() {
    test_d42_032();
    test_d91_943();
    test_plain_lights();
    test_switch();
    return 0;
}
