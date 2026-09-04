// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// z2m v26.103.0 → v26.105.0 (zigbee2mqtt 2.14.1) parity window. Pins what
// was hand-written this window:
//
//   * ZG-IR01 — dp109 temperature is reported in the unit dp111 selects;
//     a Fahrenheit reading converts back to Celsius, remembered per device.
//   * kTuyaDpFlagNumericLookup — z2m lookups over plain numbers ride a
//     Numeric datapoint; decode and encode both sides.
//   * New devices: Lincukoo R12LM-Z20T / B08LRT-Z10T / PZE2612 (and the
//     graduated R12LM-Z10T), Tuya TS0601_6gang_switch_2, Aqara ZNMHLDJ01LM,
//     Nova Digital ZCMR-1, Siemens RDZ101ZB (new vendor, running_state).
//   * TS0301_cover_1 graduation carries dp13 battery.
//
// The phaseVariant2WithPhase revert lives in zhc_tuya_packed_dp_tests and
// the manufacturer-twin matcher fallback in zhc_definition_runtime_tests.
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>
#include "definitions/tuya/_shared.hpp"
#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tuya         { extern const PreparedDefinition kDef_ZG_IR01;
                                       extern const PreparedDefinition kDef_TS0301_cover_1;
                                       extern const PreparedDefinition kDef_TS0601_6gang_switch_2; }
namespace zhc::devices::lincukoo     { extern const PreparedDefinition kDef_R12LM_Z20T;
                                       extern const PreparedDefinition kDef_R12LM_Z10T;
                                       extern const PreparedDefinition kDef_B08LRT_Z10T;
                                       extern const PreparedDefinition kDef_PZE2612; }
namespace zhc::devices::lumi         { extern const PreparedDefinition kDefZNMHLDJ01LM; }
namespace zhc::devices::nova_digital { extern const PreparedDefinition kDef_ZCMR_1; }
namespace zhc::devices::siemens      { extern const PreparedDefinition kDef_RDZ101ZB; }

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) { std::printf("  FAIL: %s\n", what); ++g_failures; }
}
bool approx(float a, float b, float eps = 0.01f) { return std::fabs(a - b) <= eps; }

// ── Tuya datapoint path ─────────────────────────────────────────────
const tuya::TuyaDatapointMap* map_of(const PreparedDefinition& def) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i) {
        const FzConverter* c = def.from_zigbee[i];
        if (c && c->family == FrameFamily::TuyaDp && c->user_config)
            return static_cast<const tuya::TuyaDatapointMap*>(c->user_config);
    }
    return nullptr;
}
bool run_dp(const PreparedDefinition& def, const TuyaDpRecord& rec,
            RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* map = map_of(def);
    if (!map) return false;
    FzConverter cvt = tuya::kFzTuyaDatapoints;
    cvt.user_config = map;
    DecodedMessage msg{};
    return tuya::fz_tuya_datapoints(std::span<const TuyaDpRecord>(&rec, 1), msg, cvt, def, ctx, out);
}
bool dp_bool(const PreparedDefinition& def, std::uint8_t dp, bool v, RuntimeContext& ctx,
             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::uint8_t b[] = { static_cast<std::uint8_t>(v ? 1 : 0) };
    return run_dp(def, {dp, 0x01, std::span<const std::uint8_t>(b, 1)}, ctx, out);
}
bool dp_num(const PreparedDefinition& def, std::uint8_t dp, std::int32_t v, RuntimeContext& ctx,
            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::uint32_t u = static_cast<std::uint32_t>(v);
    const std::uint8_t b[] = { std::uint8_t(u >> 24), std::uint8_t(u >> 16), std::uint8_t(u >> 8), std::uint8_t(u) };
    return run_dp(def, {dp, 0x02, std::span<const std::uint8_t>(b, 4)}, ctx, out);
}
bool dp_enum(const PreparedDefinition& def, std::uint8_t dp, std::uint8_t v, RuntimeContext& ctx,
             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::uint8_t b[] = { v };
    return run_dp(def, {dp, 0x04, std::span<const std::uint8_t>(b, 1)}, ctx, out);
}

