// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Sylvania (OSRAM Lightify US) 74388 contact +
// temperature sensor.
// z2m-source: sylvania.ts #74388 — fz.ias_contact_alarm_1 +
// fz.temperature + fz.battery.
//
// Bug fixed: the generated def lowered the generic kFzIasZone converter,
// which emits the bare key "alarm", while z2m's fz.ias_contact_alarm_1
// publishes the semantic key "contact". The expose declared "alarm", so
// the contact state never reached the `contact` shadow key. The def was
// graduated to a Tier-2 parent override that swaps in the typed
// kFzIasContactAlarm converter (semantic key "contact", tamper bit 2,
// battery_low bit 3) and renames the expose. The temperature axis
// (msTemperatureMeasurement 0x0402) was already wired via kFzTemperature.
//
// These tests pin, on real wire shapes, that 74388 now decodes "contact"
// (and NOT the bare "alarm") from an IAS Zone-Status-Change-Notification,
// and still decodes temperature from msTemperatureMeasurement.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::sylvania {
extern const PreparedDefinition kDef_D74388;  // contact + temperature sensor
}  // namespace zhc::devices::sylvania

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

}  // namespace

// ── contact alarm_1 (bit 0) decoder ──────────────────────────────────
static void test_contact() {
    const auto& def = devices::sylvania::kDef_D74388;

    // semantic key must be a declared expose, and the bare "alarm" key
    // (left by the old generic kFzIasZone wiring) must be gone.
    assert(def_exposes(def, "contact"));
    assert(!def_exposes(def, "alarm"));

    // z2m publishes contact = !bit0 (zoneType:"contact"): bit0 set → open
    // (false), bit0 clear → closed (true).
    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only (bit 0)
    assert(on.any_matched);
    assert(b_false(on.merged.find("contact")));
    assert(on.merged.find("alarm") == nullptr);       // bare key must be gone
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert(b_true(off.merged.find("contact")));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("contact")));        // bit0 clear here
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// ── temperature axis still decodes (msTemperatureMeasurement 0x0402) ──
static void test_temperature() {
    const auto& def = devices::sylvania::kDef_D74388;
    assert(def_exposes(def, "temperature"));

    // msTemperatureMeasurement (0x0402) Report Attributes: attr 0x0000
    // (measuredValue, int16) = 2350 -> 23.50 °C.
    const std::uint8_t kTempReport[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000
        0x29,                    // type int16
        0x2E, 0x09,              // value 2350 (le)
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
    assert(t->f > 23.0f && t->f < 24.0f);   // 2350 / 100 = 23.5 °C
    assert(r.merged.find("alarm") == nullptr);
}

int main() {
    test_contact();
    test_temperature();
    return 0;
}
