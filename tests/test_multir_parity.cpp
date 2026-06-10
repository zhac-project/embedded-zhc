// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the MultIR family. The auto-generated ports carried
// several classes of bug this fixture pins after the fix:
//
//   IAS dead-key — four IAS sensors wired the generic kFzIasZone (bare
//   "alarm") against a semantic expose key the converter never produced.
//   z2m decodes each via m.iasZoneAlarm({zoneType}) → a typed semantic key:
//     * MIR-MC100 contact     → kFzIasContactAlarm   → contact   (!bit0; z2m inverts contact)
//     * MIR-SM200 smoke       → kFzIasSmokeAlarm     → smoke     (bit0)
//     * MIR-WA100 water_leak  → kFzIasWaterLeakAlarm → water_leak(bit0)
//     * MIR-IR100 occupancy   → kFzIasMotionAlarm    → occupancy (bit0)
//   MIR-SM100-E uses zoneType "generic" + both alarms → RAW keys alarm_1
//   (bit0)/alarm_2(bit1) via kFzIasZoneStatusChange (the auto-port's
//   kFzIasZone dropped alarm_2 + the command-notification path).
//
//   Missing decoder/channel —
//     * MIR-IR100 also lost illuminance (msIlluminanceMeasurement 0x0400).
//     * MIR-TE600 lowered only kFzBattery, dropping temperature (0x0402) +
//       humidity (0x0405).
//
//   Wrong bundle — MIR-SO100 SOS button was mis-modelled as a settable
//   genOnOff switch (phantom `state` + kTzOnOff + 0x0006 binding). z2m role
//   is action via a raw ssIasZone command (byte0 0/1/128 → single/double/
//   hold). Replaced with the vendor kFzMultirSosAction → `action`.

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

