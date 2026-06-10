// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Evology PSE03-V1.1.0 sound-and-flash siren.
// z2m-source: evology.ts (fz.ias_siren, fz.ias_enroll, fz.ias_wd).
//
// Bug fixed (found by a z2m<->embedded-zhc parity pass):
//   The auto-port wired the generic kFzIasZone, whose fn reads ZoneStatus from
//   AttributeReport attr 0x0002 (payload key "2"). But this siren reports its
//   state via z2m fz.ias_siren — an ssIasZone `commandStatusChangeNotification`
//   (Command frame, cmd 0x00) where ZoneStatus rides the command body, not attr
//   0x0002. kFzIasZone therefore matched nothing on the real wire and the
//   exposed alarm/tamper/battery_low keys were dead. Now wires
//   kFzIasGenericAlarm (z2m zoneType:"alarm" siren), which decodes the command
//   notification: bit0 -> alarm, bit2 -> tamper, bit3 -> battery_low.
//
// This test pins, on a real ZCL ZoneStatusChangeNotification wire shape, that
// the siren decodes its three semantic keys (and not the bare alarm_1/alarm_2
// dead keys produced by the old generic path).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::evology {
extern const PreparedDefinition kDef_PSE03_V1_1_0;  // siren -> generic alarm
}  // namespace zhc::devices::evology

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

// ── IAS Zone Status Change Notification (cluster 0x0500, cmd 0x00) ────
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

}  // namespace

// ── siren: ssIasZone commandStatusChangeNotification -> alarm/tamper/battery_low
static void test_siren_decodes_command_notification() {
    const auto& def = devices::evology::kDef_PSE03_V1_1_0;
    assert(def_exposes(def, "alarm"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_binds(def, 0x0500));

    // alarm_1 (bit 0) asserted -> alarm true, tamper/battery_low clear.
    auto on = dispatch_ias(def, ias_notif(0x0001));
    assert(on.any_matched);
    assert(b_true(on.merged.find("alarm")));
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    // clear -> alarm false.
    auto off = dispatch_ias(def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find("alarm")));

    // tamper(bit2) + battery_low(bit3), alarm bit0 clear.
    auto tb = dispatch_ias(def, ias_notif(0x000C));
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("alarm")));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// Regression: the device's reporting path is the command notification, and the
// siren publishes the bare `alarm` key (zoneType:"alarm") — never the
// alarm_1/alarm_2 dead keys that the old generic kFzIasZone status-change path
// would emit.
static void test_no_dead_alarm_keys() {
    auto r = dispatch_ias(devices::evology::kDef_PSE03_V1_1_0, ias_notif(0x0001));
    assert(r.merged.find("alarm_1") == nullptr);
    assert(r.merged.find("alarm_2") == nullptr);
}

int main() {
    test_siren_decodes_command_notification();
    test_no_dead_alarm_keys();
    return 0;
}
