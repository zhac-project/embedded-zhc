// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// z2m v26.105.0 → v26.115.1 (zigbee2mqtt 2.14.2) parity window. Pins what
// was hand-written this window. The phaseVariant2WithPhase 24-bit reads live
// in zhc_tuya_packed_dp_tests.
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <string>
#include <vector>
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "zhc/cluster_names.hpp"
#include "zhc/devices/immax_registry.hpp"
#include "zhc/devices/qa_registry.hpp"
#include "zhc/devices/tier_e_registries.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tplink      { extern const PreparedDefinition kDef_MS100; }
namespace zhc::devices::shinasystem { extern const PreparedDefinition kDef_USM_300ZB; }
namespace zhc::devices::wmun        { extern const PreparedDefinition kDef_ZS05; }
namespace zhc::devices::tuya        { extern const PreparedDefinition kDef_TS0601_cover_5;
                                      extern const PreparedDefinition kDef_TS0601_cover_5_uqfph8ah;
                                      extern const PreparedDefinition kDef_TS0601_cover_5_waa352qv;
                                      extern const PreparedDefinition kDef_TS0601_cover_switch_2;
                                      extern const PreparedDefinition kDef_TS0601_cover_with_1_switch;
                                      extern const PreparedDefinition kDef_TS0601_cover_with_2_switch;
                                      extern const PreparedDefinition kDef_TS0601_cover_13;
                                      extern const PreparedDefinition kDef_TS0601_cover_14;
                                      extern const PreparedDefinition kDef_TZE200_mlglxwp3;
                                      extern const PreparedDefinition kDefTS0601_cover;
                                      extern const PreparedDefinition kDef_WSER40;
                                      extern const PreparedDefinition kDef_TS0601_heat_meter;
                                      extern const PreparedDefinition kDef_TS0601_fan_5_levels_and_light_switch;
                                      extern const PreparedDefinition kDef_ZHT_002;
                                      extern const PreparedDefinition kDef_Tervix_6kijc7nd; }
namespace zhc::devices::moes        { extern const PreparedDefinition kDef_ZS_SR_EUC_cover;
                                      extern const PreparedDefinition kDef_SFD02_Z; }
namespace zhc::devices::zemismart   { extern const PreparedDefinition kDef_ZN_USC1U_HT;
                                      extern const PreparedDefinition kDef_ZM25RX_08_30;
                                      extern const PreparedDefinition kDef_ZMS_206US_4; }
namespace zhc::devices::lumi        { extern const PreparedDefinition kDefJYGZ01AQ; }
namespace zhc::devices::profalux    { extern const PreparedDefinition kDef_MOT_C1ZxxC_F; }
namespace zhc::devices::purmo       { extern const PreparedDefinition kDef_Yali_Parada_Plus; }
namespace zhc::devices::bituo_technik { extern const PreparedDefinition kDef_SDM02_U01; }
namespace zhc::devices::philips     { extern const PreparedDefinition kDef_D929004610402; }
namespace zhc::devices::tuya        { extern const PreparedDefinition kDef_TS0601_wsek35um;
                                      extern const PreparedDefinition kDef_ZY_N1; }
namespace zhc::devices::avatto      { extern const PreparedDefinition kDef_ZSD20; }
namespace zhc::devices::qa          { extern const PreparedDefinition kDef_QADZ1LR; }

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) { std::printf("  FAIL: %s\n", what); ++g_failures; }
}
bool approx(float a, float b, float eps = 0.01f) { return std::fabs(a - b) <= eps; }

