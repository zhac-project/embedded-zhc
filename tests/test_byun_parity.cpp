// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the BYUN M415-5C (gas) / M415-6C (smoke) sensors.
// z2m-source: byun.ts fz.byun_{smoke,gas}_{true,false}.
//
// Graduated Tier-2 fix pinned here (DEAD-KEY + DROPPED-CHANNEL, gap c+b):
//
//   The generated ports wired the generic kFzIasZone, emitting the bare
//   keys alarm/tamper/battery_low. z2m instead declares a single boolean
//   expose (`smoke` / `gas`) driven by four bespoke converters:
//     true : ssIasZone commandStatusChangeNotification, zoneStatus == 33
//     false: smoke — pHMeasurement (0x0409) measuredValue == 0
//            gas   — raw cluster 0x040A, frame-control byte == 0x18
//   Against kFzIasZone the alarm-CLEAR channel was dropped entirely and the
//   alarm key never matched the expose. The fix wires the shared byun
//   converters; these tests assert the right keys + the right boolean
//   polarity for both the alarm and clear paths.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "definitions/byun/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::byun {
extern const PreparedDefinition kDef_M415_5C;
extern const PreparedDefinition kDef_M415_6C;
}  // namespace zhc::devices::byun

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

// ssIasZone commandStatusChangeNotification:
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)  ext(1) zoneId(1) delay(2)
std::vector<std::uint8_t> ias_notif(std::uint16_t zone_status,
                                    std::uint8_t tsn = 0x42) {
    return { 0x09, tsn, 0x00,
             static_cast<std::uint8_t>(zone_status & 0xFF),
             static_cast<std::uint8_t>(zone_status >> 8),
             0x00, 0x00, 0x00, 0x00 };
}

DispatchResult dispatch_ias(const PreparedDefinition& def,
                            const std::vector<std::uint8_t>& body) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(body.data(), body.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// pHMeasurement (0x0409) Report Attributes: attr 0x0000 (measuredValue) u16.
//   fc(1)=0x18  tsn(1)  cmd(1)=0x0A  attr_le(2) type(1)=0x21 value_le(2)
std::vector<std::uint8_t> ph_report(std::uint16_t measured,
                                    std::uint8_t tsn = 0x43) {
    return { 0x18, tsn, 0x0A,
             0x00, 0x00, 0x21,
             static_cast<std::uint8_t>(measured & 0xFF),
             static_cast<std::uint8_t>(measured >> 8) };
}

DispatchResult dispatch_ph(const PreparedDefinition& def,
                           const std::vector<std::uint8_t>& body) {
    auto raw = make_frame(0x0409, 1,
                          std::span<const std::uint8_t>(body.data(), body.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "pHMeasurement";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// raw 0x040A frame whose first byte (frame control) decides gas-clear.
// z2m's byun_gas_false keys solely off `msg.data[0]` (the frame-control
// byte); our converter mirrors that via raw_data[0]. The frame body is a
// well-formed single-attr report so decode_frame succeeds — its contents
// are irrelevant to the gas-clear decision.
DispatchResult dispatch_gas_raw(const PreparedDefinition& def,
                                std::uint8_t fc) {
    // fc tsn cmd=Report  attr_le(0x0000) type=u8(0x20) value(0x00)
    std::vector<std::uint8_t> body = { fc, 0x44, 0x0A, 0x00, 0x00, 0x20, 0x00 };
    auto raw = make_frame(0x040A, 1,
                          std::span<const std::uint8_t>(body.data(), body.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "byunGasRaw";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

const Value* find_bool(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    return (v && v->type == ValueType::Bool) ? v : nullptr;
}

}  // namespace

// ── exposes shape: single boolean smoke/gas, NO dead alarm keys ──────
static void test_shape() {
    const auto& gas = devices::byun::kDef_M415_5C;
    assert(def_exposes(gas, "gas"));
    assert(!def_exposes(gas, "alarm"));
    assert(!def_exposes(gas, "tamper"));
    assert(!def_exposes(gas, "battery_low"));

    const auto& smoke = devices::byun::kDef_M415_6C;
    assert(def_exposes(smoke, "smoke"));
    assert(!def_exposes(smoke, "alarm"));
}

// ── M415-5C gas ──────────────────────────────────────────────────────
static void test_gas() {
    const auto& def = devices::byun::kDef_M415_5C;

    // ALARM: zoneStatus == 33 → gas: true (and no phantom tamper/battery_low).
    auto alarm = dispatch_ias(def, ias_notif(33));
    assert(alarm.any_matched);
    const Value* g = find_bool(alarm, "gas");
    assert(g && g->b == true);
    assert(alarm.merged.find("tamper") == nullptr);
    assert(alarm.merged.find("battery_low") == nullptr);
    assert(alarm.merged.find("alarm") == nullptr);

    // A bit0-set status that is NOT 33 must NOT fire (z2m magic equality).
    auto bit0 = dispatch_ias(def, ias_notif(0x0001));
    assert(bit0.merged.find("gas") == nullptr);

    // CLEAR: raw 0x040A, frame-control 0x18 → gas: false.
    auto clear = dispatch_gas_raw(def, 0x18);
    assert(clear.any_matched);
    const Value* gc = find_bool(clear, "gas");
    assert(gc && gc->b == false);

    // A different frame-control byte must NOT clear.
    auto noclear = dispatch_gas_raw(def, 0x08);
    assert(noclear.merged.find("gas") == nullptr);
}

// ── M415-6C smoke ─────────────────────────────────────────────────────
static void test_smoke() {
    const auto& def = devices::byun::kDef_M415_6C;

    // ALARM: zoneStatus == 33 → smoke: true.
    auto alarm = dispatch_ias(def, ias_notif(33));
    assert(alarm.any_matched);
    const Value* s = find_bool(alarm, "smoke");
    assert(s && s->b == true);
    assert(alarm.merged.find("tamper") == nullptr);
    assert(alarm.merged.find("battery_low") == nullptr);

    auto bit0 = dispatch_ias(def, ias_notif(0x0001));
    assert(bit0.merged.find("smoke") == nullptr);

    // CLEAR: pHMeasurement measuredValue == 0 → smoke: false.
    auto clear = dispatch_ph(def, ph_report(0));
    assert(clear.any_matched);
    const Value* sc = find_bool(clear, "smoke");
    assert(sc && sc->b == false);

    // A non-zero measuredValue must NOT clear.
    auto noclear = dispatch_ph(def, ph_report(5));
    assert(noclear.merged.find("smoke") == nullptr);
}

int main() {
    test_shape();
    test_gas();
    test_smoke();
    return 0;
}
