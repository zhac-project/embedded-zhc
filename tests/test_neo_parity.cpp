// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Neo (NEO Coolcam, Tuya-OEM) family. Found by a
// z2m<->embedded-zhc parity pass over the 25 existing neo defs.
//
// Four named devices were misrouted by the generator. z2m serves them via
// Tuya-DP (manuSpecificTuya 0xEF00) legacy converters or standard ZCL
// clusters, but the generated defs wired IAS Zone (TS0601 sirens/PIR) or a
// battery-only converter (the SNZB-02 T/H sensor) — so the primary channels
// never decoded:
//
//   1. NAS-AB02B0 (_TZE200_d0yu2xgi): z2m `legacy.fz.neo_t_h_alarm`. Was
//      kFzIasZone {alarm,tamper,battery_low}. Now a Tuya-DP map: temperature
//      DP105 (/10), humidity DP106, alarm DP104, volume DP116, etc.
//   2. NAS-AB02B2 (_TZE200_t1blo2bj +2): z2m `legacy.fz.neo_alarm`. Was
//      kFzIasZone. Now a Tuya-DP map with the AO* DP ids: alarm DP13,
//      duration DP7, volume DP5, melody DP21, battpercentage DP15.
//   3. NAS-PD07 (_TZE200_7hfcudw5): z2m `legacy.fz.neo_nas_pd07`. Was
//      kFzIasZone. Now a Tuya-DP map: occupancy DP101, temperature DP104
//      (/10), humidity DP105, tamper DP103.
//   4. NAS-TH07B2 (_TZ3000_utwgoauk, SNZB-02): z2m m.temperature()+humidity()
//      +battery() on STANDARD clusters. Was kFzBattery only. Now adds
//      kFzTemperature (0x0402) + kFzHumidity (0x0405).
//
// The Tuya-DP fixtures decode real 0xEF00 DP frames through the dispatcher;
// the SNZB-02 fixtures decode standard msTemperatureMeasurement /
// msRelativeHumidity attribute reports.
//
// z2m-source: zigbee-herdsman-converters/src/devices/neo.ts + lib/legacy.ts.

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/devices/neo_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> neo_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::neo::kNeoRegistry, devices::neo::kNeoRegistryCount);
}

const PreparedDefinition& resolve(const char* manu, const char* model) {
    const auto* d = find_definition("TS0601", manu, neo_registry());
    assert(d && "neo manufacturer name must resolve to a def");
    assert(d->model && std::strcmp(d->model, model) == 0 &&
           "resolved def must be the expected model (Tuya-DP, not the IAS stub)");
    return *d;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

// ── Tuya-DP frame harness (0xEF00, command 0x02 dataReport) ───────────
DecodedMessage make_dp_msg() {
    DecodedMessage msg{};
    msg.family       = FrameFamily::TuyaDp;
    msg.type         = MessageType::Command;
    msg.cluster      = "manuSpecificTuya";
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    return msg;
}

InboundApsFrame make_dp_raw() {
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    return raw;
}

DispatchResult dispatch_dp(const PreparedDefinition& def,
                           std::span<const TuyaDpRecord> recs) {
    auto msg = make_dp_msg();
    auto raw = make_dp_raw();
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, recs, def, raw, ctx);
}

// Decode one 4-byte big-endian Value DP (dp_type 0x02).
DispatchResult decode_value(const PreparedDefinition& def, std::uint8_t dp,
                            std::uint32_t value) {
    const std::uint8_t b[] = {
        static_cast<std::uint8_t>((value >> 24) & 0xFF),
        static_cast<std::uint8_t>((value >> 16) & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF),
        static_cast<std::uint8_t>(value & 0xFF),
    };
    const TuyaDpRecord recs[] = {
        { dp, 0x02, std::span<const std::uint8_t>(b, 4) },
    };
    return dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
}

// Decode one Bool DP (dp_type 0x01, 1 wire byte).
DispatchResult decode_bool(const PreparedDefinition& def, std::uint8_t dp,
                           std::uint8_t on) {
    const std::uint8_t b[] = { on };
    const TuyaDpRecord recs[] = {
        { dp, 0x01, std::span<const std::uint8_t>(b, 1) },
    };
    return dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
}

// Decode one Enum DP (dp_type 0x04, 1 wire byte).
const char* decode_enum(const PreparedDefinition& def, std::uint8_t dp,
                        std::uint8_t wire, const char* out_key) {
    const std::uint8_t b[] = { wire };
    const TuyaDpRecord recs[] = {
        { dp, 0x04, std::span<const std::uint8_t>(b, 1) },
    };
    const auto r = dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
    assert(r.any_matched);
    const Value* v = r.merged.find(out_key);
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

double as_num(const Value* v) {
    assert(v);
    return (v->type == ValueType::Int) ? double(v->i)
         : (v->type == ValueType::Uint) ? double(v->u)
         : v->f;
}

// ── Standard ZCL attribute-report harness (for SNZB-02) ───────────────
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

// ── 1. NAS-AB02B0: neo_t_h_alarm Tuya-DP ──────────────────────────────
void test_ab02b0_temp_humidity_alarm() {
    const auto& d = resolve("_TZE200_d0yu2xgi", "NAS-AB02B0");
    // Must NOT be the dead IAS stub.
    assert(def_exposes(d, "temperature"));
    assert(def_exposes(d, "humidity"));
    assert(!def_exposes(d, "tamper") && "AB02B0 has no tamper channel in z2m");

    // temperature DP105 = 240 wire -> 24.0 C (z2m value/10).
    auto t = decode_value(d, 105, 240);
    assert(t.any_matched);
    const Value* tv = t.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);
    assert(std::fabs(as_num(tv) - 24.0) < 0.01);

    // humidity DP106 = 36 wire -> 36 (raw).
    auto h = decode_value(d, 106, 36);
    const Value* hv = h.merged.find("humidity");
    assert(hv && std::fabs(as_num(hv) - 36.0) < 0.01);

    // alarm DP104 Bool true.
    auto a = decode_bool(d, 104, 1);
    const Value* av = a.merged.find("alarm");
    assert(av && av->type == ValueType::Bool && av->b);

    // volume DP116 enum {2:low,1:medium,0:high}.
    assert(std::strcmp(decode_enum(d, 116, 2, "volume"), "low") == 0);
    assert(std::strcmp(decode_enum(d, 116, 0, "volume"), "high") == 0);
}

