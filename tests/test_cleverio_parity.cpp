// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Cleverio (Tuya-OEM) family. Found by a
// z2m<->embedded-zhc parity pass over the 2 cleverio defs (cleverio.ts).
//
// Bugs fixed:
//   * SA100 (TS0219 smart siren): z2m decodes the alarm via
//     fz.ias_alarm_only_alarm_1 — ssIasZone ZoneStatus (attr 0x0002) bit 0 →
//     `alarm` ONLY (generic zoneType; bare `alarm` is correct). The generated
//     def lowered the broader kFzIasZone (also emits tamper + battery_low) and
//     declared phantom `tamper` / `battery_low` exposes z2m never surfaces.
//     Re-pointed at kFzIasZoneAlarmOnly and dropped the two phantom exposes.
//   * SS300 (SM0201 temp/humidity sensor): z2m decodes fz.temperature +
//     fz.humidity + fz.battery and exposes temperature/humidity/battery. The
//     generated def wired ONLY kFzBattery and dropped the two primary sensor
//     channels. Re-added kFzTemperature + kFzHumidity converters and exposes.
//
// z2m-source: zigbee-herdsman-converters/src/devices/cleverio.ts.

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::cleverio {
extern const PreparedDefinition kDef_SA100;  // siren  → IAS alarm-only
extern const PreparedDefinition kDef_SS300;  // temp/humidity sensor
}  // namespace zhc::devices::cleverio

using namespace zhc;

namespace {

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

double as_num(const Value* v) {
    assert(v);
    return (v->type == ValueType::Int) ? double(v->i)
         : (v->type == ValueType::Uint) ? double(v->u)
         : v->f;
}

// ── ZCL attribute-report harness ──────────────────────────────────────
InboundApsFrame make_zcl_frame(std::uint16_t cluster,
                               std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};   // fc, tsn, cmd=ReportAttributes
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def,
                            std::uint16_t cluster_id, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_zcl_frame(cluster_id, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ssIasZone ZoneStatus (attr 0x0002, MAP16 0x19).
DispatchResult dispatch_zonestatus(const PreparedDefinition& def,
                                   std::uint16_t zone_status) {
    const std::uint8_t zs[] = {
        static_cast<std::uint8_t>(zone_status & 0xFF),
        static_cast<std::uint8_t>(zone_status >> 8),
    };
    return dispatch_zcl(def, 0x0500, "ssIasZone", attr_report(0x0002, 0x19, zs));
}

// measuredValue (attr 0x0000) as a signed/unsigned 16-bit report.
DispatchResult dispatch_u16(const PreparedDefinition& def, std::uint16_t cluster,
                            const char* cluster_name, std::uint8_t type,
                            std::uint16_t raw) {
    const std::uint8_t b[] = {
        static_cast<std::uint8_t>(raw & 0xFF),
        static_cast<std::uint8_t>(raw >> 8),
    };
    return dispatch_zcl(def, cluster, cluster_name, attr_report(0x0000, type, b));
}

// ── SA100 — siren: IAS alarm-only (no phantom tamper / battery_low) ───
void test_sa100_alarm_only() {
    const auto& d = devices::cleverio::kDef_SA100;
    assert(def_exposes(d, "alarm"));
    assert(def_exposes(d, "warning"));
    // Phantom exposes z2m never surfaces (fz.ias_alarm_only_alarm_1).
    assert(!def_exposes(d, "tamper"));
    assert(!def_exposes(d, "battery_low"));

    // bit 0 asserted → alarm true, no tamper / battery_low keys.
    auto on = dispatch_zonestatus(d, 0x0001);
    assert(on.any_matched);
    const Value* av = on.merged.find("alarm");
    assert(av && av->type == ValueType::Bool && av->b);
    assert(on.merged.find("tamper") == nullptr);
    assert(on.merged.find("battery_low") == nullptr);

    // Clear → alarm false.
    auto off = dispatch_zonestatus(d, 0x0000);
    assert(off.any_matched);
    const Value* off_av = off.merged.find("alarm");
    assert(off_av && off_av->type == ValueType::Bool && !off_av->b);

    // Even with tamper(bit2)+battery_low(bit3) set on the wire, only `alarm`
    // surfaces — matching z2m's alarm-only converter.
    auto tb = dispatch_zonestatus(d, 0x000C);
    assert(tb.any_matched);
    assert(tb.merged.find("tamper") == nullptr);
    assert(tb.merged.find("battery_low") == nullptr);
}

// ── SS300 — temp/humidity sensor: real sensor channels restored ───────
void test_ss300_temp_humidity_battery() {
    const auto& d = devices::cleverio::kDef_SS300;
    assert(def_exposes(d, "temperature"));
    assert(def_exposes(d, "humidity"));
    assert(def_exposes(d, "battery"));
    assert(def_exposes(d, "voltage"));

    // msTemperatureMeasurement (0x0402) measuredValue /100: raw 2150 → 21.5 C.
    auto t = dispatch_u16(d, 0x0402, "msTemperatureMeasurement", 0x29 /*int16*/, 2150);
    assert(t.any_matched);
    const Value* tv = t.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);
    assert(std::fabs(as_num(tv) - 21.5) < 0.01);

    // msRelativeHumidity (0x0405) measuredValue /100: raw 4830 → 48.3 %.
    auto h = dispatch_u16(d, 0x0405, "msRelativeHumidity", 0x21 /*uint16*/, 4830);
    assert(h.any_matched);
    const Value* hv = h.merged.find("humidity");
    assert(hv && hv->type == ValueType::Float);
    assert(std::fabs(as_num(hv) - 48.3) < 0.01);

    // genPowerCfg (0x0001) batteryPercentageRemaining (attr 0x0021, u8 /2):
    // raw 0xC8 (200) → 100 %.
    const std::uint8_t bat[] = {0xC8};
    auto b = dispatch_zcl(d, 0x0001, "genPowerCfg",
                          attr_report(0x0021, 0x20 /*uint8*/, bat));
    assert(b.any_matched);
    const Value* bv = b.merged.find("battery");
    assert(bv && std::fabs(as_num(bv) - 100.0) < 0.01);
}

}  // namespace

int main() {
    test_sa100_alarm_only();
    test_ss300_temp_humidity_battery();
    return 0;
}
