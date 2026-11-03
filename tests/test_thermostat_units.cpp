// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Thermostat temperatures in degrees Celsius (review 2026-10-24, F-01).
//
// The generic hvacThermostat decoder published LocalTemperature and
// OccupiedHeatingSetpoint as raw 1/100 °C integers (21.5 °C -> 2150), while
// the setpoint write took °C, multiplied by 100 and masked to 16 bits with no
// range check: writing a reported value back sent 2150 x 100 = 183.92 °C. The
// write also went out as a cluster-specific command 0x02 (GetWeeklySchedule)
// instead of the profile-wide writeAttributes. Pins the z2m behaviour:
// fz.thermostat publishes °C (raw / 100; below -273.15 °C, i.e. the 0x8000
// "invalid" marker, is not published); the writes take °C, round to the wire
// unit and refuse what does not fit the attribute's ZCL range.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <limits>
#include <span>
#include <vector>

#include "definitions/_generic/_shared.hpp"
#include "definitions/eurotronic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"
#include "test_ctx.hpp"

namespace zhc::devices::eurotronic { extern const PreparedDefinition kDef_SPZB0001; }
namespace zhc::devices::elko       { extern const PreparedDefinition kDef_D4523430; }

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) { std::printf("  FAIL: %s\n", what); ++g_failures; }
}

// A thermostat wired with the generic converters only.
const FzConverter* const kFz[] = { &generic::kFzThermostat };
const TzConverter* const kTz[] = { &generic::kTzThermostat,
                                   &generic::kTzMinHeatSetpointLimit };
const PreparedDefinition kDef{
    .zigbee_models = nullptr, .zigbee_models_count = 0,
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names = nullptr, .manufacturer_names_count = 0,
    .model = "test-thermostat", .vendor = "test",
    .meta = nullptr, .exposes = nullptr, .exposes_count = 0,
    .white_labels = nullptr, .white_labels_count = 0,
    .from_zigbee = kFz, .from_zigbee_count = 1,
    .to_zigbee = kTz, .to_zigbee_count = 2,
    .configure = nullptr, .on_event = nullptr,
};

// One hvacThermostat attribute report (cmd 0x0A, one int16 or int8 record),
// manufacturer-specific when `mfg` is set.
DispatchResult report(const PreparedDefinition& def, std::uint16_t attr, std::uint8_t type,
                      std::int16_t value, std::uint16_t mfg = 0) {
    std::vector<std::uint8_t> bytes{0x18};
    if (mfg) {
        bytes[0] = 0x1C;
        bytes.push_back(static_cast<std::uint8_t>(mfg & 0xFF));
        bytes.push_back(static_cast<std::uint8_t>(mfg >> 8));
    }
    const auto u = static_cast<std::uint16_t>(value);
    for (const std::uint8_t b : {std::uint8_t{0x42}, std::uint8_t{0x0A},
                                 static_cast<std::uint8_t>(attr & 0xFF),
                                 static_cast<std::uint8_t>(attr >> 8), type,
                                 static_cast<std::uint8_t>(u & 0xFF)})
        bytes.push_back(b);
    if (type == 0x29) bytes.push_back(static_cast<std::uint8_t>(u >> 8));
    InboundApsFrame raw{};
    raw.cluster_id = 0x0201; raw.src_endpoint = 1; raw.dst_endpoint = 1;
    raw.linkquality = 0xC8; raw.data = bytes;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacThermostat";
    return dispatch_from_zigbee(msg, {}, def, raw, test_ctx());
}
DispatchResult report_s16(std::uint16_t attr, std::int16_t value) {
    return report(kDef, attr, 0x29, value);
}

bool is_celsius(const DispatchResult& r, const char* key, float want) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Float && std::fabs(v->f - want) < 0.001f;
}

struct Write { bool ok; std::vector<std::uint8_t> frame; };
Write write(const char* key, Value v) {
    std::uint8_t frame[32]{};
    const auto r = dispatch_to_zigbee(kDef, key, v, test_ctx(), frame);
    return {r.ok, std::vector<std::uint8_t>(frame, frame + (r.ok ? r.frame_size : 0))};
}
Value f32(float f)        { Value v{}; v.type = ValueType::Float; v.f = f; return v; }
Value i64(std::int64_t i) { Value v{}; v.type = ValueType::Int;   v.i = i; return v; }
Value str(const char* s)  { Value v{}; v.type = ValueType::StringRef; v.str = s; return v; }

