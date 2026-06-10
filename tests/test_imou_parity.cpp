// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the IMOU (Dahua) battery-sensor family.
// z2m-source: zigbee-herdsman-converters/src/devices/imou.ts +
//             lib/modernExtend.ts `iasZoneAlarm`.
//
// Bugs fixed (all on the ssIasZone 0x0500 cluster):
//   * ZP1-EN (PIR), ZR1-EN (siren), ZD1-EN (contact): the generated defs
//     lowered the generic kFzIasZone, which emits `alarm_1`/`alarm_2` —
//     never the semantic key the expose declared (occupancy / alarm /
//     alarm). The primary state was dead. ZP1 now uses kFzIasMotionAlarm
//     (-> `occupancy`); ZR1/ZD1 use kFzIasGenericAlarm (-> bare `alarm`,
//     matching z2m's `alarm` zoneType which collapses both bits).
//   * ZGA1-EN (gas): z2m requests BOTH alarm bits, so it emits the split
//     keys `gas_alarm_1`(bit0) + `gas_alarm_2`(bit1) + tamper(bit2) +
//     test(bit8). The generated def emitted bare `alarm` + a phantom
//     `battery_low`, and dropped `test`. Now uses the imou-local
//     kFzImouGasAlarm.
//   * ZTM1-EN (temp/hum): generated def kept only battery — temperature
//     (0x0402) + humidity (0x0405) were dropped. Re-wired.
//   * ZE1-EN (button): z2m `imouAlarmButton()` emits `action:"press"` on
//     zoneStatus===2. Generated def kept only battery, dropping it.
//     Now uses the imou-local kFzImouAlarmButton.
//
// These pin, on real wire shapes, that each fixed channel decodes under
// its z2m key (and the old dead bare-`alarm`/`alarm_1` key is gone).

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

namespace zhc::devices::imou {
extern const PreparedDefinition kDef_ZP1_EN;   // PIR    -> occupancy
extern const PreparedDefinition kDef_ZR1_EN;   // siren  -> alarm
extern const PreparedDefinition kDef_ZD1_EN;   // contact-> alarm
extern const PreparedDefinition kDef_ZGA1_EN;  // gas    -> gas_alarm_1/2
extern const PreparedDefinition kDef_ZTM1_EN;  // temp/humidity
extern const PreparedDefinition kDef_ZE1_EN;   // button -> action=press
}  // namespace zhc::devices::imou

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

// 9-byte IAS Zone Status Change Notification:
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
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

// ── ZCL attribute report builder (cluster-specific=false, Report=0x0A) ─
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}
DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }
bool str_is(const Value* v, const char* want) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, want) == 0;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// alarm_1 (bit 0) decoders that emit a single semantic key + tamper/batt.
void check_alarm1(const PreparedDefinition& def, const char* sem) {
    assert(def_exposes(def, sem));

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm_1") == nullptr);     // generic dead key gone
    if (std::strcmp(sem, "alarm") != 0)
        assert(on.merged.find("alarm") == nullptr);
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+batt(bit3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

}  // namespace

// ── ZP1-EN — PIR motion via IAS occupancy zoneType ────────────────────
static void test_zp1_occupancy() {
    const auto& def = devices::imou::kDef_ZP1_EN;
    check_alarm1(def, "occupancy");
    assert(!def_exposes(def, "alarm"));  // bare key removed from expose set
}

// ── ZR1-EN / ZD1-EN — `alarm` zoneType (bare collapsed key) ───────────
static void test_zr1_alarm() { check_alarm1(devices::imou::kDef_ZR1_EN, "alarm"); }
static void test_zd1_alarm() { check_alarm1(devices::imou::kDef_ZD1_EN, "alarm"); }

// ── ZGA1-EN — gas, both alarm bits split + test, no battery_low ───────
static void test_zga1_gas() {
    const auto& def = devices::imou::kDef_ZGA1_EN;
    assert(def_exposes(def, "gas_alarm_1"));
    assert(def_exposes(def, "gas_alarm_2"));
    assert(def_exposes(def, "test"));
    assert(def_exposes(def, "tamper"));
    assert(!def_exposes(def, "battery_low"));  // phantom removed
    assert(!def_exposes(def, "alarm"));        // dead bare key removed

    // bit 0 -> gas_alarm_1 only
    auto a1 = dispatch_ias(def, ias_notif(0x0001));
    assert(a1.any_matched);
    assert(b_true(a1.merged.find("gas_alarm_1")));
    assert(b_false(a1.merged.find("gas_alarm_2")));
    assert(a1.merged.find("gas") == nullptr);
    assert(a1.merged.find("alarm") == nullptr);

    // bit 1 -> gas_alarm_2 only
    auto a2 = dispatch_ias(def, ias_notif(0x0002));
    assert(a2.any_matched);
    assert(b_false(a2.merged.find("gas_alarm_1")));
    assert(b_true(a2.merged.find("gas_alarm_2")));

    // bit 8 -> test ; bit 2 -> tamper
    auto tt = dispatch_ias(def, ias_notif(0x0104));
    assert(tt.any_matched);
    assert(b_true(tt.merged.find("test")));
    assert(b_true(tt.merged.find("tamper")));
    // battery_low never emitted by this converter
    assert(tt.merged.find("battery_low") == nullptr);
}

// ── ZTM1-EN — temperature + humidity (were dropped) ───────────────────
static void test_ztm1_temp_humidity() {
    const auto& def = devices::imou::kDef_ZTM1_EN;
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "battery"));

    // temperature: msTemperatureMeasurement 0x0402 attr 0x0000 s16 (2292 -> 22.92C)
    const std::uint8_t t[] = {0xF4, 0x08};
    auto rt = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement",
                           attr_report(0x0000, 0x29, t));
    assert(rt.any_matched);
    assert(rt.merged.find("temperature") != nullptr);

    // humidity: msRelativeHumidity 0x0405 attr 0x0000 u16 (5000 -> 50.00%)
    const std::uint8_t h[] = {0x88, 0x13};
    auto rh = dispatch_zcl(def, 0x0405, "msRelativeHumidity",
                           attr_report(0x0000, 0x21, h));
    assert(rh.any_matched);
    assert(rh.merged.find("humidity") != nullptr);
}

// ── ZE1-EN — wireless switch: action=press only on zoneStatus==2 ──────
static void test_ze1_button() {
    const auto& def = devices::imou::kDef_ZE1_EN;
    assert(def_exposes(def, "action"));

    // zoneStatus == 2 -> action "press"
    auto press = dispatch_ias(def, ias_notif(0x0002));
    assert(press.any_matched);
    assert(str_is(press.merged.find("action"), "press"));

    // any other status -> no action (button converter returns false; battery
    // converter does not match an IAS frame either, so nothing is published)
    auto idle = dispatch_ias(def, ias_notif(0x0000));
    assert(idle.merged.find("action") == nullptr);
    auto other = dispatch_ias(def, ias_notif(0x0001));
    assert(other.merged.find("action") == nullptr);
}

int main() {
    test_zp1_occupancy();
    test_zr1_alarm();
    test_zd1_alarm();
    test_zga1_gas();
    test_ztm1_temp_humidity();
    test_ze1_button();
    return 0;
}
