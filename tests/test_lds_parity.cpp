// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the LDS (Hive / Ynoa OEM) family. Two real z2m<->ez gaps
// pinned here after the fix:
//
//   Dropped color/colorTemp axis — ZBT-RGBWLight-A0000 is z2m
//   m.light({colorTemp:{range:[153,555]}, color:true}) → on/off + brightness
//   + colorTemp (mireds) + color (xy + hs). The auto-port collapsed it to
//   on/off + brightness only. Restored kFzColorTemperature + kFzColor
//   (+ kTzColorTemp/kTzColor), color_temp/color_x/color_y/hue/saturation
//   exposes, bind 0x0300.
//
//   Missing definition — PFMOT001 (Hive Pet Friendly Motion sensor) was
//   dropped entirely by the auto-port. z2m = m.battery() +
//   m.iasZoneAlarm({zoneType:"occupancy", zoneAttributes:["alarm_1","tamper",
//   "battery_low"]}) → fz.ias_occupancy_alarm_1 (zoneStatus bit 0) publishes
//   the semantic key `occupancy` (kFzIasMotionAlarm) + tamper/battery_low,
//   plus battery (kFzBattery). Added the whole definition.
//
//   FWBulb03UK (bare m.light()) is on/off + brightness only — correct as-is,
//   regression-guarded below.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lds.ts

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

namespace zhc::devices::lds {
extern const PreparedDefinition kDef_ZBT_RGBWLight_A0000;  // RGBW: color + CT
extern const PreparedDefinition kDef_FWBulb03UK;           // dimmable only
extern const PreparedDefinition kDef_PFMOT001;             // IAS motion -> occupancy
}  // namespace zhc::devices::lds

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

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

DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report builder (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── IAS: a 9-byte Zone Status Change Notification (ssIasZone cmd 0x00). ─────
struct IasFrame { std::array<std::uint8_t, 9> bytes; };

IasFrame ias_notif(std::uint16_t zone_status) {
    IasFrame f{};
    f.bytes = { 0x09, 0x42, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

// ── ZBT-RGBWLight-A0000: color + colorTemp axes restored. ──────────────────
void test_rgbw_light() {
    using namespace zhc::devices::lds;
    const auto& def = kDef_ZBT_RGBWLight_A0000;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));   // was dropped
    assert(def_exposes(def, "color_x"));
    assert(def_exposes(def, "color_y"));
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0300));        // lightingColorCtrl was missing
    assert(def.to_zigbee_count >= 4);         // on/off + brightness + ct + color

    // colorTemperature (0x0007, u16 mireds=370) on lightingColorCtrl -> color_temp.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> ct = {0x72, 0x01};   // 370 LE
    auto cr = dispatch_zcl_ep(c1, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0007, 0x21, std::span<const std::uint8_t>(ct.data(), ct.size())));
    assert(cr.any_matched);
    const Value* cv = cr.merged.find("color_temp");
    assert(cv && cv->type == ValueType::Uint && cv->u == 370);

    // currentX (0x0003, u16) on lightingColorCtrl -> color_x (float in [0,1]).
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> x = {0x00, 0x80};    // 0x8000
    auto xr = dispatch_zcl_ep(c2, def, 0x0300, 1, "lightingColorCtrl",
                              attr_report(0x0003, 0x21, std::span<const std::uint8_t>(x.data(), x.size())));
    assert(xr.any_matched);
    assert(xr.merged.find("color_x") != nullptr);

    // brightness (genLevelCtrl currentLevel 0x0000) still decodes.
    RuntimeContext c3{};
    const std::array<std::uint8_t, 1> lvl = {0x7F};
    auto br = dispatch_zcl_ep(c3, def, 0x0008, 1, "genLevelCtrl",
                             attr_report(0x0000, 0x20, std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    assert(br.any_matched);
    assert(br.merged.find("brightness") != nullptr);
}

// ── FWBulb03UK: bare dimmable light — on/off + brightness, NO color. ───────
void test_dimmable_light() {
    using namespace zhc::devices::lds;
    const auto& def = kDef_FWBulb03UK;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(!def_exposes(def, "color_temp"));  // bare m.light() -> no color axis
    assert(!def_exposes(def, "color_x"));
    assert(!def_binds(def, 1, 0x0300));

    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto sr = dispatch_zcl_ep(c1, def, 0x0006, 1, "genOnOff",
                              attr_report(0x0000, 0x10, std::span<const std::uint8_t>(on.data(), on.size())));
    assert(sr.any_matched);
    assert(b_true(sr.merged.find("state")));
}

// ── PFMOT001: IAS motion -> occupancy (bit 0) + tamper/battery_low + batt. ─
void test_motion_sensor() {
    using namespace zhc::devices::lds;
    const auto& def = kDef_PFMOT001;
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "alarm"));       // no bare IAS dead-key
    assert(def.to_zigbee_count == 0);         // sensor — no write path
    assert(def_binds(def, 1, 0x0001));        // genPowerCfg
    assert(def_binds(def, 1, 0x0500));        // ssIasZone

    // zoneStatus bit 0 set -> occupancy true.
    RuntimeContext c1{};
    auto f1 = ias_notif(0x0001);
    auto on = dispatch_zcl_ep(c1, def, 0x0500, 1, "ssIasZone",
                              std::span<const std::uint8_t>(f1.bytes.data(), f1.bytes.size()));
    assert(on.any_matched);
    assert(b_true(on.merged.find("occupancy")));
    assert(on.merged.find("alarm") == nullptr);

    // clear -> occupancy false.
    RuntimeContext c2{};
    auto f2 = ias_notif(0x0000);
    auto off = dispatch_zcl_ep(c2, def, 0x0500, 1, "ssIasZone",
                               std::span<const std::uint8_t>(f2.bytes.data(), f2.bytes.size()));
    assert(off.any_matched);
    assert(b_false(off.merged.find("occupancy")));

    // tamper(bit2) + battery_low(bit3) co-decode.
    RuntimeContext c3{};
    auto f3 = ias_notif(0x000C);
    auto tb = dispatch_zcl_ep(c3, def, 0x0500, 1, "ssIasZone",
                              std::span<const std::uint8_t>(f3.bytes.data(), f3.bytes.size()));
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
    assert(b_false(tb.merged.find("occupancy")));

    // battery percentage (genPowerCfg 0x0021, 0xC8=200 half-pct -> 100 %).
    RuntimeContext c4{};
    const std::array<std::uint8_t, 1> pct = {0xC8};
    auto br = dispatch_zcl_ep(c4, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20, std::span<const std::uint8_t>(pct.data(), pct.size())));
    assert(br.any_matched);
    const Value* bv = br.merged.find("battery");
    assert(bv && bv->type == ValueType::Uint && bv->u == 100);
}

}  // namespace

int main() {
    test_rgbw_light();
    test_dimmable_light();
    test_motion_sensor();
    return 0;
}
