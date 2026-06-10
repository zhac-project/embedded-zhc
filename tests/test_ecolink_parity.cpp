// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Ecolink sensor family.
// z2m-source: ecolink.ts
//   - 4655BC0-R    : fz.temperature + fz.ias_contact_alarm_1,
//                    exposes temperature/contact/battery_low/tamper.
//   - FFZB1-SM-ECO : m.temperature() + m.iasZoneAlarm({zoneType:"alarm"}) +
//                    m.battery(), exposes battery/voltage/alarm/tamper/
//                    battery_low/temperature.
//
// Bugs fixed:
//  (c) IAS dead-key — 4655BC0-R lowered the generic kFzIasZone converter, which
//      emits the bare key "alarm", while the expose declared the semantic key
//      "contact". With no rename layer the contact state never reached the
//      shadow. Graduated to a Tier-2 override swapping in kFzIasContactAlarm
//      (ssIasZone command 0x00 → z2m-polarity contact = !zoneStatus bit0, plus
//      tamper/battery_low). FFZB1-SM-ECO keeps the bare "alarm" key, which is
//      CORRECT (z2m zoneType "alarm" collapses alarm_1 onto `alarm`).
//  (b) Dropped channel — both devices dropped the temperature channel. z2m wires
//      fz.temperature / m.temperature() (msTemperatureMeasurement 0x0402, /100)
//      on each. Restored kFzTemperature + the temperature expose + the 0x0402
//      binding on both.
//
// These tests pin, on real wire shapes, that 4655BC0-R decodes the inverted
// "contact" semantic key (and NOT the bare "alarm" key), that FFZB1-SM-ECO keeps
// its correct bare "alarm" key, and that both decode temperature from 0x0402.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::ecolink {
extern const PreparedDefinition kDef_D4655BC0_R;   // temperature + contact
extern const PreparedDefinition kDef_FFZB1_SM_ECO; // temperature + alarm + battery
}  // namespace zhc::devices::ecolink

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// 9-byte IAS Zone Status Change Notification (command 0x00):
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

DispatchResult dispatch_ias_notif(const PreparedDefinition& def, const IasFrame& f) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// IAS ZoneStatus attribute report (attr 0x0002, u16):
//   fc(1)=0x18  tsn(1)  cmd(1)=0x0A  attr_le(2)=0x0002  type(1)=0x21(u16)
//   value_le(2)
struct IasAttrFrame { std::array<std::uint8_t, 8> bytes; };

IasAttrFrame ias_attr(std::uint16_t zone_status, std::uint8_t tsn = 0x43) {
    IasAttrFrame f{};
    f.bytes = { 0x18, tsn, 0x0A,
                0x02, 0x00,            // attr 0x0002 (ZoneStatus)
                0x21,                  // type uint16
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8) };
    return f;
}

DispatchResult dispatch_ias_attr(const PreparedDefinition& def, const IasAttrFrame& f) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// kFzTemperature decodes msTemperatureMeasurement (0x0402) attr 0x0000
// (measuredValue, s16, /100); the `temperature` expose is declared and the
// device binds 0x0402.
void check_temperature(const PreparedDefinition& def) {
    assert(def_exposes(def, "temperature"));
    assert(def_binds(def, 0x0402));

    const std::uint8_t kTempReport[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000 (measuredValue)
        0x29,                    // type int16
        0x39, 0x08,              // 0x0839 = 2105 (LE) -> 21.05 °C
    };
    auto raw = make_frame(0x0402, 1, kTempReport);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msTemperatureMeasurement";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* t = r.merged.find("temperature");
    assert(t && t->type == ValueType::Float);
    assert(t->f > 21.0f && t->f < 21.1f);
}

}  // namespace

// ── 4655BC0-R contact sensor ─────────────────────────────────────────
// fz.ias_contact_alarm_1: z2m publishes contact = !(zoneStatus bit0). So a
// bit-0-set frame (door open) yields contact:false and a bit-0-clear frame
// (door closed) yields contact:true. The bare "alarm" key must NOT appear.
static void test_4655_contact() {
    const auto& def = devices::ecolink::kDef_D4655BC0_R;
    assert(def_exposes(def, "contact"));

    auto open = dispatch_ias_notif(def, ias_notif(0x0001));   // bit0 set -> open
    assert(open.any_matched);
    assert(b_false(open.merged.find("contact")));
    assert(open.merged.find("alarm") == nullptr);             // dead-key gone
    assert(b_false(open.merged.find("tamper")));
    assert(b_false(open.merged.find("battery_low")));

    auto closed = dispatch_ias_notif(def, ias_notif(0x0000)); // bit0 clear -> closed
    assert(closed.any_matched);
    assert(b_true(closed.merged.find("contact")));

    auto tb = dispatch_ias_notif(def, ias_notif(0x000C));     // tamper(bit2)+batt_low(bit3)
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("contact")));                // bit0 clear -> closed
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

static void test_4655_temp() { check_temperature(devices::ecolink::kDef_D4655BC0_R); }

// ── FFZB1-SM-ECO audio detector ──────────────────────────────────────
// m.iasZoneAlarm({zoneType:"alarm"}) keeps the bare "alarm" key — CORRECT.
// kFzIasZone decodes the ZoneStatus attribute report (attr 0x0002): bit0 ->
// alarm, bit2 -> tamper, bit3 -> battery_low.
static void test_ffzb1_alarm() {
    const auto& def = devices::ecolink::kDef_FFZB1_SM_ECO;
    assert(def_exposes(def, "alarm"));

    auto on = dispatch_ias_attr(def, ias_attr(0x0001));       // bit0 set -> alarm
    assert(on.any_matched);
    assert(b_true(on.merged.find("alarm")));
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto tb = dispatch_ias_attr(def, ias_attr(0x000C));       // tamper(bit2)+batt_low(bit3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("alarm")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

static void test_ffzb1_temp() { check_temperature(devices::ecolink::kDef_FFZB1_SM_ECO); }

int main() {
    test_4655_contact();
    test_4655_temp();
    test_ffzb1_alarm();
    test_ffzb1_temp();
    return 0;
}
