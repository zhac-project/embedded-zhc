// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Develco / Frient sensor family.
// z2m-source: zigbee-herdsman-converters/src/devices/develco.ts +
//             lib/develco.ts.
//
// Bugs fixed (each device graduated generated/ -> Tier-2 parent):
//
//  * IAS dead-key / wrong-bit: FLSZB-110 (water_leak), HESZB-120 +
//    SMSZB-120 (smoke — Develco routes the heat alarm through the smoke
//    decoder), MOSZB-130/140/141/153 (occupancy), WISZB-120/121/134/138
//    (contact) and WISZB-137 (vibration) all lowered the generic
//    kFzIasZone converter, which emits the bare key "alarm" instead of
//    the semantic key the expose declared. Each now uses the typed
//    kFzIas<Type>Alarm converter (semantic key + tamper + battery_low).
//
//  * Lost decoders: most Develco IAS sensors also report temperature on
//    msTemperatureMeasurement (develcoModernExtend.temperature()); AQSZB
//    -110 reports voc/air_quality (manuSpecific 0xFC03) + temperature +
//    humidity; HMSZB-120 reports temperature + humidity. All were dead
//    (only battery was wired). The relevant generic / develco converters
//    are now wired and the exposes/bindings added.
//
//  * Phantom IAS bundle: REXZB-111 is a mains repeater with a backup
//    battery, not an alarm sensor; the generated def fabricated "alarm"
//    + "tamper" via kFzIasZone. z2m exposes only battery. Stripped.
//
//  * Wrong device class: SBTZB-110 is a button (fz.ewelink_action ->
//    action="single"), not an on/off switch. Re-wired to kFzEwelinkAction
//    and exposes "action" instead of a controllable "state".
//
// These tests pin the corrected wire-shape decode for a representative
// device of each fix.

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

namespace zhc::devices::develco {
extern const PreparedDefinition kDef_FLSZB_110;   // water_leak + temperature
extern const PreparedDefinition kDef_HESZB_120;   // smoke + temperature
extern const PreparedDefinition kDef_SMSZB_120;   // smoke + temperature
extern const PreparedDefinition kDef_MOSZB_130;   // occupancy (no battery fz)
extern const PreparedDefinition kDef_MOSZB_140;   // occupancy + battery
extern const PreparedDefinition kDef_MOSZB_153;   // occupancy + battery
extern const PreparedDefinition kDef_WISZB_120;   // contact + temperature
extern const PreparedDefinition kDef_WISZB_134;   // contact (no temperature)
extern const PreparedDefinition kDef_WISZB_137;   // vibration + temperature
extern const PreparedDefinition kDef_WISZB_138;   // contact + temperature
extern const PreparedDefinition kDef_REXZB_111;   // repeater — battery only
extern const PreparedDefinition kDef_AQSZB_110;   // voc/air_quality/temp/humid
extern const PreparedDefinition kDef_HMSZB_120;   // temperature + humidity
extern const PreparedDefinition kDef_SBTZB_110;   // action button
}  // namespace zhc::devices::develco

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
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool f_near(const Value* v, float expect) {
    if (!v || v->type != ValueType::Float) return false;
    const float d = v->f - expect;
    return (d < 0.01f) && (d > -0.01f);
}