// ── Tuya datapoint path ─────────────────────────────────────────────
[[maybe_unused]] const tuya::TuyaDatapointMap* map_of(const PreparedDefinition& def) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i) {
        const FzConverter* c = def.from_zigbee[i];
        if (c && c->family == FrameFamily::TuyaDp && c->user_config)
            return static_cast<const tuya::TuyaDatapointMap*>(c->user_config);
    }
    return nullptr;
}
[[maybe_unused]] bool run_dp(const PreparedDefinition& def, const TuyaDpRecord& rec,
            RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* map = map_of(def);
    if (!map) return false;
    FzConverter cvt = tuya::kFzTuyaDatapoints;
    cvt.user_config = map;
    DecodedMessage msg{};
    return tuya::fz_tuya_datapoints(std::span<const TuyaDpRecord>(&rec, 1), msg, cvt, def, ctx, out);
}
[[maybe_unused]] bool dp_bool(const PreparedDefinition& def, std::uint8_t dp, bool v, RuntimeContext& ctx,
             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::uint8_t b[] = { static_cast<std::uint8_t>(v ? 1 : 0) };
    return run_dp(def, {dp, 0x01, std::span<const std::uint8_t>(b, 1)}, ctx, out);
}
[[maybe_unused]] bool dp_num(const PreparedDefinition& def, std::uint8_t dp, std::int32_t v, RuntimeContext& ctx,
            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::uint32_t u = static_cast<std::uint32_t>(v);
    const std::uint8_t b[] = { std::uint8_t(u >> 24), std::uint8_t(u >> 16), std::uint8_t(u >> 8), std::uint8_t(u) };
    return run_dp(def, {dp, 0x02, std::span<const std::uint8_t>(b, 4)}, ctx, out);
}
[[maybe_unused]] bool dp_enum(const PreparedDefinition& def, std::uint8_t dp, std::uint8_t v, RuntimeContext& ctx,
             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::uint8_t b[] = { v };
    return run_dp(def, {dp, 0x04, std::span<const std::uint8_t>(b, 1)}, ctx, out);
}
[[maybe_unused]] bool dp_raw(const PreparedDefinition& def, std::uint8_t dp, std::span<const std::uint8_t> b,
            RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return run_dp(def, {dp, 0x00, b}, ctx, out);
}

[[maybe_unused]] float float_of(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& p, const char* key) {
    const Value* v = p.find(key);
    if (!v) return -1e9f;
    if (v->type == ValueType::Float) return v->f;
    if (v->type == ValueType::Int)   return static_cast<float>(v->i);
    if (v->type == ValueType::Uint)  return static_cast<float>(v->u);
    return -1e9f;
}
[[maybe_unused]] bool str_is(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& p, const char* key, const char* want) {
    const Value* v = p.find(key);
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, want) == 0;
}
[[maybe_unused]] bool bool_is(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& p, const char* key, bool want) {
    const Value* v = p.find(key);
    return v && v->type == ValueType::Bool && v->b == want;
}

// Outbound: encode one key through the definition's to_zigbee list.
struct Encoded { bool ok = false; std::uint16_t cluster = 0; std::uint8_t ep = 0; std::vector<std::uint8_t> frame; };
[[maybe_unused]] Encoded encode(const PreparedDefinition& def, const char* key, const Value& v) {
    Encoded e{};
    RuntimeContext ctx{};
    std::uint8_t buf[96] = {};
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* c = def.to_zigbee[i];
        if (!c || (c->key && std::strcmp(c->key, key) != 0)) continue;   // null key = wildcard
        std::size_t n = 0;
        if (c->fn(key, v, *c, def, ctx, std::span<std::uint8_t>(buf, sizeof(buf)), n) && n) {
            e.ok = true; e.cluster = c->cluster_id; e.ep = c->endpoint;
            e.frame.assign(buf, buf + n);
            return e;
        }
    }
    return e;
}
[[maybe_unused]] Value num(double d)          { Value v{}; v.type = ValueType::Float; v.f = static_cast<float>(d); return v; }
[[maybe_unused]] Value str(const char* s)     { Value v{}; v.type = ValueType::StringRef; v.str = s; return v; }
[[maybe_unused]] Value boolean(bool b)        { Value v{}; v.type = ValueType::Bool; v.b = b; return v; }

