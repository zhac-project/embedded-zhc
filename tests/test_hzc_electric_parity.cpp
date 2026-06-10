// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the HZC Electric sensor family.
// z2m-source: hzc_electric.ts
//
// Three graduated Tier-2 fixes are pinned here:
//
//  (1) S093TH-ZG — temp/humidity sensor. The generated port wired the
//      generic kFzOnOff + a phantom `state` expose (and a kTzOnOff write
//      path) instead of z2m's fz.temperature + fz.humidity. Re-pointed at
//      kFzTemperature (msTemperatureMeasurement 0x0402, /100) + kFzHumidity
//      (msRelativeHumidity 0x0405, /100); restored the temperature +
//      humidity exposes and bindings, dropped the dead on/off + toZigbee.
//
//  (2) S900W-ZG — water leak sensor. IAS dead-key: the port lowered the
//      generic kFzIasZone (bare `alarm`) while z2m's fz.ias_water_leak_alarm_1
//      publishes `water_leak` (zoneStatus bit 0). Re-pointed at the typed
//      kFzIasWaterLeakAlarm; renamed the expose alarm -> water_leak.
//
//  (3) S902M-ZG — motion sensor. Two bugs: IAS dead-key (bare `alarm`
//      vs z2m's fz.ias_occupancy_alarm_1 -> `occupancy`, bit 0) AND a
//      dropped illuminance channel (z2m m.illuminance(),
//      msIlluminanceMeasurement 0x0400). Re-pointed at kFzIasMotionAlarm +
//      added kFzIlluminance; renamed alarm -> occupancy + added illuminance.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::hzc_electric {
extern const PreparedDefinition kDef_S093TH_ZG;  // temperature + humidity
extern const PreparedDefinition kDef_S900W_ZG;   // water_leak (IAS alarm_1)
extern const PreparedDefinition kDef_S902M_ZG;   // occupancy (IAS alarm_1) + illuminance
}  // namespace zhc::devices::hzc_electric

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

bool def_has_fz(const PreparedDefinition& def, const FzConverter* cvt) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i)
        if (def.from_zigbee[i] == cvt) return true;
    return false;
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

// Assert: alarm_1 (bit 0) asserted -> semantic key true, "alarm" absent,
// tamper/battery_low reflect bits 2/3.
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

}  // namespace

// ── S093TH-ZG: temperature + humidity ────────────────────────────────
static void test_s093th_temperature() {
    const auto& def = devices::hzc_electric::kDef_S093TH_ZG;
    assert(def_exposes(def, "temperature"));
    assert(def_binds(def, 0x0402));
    // No phantom on/off survived the graduation.
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);

    // msTemperatureMeasurement Report Attributes: attr 0x0000 (int16) = 2105
    // -> 21.05 °C.
    const std::uint8_t kTempReport[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000 (measuredValue)
        0x29,                    // type int16
        0x39, 0x08,              // 0x0839 = 2105 (LE)
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
    // The phantom on/off path is gone: a genOnOff state must not be synthesised.
    assert(r.merged.find("state") == nullptr);
}

static void test_s093th_humidity() {
    const auto& def = devices::hzc_electric::kDef_S093TH_ZG;
    assert(def_exposes(def, "humidity"));
    assert(def_binds(def, 0x0405));

    // msRelativeHumidity Report Attributes: attr 0x0000 (uint16) = 5512
    // -> 55.12 %.
    const std::uint8_t kHumReport[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000 (measuredValue)
        0x21,                    // type uint16
        0x88, 0x15,              // 0x1588 = 5512 (LE)
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
    assert(h->f > 55.0f && h->f < 55.2f);
}

// ── S900W-ZG: water leak (IAS alarm_1, bit 0) ────────────────────────
static void test_s900w_water_leak() {
    check_alarm1(devices::hzc_electric::kDef_S900W_ZG, "water_leak");
}

// ── S902M-ZG: occupancy (IAS alarm_1, bit 0) ─────────────────────────
static void test_s902m_occupancy() {
    check_alarm1(devices::hzc_electric::kDef_S902M_ZG, "occupancy");
}

// ── S902M-ZG: restored illuminance channel (msIlluminanceMeasurement) ─
static void test_s902m_illuminance() {
    const auto& def = devices::hzc_electric::kDef_S902M_ZG;
    assert(def_exposes(def, "illuminance"));
    assert(def_binds(def, 0x0400));
    assert(def_has_fz(def, &::zhc::generic::kFzIlluminance));

    // msIlluminanceMeasurement Report Attributes: attr 0x0000 (uint16) = 12345.
    const std::uint8_t kIllumReport[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000 (measuredValue)
        0x21,                    // type uint16
        0x39, 0x30,              // 0x3039 = 12345 (LE)
    };
    auto raw = make_frame(0x0400, 1, kIllumReport);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msIlluminanceMeasurement";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* l = r.merged.find("illuminance");
    assert(l && l->type == ValueType::Uint);
    assert(l->u == 12345);
}

int main() {
    test_s093th_temperature();
    test_s093th_humidity();
    test_s900w_water_leak();
    test_s902m_occupancy();
    test_s902m_illuminance();
    return 0;
}