float float_of(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& p, const char* key) {
    const Value* v = p.find(key);
    if (!v) return -1e9f;
    if (v->type == ValueType::Float) return v->f;
    if (v->type == ValueType::Int)   return static_cast<float>(v->i);
    if (v->type == ValueType::Uint)  return static_cast<float>(v->u);
    return -1e9f;
}
bool str_is(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& p, const char* key, const char* want) {
    const Value* v = p.find(key);
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, want) == 0;
}
bool bool_is(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& p, const char* key, bool want) {
    const Value* v = p.find(key);
    return v && v->type == ValueType::Bool && v->b == want;
}

// ── ZCL path ────────────────────────────────────────────────────────
DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster_id,
                        std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    InboundApsFrame raw{};
    raw.cluster_id   = cluster_id;
    raw.src_endpoint = src_ep;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = bytes;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}
struct Report {
    std::vector<std::uint8_t> v{0x18, 0x01, 0x0A};
    Report& u8(std::uint16_t attr, std::uint8_t type, std::uint8_t val) {
        v.insert(v.end(), {std::uint8_t(attr & 0xFF), std::uint8_t(attr >> 8), type, val}); return *this; }
    Report& u16(std::uint16_t attr, std::uint8_t type, std::uint16_t val) {
        v.insert(v.end(), {std::uint8_t(attr & 0xFF), std::uint8_t(attr >> 8), type,
                           std::uint8_t(val & 0xFF), std::uint8_t(val >> 8)}); return *this; }
};
bool r_str(const DispatchResult& r, const char* key, const char* want) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, want) == 0;
}
bool r_float(const DispatchResult& r, const char* key, float want) {
    const Value* v = r.merged.find(key);
    if (!v) return false;
    const float f = v->type == ValueType::Float ? v->f
                  : v->type == ValueType::Int   ? static_cast<float>(v->i)
                  : v->type == ValueType::Uint  ? static_cast<float>(v->u) : -1e9f;
    return approx(f, want);
}
bool has_manu(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.manufacturer_names_count; ++i)
        if (d.manufacturer_names[i] && std::strcmp(d.manufacturer_names[i], name) == 0) return true;
    return false;
}
bool has_expose(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.exposes_count; ++i)
        if (std::strcmp(d.exposes[i].name, name) == 0) return true;
    return false;
}

// ── tests ───────────────────────────────────────────────────────────

void test_zg_ir01_display_unit() {
    std::printf("ZG-IR01 temperature follows the display unit, per device\n");
    const auto& def = devices::tuya::kDef_ZG_IR01;
    RuntimeStore<2> store{};
    RuntimeContext ctx{};
    ctx.store     = &store;
    ctx.store_get = &RuntimeStore<2>::get;
    ctx.device_index = 0;

    // No unit report yet: the reading is taken as Celsius.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; check(dp_num(def, 109, 225, ctx, o), "dp109 decodes");
      check(approx(float_of(o, "temperature"), 22.5f), "22.5 as Celsius before any dp111"); }
    // Device switches its display to Fahrenheit ...
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; check(dp_enum(def, 111, 1, ctx, o), "dp111 decodes");
      check(str_is(o, "temperature_unit", "fahrenheit"), "temperature_unit fahrenheit"); }
    // ... and 72.5 F now arrives on dp109 -> 22.5 C.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; check(dp_num(def, 109, 725, ctx, o), "dp109 decodes (F)");
      check(approx(float_of(o, "temperature"), 22.5f), "72.5 F converted to 22.5 C"); }
    // A second device on the same hub is unaffected.
    { RuntimeContext c1 = ctx; c1.device_index = 1;
      FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; check(dp_num(def, 109, 725, c1, o), "dp109 decodes (dev 1)");
      check(approx(float_of(o, "temperature"), 72.5f), "device 1 still Celsius (72.5)"); }
    // Back to Celsius.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(def, 111, 0, ctx, o);
      check(str_is(o, "temperature_unit", "celsius"), "temperature_unit celsius");
      FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o2{}; dp_num(def, 109, 225, ctx, o2);
      check(approx(float_of(o2, "temperature"), 22.5f), "22.5 C again"); }
    // Without a store (host default) the reading stays Celsius and never crashes.
    { RuntimeContext bare{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{};
      check(dp_num(def, 109, 300, bare, o), "dp109 decodes without a store");
      check(approx(float_of(o, "temperature"), 30.0f), "30.0 without a store"); }
    check(has_expose(def, "temperature"), "temperature exposed");
}