// ── ZCL path ────────────────────────────────────────────────────────
[[maybe_unused]] DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster_id,
                        std::uint8_t src_ep, std::span<const std::uint8_t> bytes,
                        RuntimeContext* ctx_in = nullptr) {
    InboundApsFrame raw{};
    raw.cluster_id   = cluster_id;
    raw.src_endpoint = src_ep;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = bytes;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx_in ? *ctx_in : ctx);
}
// Attribute report builder: frame control 0x18 (server->client, no default
// response), or 0x1C with a manufacturer code.
struct Report {
    std::vector<std::uint8_t> v;
    Report() : v{0x18, 0x01, 0x0A} {}
    explicit Report(std::uint16_t manu) : v{0x1C, std::uint8_t(manu & 0xFF), std::uint8_t(manu >> 8), 0x01, 0x0A} {}
    Report& u8(std::uint16_t attr, std::uint8_t type, std::uint8_t val) {
        v.insert(v.end(), {std::uint8_t(attr & 0xFF), std::uint8_t(attr >> 8), type, val}); return *this; }
    Report& u16(std::uint16_t attr, std::uint8_t type, std::uint16_t val) {
        v.insert(v.end(), {std::uint8_t(attr & 0xFF), std::uint8_t(attr >> 8), type,
                           std::uint8_t(val & 0xFF), std::uint8_t(val >> 8)}); return *this; }
    Report& u32(std::uint16_t attr, std::uint8_t type, std::uint32_t val) {
        v.insert(v.end(), {std::uint8_t(attr & 0xFF), std::uint8_t(attr >> 8), type,
                           std::uint8_t(val), std::uint8_t(val >> 8), std::uint8_t(val >> 16), std::uint8_t(val >> 24)}); return *this; }
    std::span<const std::uint8_t> span() const { return std::span<const std::uint8_t>(v.data(), v.size()); }
};
[[maybe_unused]] const Value* r_find(const DispatchResult& r, const char* key) { return r.merged.find(key); }
[[maybe_unused]] bool r_str(const DispatchResult& r, const char* key, const char* want) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, want) == 0;
}
[[maybe_unused]] bool r_bool(const DispatchResult& r, const char* key, bool want) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Bool && v->b == want;
}
[[maybe_unused]] float r_float(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    if (!v) return -1e9f;
    if (v->type == ValueType::Float) return v->f;
    if (v->type == ValueType::Int)   return static_cast<float>(v->i);
    if (v->type == ValueType::Uint)  return static_cast<float>(v->u);
    return -1e9f;
}
[[maybe_unused]] bool has_manu(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.manufacturer_names_count; ++i)
        if (d.manufacturer_names[i] && std::strcmp(d.manufacturer_names[i], name) == 0) return true;
    return false;
}
[[maybe_unused]] bool has_model(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.zigbee_models_count; ++i)
        if (d.zigbee_models[i] && std::strcmp(d.zigbee_models[i], name) == 0) return true;
    return false;
}
[[maybe_unused]] bool has_expose(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.exposes_count; ++i)
        if (std::strcmp(d.exposes[i].name, name) == 0) return true;
    return false;
}

// ── tests ───────────────────────────────────────────────────────────

void test_illuminance_lux() {
    std::printf("generic illuminance publishes lux, 0 stays 0\n");
    const auto& def = devices::tplink::kDef_MS100;   // z2m m.illuminance()
    { Report r; r.u16(0x0000, 0x21, 30001);
      auto d = dispatch(def, 0x0400, 1, r.span());
      check(approx(r_float(d, "illuminance"), 1000.0f, 0.1f), "30001 -> 1000 lx"); }
    { Report r; r.u16(0x0000, 0x21, 0);
      auto d = dispatch(def, 0x0400, 1, r.span());
      const Value* v = r_find(d, "illuminance");
      check(v && approx(r_float(d, "illuminance"), 0.0f, 0.0001f), "0 -> 0 lx (too low to measure)"); }
    { Report r; r.u16(0x0000, 0x21, 1);
      auto d = dispatch(def, 0x0400, 1, r.span());
      check(approx(r_float(d, "illuminance"), 1.0f, 0.0001f), "1 -> 1 lx"); }
    // ShinaSystem USM-300ZB is `m.illuminance({scale: (value) => value})`.
    { Report r; r.u16(0x0000, 0x21, 345);
      auto d = dispatch(devices::shinasystem::kDef_USM_300ZB, 0x0400, 1, r.span());
      check(approx(r_float(d, "illuminance"), 345.0f), "USM-300ZB stays unscaled (345)"); }
}

