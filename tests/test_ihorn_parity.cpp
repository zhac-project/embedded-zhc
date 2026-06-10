// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the iHORN sensor family.
// z2m-source: ihorn.ts — fz.ias_occupancy_alarm_1 / fz.ias_smoke_alarm_1 /
// fz.ias_contact_alarm_1 / m.iasZoneAlarm(zoneType:"contact") plus
// fz.temperature / fz.humidity / fz.battery on the LH-32ZB T+H sensor.
//
// Bugs fixed:
//  (c) IAS dead-key — every IAS def lowered the generic kFzIasZone converter,
//      which emits the bare key "alarm", while the expose declared the
//      *semantic* key (occupancy / contact / smoke). With no rename layer the
//      primary sensor state never reached the shadow. Each def was graduated
//      to a Tier-2 override swapping in the typed kFzIas<Type>Alarm converter:
//        - LH-992ZB / LH-990ZB / LH-990F → kFzIasMotionAlarm   (occupancy, bit0)
//        - HO-09ZB  / LH03121            → kFzIasContactAlarm   (contact,   bit0)
//        - LH-09521 (siren)             → kFzIasSmokeAlarm     (smoke,     bit0)
//  (b) Dropped channels — LH-32ZB (T+H sensor) was reduced to battery-only,
//      dropping fz.temperature (msTemperatureMeasurement 0x0402, /100) and
//      fz.humidity (msRelativeHumidity 0x0405, /100). Restored both decoders +
//      the temperature/humidity exposes + the 0x0402/0x0405 bindings.
//
// These tests pin, on real wire shapes, that each representative device decodes
// its semantic key (and NOT the bare "alarm" key) and that the T+H channels
// decode from the right clusters.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::ihorn {
extern const PreparedDefinition kDef_LH_09521;  // smoke (siren)
extern const PreparedDefinition kDef_LH_992ZB;  // occupancy
extern const PreparedDefinition kDef_LH_990ZB;  // occupancy
extern const PreparedDefinition kDef_LH_990F;   // occupancy
extern const PreparedDefinition kDef_HO_09ZB;   // contact + battery
extern const PreparedDefinition kDef_LH03121;   // contact + battery
extern const PreparedDefinition kDef_LH_32ZB;   // temperature + humidity + battery
}  // namespace zhc::devices::ihorn

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

// Assert: alarm_1 (bit 0) asserted -> semantic key true, "alarm" absent,
// tamper/battery_low reflect bits 2/3. The semantic key must also be a declared
// expose (regression guard against generic kFzIasZone, whose "alarm" key never
// matched the expose).
void check_alarm1(const PreparedDefinition& def, const char* sem) {
    assert(def_exposes(def, sem));

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);       // bare key must be gone
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// Restored temperature channel: kFzTemperature decodes msTemperatureMeasurement
// (0x0402) attr 0x0000 (measuredValue, s16, /100); the `temperature` expose is
// declared and the device binds 0x0402.
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

// Restored humidity channel: kFzHumidity decodes msRelativeHumidity (0x0405)
// attr 0x0000 (measuredValue, u16, /100); the `humidity` expose is declared and
// the device binds 0x0405.
void check_humidity(const PreparedDefinition& def) {
    assert(def_exposes(def, "humidity"));
    assert(def_binds(def, 0x0405));

    const std::uint8_t kHumReport[] = {
        0x18, 0x43, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000 (measuredValue)
        0x21,                    // type uint16
        0x14, 0x17,              // 0x1714 = 5908 (LE) -> 59.08 %
    };
    auto raw = make_frame(0x0405, 1, kHumReport);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msRelativeHumidity";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* h = r.merged.find("humidity");
    assert(h && h->type == ValueType::Float);
    assert(h->f > 59.0f && h->f < 59.1f);
}

}  // namespace

// ── per-type alarm_1 (bit 0) decoders ────────────────────────────────
static void test_lh09521_smoke()    { check_alarm1(devices::ihorn::kDef_LH_09521, "smoke"); }
static void test_lh992_occupancy()  { check_alarm1(devices::ihorn::kDef_LH_992ZB, "occupancy"); }
static void test_lh990zb_occ()      { check_alarm1(devices::ihorn::kDef_LH_990ZB, "occupancy"); }
static void test_lh990f_occ()       { check_alarm1(devices::ihorn::kDef_LH_990F,  "occupancy"); }
static void test_ho09zb_contact()   { check_alarm1(devices::ihorn::kDef_HO_09ZB,  "contact"); }
static void test_lh03121_contact()  { check_alarm1(devices::ihorn::kDef_LH03121,  "contact"); }

// ── restored T+H channels on LH-32ZB ─────────────────────────────────
static void test_lh32_temp() { check_temperature(devices::ihorn::kDef_LH_32ZB); }
static void test_lh32_hum()  { check_humidity(devices::ihorn::kDef_LH_32ZB); }

int main() {
    test_lh09521_smoke();
    test_lh992_occupancy();
    test_lh990zb_occ();
    test_lh990f_occ();
    test_ho09zb_contact();
    test_lh03121_contact();

    test_lh32_temp();
    test_lh32_hum();
    return 0;
}
