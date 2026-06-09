// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Sinopé (Sinope) device family. Three real gaps in
// the auto-generated ports are pinned here against z2m wire shapes:
//
//   1. Leak detectors WL4200 / WL4200S / WL4210 and the Calypso water-
//      heater controller RM3500ZB wired the generic kFzIasZone (emits the
//      bare key "alarm") behind an expose that declared "alarm" — so the
//      semantic `water_leak` state z2m publishes (fz.ias_water_leak_alarm_1
//      / fzLocal.ias_water_leak_alarm) never reached the shadow. They also
//      dropped the on-board temperature sensor (fz.temperature). Fixed by
//      the typed kFzIasWaterLeakAlarm + kFzTemperature.
//
//   2. Water valves VA4200WZ / VA4201WZ / VA4220ZB used the generic cover
//      converters, which read/write `closuresWindowCovering`. The valves
//      actually report position on genLevelCtrl currentLevel and open/closed
//      on genOnOff (z2m fz.cover_position_via_brightness +
//      fz.cover_state_via_onoff, tz.cover_via_brightness). As generated,
//      position decode was dead and `state` was absent. Fixed by the Sinopé
//      valve converters (kFzSinopeValvePosition / kFzSinopeValveState /
//      kTzSinopeValvePosition).
//
//   3. Tank-level monitor LM4110ZB only wired kFzBattery, dropping its whole
//      purpose: the gauge fill level (z2m fzLocal.tank_level, genAnalogInput
//      presentValue → %) and temperature. Fixed by kFzSinopeTankLevel +
//      kFzTemperature.
//
// z2m-source: zigbee-herdsman-converters/src/devices/sinope.ts.

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::sinope {
extern const PreparedDefinition kDef_WL4200;     // leak  → water_leak + temperature
extern const PreparedDefinition kDef_WL4200S;
extern const PreparedDefinition kDef_WL4210;
extern const PreparedDefinition kDef_RM3500ZB;   // calypso → water_leak + temperature
extern const PreparedDefinition kDef_VA4200WZ;   // valve → state + position
extern const PreparedDefinition kDef_VA4201WZ;
extern const PreparedDefinition kDef_VA4220ZB;
extern const PreparedDefinition kDef_LM4110ZB;   // tank → tank_level + temperature
}  // namespace zhc::devices::sinope

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

bool str_is(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// Dispatch a decoded message, supplying the cluster name the wire frame
// implies (decode_frame leaves cluster id→name mapping to the caller).
DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── IAS Zone Status Change Notification (ssIasZone cmd 0x00) ──────────
struct IasFrame { std::array<std::uint8_t, 9> bytes; };
IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = { 0x09, tsn, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}
DispatchResult dispatch_ias(const PreparedDefinition& def, const IasFrame& f) {
    return dispatch_zcl(def, 0x0500, "ssIasZone",
                        std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
}

// ── ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then records ─
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── Leak / Calypso: typed water_leak + temperature ────────────────────
void check_water_leak(const PreparedDefinition& def) {
    // Regression guard: semantic key exposed, bare "alarm" gone.
    assert(def_exposes(def, "water_leak"));
    assert(!def_exposes(def, "alarm"));
    assert(def_exposes(def, "temperature"));

    auto leak = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 (bit 0)
    assert(leak.any_matched);
    assert(b_true(leak.merged.find("water_leak")));
    assert(leak.merged.find("alarm") == nullptr);       // dead key must be gone

    auto clear = dispatch_ias(def, ias_notif(0x0000));
    assert(clear.any_matched);
    assert(b_false(clear.merged.find("water_leak")));

    // Temperature: msTemperatureMeasurement attr 0x0000, s16 (×0.01).
    const std::uint8_t t[] = {0x09, 0x09};               // 0x0909 = 2313 → 23.13 °C
    auto r = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement",
                          attr_report(0x0000, 0x29, t));
    assert(r.any_matched);
    const Value* tv = r.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);
    assert(std::fabs(tv->f - 23.13f) < 0.01f);
}