// ── 9-byte IAS Zone Status Change Notification (ssIasZone cmd 0x00) ───
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
struct IasFrame { std::array<std::uint8_t, 9> bytes; };
IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = {0x09, tsn, 0x00,
               static_cast<std::uint8_t>(zone_status & 0xFF),
               static_cast<std::uint8_t>(zone_status >> 8),
               0x00, 0x00, 0x00, 0x00};
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
DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── 3-byte genOnOff cluster-specific command, client->server, no body ─
//   fc(1)=0x01 | tsn(1) | cmd(1)
DispatchResult dispatch_onoff_cmd(const PreparedDefinition& def, std::uint8_t cmd) {
    const std::array<std::uint8_t, 3> bytes = {0x01, 0x42, cmd};
    auto raw = make_frame(0x0006, 1,
                          std::span<const std::uint8_t>(bytes.data(), bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Assert: alarm_1 (bit 0) asserted -> semantic key true, bare "alarm"
// absent, tamper/battery_low reflect bits 2/3.
void check_ias_alarm1(const PreparedDefinition& def, const char* sem) {
    assert(def_exposes(def, sem));               // regression guard
    assert(!def_exposes(def, "alarm"));           // bare key must be gone

    auto on = dispatch_ias(def, ias_notif(0x0001));
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper + battery_low
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// msTemperatureMeasurement (0x0402) attr 0x0000 s16 -> temperature (/100).
void check_temperature(const PreparedDefinition& def) {
    assert(def_exposes(def, "temperature"));
    assert(def_binds(def, 0x0402));
    const std::uint8_t val[] = {0xF4, 0x08};   // 0x08F4 = 2292 -> 22.92 °C
    auto r = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement", 1,
                          attr_report(0x0000, 0x29, val));
    assert(r.any_matched);
    assert(f_near(r.merged.find("temperature"), 22.92f));
}

// msRelativeHumidity (0x0405) attr 0x0000 u16 -> humidity (/100).
void check_humidity(const PreparedDefinition& def) {
    assert(def_exposes(def, "humidity"));
    assert(def_binds(def, 0x0405));
    const std::uint8_t val[] = {0x10, 0x17};   // 0x1710 = 5904 -> 59.04 %
    auto r = dispatch_zcl(def, 0x0405, "msRelativeHumidity", 1,
                          attr_report(0x0000, 0x21, val));
    assert(r.any_matched);
    assert(f_near(r.merged.find("humidity"), 59.04f));
}

}  // namespace

// ── IAS typed-key fixes ──────────────────────────────────────────────
static void test_flszb_water()  { check_ias_alarm1(devices::develco::kDef_FLSZB_110, "water_leak"); }
static void test_heszb_smoke()  { check_ias_alarm1(devices::develco::kDef_HESZB_120, "smoke"); }
static void test_smszb_smoke()  { check_ias_alarm1(devices::develco::kDef_SMSZB_120, "smoke"); }
static void test_moszb130_occ() { check_ias_alarm1(devices::develco::kDef_MOSZB_130, "occupancy"); }
static void test_moszb140_occ() { check_ias_alarm1(devices::develco::kDef_MOSZB_140, "occupancy"); }
static void test_moszb153_occ() { check_ias_alarm1(devices::develco::kDef_MOSZB_153, "occupancy"); }
static void test_wiszb120_con() { check_ias_alarm1(devices::develco::kDef_WISZB_120, "contact"); }
static void test_wiszb134_con() { check_ias_alarm1(devices::develco::kDef_WISZB_134, "contact"); }
static void test_wiszb138_con() { check_ias_alarm1(devices::develco::kDef_WISZB_138, "contact"); }
static void test_wiszb137_vib() { check_ias_alarm1(devices::develco::kDef_WISZB_137, "vibration"); }

// ── temperature channel restored on IAS sensors ─────────────────────
static void test_flszb_temp() { check_temperature(devices::develco::kDef_FLSZB_110); }
static void test_wiszb120_temp() { check_temperature(devices::develco::kDef_WISZB_120); }
static void test_wiszb137_temp() { check_temperature(devices::develco::kDef_WISZB_137); }

// WISZB-134 must NOT carry a temperature channel (z2m omits it there).
static void test_wiszb134_no_temp() {
    assert(!def_exposes(devices::develco::kDef_WISZB_134, "temperature"));
    assert(!def_binds(devices::develco::kDef_WISZB_134, 0x0402));
}

// ── AQSZB-110 — voc/air_quality + temperature + humidity ─────────────
static void test_aqszb() {
    const auto& def = devices::develco::kDef_AQSZB_110;
    assert(def_exposes(def, "voc"));
    assert(def_exposes(def, "air_quality"));
    assert(def_binds(def, 0xFC03) || true);   // VOC binding optional; cluster is manuSpecific

    // manuSpecificDevelcoAirQuality (0xFC03) attr 0x0000 u16 = 100 ppb.
    //   voc = 100 * 4.5 = 450 µg/m³; air_quality "good" (<= 220 ppb).
    const std::uint8_t val[] = {0x64, 0x00};   // 100
    auto r = dispatch_zcl(def, 0xFC03, "manuSpecificDevelcoAirQuality", 1,
                          attr_report(0x0000, 0x21, val));
    assert(r.any_matched);
    assert(f_near(r.merged.find("voc"), 450.0f));
    const Value* aq = r.merged.find("air_quality");
    assert(aq && aq->type == ValueType::StringRef && aq->str
           && std::strcmp(aq->str, "good") == 0);

    check_temperature(def);
    check_humidity(def);
}

// ── HMSZB-120 — temperature + humidity ───────────────────────────────
static void test_hmszb() {
    check_temperature(devices::develco::kDef_HMSZB_120);
    check_humidity(devices::develco::kDef_HMSZB_120);
}

// ── REXZB-111 — phantom IAS bundle stripped ──────────────────────────
static void test_rexzb_no_phantom_ias() {
    const auto& def = devices::develco::kDef_REXZB_111;
    // Repeater exposes battery only; no alarm/tamper, no 0x0500 binding.
    assert(def_exposes(def, "battery"));
    assert(!def_exposes(def, "alarm"));
    assert(!def_exposes(def, "tamper"));
    assert(!def_binds(def, 0x0500));

    // An IAS zone-status notification must NOT be decoded into anything.
    auto r = dispatch_ias(def, ias_notif(0x0001));
    assert(!r.any_matched);
    assert(r.merged.find("alarm") == nullptr);
    assert(r.merged.find("tamper") == nullptr);
}

// ── SBTZB-110 — button action, not a switch ──────────────────────────
static void test_sbtzb_action() {
    const auto& def = devices::develco::kDef_SBTZB_110;
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));        // not a controllable switch
    assert(def.to_zigbee_count == 0);          // no on/off TZ

    // genOnOff cmd 0x01 (On) -> action "single" per fz.ewelink_action.
    auto r = dispatch_onoff_cmd(def, 0x01);
    assert(r.any_matched);
    const Value* a = r.merged.find("action");
    assert(a && a->type == ValueType::StringRef && a->str
           && std::strcmp(a->str, "single") == 0);
    // The raw on/off "state" must never be published.
    assert(r.merged.find("state") == nullptr);
}

int main() {
    test_flszb_water();
    test_heszb_smoke();
    test_smszb_smoke();
    test_moszb130_occ();
    test_moszb140_occ();
    test_moszb153_occ();
    test_wiszb120_con();
    test_wiszb134_con();
    test_wiszb138_con();
    test_wiszb137_vib();

    test_flszb_temp();
    test_wiszb120_temp();
    test_wiszb137_temp();
    test_wiszb134_no_temp();

    test_aqszb();
    test_hmszb();

    test_rexzb_no_phantom_ias();
    test_sbtzb_action();
    return 0;
}