bool frame_is(const Write& w, std::initializer_list<std::uint8_t> want) {
    return w.ok && w.frame == std::vector<std::uint8_t>(want);
}

void test_reports_publish_celsius() {
    std::printf("reports publish °C\n");
    auto r = report_s16(0x0000, 2150);
    check(is_celsius(r, "local_temperature", 21.5f), "local_temperature 2150 -> 21.5 °C (Float)");
    r = report_s16(0x0012, 2000);
    check(is_celsius(r, "current_heating_setpoint", 20.0f), "setpoint 2000 -> 20.0 °C (Float)");
    r = report_s16(0x0000, -505);
    check(is_celsius(r, "local_temperature", -5.05f), "negative reading -505 -> -5.05 °C");
    // ZCL 0x8000 = "invalid"; z2m drops anything below -273.15 °C.
    r = report_s16(0x0000, static_cast<std::int16_t>(0x8000));
    check(r.merged.find("local_temperature") == nullptr, "0x8000 local_temperature not published");
    r = report_s16(0x0012, static_cast<std::int16_t>(0x8000));
    check(r.merged.find("current_heating_setpoint") == nullptr, "0x8000 setpoint not published");
    r = report_s16(0x0000, -27315);
    check(is_celsius(r, "local_temperature", -273.15f), "-273.15 °C itself is published");
}

void test_setpoint_write_takes_celsius() {
    std::printf("setpoint write takes °C\n");
    // Profile-wide writeAttributes (fc 0x10), attr 0x0012, int16 0.01 °C.
    check(frame_is(write("current_heating_setpoint", f32(21.5f)),
                   {0x10, 0x00, 0x02, 0x12, 0x00, 0x29, 0x66, 0x08}), "21.5 °C -> 2150 writeAttributes");
    check(frame_is(write("occupied_heating_setpoint", i64(21)),
                   {0x10, 0x00, 0x02, 0x12, 0x00, 0x29, 0x34, 0x08}), "21 °C (Int) -> 2100");
    check(frame_is(write("current_heating_setpoint", f32(21.555f)),
                   {0x10, 0x00, 0x02, 0x12, 0x00, 0x29, 0x6C, 0x08}), "21.555 °C rounds to 2156");
    check(frame_is(write("current_heating_setpoint", f32(-273.15f)),
                   {0x10, 0x00, 0x02, 0x12, 0x00, 0x29, 0x4D, 0x95}), "-273.15 °C is the ZCL minimum");
    check(frame_is(write("current_heating_setpoint", f32(327.67f)),
                   {0x10, 0x00, 0x02, 0x12, 0x00, 0x29, 0xFF, 0x7F}), "327.67 °C is the ZCL maximum");
    // The old raw reading written back as °C must not wrap into 16 bits.
    check(!write("current_heating_setpoint", i64(2150)).ok, "2150 °C refused (was 183.92 °C on air)");
    check(!write("current_heating_setpoint", f32(327.68f)).ok, "327.68 °C refused");
    check(!write("current_heating_setpoint", f32(-273.16f)).ok, "-273.16 °C refused");
    check(!write("current_heating_setpoint", f32(std::numeric_limits<float>::quiet_NaN())).ok, "NaN refused");
    check(!write("current_heating_setpoint", f32(std::numeric_limits<float>::infinity())).ok, "inf refused");
}

void test_system_mode_write() {
    std::printf("system_mode write\n");
    check(frame_is(write("system_mode", str("heat")),
                   {0x10, 0x00, 0x02, 0x1C, 0x00, 0x30, 0x04}), "heat -> writeAttributes 0x001C enum8 4");
    Value u{}; u.type = ValueType::Uint; u.u = 0;
    check(frame_is(write("system_mode", u), {0x10, 0x00, 0x02, 0x1C, 0x00, 0x30, 0x00}), "0 -> off");
    check(!write("system_mode", str("boost")).ok, "unknown mode refused");
}