void test_attr_map_and_write_multiplier() {
    std::printf("data-driven attribute decoder and write multiplier\n");
    static constexpr generic::ZclWriteLookup kOnOff[] = { {"OFF", 0}, {"ON", 1} };
    static constexpr generic::ZclAttrRow kRows[] = {
        { 0x0340, "vpd", 100 },
        { 0x0020, "overheating", 1, kOnOff, 2 },
        { 0x0021, "flag", 1, nullptr, 0, generic::kZclAttrFlagBool },
        { 0x0022, "plain" },
    };
    static constexpr generic::ZclAttrMap kMap{ kRows, 4 };
    static constexpr FzConverter kFz = generic::zcl_attr_fz("msTemperatureMeasurement", &kMap);
    static const FzConverter* const kFzList[] = { &kFz };
    PreparedDefinition def{};
    def.from_zigbee = kFzList; def.from_zigbee_count = 1;
    Report r; r.u16(0x0340, 0x29, 0xFF38 /* -200 */).u8(0x0020, 0x10, 1).u8(0x0021, 0x20, 0).u16(0x0022, 0x21, 1234);
    auto d = dispatch(def, 0x0402, 1, r.span());
    check(approx(r_float(d, "vpd"), -2.0f), "s16 -200 / 100 = -2.00");
    check(r_str(d, "overheating", "ON"), "bool 1 -> ON via lookup");
    check(r_bool(d, "flag", false), "u8 0 -> false");
    check(approx(r_float(d, "plain"), 1234.0f), "plain value passes through");

    // Manufacturer-gated map ignores frames without the code.
    static constexpr generic::ZclAttrMap kGated{ kRows, 4, 0x1286 };
    static constexpr FzConverter kFzG = generic::zcl_attr_fz(nullptr, &kGated);
    static const FzConverter* const kFzListG[] = { &kFzG };
    PreparedDefinition g{};
    g.from_zigbee = kFzListG; g.from_zigbee_count = 1;
    auto dn = dispatch(g, 0xFC11, 1, r.span());
    check(r_find(dn, "plain") == nullptr, "no manufacturer code -> ignored");
    Report rm(0x1286); rm.u16(0x0022, 0x21, 77);
    auto dm = dispatch(g, 0xFC11, 1, rm.span());
    check(approx(r_float(dm, "plain"), 77.0f), "matching manufacturer code -> decoded");

    // Write multiplier: z2m `scale: 10` writes value * 10.
    static constexpr generic::ZclWriteSpec kSpec{ "calib", 0x8001, 0x29, 0x128B, nullptr, 0, 0, 10 };
    static constexpr TzConverter kTz = generic::zcl_write_tz("nodonIrExtender", 0xFC82, &kSpec);
    static const TzConverter* const kTzList[] = { &kTz };
    PreparedDefinition w{};
    w.to_zigbee = kTzList; w.to_zigbee_count = 1;
    auto e = encode(w, "calib", num(-1.5));
    check(e.ok && e.frame.size() == 10, "manufacturer-specific write frame");
    if (e.ok && e.frame.size() == 10) {
        check(e.frame[0] == 0x14 && e.frame[1] == 0x8B && e.frame[2] == 0x12, "fc + manu code");
        check(e.frame[5] == 0x01 && e.frame[6] == 0x80 && e.frame[7] == 0x29, "attr 0x8001 int16");
        check(e.frame[8] == 0xF1 && e.frame[9] == 0xFF, "-1.5 * 10 = -15 on the wire");
    }
}


void test_zosung_learn_stop() {
    std::printf("zosung learn_ir_code OFF stops learning\n");
    const auto& def = devices::wmun::kDef_ZS05;
    auto body = [](const Encoded& e) {
        return e.frame.size() > 4 ? std::string(e.frame.begin() + 4, e.frame.end()) : std::string();
    };
    auto on  = encode(def, "learn_ir_code", str("ON"));
    auto off = encode(def, "learn_ir_code", str("OFF"));
    auto f   = encode(def, "learn_ir_code", boolean(false));
    check(on.ok && body(on) == "{\"study\":0}", "ON starts learning (study 0)");
    check(off.ok && body(off) == "{\"study\":1}", "OFF stops learning (study 1)");
    check(f.ok && body(f) == "{\"study\":1}", "false stops learning (study 1)");
}

