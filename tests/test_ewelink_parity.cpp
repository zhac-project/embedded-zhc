// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity fixtures for the eWeLink (Coolkit / Sonoff-OEM) vendor.
//
// Headline regression: the WS01 rainfall sensor reports rain over
// ssIasZone commandStatusChangeNotification (zoneStatus bit 0). z2m maps
// it with `fzLocal.WS01_rain` -> {rain: bool}. Its generated port
// declared the `rain` expose but wired the generic kFzIasZoneStatusChange,
// which only emits alarm_1/alarm_2/tamper/battery_low — so the `rain`
// expose stayed permanently empty. It now wires the typed kFzIasRainAlarm
// (zoneStatus bit 0 -> "rain"), mirroring z2m exactly.
//
// Also guards two already-correct vendor devices against regression:
//   - SNZB-05 water sensor: typed water_leak decode (not bare "alarm").
//   - CK-BL702-ROUTER USB repeater: z2m exposes:[] -> no phantom on/off.
//
// z2m-source: ewelink.ts #WS01 / #SNZB-05 / #CK-BL702-ROUTER-01(7018).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::ewelink {
extern const PreparedDefinition kDef_WS01;                    // rainfall (IAS bit 0 -> rain)
extern const PreparedDefinition kDef_SNZB_05;                 // water leak (alarm_1)
extern const PreparedDefinition kDef_CK_BL702_ROUTER_01_7018_;// USB router (no exposes)
}  // namespace zhc::devices::ewelink

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

}  // namespace

// ── WS01 rainfall sensor: zoneStatus bit 0 -> "rain" ─────────────────
static void test_ws01_rain_decode() {
    const auto& def = devices::ewelink::kDef_WS01;

    // The expose advertises "rain"; the decoder must actually emit it.
    assert(def_exposes(def, "rain"));

    // bit 0 set -> rain true, and the generic bare "alarm" key must NOT
    // appear (regression guard vs the old kFzIasZoneStatusChange wiring).
    auto wet = dispatch_ias(def, ias_notif(0x0001));
    assert(wet.any_matched);
    assert(b_true(wet.merged.find("rain")));
    assert(wet.merged.find("alarm")   == nullptr);
    assert(wet.merged.find("alarm_1") == nullptr);

    // bit 0 clear -> rain false.
    auto dry = dispatch_ias(def, ias_notif(0x0000));
    assert(dry.any_matched);
    assert(b_false(dry.merged.find("rain")));
}

// ── WS01 is a pure sensor: no on/off control, binds only ssIasZone ───
// z2m: fromZigbee=[WS01_rain], toZigbee=[], exposes=[e.rain()]. The
// generic generator wrongly added kFzOnOff/kTzOnOff + a genOnOff bind;
// guard that they stay gone.
static void test_ws01_no_phantom_onoff() {
    const auto& def = devices::ewelink::kDef_WS01;
    assert(def.to_zigbee_count == 0);            // nothing controllable
    assert(!def_exposes(def, "state"));          // no on/off expose
    assert(!def_binds(def, 0x0006));             // no genOnOff bind
    assert(def_binds(def, 0x0500));              // ssIasZone bound
}

// ── SNZB-05 water sensor: typed water_leak decode (alarm_1) ──────────
static void test_snzb05_water_leak() {
    const auto& def = devices::ewelink::kDef_SNZB_05;
    assert(def_exposes(def, "water_leak"));

    auto leak = dispatch_ias(def, ias_notif(0x0001));
    assert(leak.any_matched);
    assert(b_true(leak.merged.find("water_leak")));
    assert(leak.merged.find("alarm") == nullptr);  // not the bare key

    auto tb = dispatch_ias(def, ias_notif(0x000C)); // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert(b_true(tb.merged.find("battery_low")));
}

// ── CK-BL702-ROUTER USB repeater: z2m exposes:[] -> nothing phantom ──
static void test_router_no_phantom() {
    const auto& def = devices::ewelink::kDef_CK_BL702_ROUTER_01_7018_;
    assert(def.exposes_count == 0);
    assert(def.from_zigbee_count == 0);
    assert(def.to_zigbee_count == 0);
    assert(def.bindings_count == 0);
}

int main() {
    test_ws01_rain_decode();
    test_ws01_no_phantom_onoff();
    test_snzb05_water_leak();
    test_router_no_phantom();
    return 0;
}
