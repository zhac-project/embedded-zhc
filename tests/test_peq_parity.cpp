// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the PEQ 3300-P door & window contact sensor.
// z2m-source: peq.ts #3300-P — fromZigbee:[fz.temperature,
//   fz.ias_contact_alarm_1, fz.battery]; exposes temperature/contact/
//   battery_low/tamper/battery.
//
// Bugs fixed (two real gaps vs z2m):
//   (1) IAS dead-key: the generated def lowered the generic kFzIasZone
//       converter (emits the bare key "alarm"), so the contact state
//       never matched the declared expose. Swapped in the typed
//       kFzIasContactAlarm (emits "contact", z2m-inverted:
//       contact = !(zoneStatus bit0)).
//   (2) Dropped temperature channel: z2m carries fz.temperature
//       (msTemperatureMeasurement 0x0402, /100) but the port dropped the
//       decoder, the `temperature` expose, and the 0x0402 binding —
//       all three restored.
//
// These tests pin, on real wire shapes, that the contact channel decodes
// the semantic "contact" key (and NOT the bare "alarm"), with z2m
// inversion, and that the restored temperature channel decodes from
// msTemperatureMeasurement.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::peq {
extern const PreparedDefinition kDef_D3300_P;  // contact + temperature + battery
}  // namespace zhc::devices::peq

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

}  // namespace

// ── contact via typed kFzIasContactAlarm (z2m-inverted bit 0) ────────────
// z2m fz.ias_contact_alarm_1 publishes contact = !(zoneStatus bit0):
//   bit0 SET (open)   -> contact:false
//   bit0 CLEAR (shut) -> contact:true
// The bare "alarm" key must never appear, and "contact" must be a declared
// expose (regression guard against the generic kFzIasZone the port used).
static void test_contact() {
    const auto& def = devices::peq::kDef_D3300_P;
    assert(def_exposes(def, "contact"));
    assert(def_binds(def, 0x0500));

    auto open = dispatch_ias(def, ias_notif(0x0001));  // alarm_1 (bit 0) set
    assert(open.any_matched);
    assert(b_false(open.merged.find("contact")));      // inverted -> false
    assert(open.merged.find("alarm") == nullptr);      // bare key must be gone
    assert(b_false(open.merged.find("tamper")));
    assert(b_false(open.merged.find("battery_low")));

    auto shut = dispatch_ias(def, ias_notif(0x0000));  // bit 0 clear
    assert(shut.any_matched);
    assert(b_true(shut.merged.find("contact")));        // inverted -> true

    auto tb = dispatch_ias(def, ias_notif(0x000C));    // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("contact")));          // bit0 clear -> true
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// ── restored temperature channel (msTemperatureMeasurement 0x0402, /100) ─
static void test_temperature() {
    const auto& def = devices::peq::kDef_D3300_P;
    assert(def_exposes(def, "temperature"));
    assert(def_binds(def, 0x0402));

    // msTemperatureMeasurement Report Attributes: attr 0x0000 (int16) = 2105
    // -> 21.05 °C.
    const std::uint8_t kTempReport[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000 (measuredValue)
        0x29,                    // type int16
        0x39, 0x08,              // 2105 LE
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

int main() {
    test_contact();
    test_temperature();
    return 0;
}