void test_tuya_covers() {
    std::printf("Tuya covers: motor_direction, cover_12/13/14, WSER40, cover_with_1_switch\n");
    using namespace devices::tuya;
    RuntimeContext ctx{};
    // #13207: DP8 motor_steering -> motor_direction {normal, reversed}.
    for (const PreparedDefinition* d : { &kDef_TS0601_cover_5, &kDef_TS0601_cover_5_uqfph8ah,
                                         &kDef_TS0601_cover_5_waa352qv, &kDef_TS0601_cover_switch_2,
                                         &kDef_TS0601_cover_with_1_switch, &kDef_TS0601_cover_with_2_switch }) {
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{};
        check(dp_enum(*d, 8, 1, ctx, o), d->model);
        check(str_is(o, "motor_direction", "reversed"), "DP8 1 -> motor_direction reversed");
        check(o.find("motor_steering") == nullptr, "no motor_steering any more");
        check(has_expose(*d, "motor_direction") && !has_expose(*d, "motor_steering"), "motor_direction exposed");
        auto e = encode(*d, "motor_direction", str("normal"));
        check(e.ok, "motor_direction writable");
    }
    check(has_manu(kDef_TS0601_cover_5, "_TZE284_pxwixtky"), "cover_5 + _TZE284_pxwixtky (#13261)");
    check(has_manu(kDef_TS0601_cover_5, "_TZE204_p6vz3wzt"), "cover_5 lists _TZE204_p6vz3wzt");
    // _TZE284_waa352qv reports DP1 in another order.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(kDef_TS0601_cover_5_waa352qv, 1, 0, ctx, o);
      check(str_is(o, "state", "STOP"), "waa352qv DP1 0 = STOP"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(kDef_TS0601_cover_5, 1, 0, ctx, o);
      check(str_is(o, "state", "OPEN"), "cover_5 DP1 0 = OPEN"); }
    check(has_expose(kDef_TS0601_cover_5_uqfph8ah, "quick_calibration") &&
          !has_expose(kDef_TS0601_cover_5_uqfph8ah, "child_lock"), "BSEED variant exposes quick_calibration");
    // _TZE200_jhkttplm is a curtain switch (DP1 cover action), not a contact sensor.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(kDef_TS0601_cover_with_1_switch, 1, 2, ctx, o);
      check(str_is(o, "state", "CLOSE") && o.find("contact") == nullptr, "jhkttplm DP1 is the cover action"); }
    // TS0601_cover_12 family: motor_state with stopped; variant-only datapoints.
    for (const PreparedDefinition* d : { &kDef_TZE200_mlglxwp3, &kDef_TS0601_cover_13, &kDef_TS0601_cover_14 }) {
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{};
        dp_enum(*d, 7, 2, ctx, o);
        check(str_is(o, "motor_state", "stopped") && o.find("work_state") == nullptr, "DP7 motor_state stopped");
    }
    check(std::strcmp(kDef_TZE200_mlglxwp3.model, "TS0601_cover_12") == 0, "mlglxwp3 is TS0601_cover_12");
    check(has_manu(kDef_TS0601_cover_14, "_TZE284_a0hirjnh"), "cover_14 fingerprint (#13295)");
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(kDef_TS0601_cover_14, 19, 40, ctx, o);
      check(approx(float_of(o, "favorite_position"), 40.f), "cover_14 DP19 favorite_position"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_bool(kDef_TS0601_cover_13, 6, true, ctx, o);
      check(bool_is(o, "auto_power", true), "cover_13 DP6 auto_power boolean"); }
    // WSER40 off the legacy layout; cover_2 + ZMS1-TYZ manufacturers.
    check(!has_manu(kDefTS0601_cover, "_TZE200_pk0sfzvr") && has_manu(kDef_WSER40, "_TZE200_pk0sfzvr"), "pk0sfzvr -> WSER40");
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(kDef_WSER40, 103, 55, ctx, o);
      check(approx(float_of(o, "position"), 55.f), "WSER40 DP103 position"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(kDef_WSER40, 1, 1, ctx, o);
      check(str_is(o, "state", "CLOSE"), "WSER40 DP1 1 = CLOSE"); }
    check(has_manu(kDefTS0601_cover, "_TZE200_fu14oapz"), "TS0601_cover_2 + _TZE200_fu14oapz (#13208)");
    check(has_manu(kDefTS0601_cover, "_TZE284_zuq5xxib"), "ZMS1-TYZ + _TZE284_zuq5xxib (#13222)");
}