// ── Valves: state (genOnOff) + position (genLevelCtrl) ────────────────
void check_valve(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "position"));

    // genOnOff onOff=1 → state OPEN; =0 → CLOSE.
    const std::uint8_t on[]  = {0x01};
    auto ro = dispatch_zcl(def, 0x0006, "genOnOff", attr_report(0x0000, 0x10, on));
    assert(ro.any_matched);
    assert(str_is(ro.merged.find("state"), "OPEN"));

    const std::uint8_t off[] = {0x00};
    auto rc = dispatch_zcl(def, 0x0006, "genOnOff", attr_report(0x0000, 0x10, off));
    assert(str_is(rc.merged.find("state"), "CLOSE"));

    // genLevelCtrl currentLevel=255 → position 100, state OPEN.
    const std::uint8_t full[] = {0xFF};
    auto rf = dispatch_zcl(def, 0x0008, "genLevelCtrl", attr_report(0x0000, 0x20, full));
    assert(rf.any_matched);
    const Value* p = rf.merged.find("position");
    assert(p && p->type == ValueType::Uint && p->u == 100);
    assert(str_is(rf.merged.find("state"), "OPEN"));

    // currentLevel=0 → position 0, state CLOSE.
    const std::uint8_t zero[] = {0x00};
    auto rz = dispatch_zcl(def, 0x0008, "genLevelCtrl", attr_report(0x0000, 0x20, zero));
    const Value* p0 = rz.merged.find("position");
    assert(p0 && p0->u == 0);
    assert(str_is(rz.merged.find("state"), "CLOSE"));

    // Write path: position 100 → genLevelCtrl moveToLevelWithOnOff, level 255.
    RuntimeContext ctx{};
    std::array<std::uint8_t, 16> frame{};
    Value pv{}; pv.type = ValueType::Uint; pv.u = 100;
    auto w = dispatch_to_zigbee(def, "position", pv, ctx, frame);
    assert(w.ok);
    assert(w.cluster_id == 0x0008);
    assert(w.frame_size == 6);
    assert(frame[2] == 0x04);          // moveToLevelWithOnOff
    assert(frame[3] == 0xFF);          // 100% → 255

    // Write path: state "OPEN" → level 255; "CLOSE" → 0; "STOP" → cmd 0x03.
    Value so{}; so.type = ValueType::StringRef; so.str = "OPEN";
    auto wo = dispatch_to_zigbee(def, "state", so, ctx, frame);
    assert(wo.ok && frame[2] == 0x04 && frame[3] == 0xFF);

    Value sc{}; sc.type = ValueType::StringRef; sc.str = "CLOSE";
    auto wc = dispatch_to_zigbee(def, "state", sc, ctx, frame);
    assert(wc.ok && frame[2] == 0x04 && frame[3] == 0x00);

    Value ss{}; ss.type = ValueType::StringRef; ss.str = "STOP";
    auto ws = dispatch_to_zigbee(def, "state", ss, ctx, frame);
    assert(ws.ok && ws.frame_size == 3 && frame[2] == 0x03);
}

// ── Tank-level monitor: tank_level (genAnalogInput) + temperature ─────
void check_tank() {
    const auto& def = devices::sinope::kDef_LM4110ZB;
    assert(def_exposes(def, "tank_level"));
    assert(def_exposes(def, "temperature"));

    // presentValue is single-float (type 0x39). Pick x=258 (within calibration
    // band): y=(258-110)/(406-110)=0.5 → 0.5*70+10 = 45 %.
    const float x = 258.0f;
    std::uint8_t fb[4];
    std::memcpy(fb, &x, 4);             // host LE matches wire LE on the test box
    auto r = dispatch_zcl(def, 0x000C, "genAnalogInput",
                          attr_report(0x0055, 0x39, std::span<const std::uint8_t>(fb, 4)));
    assert(r.any_matched);
    const Value* lv = r.merged.find("tank_level");
    assert(lv && lv->type == ValueType::Float);
    assert(std::fabs(lv->f - 45.0f) < 0.5f);

    // presentValue == -1 → empty (0 %).
    const float empty = -1.0f;
    std::memcpy(fb, &empty, 4);
    auto re = dispatch_zcl(def, 0x000C, "genAnalogInput",
                           attr_report(0x0055, 0x39, std::span<const std::uint8_t>(fb, 4)));
    const Value* ev = re.merged.find("tank_level");
    assert(ev && std::fabs(ev->f - 0.0f) < 0.01f);
}

}  // namespace

int main() {
    check_water_leak(devices::sinope::kDef_WL4200);
    check_water_leak(devices::sinope::kDef_WL4200S);
    check_water_leak(devices::sinope::kDef_WL4210);
    check_water_leak(devices::sinope::kDef_RM3500ZB);

    check_valve(devices::sinope::kDef_VA4200WZ);
    check_valve(devices::sinope::kDef_VA4201WZ);
    check_valve(devices::sinope::kDef_VA4220ZB);

    check_tank();
    return 0;
}
