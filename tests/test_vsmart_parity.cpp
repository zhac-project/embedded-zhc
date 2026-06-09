// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the VSmart (HumanSmart) family.
//
// Real parity gap fixed (HS-SEDR00ZB-VNM door/window sensor):
//   The Tier-1 auto-port wired the generic `kFzIasZone`, which emits a
//   generic `"alarm"` boolean — never the `"contact"` key that z2m
//   advertises for `m.iasZoneAlarm({zoneType:"contact", ...})`. The
//   advertised expose was therefore dead against the wire shape. Re-wired
//   to the typed `kFzIasContactAlarm` (emits contact / tamper /
//   battery_low) and renamed the expose `alarm` -> `contact`.
//
//   Polarity: z2m sets invertAlarmPayload=true for zoneType "contact",
//   then this device's explicit invertAlarm:true flips it back to false,
//   so contact = (zoneStatus & 1). kFzIasContactAlarm emits the same, so
//   the polarity matches (no inversion).
//
// Regression coverage (already-correct ports, pinned here):
//   * HS-SEOC00ZB-VNM occupancy sensor: occupancy via lumi kFzOccupancy
//     (msOccupancySensing 0x0000 bit 0) + battery exposes.
//   * HS-SW100ZB-VNM / HS-SW200ZB-VNM wall switches: on/off via generic
//     kFzOnOff; SW100 carries NO endpoint_map (single gang), SW200 a
//     2-entry map (multi-gang). VSmart custom write-path commands
//     (LED colour cmd 0xF3, vibration cmd 0xF0, per-period LED intensity
//     cmd 0xF2) encode the manu-specific cluster-specific frame.
//
// z2m-source: zigbee-herdsman-converters/src/devices/vsmart.ts +
//             lib/modernExtend.ts (iasZoneAlarm / onOff / occupancy).

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
#include "definitions/vsmart/_shared.hpp"

namespace zhc::devices::vsmart {
extern const PreparedDefinition kDef_HS_SEDR00ZB_VNM;
extern const PreparedDefinition kDef_HS_SEOC00ZB_VNM;
extern const PreparedDefinition kDef_HS_SW100ZB_VNM;
extern const PreparedDefinition kDef_HS_SW200ZB_VNM;
}  // namespace zhc::devices::vsmart

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

// ── HS-SEDR00ZB-VNM — the parity fix ──────────────────────────────────
void check_sedr_contact() {
    const auto& def = devices::vsmart::kDef_HS_SEDR00ZB_VNM;

    // Expose surface: contact (NOT the stale generic "alarm"), tamper,
    // battery_low.
    assert(def_exposes(def, "contact"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(!def_exposes(def, "alarm"));  // regression guard

    // zoneStatus bit0 = 1 -> contact = true (open, per double-invert).
    auto open = dispatch_ias(def, ias_notif(0x0001));
    assert(open.any_matched);
    const Value* cv = open.merged.find("contact");
    assert(cv && cv->type == ValueType::Bool && cv->b == true);
    // The stale generic "alarm" key must never be produced.
    assert(open.merged.find("alarm") == nullptr);

    // zoneStatus = 0 -> contact = false (closed).
    auto closed = dispatch_ias(def, ias_notif(0x0000));
    const Value* cz = closed.merged.find("contact");
    assert(cz && cz->type == ValueType::Bool && cz->b == false);

    // tamper (bit2) + battery_low (bit3) decode alongside.
    auto tb = dispatch_ias(def, ias_notif(0x0004 | 0x0008));
    const Value* tp = tb.merged.find("tamper");
    const Value* bl = tb.merged.find("battery_low");
    assert(tp && tp->type == ValueType::Bool && tp->b == true);
    assert(bl && bl->type == ValueType::Bool && bl->b == true);
}

// ── HS-SEOC00ZB-VNM occupancy (regression) ────────────────────────────
void check_seoc_occupancy() {
    const auto& def = devices::vsmart::kDef_HS_SEOC00ZB_VNM;

    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));

    const std::uint8_t occ_on[] = {0x01};
    auto ro = dispatch_zcl(def, 0x0406, "msOccupancySensing", 1,
                           attr_report(0x0000, 0x18, occ_on));
    assert(ro.any_matched);
    const Value* ov = ro.merged.find("occupancy");
    assert(ov && ov->type == ValueType::Bool && ov->b == true);

    const std::uint8_t occ_off[] = {0x00};
    auto rz = dispatch_zcl(def, 0x0406, "msOccupancySensing", 1,
                           attr_report(0x0000, 0x18, occ_off));
    const Value* oz = rz.merged.find("occupancy");
    assert(oz && oz->type == ValueType::Bool && oz->b == false);
}