void test_tuya_thermostats_meters() {
    std::printf("Tuya heat meter, fan, ZHT-002, Tervix, ZS-SR-EUC, ZN-USC1U-HT\n");
    using namespace devices::tuya;
    RuntimeContext ctx{};
    // #13184: DP7 is the metering switch, DP8 the cumulative heat.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_bool(kDef_TS0601_heat_meter, 7, true, ctx, o);
      check(bool_is(o, "prepayment_switch", true) && o.find("cumulative_heat") == nullptr, "DP7 prepayment_switch"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(kDef_TS0601_heat_meter, 8, 12345, ctx, o);
      check(approx(float_of(o, "cumulative_heat"), 123.45f), "DP8 cumulative_heat /100"); }
    { const std::uint8_t b[] = {0,0,0,0, 0,0,0x30,0x39};   // bytes 4..7 = 12345 -> 12.345 m3
      FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_raw(kDef_TS0601_heat_meter, 2, b, ctx, o);
      check(approx(float_of(o, "monthly_water_consumption"), 12.345f, 0.001f), "DP2 waterConsumption bytes 4..7 / 1000"); }
    { const std::uint8_t b[] = {0,0,0x04,0xD2};             // 1234 -> 1.234 m3/h
      FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_raw(kDef_TS0601_heat_meter, 19, b, ctx, o);
      check(approx(float_of(o, "instantaneous_flow_rate"), 1.234f, 0.001f), "DP19 flow rate / 1000"); }
    { const std::uint8_t b[] = {0x05};                      // bits 0 + 2
      FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; run_dp(kDef_TS0601_heat_meter, {5, 0x05, std::span<const std::uint8_t>(b, 1)}, ctx, o);
      check(str_is(o, "fault", "battery_alarm, cover_alarm"), "DP5 fault names joined with \", \""); }
    { const std::uint8_t b[] = {0x00};
      FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; run_dp(kDef_TS0601_heat_meter, {5, 0x05, std::span<const std::uint8_t>(b, 1)}, ctx, o);
      check(str_is(o, "fault", "OK"), "DP5 zero fault -> OK"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(kDef_TS0601_heat_meter, 24, 360, ctx, o);
      check(approx(float_of(o, "voltage"), 3600.f), "DP24 voltage x10"); }
    // #13151: fan speed key.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(kDef_TS0601_fan_5_levels_and_light_switch, 3, 2, ctx, o);
      check(str_is(o, "speed", "3") && o.find("fan_speed") == nullptr, "DP3 speed '3'"); }
    check(has_manu(kDef_TS0601_fan_5_levels_and_light_switch, "_TZE204_lawxy9e2"), "fan lists _TZE204_lawxy9e2");
    // ZHT-002: DP47 running_state, DP1 system_mode next to state.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(kDef_ZHT_002, 47, 1, ctx, o);
      check(str_is(o, "running_state", "heat") && o.find("valve_state") == nullptr, "ZHT-002 DP47 running_state heat"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_bool(kDef_ZHT_002, 1, false, ctx, o);
      check(str_is(o, "system_mode", "off") && bool_is(o, "state", false), "ZHT-002 DP1 -> state + system_mode off"); }
    // #13329: Tervix DP1 is a boolean system_mode.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_bool(kDef_Tervix_6kijc7nd, 1, true, ctx, o);
      check(str_is(o, "system_mode", "heat"), "Tervix DP1 true -> heat"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(kDef_Tervix_6kijc7nd, 58, 2, ctx, o);
      check(str_is(o, "run_mode", "cool_mode"), "Tervix DP58 numeric lookup"); }
    { auto e = encode(kDef_Tervix_6kijc7nd, "system_mode", str("heat"));
      check(e.ok, "Tervix system_mode writable"); }
    // Moes ZS-SR-EUC / Zemismart ZN-USC1U-HT: motor_direction, overshoot clamp.
    for (const PreparedDefinition* d : { &devices::moes::kDef_ZS_SR_EUC_cover, &devices::zemismart::kDef_ZN_USC1U_HT }) {
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(*d, 8, 1, ctx, o);
        check(str_is(o, "motor_direction", "reversed"), "DP8 motor_direction reversed");
    }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(devices::moes::kDef_ZS_SR_EUC_cover, 2, 254, ctx, o);
      check(approx(float_of(o, "position"), 0.f), "ZS-SR-EUC 254 -> 0 (closed overshoot)"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(devices::moes::kDef_ZS_SR_EUC_cover, 2, 102, ctx, o);
      check(approx(float_of(o, "position"), 100.f), "ZS-SR-EUC 102 -> 100 (open overshoot)"); }
}

// IAS Zone Status Change Notification (cmd 0x00, cluster-specific, server->client).
std::vector<std::uint8_t> ias_notif(std::uint16_t status) {
    return {0x09, 0x10, 0x00, std::uint8_t(status & 0xFF), std::uint8_t(status >> 8), 0x00, 0x01, 0x00, 0x00};
}