namespace zhc::devices::multir {
extern const PreparedDefinition kDef_MIR_MC100;     // contact
extern const PreparedDefinition kDef_MIR_SM200;     // smoke
extern const PreparedDefinition kDef_MIR_WA100;     // water_leak
extern const PreparedDefinition kDef_MIR_IR100;     // occupancy + illuminance
extern const PreparedDefinition kDef_MIR_SM100_E;   // generic alarm_1/alarm_2
extern const PreparedDefinition kDef_MIR_TE600;     // temperature + humidity
extern const PreparedDefinition kDef_MIR_SO100;     // SOS action
}  // namespace zhc::devices::multir

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

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report builder (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool action_is(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

// ── IAS: a 9-byte Zone Status Change Notification (ssIasZone cmd 0x00). ─────
struct IasFrame { std::array<std::uint8_t, 9> bytes; };

IasFrame ias_notif(std::uint16_t zone_status) {
    IasFrame f{};
    f.bytes = { 0x09, 0x42, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

DispatchResult dispatch_ias(RuntimeContext& ctx, const PreparedDefinition& def,
                            const IasFrame& f) {
    return dispatch_zcl_ep(ctx, def, 0x0500, 1, "ssIasZone",
                           std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
}

// Typed IAS sensor: the semantic key is exposed (and bare "alarm" is gone),
// an alarm on bit0 decodes to it, a clear drops it, tamper + battery_low
// (status bits 2/3) co-decode.
//
// `expect_on_bit0`: most zone types publish the semantic key raw on bit 0.
// zoneType "contact" inverts in z2m (contact = !bit0: bit0 set -> contact:
// false), so the contact caller passes false. tamper/battery_low (bits 2/3)
// are unaffected.
void check_ias(const PreparedDefinition& def, const char* sem,
               bool expect_on_bit0 = true) {
    assert(def_exposes(def, sem));
    assert(!def_exposes(def, "alarm"));

    RuntimeContext c1{};
    auto on = dispatch_ias(c1, def, ias_notif(0x0001));   // bit0 set
    assert(on.any_matched);
    if (expect_on_bit0) assert(b_true(on.merged.find(sem)));
    else                assert(b_false(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);   // legacy bare key gone

    RuntimeContext c2{};
    auto off = dispatch_ias(c2, def, ias_notif(0x0000));  // bit0 clear
    assert(off.any_matched);
    if (expect_on_bit0) assert(b_false(off.merged.find(sem)));
    else                assert(b_true(off.merged.find(sem)));

    RuntimeContext c3{};
    auto tb = dispatch_ias(c3, def, ias_notif(0x000C));   // tamper(b2)+battery_low(b3), bit0 clear
    assert(tb.any_matched);
    if (expect_on_bit0) assert(b_false(tb.merged.find(sem)));
    else                assert(b_true(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

void test_ias_typed_sensors() {
    using namespace zhc::devices::multir;
    check_ias(kDef_MIR_MC100, "contact", /*expect_on_bit0=*/false);  // z2m inverts contact
    check_ias(kDef_MIR_SM200, "smoke");
    check_ias(kDef_MIR_IR100, "occupancy");

    // WA100: water_leak + battery_low, but z2m omits the tamper expose.
    check_ias(kDef_MIR_WA100, "water_leak");
    assert(!def_exposes(kDef_MIR_WA100, "tamper"));
}

// ── MIR-SM100-E: generic zoneType, both alarms → alarm_1 + alarm_2. ─────────
void test_sm100e_generic() {
    using namespace zhc::devices::multir;
    const auto& def = kDef_MIR_SM100_E;
    assert(def_exposes(def, "alarm_1"));
    assert(def_exposes(def, "alarm_2"));
    assert(!def_exposes(def, "alarm"));   // not the single-key generic
    assert(!def_exposes(def, "smoke"));   // generic zoneType → no semantic key

    // alarm_2 on bit1 must fire alarm_2 (not alarm_1).
    RuntimeContext c1{};
    auto a2 = dispatch_ias(c1, def, ias_notif(0x0002));
    assert(a2.any_matched);
    assert(b_true(a2.merged.find("alarm_2")));
    assert(b_false(a2.merged.find("alarm_1")));

    // alarm_1 on bit0.
    RuntimeContext c2{};
    auto a1 = dispatch_ias(c2, def, ias_notif(0x0001));
    assert(b_true(a1.merged.find("alarm_1")));
    assert(b_false(a1.merged.find("alarm_2")));
}

// ── MIR-IR100: illuminance channel regained. ───────────────────────────────
void test_ir100_illuminance() {
    using namespace zhc::devices::multir;
    const auto& def = kDef_MIR_IR100;
    assert(def_exposes(def, "illuminance"));
    assert(def_exposes(def, "occupancy"));
    assert(def_binds(def, 1, 0x0400));

    // measuredValue (0x0000, u16) on msIlluminanceMeasurement → illuminance.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> lx = {0xDC, 0x05};   // 1500 LE
    auto lr = dispatch_zcl_ep(c1, def, 0x0400, 1, "msIlluminanceMeasurement",
                              attr_report(0x0000, 0x21, std::span<const std::uint8_t>(lx.data(), lx.size())));
    assert(lr.any_matched);
    assert(lr.merged.find("illuminance") != nullptr);
}

// ── MIR-TE600: temperature (0x0402) + humidity (0x0405) regained. ──────────
void test_te600() {
    using namespace zhc::devices::multir;
    const auto& def = kDef_MIR_TE600;
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "battery"));
    assert(def_binds(def, 1, 0x0402));
    assert(def_binds(def, 1, 0x0405));

    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> t = {0x66, 0x08};   // 2150 LE → 21.5
    auto tr = dispatch_zcl_ep(c1, def, 0x0402, 1, "msTemperatureMeasurement",
                              attr_report(0x0000, 0x29, std::span<const std::uint8_t>(t.data(), t.size())));
    assert(tr.any_matched);
    const Value* tv = tr.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);

    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> h = {0x7C, 0x15};   // 5500 LE
    auto hr = dispatch_zcl_ep(c2, def, 0x0405, 1, "msRelativeHumidity",
                              attr_report(0x0000, 0x21, std::span<const std::uint8_t>(h.data(), h.size())));
    assert(hr.any_matched);
    assert(hr.merged.find("humidity") != nullptr);
}

// ── MIR-SO100: SOS button is action, not a settable switch. ────────────────
DispatchResult dispatch_sos(RuntimeContext& ctx, std::uint8_t code) {
    using namespace zhc::devices::multir;
    // Raw ssIasZone cluster-specific command: [fc][tsn][cmd][code...].
    const std::array<std::uint8_t, 4> bytes = {0x09, 0x42, 0x00, code};
    return dispatch_zcl_ep(ctx, kDef_MIR_SO100, 0x0500, 1, "ssIasZone",
                           std::span<const std::uint8_t>(bytes.data(), bytes.size()));
}

void test_so100_action() {
    using namespace zhc::devices::multir;
    const auto& def = kDef_MIR_SO100;
    // Shape: action present, no settable state, no write path.
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);
    assert(!def_binds(def, 1, 0x0006));   // phantom genOnOff binding gone

    RuntimeContext c1{};
    auto single = dispatch_sos(c1, 0);
    assert(single.any_matched);
    assert(action_is(single, "action", "single"));

    RuntimeContext c2{};
    auto dbl = dispatch_sos(c2, 1);
    assert(action_is(dbl, "action", "double"));

    RuntimeContext c3{};
    auto hold = dispatch_sos(c3, 128);
    assert(action_is(hold, "action", "hold"));

    // Unknown code is dropped.
    RuntimeContext c4{};
    auto none = dispatch_sos(c4, 42);
    assert(none.merged.find("action") == nullptr);
}

}  // namespace

int main() {
    test_ias_typed_sensors();
    test_sm100e_generic();
    test_ir100_illuminance();
    test_te600();
    test_so100_action();
    return 0;
}
