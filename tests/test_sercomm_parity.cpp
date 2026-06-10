// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Sercomm (Telstra/SmartThings OEM) sensor family.
// z2m-source: sercomm.ts.
//
// Bug fixed: SZ-PIR04N is a PIR motion + temperature sensor that z2m
// additionally extends with `m.illuminance()` (msIlluminanceMeasurement
// 0x0400 measuredValue -> `illuminance`). The auto-port dropped the
// illuminance channel entirely (no kFzIlluminance decoder, no `illuminance`
// expose, no 0x0400 binding), so ambient-light readings never reached the
// shadow. The def was graduated to a Tier-2 parent override wiring the generic
// kFzIlluminance converter, the `illuminance` expose, and a 0x0400 binding.
//
// These tests pin, on real wire shapes, that SZ-PIR04N now decodes
// illuminance (and still decodes its IAS occupancy + temperature channels),
// and structurally guard the rest of the family against regressions:
//   - the contact/motion/water IAS sensors emit the *semantic* key
//     (the `_alarm_1` variant per z2m), not the bare "alarm" key. Note z2m
//     inverts contact (`contact = !(zoneStatus bit0)`), so the contact
//     assertion is flipped; motion/water remain raw bit 0;
//   - the metering plugs keep their energy/power (0x0702) channel and, for
//     SZ-ESW01-AU, the electrical-measurement (0x0B04) channel.

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

namespace zhc::devices::sercomm {
extern const PreparedDefinition kDef_SZ_PIR04N;     // motion + temp + illuminance
extern const PreparedDefinition kDef_XHS2_SE;       // contact + temp + battery
extern const PreparedDefinition kDef_SZ_WTD03;      // water leak
extern const PreparedDefinition kDef_AL_PIR02;      // motion only
extern const PreparedDefinition kDef_SZ_ESW01_AU;   // plug: metering + electrical
extern const PreparedDefinition kDef_SZ_ESW02;      // plug: metering only
}  // namespace zhc::devices::sercomm

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

// Report Attributes wire shape for a single u16 attribute.
//   fc(1)=0x18  tsn(1)  cmd(1)=0x0A  attr_le(2)  type(1)=0x21(u16)  val_le(2)
DispatchResult dispatch_u16_report(const PreparedDefinition& def,
                                   std::uint16_t cluster, const char* cluster_name,
                                   std::uint16_t attr, std::uint16_t value) {
    const std::uint8_t bytes[] = {
        0x18, 0x42, 0x0A,
        static_cast<std::uint8_t>(attr & 0xFF),
        static_cast<std::uint8_t>(attr >> 8),
        0x21,
        static_cast<std::uint8_t>(value & 0xFF),
        static_cast<std::uint8_t>(value >> 8),
    };
    auto raw = make_frame(cluster, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Report Attributes wire shape for a single s16 attribute (temperature).
DispatchResult dispatch_s16_report(const PreparedDefinition& def,
                                   std::uint16_t cluster, const char* cluster_name,
                                   std::uint16_t attr, std::int16_t value) {
    const std::uint8_t bytes[] = {
        0x18, 0x42, 0x0A,
        static_cast<std::uint8_t>(attr & 0xFF),
        static_cast<std::uint8_t>(attr >> 8),
        0x29,  // s16
        static_cast<std::uint8_t>(static_cast<std::uint16_t>(value) & 0xFF),
        static_cast<std::uint8_t>(static_cast<std::uint16_t>(value) >> 8),
    };
    auto raw = make_frame(cluster, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }
bool is_num(const Value* v)  {
    return v && (v->type == ValueType::Uint || v->type == ValueType::Int ||
                 v->type == ValueType::Float);
}

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

// `invert`: z2m publishes `contact = !(zoneStatus bit0)` for zoneType:"contact"
// devices (kFzIasContactAlarm). For those the bit-0-set frame yields the
// semantic key FALSE and the clear frame yields TRUE. Non-contact keys
// (occupancy / water_leak / …) read raw bit 0, so default `invert=false`.
void check_alarm1(const PreparedDefinition& def, const char* sem,
                  bool invert = false) {
    assert(def_exposes(def, sem));
    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only (bit 0)
    assert(on.any_matched);
    assert(invert ? b_false(on.merged.find(sem)) : b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);       // bare key must be gone
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(invert ? b_true(off.merged.find(sem)) : b_false(off.merged.find(sem)));
}

}  // namespace

// ── SZ-PIR04N — the fix: illuminance channel restored ────────────────
static void test_pir04n_illuminance() {
    const auto& def = devices::sercomm::kDef_SZ_PIR04N;

    // Structural: expose + binding present (regression guards).
    assert(def_exposes(def, "illuminance"));
    assert(def_binds(def, 0x0400));

    // Decode: msIlluminanceMeasurement 0x0400 attr 0x0000 measuredValue.
    auto r = dispatch_u16_report(def, 0x0400, "msIlluminanceMeasurement",
                                 0x0000, 12345);
    assert(r.any_matched);
    assert(is_num(r.merged.find("illuminance")));
}

// SZ-PIR04N must still decode its original channels (occupancy + temp).
static void test_pir04n_motion_and_temp() {
    const auto& def = devices::sercomm::kDef_SZ_PIR04N;
    check_alarm1(def, "occupancy");

    auto t = dispatch_s16_report(def, 0x0402, "msTemperatureMeasurement",
                                 0x0000, 2150);  // 21.50 C raw
    assert(t.any_matched);
    assert(is_num(t.merged.find("temperature")));
}

// ── IAS family — semantic keys (bit 0 / _alarm_1) ────────────────────
static void test_contact()    { check_alarm1(devices::sercomm::kDef_XHS2_SE,  "contact", /*invert=*/true); }
static void test_water_leak() { check_alarm1(devices::sercomm::kDef_SZ_WTD03, "water_leak"); }
static void test_motion()     { check_alarm1(devices::sercomm::kDef_AL_PIR02, "occupancy"); }

// ── metering plugs — channels intact ─────────────────────────────────
static void test_esw01_au_channels() {
    const auto& def = devices::sercomm::kDef_SZ_ESW01_AU;
    // electricityMeter default/both -> energy (0x0702) + electrical (0x0B04).
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    assert(def_binds(def, 0x0702));
    assert(def_binds(def, 0x0B04));
}

static void test_esw02_metering_only() {
    const auto& def = devices::sercomm::kDef_SZ_ESW02;
    // z2m exposes switch + power via fz.metering (0x0702) only.
    assert(def_exposes(def, "power"));
    assert(def_binds(def, 0x0702));
    assert(!def_binds(def, 0x0B04));
}

int main() {
    test_pir04n_illuminance();
    test_pir04n_motion_and_temp();
    test_contact();
    test_water_leak();
    test_motion();
    test_esw01_au_channels();
    test_esw02_metering_only();
    return 0;
}