// ── HS-SW100/200 wall switches (regression) ────────────────────────────
void check_switches() {
    const auto& sw1 = devices::vsmart::kDef_HS_SW100ZB_VNM;
    const auto& sw2 = devices::vsmart::kDef_HS_SW200ZB_VNM;

    assert(def_exposes(sw1, "state"));
    assert(def_exposes(sw2, "state"));

    // Single-gang carries NO endpoint_map; multi-gang DOES (project rule:
    // 1-channel devices are unsuffixed, multi-gang get <key>_<label>).
    assert(sw1.endpoint_map_count == 0);
    assert(sw2.endpoint_map_count == 2);

    // genOnOff attr 0x0000 (bool): 1 -> on.
    const std::uint8_t on[] = {0x01};
    auto ron = dispatch_zcl(sw1, 0x0006, "genOnOff", 1,
                            attr_report(0x0000, 0x10, on));
    assert(ron.any_matched);
    const Value* sv = ron.merged.find("state");
    assert(sv != nullptr);

    // power_on_behavior is z2m `powerOnBehavior:false` -> must be absent.
    assert(!def_exposes(sw1, "power_on_behavior"));
}

// ── VSmart custom write-path command encoders (regression) ────────────
//
// Frame layout from vsmart::tz_vsmart_raw_cmd:
//   fc(0x15)  manu_lo  manu_hi  tsn(0x00)  cmd  payload...
// manu code = 0x1379 -> lo 0x79, hi 0x13.
void check_tz_encoders() {
    const PreparedDefinition& def = devices::vsmart::kDef_HS_SW100ZB_VNM;
    RuntimeContext ctx{};
    std::array<std::uint8_t, 32> frame{};
    std::size_t n = 0;

    // setLedColor (cmd 0xF3): "#ff0000" ON -> [R,G,B,state=0x01].
    {
        Value v{}; v.type = ValueType::StringRef; v.str = "#ff0000";
        bool ok = vsmart::tz_vsmart_raw_cmd("led_indicator_color_on", v,
                                            vsmart::kTzLedIndicatorColorOn,
                                            def, ctx, frame, n);
        assert(ok);
        // fc, manu_lo, manu_hi, tsn, cmd, R, G, B, state
        const std::uint8_t want[] = {0x15, 0x79, 0x13, 0x00, 0xF3,
                                     0xFF, 0x00, 0x00, 0x01};
        assert(n == sizeof(want));
        assert(std::memcmp(frame.data(), want, n) == 0);
    }

    // setVibrationIntensity (cmd 0xF0): 100% -> 0xFF.
    {
        Value v{}; v.type = ValueType::Uint; v.u = 100;
        bool ok = vsmart::tz_vsmart_raw_cmd("vibration_intensity", v,
                                            vsmart::kTzVibrationIntensity,
                                            def, ctx, frame, n);
        assert(ok);
        const std::uint8_t want[] = {0x15, 0x79, 0x13, 0x00, 0xF0, 0xFF};
        assert(n == sizeof(want));
        assert(std::memcmp(frame.data(), want, n) == 0);
    }

    // setLedIntensity (cmd 0xF2): evening (endpoint index 1), 50% ->
    //   round(50*255/100)=128 (0x80); payload [intensity, idx, 0x00].
    {
        Value v{}; v.type = ValueType::Uint; v.u = 50;
        bool ok = vsmart::tz_vsmart_raw_cmd("evening_led_intensity", v,
                                            vsmart::kTzEveningLedIntensity,
                                            def, ctx, frame, n);
        assert(ok);
        const std::uint8_t want[] = {0x15, 0x79, 0x13, 0x00, 0xF2,
                                     0x80, 0x01, 0x00};
        assert(n == sizeof(want));
        assert(std::memcmp(frame.data(), want, n) == 0);
    }

    // Out-of-range percent is rejected.
    {
        Value v{}; v.type = ValueType::Uint; v.u = 101;
        bool ok = vsmart::tz_vsmart_raw_cmd("vibration_intensity", v,
                                            vsmart::kTzVibrationIntensity,
                                            def, ctx, frame, n);
        assert(!ok);
    }
}

}  // namespace

int main() {
    check_sedr_contact();
    check_seoc_occupancy();
    check_switches();
    check_tz_encoders();
    return 0;
}
