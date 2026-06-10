// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the GS (Linkind-OEM) device family.
// z2m-source: zigbee-herdsman-converters/src/devices/gs.ts +
//             lib/modernExtend.ts (iasZoneAlarm / electricityMeter / light).
//
// Bugs fixed (each generated def graduated to a Tier-2 parent override):
//   * 6 IAS sensors lowered the generic kFzIasZone converter, which emits
//     the bare key "alarm" off zoneStatus bit 0, while z2m's
//     m.iasZoneAlarm publishes a SEMANTIC key per zoneType. Swapped to the
//     typed kFzIas<Type>Alarm converter:
//        SMHM-I1  occupancy  (alarm_1, bit 0)
//        SOHM-I1  contact    (alarm_1, bit 0)
//        SSHM-I1  smoke      (alarm_1, bit 0)
//        SWHM-I1  water_leak (alarm_1, bit 0)
//        SGMHM-I1 gas        (alarm_2, bit 1)  -> kFzIasGasAlarm2
//        SGPHM-I1 gas        (alarm_2, bit 1)  -> kFzIasGasAlarm2
//   * SKHMP30-I1 smart socket: z2m electricityMeter() defaults to cluster
//     "both" (0x0702 energy + 0x0B04 power/voltage/current), but the
//     auto-port dropped the 0x0B04 half. Added kFzElectricalMeasurement +
//     voltage/current exposes + the 0x0B04 binding.
//   * BRHM8E27W70-I1 (RGB+CT) / BDHM8E27W70-I1 (CT) bulbs were collapsed
//     to on/off+brightness; restored colorTemp (+ HS colour on BRHM) and
//     the 0x0300 binding.
//
// These tests pin, on real wire shapes, that each fixed decoder now emits
// the right semantic key (and NOT the bare "alarm"), reads the right
// zoneStatus bit, and that the plug + bulb axes are present.

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

namespace zhc::devices::gs {
extern const PreparedDefinition kDef_SMHM_I1;        // occupancy (alarm_1)
extern const PreparedDefinition kDef_SOHM_I1;        // contact   (alarm_1)
extern const PreparedDefinition kDef_SSHM_I1;        // smoke     (alarm_1)
extern const PreparedDefinition kDef_SWHM_I1;        // water_leak(alarm_1)
extern const PreparedDefinition kDef_SGMHM_I1;       // gas       (alarm_2, bit 1)
extern const PreparedDefinition kDef_SGPHM_I1;       // gas       (alarm_2, bit 1)
extern const PreparedDefinition kDef_SKHMP30_I1;     // socket    (0x0702 + 0x0B04)
extern const PreparedDefinition kDef_BRHM8E27W70_I1; // RGB+CT bulb
extern const PreparedDefinition kDef_BDHM8E27W70_I1; // CT bulb
}  // namespace zhc::devices::gs

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
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

// ── IAS Zone Status Change Notification (9 bytes) ────────────────────
//   fc(1)=0x09 tsn(1) cmd(1)=0x00 zoneStatus_le(2) extStatus(1)
//   zoneId(1) delay_le(2)
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
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Assert: an alarm_1 (bit 0) sensor decodes its semantic key, never the
// bare "alarm", and tracks tamper/battery_low on bits 2/3.
void check_alarm1(const PreparedDefinition& def, const char* sem) {
    assert(def_exposes(def, sem));           // regression guard vs kFzIasZone

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(2)+battery_low(3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// Assert: a gas alarm_2 (bit 1) sensor reads bit 1, not bit 0.
void check_gas_alarm2(const PreparedDefinition& def) {
    assert(def_exposes(def, "gas"));

    auto bit0 = dispatch_ias(def, ias_notif(0x0001));  // bit 0 -> gas stays false
    assert(bit0.any_matched);
    assert(b_false(bit0.merged.find("gas")));

    auto bit1 = dispatch_ias(def, ias_notif(0x0002));  // bit 1 -> gas true
    assert(bit1.any_matched);
    assert(b_true(bit1.merged.find("gas")));
    assert(bit1.merged.find("alarm") == nullptr);
}

// ── ZCL attribute report builder: fc=0x18 tsn cmd=0x0A then records ──
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}

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

}  // namespace

// ── IAS sensors: semantic-key dead-key fix ───────────────────────────
static void test_occupancy()  { check_alarm1(devices::gs::kDef_SMHM_I1, "occupancy"); }
static void test_contact()    { check_alarm1(devices::gs::kDef_SOHM_I1, "contact"); }
static void test_smoke()      { check_alarm1(devices::gs::kDef_SSHM_I1, "smoke"); }
static void test_water()      { check_alarm1(devices::gs::kDef_SWHM_I1, "water_leak"); }
static void test_gas_methane(){ check_gas_alarm2(devices::gs::kDef_SGMHM_I1); }
static void test_gas_propane(){ check_gas_alarm2(devices::gs::kDef_SGPHM_I1); }

// ── SKHMP30-I1 smart socket: 0x0B04 half restored ───────────────────
static void test_socket_electrical() {
    const auto& def = devices::gs::kDef_SKHMP30_I1;
    // Energy (0x0702) half preserved; power/voltage/current (0x0B04) added.
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    assert(def_binds(def, 0x0702));
    assert(def_binds(def, 0x0B04));

    // haElectricalMeasurement RMSVoltage (0x0505, u16) + RMSCurrent (0x0508, u16).
    auto rv = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement",
                           attr_report(0x0505, 0x21, u16(2301)));
    assert(rv.any_matched);
    const Value* volt = rv.merged.find("voltage");
    assert(volt && volt->type == ValueType::Uint && volt->u == 2301);

    auto rc = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement",
                           attr_report(0x0508, 0x21, u16(1500)));
    assert(rc.any_matched);
    const Value* cur = rc.merged.find("current");
    assert(cur && cur->type == ValueType::Uint && cur->u == 1500);
}

// ── bulbs: light completeness ────────────────────────────────────────
static void test_bulb_rgb_ct() {
    const auto& def = devices::gs::kDef_BRHM8E27W70_I1;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));
    assert(def_binds(def, 0x0300));

    // lightingColorCtrl currentX (0x0003 u16) decodes to color_x (float).
    auto rx = dispatch_zcl(def, 0x0300, "lightingColorCtrl",
                           attr_report(0x0003, 0x21, u16(32768)));
    assert(rx.any_matched);
    assert(rx.merged.find("color_x") != nullptr);

    // colorTemperature (0x0007 u16) decodes to color_temp.
    auto rt = dispatch_zcl(def, 0x0300, "lightingColorCtrl",
                           attr_report(0x0007, 0x21, u16(300)));
    assert(rt.any_matched);
    const Value* ct = rt.merged.find("color_temp");
    assert(ct && ct->type == ValueType::Uint && ct->u == 300);
}

static void test_bulb_ct() {
    const auto& def = devices::gs::kDef_BDHM8E27W70_I1;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "hue"));        // CT-only: no colour axis
    assert(def_binds(def, 0x0300));

    auto rt = dispatch_zcl(def, 0x0300, "lightingColorCtrl",
                           attr_report(0x0007, 0x21, u16(250)));
    assert(rt.any_matched);
    const Value* ct = rt.merged.find("color_temp");
    assert(ct && ct->type == ValueType::Uint && ct->u == 250);
}

int main() {
    test_occupancy();
    test_contact();
    test_smoke();
    test_water();
    test_gas_methane();
    test_gas_propane();
    test_socket_electrical();
    test_bulb_rgb_ct();
    test_bulb_ct();
    return 0;
}
