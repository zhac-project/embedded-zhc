// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Oujiabao CR701-YZ gas + carbon-monoxide alarm.
// z2m-source: oujiabao.ts #CR701-YZ wires
//   fz.ias_carbon_monoxide_alarm_1_gas_alarm_2
// which decodes a single ssIasZone commandStatusChangeNotification into
// carbon_monoxide (zoneStatus bit 0), gas (bit 1), tamper (bit 2) and
// battery_low (bit 3).
//
// Bug fixed: the auto-port lowered the generic kFzIasZone (bare "alarm"
// key) and declared a phantom "alarm" expose, so neither carbon_monoxide
// nor gas ever reached the shadow. The def now wires BOTH typed
// converters — kFzIasCoAlarm (bit 0 → carbon_monoxide) and
// kFzIasGasAlarm2 (bit 1 → gas) — whose payloads the dispatch loop
// merges. These tests pin, on real Zone-Status-Change-Notification wire
// shapes, that carbon_monoxide and gas decode from the correct,
// independent bits (and that the bare "alarm" key is gone).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::oujiabao {
extern const PreparedDefinition kDef_CR701_YZ;
}  // namespace zhc::devices::oujiabao

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

// The four semantic binaries must be declared exposes; the phantom bare
// "alarm" must be gone.
static void test_exposes() {
    const auto& def = devices::oujiabao::kDef_CR701_YZ;
    assert(def_exposes(def, "carbon_monoxide"));
    assert(def_exposes(def, "gas"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(!def_exposes(def, "alarm"));
}

// carbon_monoxide reads zoneStatus bit 0 independently of gas (bit 1).
static void test_carbon_monoxide_bit0() {
    const auto& def = devices::oujiabao::kDef_CR701_YZ;

    auto on = dispatch_ias(def, ias_notif(0x0001));  // bit 0 only
    assert(on.any_matched);
    assert(b_true(on.merged.find("carbon_monoxide")));
    assert(b_false(on.merged.find("gas")));            // bit 1 clear
    assert(on.merged.find("alarm") == nullptr);        // bare key gone

    auto off = dispatch_ias(def, ias_notif(0x0000));   // clear
    assert(off.any_matched);
    assert(b_false(off.merged.find("carbon_monoxide")));
}

// gas reads zoneStatus bit 1 (alarm_2) independently of CO (bit 0).
static void test_gas_bit1() {
    const auto& def = devices::oujiabao::kDef_CR701_YZ;

    auto on = dispatch_ias(def, ias_notif(0x0002));  // bit 1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find("gas")));
    assert(b_false(on.merged.find("carbon_monoxide")));  // bit 0 clear
    assert(on.merged.find("alarm") == nullptr);
}

// Both alarms + tamper + battery_low decode together from one frame.
static void test_all_bits_combined() {
    const auto& def = devices::oujiabao::kDef_CR701_YZ;

    // bits 0|1|2|3 = 0x000F → CO + gas + tamper + battery_low all set.
    auto r = dispatch_ias(def, ias_notif(0x000F));
    assert(r.any_matched);
    assert(b_true(r.merged.find("carbon_monoxide")));
    assert(b_true(r.merged.find("gas")));
    assert(b_true(r.merged.find("tamper")));
    assert(b_true(r.merged.find("battery_low")));

    // tamper(bit2)+battery_low(bit3) only — both alarms clear.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("carbon_monoxide")));
    assert(b_false(tb.merged.find("gas")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

int main() {
    test_exposes();
    test_carbon_monoxide_bit0();
    test_gas_bit1();
    test_all_bits_combined();
    return 0;
}