void test_report_round_trip() {
    std::printf("reported setpoint written back unchanged\n");
    for (const std::int16_t raw : {std::int16_t{500}, std::int16_t{2005}, std::int16_t{2155},
                                   std::int16_t{-1234}, std::int16_t{3000}}) {
        const auto r = report_s16(0x0012, raw);
        const Value* v = r.merged.find("current_heating_setpoint");
        check(v && v->type == ValueType::Float, "setpoint decoded as Float");
        if (!v) continue;
        const auto w = write("current_heating_setpoint", *v);
        const auto u = static_cast<std::uint16_t>(raw);
        check(w.ok && w.frame.size() == 8 && w.frame[6] == (u & 0xFF) && w.frame[7] == (u >> 8),
              "decode -> encode gives the same wire value");
    }
}

void test_limit_write_takes_celsius() {
    std::printf("setpoint limit write takes °C\n");
    check(frame_is(write("min_heat_setpoint_limit", f32(15.0f)),
                   {0x10, 0x00, 0x02, 0x15, 0x00, 0x29, 0xDC, 0x05}), "15 °C -> 1500");
    check(!write("min_heat_setpoint_limit", i64(1500)).ok, "1500 °C refused");
}

// Vendor decoders that used to copy the raw contract, without a parity test
// of their own.
void test_eurotronic() {
    std::printf("eurotronic SPZB0001\n");
    const auto& def = devices::eurotronic::kDef_SPZB0001;
    auto r = report(def, 0x0000, 0x29, 2150);
    check(is_celsius(r, "local_temperature", 21.5f), "local_temperature 21.5 °C");
    r = report(def, 0x4003, 0x29, 2050, 0x1037);   // currentHeatingSetpoint, manu
    check(is_celsius(r, "current_heating_setpoint", 20.5f), "0x4003 setpoint 2050 -> 20.5 °C");
    // Its 0x4003 writer: z2m rounds to 0.5 °C, then x100; raw values refused.
    const TzConverter& tz = eurotronic::kTzEurotronicCurrentSetpoint;
    std::uint8_t f[16]{};
    const std::span<std::uint8_t> fs(f);
    std::size_t n = 0;
    check(tz.fn("current_heating_setpoint", f32(21.3f), tz, def, test_ctx(), fs, n) && n == 10 &&
              f[0] == 0x14 && f[5] == 0x03 && f[6] == 0x40 && f[8] == 0x66 && f[9] == 0x08,
          "21.3 °C -> 0x4003 = 2150 (0.5 °C grid)");
    check(!tz.fn("current_heating_setpoint", i64(2150), tz, def, test_ctx(), fs, n), "2150 °C refused");
    check(!tz.fn("current_heating_setpoint", f32(std::numeric_limits<float>::quiet_NaN()), tz, def,
                 test_ctx(), fs, n), "NaN refused");
}

void test_elko() {
    std::printf("elko 4523430\n");
    const auto& def = devices::elko::kDef_D4523430;
    auto r = report(def, 0x0409, 0x29, 2350);   // elkoExternalTemp
    check(is_celsius(r, "floor_temp", 23.5f), "floor_temp 2350 -> 23.5 °C");
    r = report(def, 0x0417, 0x28, -15);         // elkoCalibration, z2m Math.round(v / 10)
    const Value* c = r.merged.find("local_temperature_calibration");
    check(c && c->type == ValueType::Int && c->i == -1, "calibration -15 -> -1 °C (z2m rounding)");
    r = report(def, 0x0417, 0x28, 25);
    c = r.merged.find("local_temperature_calibration");
    check(c && c->type == ValueType::Int && c->i == 3, "calibration 25 -> 3 °C");
    std::uint8_t frame[32]{};
    auto w = dispatch_to_zigbee(def, "local_temperature_calibration", f32(-1.5f), test_ctx(), frame);
    check(w.ok && w.frame_size == 7 && frame[3] == 0x17 && frame[4] == 0x04 && frame[6] == 0xF1,
          "calibration -1.5 °C -> elkoCalibration -15");
    check(!dispatch_to_zigbee(def, "local_temperature_calibration", i64(13), test_ctx(), frame).ok,
          "13 °C (130) does not fit the int8");
}

}  // namespace

int main() {
    test_reports_publish_celsius();
    test_setpoint_write_takes_celsius();
    test_system_mode_write();
    test_report_round_trip();
    test_limit_write_takes_celsius();
    test_eurotronic();
    test_elko();
    if (g_failures) { std::printf("%d failure(s)\n", g_failures); return 1; }
    std::printf("all thermostat unit tests passed\n");
    return 0;
}