void test_ts0301_cover_1() {
    std::printf("TS0301_cover_1 graduation carries battery\n");
    const auto& def = devices::tuya::kDef_TS0301_cover_1;
    RuntimeContext ctx{};
    check(has_manu(def, "_TZE210_m6lwazh9"), "fingerprint kept");
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; check(dp_num(def, 13, 77, ctx, o), "dp13 decodes");
      check(approx(float_of(o, "battery"), 77.f), "battery 77"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(def, 1, 2, ctx, o); check(str_is(o, "state", "CLOSE"), "state CLOSE"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(def, 3, 40, ctx, o); check(approx(float_of(o, "position"), 40.f), "position via dp3"); }
    check(has_expose(def, "battery"), "battery exposed");
}

void test_6gang_switch_2() {
    std::printf("TS0601_6gang_switch_2\n");
    const auto& def = devices::tuya::kDef_TS0601_6gang_switch_2;
    RuntimeContext ctx{};
    check(has_manu(def, "_TZE284_hbxadcl0"), "fingerprint");
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_bool(def, 136, true, ctx, o); check(bool_is(o, "state_all", true), "state_all ON"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_bool(def, 4, false, ctx, o); check(bool_is(o, "state_button_4", false), "state_button_4 OFF"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(def, 22, 1234, ctx, o); check(approx(float_of(o, "power"), 123.4f), "power /10"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(def, 21, 1500, ctx, o); check(approx(float_of(o, "current"), 1.5f), "current /1000"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(def, 20, 12345, ctx, o); check(approx(float_of(o, "energy"), 123.45f), "energy /100"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(def, 30, 2, ctx, o); check(str_is(o, "power_on_behavior_2", "previous"), "power_on_behavior_2 previous"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_bool(def, 109, true, ctx, o); check(bool_is(o, "indicator", true), "indicator"); }
    // Write: state_button_3 -> dp3 Bool 1.
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Bool; v.b = true;
    auto r = dispatch_to_zigbee(def, "state_button_3", v, ctx, frame);
    check(r.ok && r.cluster_id == 0xEF00, "state_button_3 routed to the Tuya cluster");
    check(r.frame_size == 10 && frame[5] == 3 && frame[6] == 0x01 && frame[8] == 1 && frame[9] == 1,
          "dp3 bool payload");
    check(!has_expose(def, "color_button_1"), "dp107 indicator colour deliberately not exposed");
}

void test_lincukoo() {
    std::printf("Lincukoo R12LM-Z20T / B08LRT-Z10T / PZE2612 / R12LM-Z10T\n");
    RuntimeContext ctx{};
    const auto& z20 = devices::lincukoo::kDef_R12LM_Z20T;
    check(has_manu(z20, "_TZE284_h7oalfxa") && has_manu(z20, "_TZE2841000000_h7oalfxa"), "Z20T both names");
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_bool(z20, 1, false, ctx, o); check(bool_is(o, "presence", true), "presence is trueFalse0"); }
    // scan_result is a lookup over a NUMERIC datapoint (plain-number lookup upstream).
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; check(dp_num(z20, 8, 2, ctx, o), "numeric dp8 decodes");
      check(str_is(o, "scan_result", "scan_success"), "scan_result scan_success"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; check(!dp_num(z20, 8, 9, ctx, o), "unmapped scan_result abstains"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(z20, 106, 450, ctx, o); check(approx(float_of(o, "set_detection_distance"), 4.5f), "distance /100"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(z20, 108, 3, ctx, o); check(str_is(o, "battery_state", "USB"), "battery_state USB"); }
    // Encode side of the numeric lookup: scan_environment is an enum, scan_result a numeric.
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::StringRef; v.str = "start";
    auto r = dispatch_to_zigbee(z20, "scan_environment", v, ctx, frame);
    check(r.ok && frame[5] == 105 && frame[6] == 0x04 && frame[9] == 0, "scan_environment start -> enum 0");
    v.str = "scanning";
    r = dispatch_to_zigbee(z20, "scan_result", v, ctx, frame);
    check(r.ok && frame[5] == 8 && frame[6] == 0x02 && frame[8] == 4 && frame[12] == 1, "scan_result scanning -> numeric 1");
    v.str = "bogus";
    r = dispatch_to_zigbee(z20, "scan_result", v, ctx, frame);
    check(!r.ok, "unknown label refused");

    const auto& b08 = devices::lincukoo::kDef_B08LRT_Z10T;
    check(has_manu(b08, "_TZE2841000000_cipobaav"), "B08LRT twin name");
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(b08, 104, 1, ctx, o);
      check(str_is(o, "action", "double_click") && str_is(o, "scene_switch", "double_click"), "dp104 fans to action + scene_switch"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(b08, 22, 231, ctx, o); check(approx(float_of(o, "temperature"), 23.1f), "temperature /10"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(b08, 26, 1, ctx, o); check(str_is(o, "temp_unit", "f"), "temp_unit f"); }

    const auto& plug = devices::lincukoo::kDef_PZE2612;
    check(plug.zigbee_models_count == 1 && std::strcmp(plug.zigbee_models[0], "CK-BL702-MSW-01(7011)") == 0, "PZE2612 modelID literal");
    check(plug.endpoint_map_count == 2 && has_expose(plug, "state_2"), "PZE2612 two sockets");

    const auto& z10 = devices::lincukoo::kDef_R12LM_Z10T;
    check(map_of(z10) != nullptr, "R12LM-Z10T now ships a datapoint map");
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(z10, 107, 3, ctx, o); check(approx(float_of(o, "radar_sensitivity"), 3.f), "Z10T radar_sensitivity"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(z10, 102, 1, ctx, o); check(str_is(o, "work_mode", "combine_mode"), "Z10T work_mode"); }
}

void test_znmhldj01lm() {
    std::printf("Aqara ZNMHLDJ01LM lift + tilt\n");
    const auto& def = devices::lumi::kDefZNMHLDJ01LM;
    Report rep; rep.u8(0x0008, 0x20, 60).u8(0x0009, 0x20, 30);
    auto r = dispatch(def, 0x0102, 1, rep.v);
    check(r.any_matched, "windowCovering report matched");
    check(r_float(r, "position", 60.f), "position 60 (passed through, device is coverInverted)");
    check(r_float(r, "tilt", 30.f), "tilt 30");
    RuntimeContext ctx{}; std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Uint; v.u = 40;
    auto w = dispatch_to_zigbee(def, "tilt", v, ctx, frame);
    check(w.ok && w.cluster_id == 0x0102 && w.command_id == 0x08, "tilt -> goToTiltPercentage");
    v.type = ValueType::StringRef; v.str = "STOP";
    w = dispatch_to_zigbee(def, "state", v, ctx, frame);
    // The result's command_id is the converter's declared id; the frame
    // carries the real cluster-specific command in byte 2.
    check(w.ok && w.cluster_id == 0x0102 && w.frame_size >= 3 && frame[2] == 0x02, "state STOP -> cmd 0x02");
}

void test_zcmr_1() {
    std::printf("Nova Digital ZCMR-1\n");
    const auto& def = devices::nova_digital::kDef_ZCMR_1;
    RuntimeContext ctx{};
    check(has_manu(def, "_TZE204_dqy15zxy"), "fingerprint");
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(def, 1, 2, ctx, o); check(str_is(o, "state", "CLOSE"), "state CLOSE"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(def, 3, 55, ctx, o); check(approx(float_of(o, "position"), 55.f), "position via dp3"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(def, 11, 1, ctx, o); check(str_is(o, "situation_set", "fully_close"), "situation_set"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(def, 10, 12000, ctx, o); check(approx(float_of(o, "travel_time"), 12000.f), "travel_time raw"); }
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::StringRef; v.str = "down";
    auto r = dispatch_to_zigbee(def, "border", v, ctx, frame);
    check(r.ok && frame[5] == 16 && frame[6] == 0x04 && frame[9] == 1, "border down -> dp16 enum 1");
}

void test_rdz101zb() {
    std::printf("Siemens RDZ101ZB thermostat\n");
    const auto& def = devices::siemens::kDef_RDZ101ZB;
    check(def.zigbee_models_count == 1 && std::strcmp(def.zigbee_models[0], "RDZ101") == 0, "modelID RDZ101");
    Report rep;
    rep.u16(0x0000, 0x29, 2150)   // localTemperature 21.50
       .u16(0x0012, 0x29, 2000)   // occupiedHeatingSetpoint 20.00
       .u8 (0x001C, 0x30, 4)      // systemMode heat
       .u16(0x0029, 0x19, 0x0001);// runningState: heat bit
    auto r = dispatch(def, 0x0201, 1, rep.v);
    check(r.any_matched, "thermostat report matched");
    // The generic thermostat decoder publishes the raw ZCL values (x100
    // temperatures, the systemMode enum number); scaling and naming happen
    // in the JSON layer, as for every other hvacThermostat port.
    check(r_float(r, "local_temperature", 2150.f), "local_temperature raw 2150 (21.50)");
    check(r_float(r, "current_heating_setpoint", 2000.f), "current_heating_setpoint raw 2000 (20.00)");
    check(r_float(r, "system_mode", 4.f), "system_mode 4 (heat)");
    check(r_str(r, "running_state", "heat"), "running_state heat");
    Report idle; idle.u16(0x0029, 0x19, 0x0000);
    r = dispatch(def, 0x0201, 1, idle.v);
    check(r_str(r, "running_state", "idle"), "running_state idle");
    Report bat; bat.u8(0x0021, 0x20, 200);
    r = dispatch(def, 0x0001, 1, bat.v);
    check(r_float(r, "battery", 100.f), "battery 100 %");

    RuntimeContext ctx{}; std::uint8_t frame[48]{};
    Value v{}; v.type = ValueType::Float; v.f = 21.5f;
    auto w = dispatch_to_zigbee(def, "current_heating_setpoint", v, ctx, frame);
    check(w.ok && w.cluster_id == 0x0201 && w.command_id == 0x02, "setpoint -> thermostat write");
    bool found = false;
    for (std::size_t i = 0; i + 4 < w.frame_size; ++i)
        if (frame[i] == 0x12 && frame[i+1] == 0x00 && frame[i+2] == 0x29 && frame[i+3] == 0x66 && frame[i+4] == 0x08) found = true;
    check(found, "setpoint 21.5 -> attr 0x0012 s16 2150");
    v.type = ValueType::StringRef; v.str = "off";
    w = dispatch_to_zigbee(def, "system_mode", v, ctx, frame);
    check(w.ok && w.cluster_id == 0x0201, "system_mode off writes");
    check(def.reports_count == 5 && def.bindings_count == 2, "reports + bindings declared");
    check(has_expose(def, "running_state"), "running_state exposed");
}

}  // namespace

int main() {
    test_zg_ir01_display_unit();
    test_ts0301_cover_1();
    test_6gang_switch_2();
    test_lincukoo();
    test_znmhldj01lm();
    test_zcmr_1();
    test_rdz101zb();
    if (g_failures) { std::printf("%d failure(s)\n", g_failures); return 1; }
    std::printf("all parity-26105 tests passed\n");
    return 0;
}
