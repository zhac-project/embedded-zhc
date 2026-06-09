// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Nous (Tuya-OEM) family. Found by a z2m<->embedded-zhc
// parity pass over the 12 existing nous defs.
//
// Two LCD temperature/humidity sensors were dead on the primary channels.
// z2m serves both via the Tuya-DP (manuSpecificTuya 0xEF00) legacy converter
// `legacy.fz.nous_lcd_temperature_humidity_sensor`, but the generated defs
// wired a genPowerCfg-only battery converter (kFzBattery). TS0601 DP devices
// never send genPowerCfg/msTemperature/msRelativeHumidity reports, so
// temperature, humidity AND battery all failed to decode:
//
//   1. E6     (_TZE284_wtikaxzs +3): was kFzBattery only. Now a Tuya-DP map:
//      temperature DP1 (/10), humidity DP2, battery DP4, min/max temp DP10/11
//      (/10), min/max humidity DP12/13, temperature/humidity alarm DP14/15.
//   2. SZ-T04 (_TZE200_locansqn +7): was kFzBattery only. Same DP map plus
//      report intervals DP17/18 and sensitivity DP19 (/10) / DP20.
//
// Both also gain the inverse tz (write) path so the STATE_SET config keys
// (min/max, report interval, unit) round-trip through the Tuya MCU.
//
// z2m-source: zigbee-herdsman-converters/src/devices/nous.ts + lib/legacy.ts
//             fz.nous_lcd_temperature_humidity_sensor (dataPoints.nous*).

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/nous_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> nous_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::nous::kNousRegistry, devices::nous::kNousRegistryCount);
}

const PreparedDefinition& resolve(const char* manu, const char* model) {
    const auto* d = find_definition("TS0601", manu, nous_registry());
    assert(d && "nous manufacturer name must resolve to a def");
    assert(d->model && std::strcmp(d->model, model) == 0 &&
           "resolved def must be the expected model (Tuya-DP, not battery stub)");
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

// Decode one Enum DP (dp_type 0x04, 1 wire byte) and return the StringRef.
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

// Assert temperature DP1 -> /10 Float, humidity DP2 raw, battery DP4 raw.
void check_core_th_battery(const PreparedDefinition& d) {
    assert(def_exposes(d, "temperature"));
    assert(def_exposes(d, "humidity"));
    assert(def_exposes(d, "battery"));

    // temperature DP1 = 235 wire -> 23.5 C (z2m value/10).
    auto t = decode_value(d, 1, 235);
    assert(t.any_matched);
    const Value* tv = t.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);
    assert(std::fabs(as_num(tv) - 23.5) < 0.01);

    // humidity DP2 = 55 wire -> 55 (raw).
    auto h = decode_value(d, 2, 55);
    const Value* hv = h.merged.find("humidity");
    assert(hv && std::fabs(as_num(hv) - 55.0) < 0.01);

    // battery DP4 = 90 wire -> 90 (raw, not from genPowerCfg).
    auto b = decode_value(d, 4, 90);
    const Value* bv = b.merged.find("battery");
    assert(bv && std::fabs(as_num(bv) - 90.0) < 0.01);

    // max/min temperature DP10/11 also /10.
    auto mx = decode_value(d, 10, 600);
    const Value* mxv = mx.merged.find("max_temperature");
    assert(mxv && std::fabs(as_num(mxv) - 60.0) < 0.01);
}

// ── 1. E6 ─────────────────────────────────────────────────────────────
void test_e6_tuya_dp_decode() {
    const auto& d = resolve("_TZE200_nnrfa68v", "E6");
    check_core_th_battery(d);

    // temperature_unit_convert enum DP9: 1 -> "fahrenheit".
    const char* unit = decode_enum(d, 9, 1, "temperature_unit_convert");
    assert(unit && std::strcmp(unit, "fahrenheit") == 0);

    // temperature_alarm enum DP14: 2 -> "canceled".
    const char* al = decode_enum(d, 14, 2, "temperature_alarm");
    assert(al && std::strcmp(al, "canceled") == 0);

    // Write path is wired (config keys settable via Tuya MCU).
    assert(d.to_zigbee_count >= 1 && d.to_zigbee != nullptr);
}

// ── 2. SZ-T04 ─────────────────────────────────────────────────────────
void test_sz_t04_tuya_dp_decode() {
    const auto& d = resolve("_TZE200_locansqn", "SZ-T04");
    check_core_th_battery(d);

    // report intervals DP17/18 (raw minutes).
    auto ti = decode_value(d, 17, 30);
    const Value* tiv = ti.merged.find("temperature_report_interval");
    assert(tiv && std::fabs(as_num(tiv) - 30.0) < 0.01);

    // temperature_sensitivity DP19 -> /10 (z2m value/10).
    auto ts = decode_value(d, 19, 5);
    const Value* tsv = ts.merged.find("temperature_sensitivity");
    assert(tsv && tsv->type == ValueType::Float);
    assert(std::fabs(as_num(tsv) - 0.5) < 0.01);

    // White-label TH01Z preserved.
    assert(d.white_labels_count >= 1);
    assert(d.to_zigbee_count >= 1 && d.to_zigbee != nullptr);
}

}  // namespace

int main() {
    test_e6_tuya_dp_decode();
    test_sz_t04_tuya_dp_decode();
    return 0;
}
