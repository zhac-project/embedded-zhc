// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the EZVIZ family.
//
// Real parity gap fixed (CS-T2C open/close sensor):
//   The Tier-1 auto-port wired the generic `kFzIasZone`, which emits a bare,
//   NON-inverted `"alarm"` boolean. z2m ships this device via
//     m.iasZoneAlarm({zoneType:"contact",
//       zoneAttributes:["alarm_1","alarm_2","tamper","battery_low"]}).
//   zoneType:"contact" sets invertAlarmPayload=true, and BOTH alarm_1+alarm_2
//   are present, so z2m takes its `bothAlarms` branch and publishes two
//   semantic, INVERTED keys: contact_alarm_1 (=!bit0) and contact_alarm_2
//   (=!bit1) plus tamper (bit2) + battery_low (bit3). The auto-port's bare
//   `alarm` key was both the wrong name AND the wrong polarity.
//
//   Re-wired to the typed two-key inverted converters kFzIasContactAlarm1 +
//   kFzIasContactAlarm2 and renamed the exposes accordingly.
//
// z2m-source: zigbee-herdsman-converters/src/devices/ezviz.ts #CS-T2C +
//             lib/modernExtend.ts (iasZoneAlarm / battery).

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
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ezviz {
extern const PreparedDefinition kDef_CS_T2C;
}  // namespace zhc::devices::ezviz

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

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── ssIasZone ZoneStatusChangeNotification (server->client): ──────────
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
struct IasFrame { std::array<std::uint8_t, 9> bytes; };
IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = {0x09, tsn, 0x00,
               static_cast<std::uint8_t>(zone_status & 0xFF),
               static_cast<std::uint8_t>(zone_status >> 8),
               0x00, 0x00, 0x00, 0x00};
    return f;
}
DispatchResult dispatch_ias(const PreparedDefinition& def, const IasFrame& f) {
    return dispatch_zcl(def, 0x0500, "ssIasZone", 1,
                        std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
}

// ── ZCL attribute report: fc=0x18, tsn, cmd=0x0A, then record(s). ─────
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── CS-T2C contact — the parity fix ───────────────────────────────────
void check_cst2c_contact() {
    const auto& def = devices::ezviz::kDef_CS_T2C;

    // Expose surface: the two semantic contact keys (NOT the stale generic
    // "alarm"), plus tamper / battery_low / battery / voltage.
    assert(def_exposes(def, "contact_alarm_1"));
    assert(def_exposes(def, "contact_alarm_2"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));
    assert(!def_exposes(def, "alarm"));  // regression guard

    // zoneStatus = 0 -> both alarms INVERTED to true (closed magnet present).
    auto closed = dispatch_ias(def, ias_notif(0x0000));
    assert(closed.any_matched);
    const Value* a1c = closed.merged.find("contact_alarm_1");
    const Value* a2c = closed.merged.find("contact_alarm_2");
    assert(a1c && a1c->type == ValueType::Bool && a1c->b == true);
    assert(a2c && a2c->type == ValueType::Bool && a2c->b == true);
    // The stale generic "alarm" key must never be produced.
    assert(closed.merged.find("alarm") == nullptr);

    // zoneStatus bit0 = 1 -> contact_alarm_1 inverts to false; bit1 still 0
    // -> contact_alarm_2 stays true.
    auto open1 = dispatch_ias(def, ias_notif(0x0001));
    const Value* a1o = open1.merged.find("contact_alarm_1");
    const Value* a2o = open1.merged.find("contact_alarm_2");
    assert(a1o && a1o->type == ValueType::Bool && a1o->b == false);
    assert(a2o && a2o->type == ValueType::Bool && a2o->b == true);

    // zoneStatus bit1 = 1 -> contact_alarm_2 inverts to false.
    auto open2 = dispatch_ias(def, ias_notif(0x0002));
    const Value* a2b = open2.merged.find("contact_alarm_2");
    assert(a2b && a2b->type == ValueType::Bool && a2b->b == false);

    // tamper (bit2) + battery_low (bit3) decode alongside, NON-inverted.
    auto tb = dispatch_ias(def, ias_notif(0x0004 | 0x0008));
    const Value* tp = tb.merged.find("tamper");
    const Value* bl = tb.merged.find("battery_low");
    assert(tp && tp->type == ValueType::Bool && tp->b == true);
    assert(bl && bl->type == ValueType::Bool && bl->b == true);
}

// ── CS-T2C battery (genPowerCfg) regression ───────────────────────────
void check_cst2c_battery() {
    const auto& def = devices::ezviz::kDef_CS_T2C;

    // attr 0x0021 batteryPercentageRemaining (u8, ZCL half-percent) = 0xC8
    // (200) -> 100 %.
    const std::uint8_t pct[] = {0xC8};
    auto rp = dispatch_zcl(def, 0x0001, "genPowerCfg", 1,
                           attr_report(0x0021, 0x20, pct));
    assert(rp.any_matched);
    const Value* bv = rp.merged.find("battery");
    assert(bv && bv->type == ValueType::Uint && bv->u == 100);

    // attr 0x0020 batteryVoltage (u8, 100 mV units) = 30 -> 3000 mV.
    const std::uint8_t volt[] = {30};
    auto rv = dispatch_zcl(def, 0x0001, "genPowerCfg", 1,
                           attr_report(0x0020, 0x20, volt));
    const Value* vv = rv.merged.find("voltage");
    assert(vv && vv->type == ValueType::Uint && vv->u == 3000);
}

}  // namespace

int main() {
    check_cst2c_contact();
    check_cst2c_battery();
    return 0;
}