void test_misc_fixes() {
    std::printf("JY-GZ-01AQ smoke, MOT-C1Z temperature, Yali keypad lock, SDM02 phases, ZM25RX, detects\n");
    RuntimeContext ctx{};
    // #13338: smoke via IAS zone.
    { auto f = ias_notif(0x0001);
      auto d = dispatch(devices::lumi::kDefJYGZ01AQ, 0x0500, 1, std::span<const std::uint8_t>(f.data(), f.size()));
      check(r_bool(d, "smoke", true), "JY-GZ-01AQ IAS bit0 -> smoke"); }
    check(has_expose(devices::lumi::kDefJYGZ01AQ, "smoke"), "JY-GZ-01AQ exposes smoke");
    // #13136: device temperature on endpoint 2.
    { Report r; r.u16(0x0000, 0x29, 23);
      auto d = dispatch(devices::profalux::kDef_MOT_C1ZxxC_F, 0x0002, 2, r.span());
      check(approx(r_float(d, "device_temperature"), 23.f), "MOT-C1Z device_temperature 23"); }
    // #13247: keypad lockout decode + write.
    { Report r; r.u8(0x0001, 0x30, 1);
      auto d = dispatch(devices::purmo::kDef_Yali_Parada_Plus, 0x0204, 1, r.span());
      check(r_str(d, "keypad_lockout", "lock1"), "Yali keypad_lockout lock1"); }
    { auto e = encode(devices::purmo::kDef_Yali_Parada_Plus, "keypad_lockout", str("lock2"));
      check(e.ok && e.cluster == 0x0204 && e.frame.size() == 7 && e.frame[3] == 0x01 && e.frame[5] == 0x30 && e.frame[6] == 2,
            "Yali keypad_lockout write enum8 2"); }
    // #13115: per-phase energy exposed.
    check(has_expose(devices::bituo_technik::kDef_SDM02_U01, "energy_phase_a") &&
          has_expose(devices::bituo_technik::kDef_SDM02_U01, "produced_energy_phase_b"), "SDM02-U01 phase energy exposed");
    // #13293: ZM25RX motor_state stopped; DP1 action; motor_direction 'reversed'.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(devices::zemismart::kDef_ZM25RX_08_30, 7, 2, ctx, o);
      check(str_is(o, "motor_state", "stopped"), "ZM25RX DP7 stopped"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(devices::zemismart::kDef_ZM25RX_08_30, 5, 1, ctx, o);
      check(str_is(o, "motor_direction", "reversed"), "ZM25RX DP5 reversed (was 'reverse')"); }
    { auto e = encode(devices::zemismart::kDef_ZM25RX_08_30, "state", str("CLOSE"));
      check(e.ok, "ZM25RX state writable"); }
    // Detects.
    check(has_manu(devices::zemismart::kDef_ZMS_206US_4, "_TZE28C1000000_pmbxyf97"), "ZMS-206US-4 + pmbxyf97 (#13241)");
    check(has_manu(devices::moes::kDef_SFD02_Z, "_TZE284_z98viqa6"), "SFD02-Z + z98viqa6 (#13280)");
    check(has_model(devices::philips::kDef_D929004610402, "929004610603"), "929004610402 + 929004610603 (#13313)");
}