// ── 2. NAS-AB02B2: neo_alarm Tuya-DP (AO* dp ids) ─────────────────────
void test_ab02b2_alarm_volume_duration() {
    const auto& d = resolve("_TZE200_t1blo2bj", "NAS-AB02B2");
    // Sibling fingerprints resolve to the same def.
    assert(&resolve("_TZE204_q76rtoa9", "NAS-AB02B2") == &d);
    assert(def_exposes(d, "alarm"));
    assert(!def_exposes(d, "temperature") && "AB02B2 is a bare siren");

    // alarm DP13 Bool.
    auto a = decode_bool(d, 13, 1);
    const Value* av = a.merged.find("alarm");
    assert(av && av->type == ValueType::Bool && av->b);

    // duration DP7 = 10 s.
    auto dur = decode_value(d, 7, 10);
    const Value* dv = dur.merged.find("duration");
    assert(dv && std::fabs(as_num(dv) - 10.0) < 0.01);

    // volume DP5 enum {0:low,1:medium,2:high} — note: opposite order to B0.
    assert(std::strcmp(decode_enum(d, 5, 0, "volume"), "low") == 0);
    assert(std::strcmp(decode_enum(d, 5, 2, "volume"), "high") == 0);

    // battpercentage DP15.
    auto b = decode_value(d, 15, 88);
    const Value* bv = b.merged.find("battpercentage");
    assert(bv && std::fabs(as_num(bv) - 88.0) < 0.01);
}

// ── 3. NAS-PD07: neo_nas_pd07 Tuya-DP ─────────────────────────────────
void test_pd07_occupancy_temp_humidity() {
    const auto& d = resolve("_TZE200_7hfcudw5", "NAS-PD07");
    assert(def_exposes(d, "occupancy"));
    assert(def_exposes(d, "temperature"));
    assert(def_exposes(d, "humidity"));

    // occupancy DP101 Bool.
    auto o = decode_bool(d, 101, 1);
    const Value* ov = o.merged.find("occupancy");
    assert(ov && ov->type == ValueType::Bool && ov->b);

    // tamper DP103 Bool.
    auto tm = decode_bool(d, 103, 1);
    const Value* tmv = tm.merged.find("tamper");
    assert(tmv && tmv->type == ValueType::Bool && tmv->b);

    // temperature DP104 = 215 -> 21.5 C (value/10).
    auto t = decode_value(d, 104, 215);
    const Value* tv = t.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);
    assert(std::fabs(as_num(tv) - 21.5) < 0.01);

    // humidity DP105 = 50 raw.
    auto h = decode_value(d, 105, 50);
    const Value* hv = h.merged.find("humidity");
    assert(hv && std::fabs(as_num(hv) - 50.0) < 0.01);
}

// ── 4. NAS-TH07B2: standard-cluster T/H (SNZB-02) ─────────────────────
void test_th07b2_standard_temp_humidity() {
    const auto* d = find_definition("SNZB-02", "_TZ3000_utwgoauk", neo_registry());
    assert(d && d->model && std::strcmp(d->model, "NAS-TH07B2") == 0);
    assert(def_exposes(*d, "temperature"));
    assert(def_exposes(*d, "humidity"));

    // temperature: msTemperatureMeasurement 0x0402 attr 0x0000 s16 (×0.01).
    const std::uint8_t t[] = {0x09, 0x09};   // 0x0909 = 2313 -> 23.13 C
    auto rt = dispatch_zcl(*d, 0x0402, "msTemperatureMeasurement",
                           attr_report(0x0000, 0x29, t));
    assert(rt.any_matched);
    const Value* tv = rt.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);
    assert(std::fabs(as_num(tv) - 23.13) < 0.01);

    // humidity: msRelativeHumidity 0x0405 attr 0x0000 u16 (×0.01).
    const std::uint8_t h[] = {0xE8, 0x13};   // 0x13E8 = 5096 -> 50.96 %
    auto rh = dispatch_zcl(*d, 0x0405, "msRelativeHumidity",
                           attr_report(0x0000, 0x21, h));
    assert(rh.any_matched);
    const Value* hv = rh.merged.find("humidity");
    assert(hv && hv->type == ValueType::Float);
    assert(std::fabs(as_num(hv) - 50.96) < 0.01);
}

}  // namespace

int main() {
    test_ab02b0_temp_humidity_alarm();
    test_ab02b2_alarm_volume_duration();
    test_pd07_occupancy_temp_humidity();
    test_th07b2_standard_temp_humidity();
    return 0;
}