// New devices: resolved through the same registries the adapter walks
// (Tuya, Immax, QA and the tier-E vendors), then one datapoint each where
// the mapping is not a plain numeric.
void test_new_devices() {
    std::printf("New devices resolve; ZY-N1, wsek35um, ZSD20, QADZ1LR decode\n");
    std::vector<const PreparedDefinition*> reg;
    auto add = [&](const PreparedDefinition* const* r, std::size_t n) { reg.insert(reg.end(), r, r + n); };
    add(devices::tuya::kTuyaRegistry, devices::tuya::kTuyaRegistryCount);
    add(devices::immax::kImmaxRegistry, devices::immax::kImmaxRegistryCount);
    add(devices::qa::kQaRegistry, devices::qa::kQaRegistryCount);
    for (std::size_t i = 0; i < devices::tier_e::kTierERegistriesCount; ++i)
        add(devices::tier_e::kTierERegistries[i].reg, devices::tier_e::kTierERegistries[i].count);
    const std::span<const PreparedDefinition* const> all(reg.data(), reg.size());
    struct Want { const char* zm; const char* manu; const char* model; };
    const Want wants[] = {
        {"TS0601", "_TZE284_1oft6qso", "CH8Z"},
        {"TS0601", "_TZE284_16m4bgsv", "1443ZK"},
        {"TS011F", "_TZ3218_fv20refe", "ZOT60"},
        {"TS0601", "_TZE284_zeeqkb0p", "ZSD20"},
        {"TS0601", "_TZE284_rhocfd6y", "07519L"},
        {"TS0601", "_TZE200_4jvmbiph", "MG-AU03"},
        {"TS0601", "_TZE200_lq0ffndf", "MG-GPO02Z"},
        {"TS0601", "_TZE284_mexuq6lm", "MW836P"},
        {"TS0601", "_TZE28C1000000_brx4eku5", "QADZ1LR"},
        {"TS0601", "_TZE204_eaasry7v", "AE-5503-S-H-ZIGBEE"},
        {"TS0601", "_TZE284_5qfrnbqs", "CTL-Mini-DTP-TYZ/AC"},
        {"TS0202", "_TZD200_sjjp9bti", "HS208Z"},
        {"HS208Z", "HYSYIOT", "HS208Z"},
        {"TS0601", "_TZE20C_tjz9ad5g", "MG-BJQ002"},
        {"TS0601", "_TZE284_rfpyqax9", "Pro Line X10"},
        {"TS0601", "_TZE20C1000000_p3g8xiug", "TS0601_3ch_bidirectional_meter"},
        {"TS0601", "_TZE204_dak2k10o", "TS0601_air_quality_sensor_2"},
        {"TS0601", "_TZE204_pxbjch8m", "TS0601_cover_with_1_switch_limited"},
        {"TS0601", "_TZE204_wsek35um", "TS0601_wsek35um"},
        {"TS0601", "_TZE284_grxx6qek", "_TZE284_grxx6qek"},
        {"TS0601", "_TZE20C_ycab9txf", "ZAS-01P"},
        {"ZG-308Z", "HOBEIAN", "ZG-308Z"},
        {"TS0601", "_TZE204_r6kfl9ta", "ZY-N1"},
        {"TS0601", "_TZE204_6ewjlefg", "BVRF-L001"},
    };
    for (const Want& w : wants) {
        const PreparedDefinition* d = find_definition(w.zm, w.manu, all);
        if (!d || std::strcmp(d->model, w.model) != 0) {
            std::printf("  %s/%s -> %s (want %s)\n", w.zm, w.manu, d ? d->model : "none", w.model);
            check(false, "new device resolves");
        }
    }
    RuntimeContext ctx{};
    // ZY-N1: DP101 is noise_state, and noise_detected for 0/2/3.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(devices::tuya::kDef_ZY_N1, 101, 2, ctx, o);
      check(str_is(o, "noise_state", "noise_2min") && bool_is(o, "noise_detected", true), "ZY-N1 DP101 2 -> noise_2min, detected"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_enum(devices::tuya::kDef_ZY_N1, 101, 1, ctx, o);
      check(str_is(o, "noise_state", "no_noise") && bool_is(o, "noise_detected", false), "ZY-N1 DP101 1 -> no_noise"); }
    // wsek35um: mode Off reads as a 5 degree setpoint; other modes leave it alone.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(devices::tuya::kDef_TS0601_wsek35um, 2, 0, ctx, o);
      check(str_is(o, "mode", "Off") && approx(float_of(o, "current_heating_setpoint"), 5.f), "wsek35um DP2 Off -> setpoint 5"); }
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(devices::tuya::kDef_TS0601_wsek35um, 2, 3, ctx, o);
      check(str_is(o, "mode", "Day") && o.find("current_heating_setpoint") == nullptr, "wsek35um DP2 Day -> no setpoint"); }
    // ZSD20: zero fault bitmap reads "No faults".
    { const std::uint8_t b[] = {0x00};
      FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; run_dp(devices::avatto::kDef_ZSD20, {11, 0x05, std::span<const std::uint8_t>(b, 1)}, ctx, o);
      check(str_is(o, "fault", "No faults"), "ZSD20 DP11 0 -> No faults"); }
    // QADZ1LR: brightness 0..1000 on the wire, 0..254 exposed.
    { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; dp_num(devices::qa::kDef_QADZ1LR, 2, 1000, ctx, o);
      check(approx(float_of(o, "brightness"), 254.f, 0.5f), "QADZ1LR DP2 1000 -> 254"); }
}

}  // namespace

int main() {
    std::printf("== z2m parity window v26.105.0 -> v26.115.1 ==\n");
    test_illuminance_lux();
    test_attr_map_and_write_multiplier();
    test_zosung_learn_stop();
    test_tuya_covers();
    test_tuya_thermostats_meters();
    test_misc_fixes();
    test_new_devices();
    if (g_failures) { std::printf("FAILED: %d check(s)\n", g_failures); return 1; }
    std::printf("all parity-26115 checks passed\n");
    return 0;
}
