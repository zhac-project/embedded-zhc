// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// zigbee-herdsman-converters v26.125.0 → v26.133.0 parity window (R10).
// Pins what was hand-written this window; triage in
// zhac-tools/parity/catchup_26125_26133/R10_TRIAGE.md.
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>
#include "definitions/tuya/_shared.hpp"
#include "zhc/devices/adeo_registry.hpp"
#include "zhc/devices/adurosmart_registry.hpp"
#include "zhc/devices/aurora_lighting_registry.hpp"
#include "zhc/devices/bitron_registry.hpp"
#include "zhc/devices/bosch_registry.hpp"
#include "zhc/devices/candeo_registry.hpp"
#include "zhc/devices/develco_registry.hpp"
#include "zhc/devices/efekta_registry.hpp"
#include "zhc/devices/ewelink_registry.hpp"
#include "zhc/devices/feibit_registry.hpp"
#include "zhc/devices/gledopto_registry.hpp"
#include "zhc/devices/heiman_registry.hpp"
#include "zhc/devices/hive_registry.hpp"
#include "zhc/devices/ikea_registry.hpp"
#include "zhc/devices/iluminize_registry.hpp"
#include "zhc/devices/immax_registry.hpp"
#include "zhc/devices/innr_registry.hpp"
#include "zhc/devices/ledvance_registry.hpp"
#include "zhc/devices/legrand_registry.hpp"
#include "zhc/devices/lincukoo_registry.hpp"
#include "zhc/devices/lumi_registry.hpp"
#include "zhc/devices/moes_registry.hpp"
#include "zhc/devices/muller_licht_registry.hpp"
#include "zhc/devices/namron_registry.hpp"
#include "zhc/devices/nue_3a_registry.hpp"
#include "zhc/devices/orvibo_registry.hpp"
#include "zhc/devices/osram_registry.hpp"
#include "zhc/devices/owon_registry.hpp"
#include "zhc/devices/paulmann_registry.hpp"
#include "zhc/devices/philips_registry.hpp"
#include "zhc/devices/qa_registry.hpp"
#include "zhc/devices/robb_registry.hpp"
#include "zhc/devices/saswell_registry.hpp"
#include "zhc/devices/schneider_registry.hpp"
#include "zhc/devices/sengled_registry.hpp"
#include "zhc/devices/shelly_registry.hpp"
#include "zhc/devices/shinasystem_registry.hpp"
#include "zhc/devices/sinope_registry.hpp"
#include "zhc/devices/slacky_diy_registry.hpp"
#include "zhc/devices/smartthings_registry.hpp"
#include "zhc/devices/sonoff_registry.hpp"
#include "zhc/devices/sunricher_registry.hpp"
#include "zhc/devices/sylvania_registry.hpp"
#include "zhc/devices/third_reality_registry.hpp"
#include "zhc/devices/tier_e_registries.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/devices/yale_registry.hpp"
#include "zhc/devices/yokis_registry.hpp"
#include "zhc/devices/zemismart_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"
#include "test_ctx.hpp"

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) { std::printf("  FAIL: %s\n", what); ++g_failures; }
}
bool approx(float a, float b, float eps = 0.01f) { return std::fabs(a - b) <= eps; }

// The registries in zhc_adapter.cpp's order: vendor registries, tier E last.
const std::vector<const PreparedDefinition*>& registries() {
    static std::vector<const PreparedDefinition*> reg;
    if (!reg.empty()) return reg;
    auto add = [&](const PreparedDefinition* const* r, std::size_t n) { reg.insert(reg.end(), r, r + n); };
    add(devices::lumi::kLumiRegistry, devices::lumi::kLumiRegistryCount);
    add(devices::tuya::kTuyaRegistry, devices::tuya::kTuyaRegistryCount);
    add(devices::moes::kMoesRegistry, devices::moes::kMoesRegistryCount);
    add(devices::philips::kPhilipsRegistry, devices::philips::kPhilipsRegistryCount);
    add(devices::sonoff::kSonoffRegistry, devices::sonoff::kSonoffRegistryCount);
    add(devices::heiman::kHeimanRegistry, devices::heiman::kHeimanRegistryCount);
    add(devices::saswell::kSaswellRegistry, devices::saswell::kSaswellRegistryCount);
    add(devices::efekta::kEfektaRegistry, devices::efekta::kEfektaRegistryCount);
    add(devices::schneider::kSchneiderRegistry, devices::schneider::kSchneiderRegistryCount);
    add(devices::ikea::kIkeaRegistry, devices::ikea::kIkeaRegistryCount);
    add(devices::innr::kInnrRegistry, devices::innr::kInnrRegistryCount);
    add(devices::gledopto::kGledoptoRegistry, devices::gledopto::kGledoptoRegistryCount);
    add(devices::sunricher::kSunricherRegistry, devices::sunricher::kSunricherRegistryCount);
    add(devices::namron::kNamronRegistry, devices::namron::kNamronRegistryCount);
    add(devices::ledvance::kLedvanceRegistry, devices::ledvance::kLedvanceRegistryCount);
    add(devices::osram::kOsramRegistry, devices::osram::kOsramRegistryCount);
    add(devices::adeo::kAdeoRegistry, devices::adeo::kAdeoRegistryCount);
    add(devices::legrand::kLegrandRegistry, devices::legrand::kLegrandRegistryCount);
    add(devices::shinasystem::kShinasystemRegistry, devices::shinasystem::kShinasystemRegistryCount);
    add(devices::third_reality::kThirdRealityRegistry, devices::third_reality::kThirdRealityRegistryCount);
    add(devices::robb::kRobbRegistry, devices::robb::kRobbRegistryCount);
    add(devices::paulmann::kPaulmannRegistry, devices::paulmann::kPaulmannRegistryCount);
    add(devices::orvibo::kOrviboRegistry, devices::orvibo::kOrviboRegistryCount);
    add(devices::slacky_diy::kSlackyDiyRegistry, devices::slacky_diy::kSlackyDiyRegistryCount);
    add(devices::nue_3a::kNue3aRegistry, devices::nue_3a::kNue3aRegistryCount);
    add(devices::muller_licht::kMullerLichtRegistry, devices::muller_licht::kMullerLichtRegistryCount);
    add(devices::develco::kDevelcoRegistry, devices::develco::kDevelcoRegistryCount);
    add(devices::iluminize::kIluminizeRegistry, devices::iluminize::kIluminizeRegistryCount);
    add(devices::hive::kHiveRegistry, devices::hive::kHiveRegistryCount);
    add(devices::feibit::kFeibitRegistry, devices::feibit::kFeibitRegistryCount);
    add(devices::yale::kYaleRegistry, devices::yale::kYaleRegistryCount);
    add(devices::sengled::kSengledRegistry, devices::sengled::kSengledRegistryCount);
    add(devices::smartthings::kSmartthingsRegistry, devices::smartthings::kSmartthingsRegistryCount);
    add(devices::yokis::kYokisRegistry, devices::yokis::kYokisRegistryCount);
    add(devices::zemismart::kZemismartRegistry, devices::zemismart::kZemismartRegistryCount);
    add(devices::ewelink::kEwelinkRegistry, devices::ewelink::kEwelinkRegistryCount);
    add(devices::candeo::kCandeoRegistry, devices::candeo::kCandeoRegistryCount);
    add(devices::sylvania::kSylvaniaRegistry, devices::sylvania::kSylvaniaRegistryCount);
    add(devices::sinope::kSinopeRegistry, devices::sinope::kSinopeRegistryCount);
    add(devices::qa::kQaRegistry, devices::qa::kQaRegistryCount);
    add(devices::lincukoo::kLincukooRegistry, devices::lincukoo::kLincukooRegistryCount);
    add(devices::bitron::kBitronRegistry, devices::bitron::kBitronRegistryCount);
    add(devices::aurora_lighting::kAuroraLightingRegistry, devices::aurora_lighting::kAuroraLightingRegistryCount);
    add(devices::immax::kImmaxRegistry, devices::immax::kImmaxRegistryCount);
    add(devices::bosch::kBoschRegistry, devices::bosch::kBoschRegistryCount);
    add(devices::shelly::kShellyRegistry, devices::shelly::kShellyRegistryCount);
    add(devices::owon::kOwonRegistry, devices::owon::kOwonRegistryCount);
    add(devices::adurosmart::kAdurosmartRegistry, devices::adurosmart::kAdurosmartRegistryCount);
    for (std::size_t i = 0; i < devices::tier_e::kTierERegistriesCount; ++i)
        add(devices::tier_e::kTierERegistries[i].reg, devices::tier_e::kTierERegistries[i].count);
    return reg;
}
std::span<const PreparedDefinition* const> all() {
    return std::span<const PreparedDefinition* const>(registries().data(), registries().size());
}

struct Want { const char* zm; const char* manu; const char* model; };
void resolves(const Want& w) {
    const PreparedDefinition* d = find_definition(w.zm, w.manu, all());
    if (!d || std::strcmp(d->model, w.model) != 0) {
        std::printf("  %s/%s -> %s (want %s)\n", w.zm, w.manu, d ? d->model : "none", w.model);
        check(false, "resolves");
    }
}
const PreparedDefinition& def_for(const char* zm, const char* manu) {
    static const PreparedDefinition kNone{};
    const PreparedDefinition* d = find_definition(zm, manu, all());
    if (!d) { std::printf("  %s/%s resolves to nothing\n", zm, manu); check(false, "definition found"); }
    return d ? *d : kNone;
}
bool has_model(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.zigbee_models_count; ++i)
        if (d.zigbee_models[i] && std::strcmp(d.zigbee_models[i], name) == 0) return true;
    return false;
}
bool has_manu(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.manufacturer_names_count; ++i)
        if (d.manufacturer_names[i] && std::strcmp(d.manufacturer_names[i], name) == 0) return true;
    return false;
}
const Expose* find_expose(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.exposes_count; ++i)
        if (std::strcmp(d.exposes[i].name, name) == 0) return &d.exposes[i];
    return nullptr;
}
bool has_expose(const PreparedDefinition& d, const char* name) { return find_expose(d, name) != nullptr; }
bool has_wl(const PreparedDefinition& d, const char* vendor, const char* model) {
    for (std::size_t i = 0; i < d.white_labels_count; ++i)
        if (std::strcmp(d.white_labels[i].vendor, vendor) == 0 && std::strcmp(d.white_labels[i].model, model) == 0)
            return true;
    return false;
}
bool has_binding(const PreparedDefinition& d, std::uint16_t cluster) {
    for (std::size_t i = 0; i < d.bindings_count; ++i)
        if (d.bindings[i].cluster_id == cluster) return true;
    return false;
}
// Registered definitions that list `manu`.
int claims(const char* manu) {
    int n = 0;
    for (const auto* d : registries()) if (d && has_manu(*d, manu)) ++n;
    return n;
}

// ── Tuya datapoint path ─────────────────────────────────────────────
const tuya::TuyaDatapointMap* map_of(const PreparedDefinition& def) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i) {
        const FzConverter* c = def.from_zigbee[i];
        if (c && c->family == FrameFamily::TuyaDp && c->user_config)
            return static_cast<const tuya::TuyaDatapointMap*>(c->user_config);
    }
    return nullptr;
}
using Out = FixedPayload<ZHC_FIXED_PAYLOAD_CAP>;
Out run_dp(const PreparedDefinition& def, const TuyaDpRecord& rec) {
    Out out{};
    const auto* map = map_of(def);
    if (!map) return out;
    FzConverter cvt = tuya::kFzTuyaDatapoints;
    cvt.user_config = map;
    DecodedMessage msg{};
    RuntimeContext ctx{};
    tuya::fz_tuya_datapoints(std::span<const TuyaDpRecord>(&rec, 1), msg, cvt, def, ctx, out);
    return out;
}
Out dp_bool(const PreparedDefinition& def, std::uint8_t dp, bool v) {
    const std::uint8_t b[] = { static_cast<std::uint8_t>(v ? 1 : 0) };
    return run_dp(def, {dp, 0x01, std::span<const std::uint8_t>(b, 1)});
}
Out dp_num(const PreparedDefinition& def, std::uint8_t dp, std::int32_t v) {
    const std::uint32_t u = static_cast<std::uint32_t>(v);
    const std::uint8_t b[] = { std::uint8_t(u >> 24), std::uint8_t(u >> 16), std::uint8_t(u >> 8), std::uint8_t(u) };
    return run_dp(def, {dp, 0x02, std::span<const std::uint8_t>(b, 4)});
}
Out dp_enum(const PreparedDefinition& def, std::uint8_t dp, std::uint8_t v) {
    const std::uint8_t b[] = { v };
    return run_dp(def, {dp, 0x04, std::span<const std::uint8_t>(b, 1)});
}
Out dp_bytes(const PreparedDefinition& def, std::uint8_t dp, std::uint8_t type, std::span<const std::uint8_t> b) {
    return run_dp(def, {dp, type, b});
}
float float_of(const Out& p, const char* key) {
    const Value* v = p.find(key);
    if (!v) return -1e9f;
    if (v->type == ValueType::Float) return v->f;
    if (v->type == ValueType::Int)   return static_cast<float>(v->i);
    if (v->type == ValueType::Uint)  return static_cast<float>(v->u);
    return -1e9f;
}
bool str_is(const Out& p, const char* key, const char* want) {
    const Value* v = p.find(key);
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, want) == 0;
}
bool bool_is(const Out& p, const char* key, bool want) {
    const Value* v = p.find(key);
    return v && v->type == ValueType::Bool && v->b == want;
}

// Outbound: encode one key through the definition's to_zigbee list.
struct Encoded { bool ok = false; std::uint16_t cluster = 0; std::vector<std::uint8_t> frame; };
Encoded encode(const PreparedDefinition& def, const char* key, const Value& v) {
    Encoded e{};
    RuntimeContext ctx{};
    std::uint8_t buf[96] = {};
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* c = def.to_zigbee[i];
        if (!c || (c->key && std::strcmp(c->key, key) != 0)) continue;   // null key = wildcard
        std::size_t n = 0;
        if (c->fn(key, v, *c, def, ctx, std::span<std::uint8_t>(buf, sizeof(buf)), n) && n) {
            e.ok = true; e.cluster = c->cluster_id;
            e.frame.assign(buf, buf + n);
            return e;
        }
    }
    return e;
}
Value num(double d)      { Value v{}; v.type = ValueType::Float; v.f = static_cast<float>(d); return v; }
Value str(const char* s) { Value v{}; v.type = ValueType::StringRef; v.str = s; return v; }
Value boolean(bool b)    { Value v{}; v.type = ValueType::Bool; v.b = b; return v; }
// ZCL writeAttributes frame [fc (manu lo hi) tsn 0x02 attr lo hi type value…].
bool is_write(const Encoded& e, std::uint16_t cluster, std::uint16_t attr, std::uint8_t type, std::uint32_t v,
              std::uint16_t manu = 0) {
    const std::size_t o = manu ? 2 : 0;
    if (!e.ok || e.cluster != cluster || e.frame.size() < 7 + o || e.frame[2 + o] != 0x02) return false;
    if (manu && (e.frame[1] != (manu & 0xFF) || e.frame[2] != (manu >> 8))) return false;
    std::uint32_t got = 0;
    for (std::size_t i = 6 + o; i < e.frame.size(); ++i) got |= std::uint32_t(e.frame[i]) << (8 * (i - 6 - o));
    return e.frame[3 + o] == (attr & 0xFF) && e.frame[4 + o] == (attr >> 8) && e.frame[5 + o] == type && got == v;
}
// Tuya setData frame [fc tsn cmd][seq:2][dp][type][len:2][value].
bool is_dp(const Encoded& e, std::uint8_t dp, std::uint8_t type, std::uint32_t v) {
    if (!e.ok || e.cluster != 0xEF00 || e.frame.size() < 10) return false;
    std::uint32_t got = 0;
    for (std::size_t i = 9; i < e.frame.size(); ++i) got = (got << 8) | e.frame[i];
    return e.frame[5] == dp && e.frame[6] == type && got == v;
}

// ── ZCL path ────────────────────────────────────────────────────────
DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster_id, std::uint8_t src_ep,
                        std::span<const std::uint8_t> bytes) {
    InboundApsFrame raw{};
    raw.cluster_id   = cluster_id;
    raw.src_endpoint = src_ep;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = bytes;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    return dispatch_from_zigbee(msg, {}, def, raw, test_ctx());
}
// Attribute report: frame control 0x18, or 0x1C with a manufacturer code.
struct Report {
    std::vector<std::uint8_t> v;
    Report() : v{0x18, 0x01, 0x0A} {}
    explicit Report(std::uint16_t manu) : v{0x1C, std::uint8_t(manu & 0xFF), std::uint8_t(manu >> 8), 0x01, 0x0A} {}
    Report& u8(std::uint16_t attr, std::uint8_t type, std::uint8_t val) {
        v.insert(v.end(), {std::uint8_t(attr & 0xFF), std::uint8_t(attr >> 8), type, val}); return *this; }
    Report& u16(std::uint16_t attr, std::uint8_t type, std::uint16_t val) {
        v.insert(v.end(), {std::uint8_t(attr & 0xFF), std::uint8_t(attr >> 8), type,
                           std::uint8_t(val & 0xFF), std::uint8_t(val >> 8)}); return *this; }
    std::span<const std::uint8_t> span() const { return std::span<const std::uint8_t>(v.data(), v.size()); }
};
float r_float(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    if (!v) return -1e9f;
    if (v->type == ValueType::Float) return v->f;
    if (v->type == ValueType::Int)   return static_cast<float>(v->i);
    if (v->type == ValueType::Uint)  return static_cast<float>(v->u);
    return -1e9f;
}
bool r_str(const DispatchResult& r, const char* key, const char* want) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, want) == 0;
}
bool r_bool(const DispatchResult& r, const char* key, bool want) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Bool && v->b == want;
}

// ── 1. devices on the wrong def ─────────────────────────────────────

// SP-P05D / SP-P06D: Pass 3 caught `lumi.plug` and gave them ZNCZ02LM.
void sp_p05d() {
    std::printf("SP-P05D / SP-P06D\n");
    resolves({"lumi.plug.aeu005", "LUMI", "SP-P05D"});
    resolves({"lumi.plug.aeu007", "LUMI", "SP-P06D"});
    for (const char* zm : {"lumi.plug.aeu005", "lumi.plug.aeu007"}) {
        const PreparedDefinition& d = def_for(zm, "LUMI");
        Report p(0x115F); p.u8(0x0517, 0x20, 1);
        check(r_str(dispatch(d, 0xFCC0, 1, p.span()), "power_on_behavior", "previous"), "0x0517 1 -> previous");
        check(is_write(encode(d, "power_on_behavior", str("inverted")), 0xFCC0, 0x0517, 0x20, 3, 0x115F),
              "power_on_behavior inverted -> 0x0517 = 3");
        Report l(0x115F); l.u8(0x0203, 0x10, 1);
        check(r_bool(dispatch(d, 0xFCC0, 1, l.span()), "state_led", true), "0x0203 -> state_led");
        check(is_write(encode(d, "state_led", boolean(false)), 0xFCC0, 0x0203, 0x10, 0, 0x115F), "state_led write");
        Report m(0x115F); m.u8(0x02DF, 0x20, 2);
        check(r_str(dispatch(d, 0xFCC0, 1, m.span()), "led_color_mode", "daily_energy"), "0x02DF 2 -> daily_energy");
        check(is_write(encode(d, "led_color_mode", str("power")), 0xFCC0, 0x02DF, 0x20, 1, 0x115F),
              "led_color_mode write");
        check(is_write(encode(d, "child_lock", str("LOCK")), 0xFCC0, 0x0285, 0x20, 1, 0x115F), "child_lock write");
        check(has_expose(d, "power") && has_expose(d, "energy") && !has_expose(d, "device_temperature"),
              "metering, no device temperature");
    }
}

// GWA1221_DIMMER: Pass 3 caught the bare "DIMMER" of two other vendors.
void gwa1221() {
    std::printf("Gewiss GWA1221\n");
    resolves({"GWA1221_DIMMER", "Gewiss S.p.A.", "GWA1221"});
    const PreparedDefinition& d = def_for("GWA1221_DIMMER", "Gewiss S.p.A.");
    Report b; b.u8(0x0000, 0x20, 128);
    check(r_float(dispatch(d, 0x0008, 1, b.span()), "brightness") == 128.0f, "brightness");
    Report c; c.u16(0x0508, 0x21, 1500);
    check(approx(r_float(dispatch(d, 0x0B04, 1, c.span()), "current"), 1.5f), "current /1000");
    Report m; m.u8(0x0011, 0x20, 20);
    check(r_float(dispatch(d, 0x0301, 1, m.span()), "ballast_minimum_level") == 20.0f, "ballast min read");
    check(is_write(encode(d, "ballast_maximum_level", num(200)), 0x0301, 0x0012, 0x20, 200), "ballast max write");
    check(encode(d, "brightness", num(100)).ok && encode(d, "state", str("ON")).ok, "light control");
    check(has_expose(d, "device_temperature"), "device temperature");
}

// _TZE284_z98viqa6 sat on the SFD02-Z switch-style map; upstream moved it.
void sfd02_z_2() {
    std::printf("Moes SFD02-Z_2\n");
    resolves({"TS0601", "_TZE284_z98viqa6", "SFD02-Z_2"});
    resolves({"TS0601", "_TZE284_t88bjhfu", "SFD02-Z"});
    const PreparedDefinition& d = def_for("TS0601", "_TZE284_z98viqa6");
    check(str_is(dp_enum(d, 21, 2), "indicator_status", "pos"), "DP21 2 -> pos");
    check(bool_is(dp_bool(d, 26, true), "backlight_switch", true), "DP26 backlight_switch");
    check(str_is(dp_enum(d, 101, 2), "indicator_light_brightness", "2"), "DP101 -> \"2\"");
    check(str_is(dp_enum(d, 102, 3), "vibration_gear", "3"), "DP102 -> \"3\"");
    check(str_is(dp_num(d, 14, 2), "power_on_behavior", "previous"), "DP14 2 -> previous");
    check(is_dp(encode(d, "power_on_behavior", str("on")), 14, 0x02, 1), "DP14 written as a value DP");
    check(str_is(dp_num(d, 4, 1), "light_type", "incandescent"), "DP4 1 -> incandescent");
    check(approx(float_of(dp_num(d, 2, 1000), "brightness"), 254.0f, 0.5f), "DP2 1000 -> 254");
}

// 3ROR0194Z → 3RRE0194Z, an on/off repeater (no temperature).
void third_reality_repeater() {
    std::printf("Third Reality 3RRE0194Z\n");
    resolves({"3RRE0194Z", "Third Reality, Inc", "3RRE0194Z"});
    resolves({"3ROR0194Z", "Third Reality, Inc", "3RRE0194Z"});
    const PreparedDefinition& d = def_for("3RRE0194Z", "Third Reality, Inc");
    check(has_expose(d, "state") && !has_expose(d, "temperature"), "state, no temperature");
    Report s; s.u8(0x0000, 0x10, 1);
    check(r_bool(dispatch(d, 0x0006, 1, s.span()), "state", true), "genOnOff report");
    check(encode(d, "state", str("OFF")).ok, "state write");
    check(has_binding(d, 0x0006) && !has_binding(d, 0x0402), "binds genOnOff, not temperature");
}

// ── 2. fake Tuya hand defs ──────────────────────────────────────────

void fake_tuya_defs() {
    std::printf("fake Tuya hand defs re-homed\n");
    {   // _TZE200_bcusnqt8 is the SPM01 1P+N meter, not a bed sensor.
        resolves({"TS0601", "_TZE200_bcusnqt8", "SPM01"});
        const PreparedDefinition& d = def_for("TS0601", "_TZE200_bcusnqt8");
        check(approx(float_of(dp_num(d, 1, 12345), "energy"), 123.45f), "DP1 energy /100");
        check(approx(float_of(dp_num(d, 2, 678), "produced_energy"), 6.78f), "DP2 produced_energy /100");
        const std::uint8_t ph[] = {0x08, 0xFC, 0x01, 0x11, 0x70, 0x00, 0x3A, 0x98};   // 230.0 V, 70 A, 15000 W
        const Out o = dp_bytes(d, 6, 0x00, std::span<const std::uint8_t>(ph, sizeof(ph)));
        check(approx(float_of(o, "voltage"), 230.0f) && approx(float_of(o, "current"), 70.0f) &&
              float_of(o, "power") == 15000.0f, "DP6 phaseVariant4, 24-bit current");
        const std::uint8_t f[] = {0x00, 0x00, 0x40, 0x00};
        check(str_is(dp_bytes(d, 9, 0x05, std::span<const std::uint8_t>(f, 4)), "faults", "negative_power"),
              "DP9 bit 14 -> negative_power");
        check(!has_expose(d, "heart_rate") && has_expose(d, "faults"), "meter exposes");
    }
    {   // _TZE200_aqnazj70 is TS0601_switch_4_gang_1, not a scene remote.
        const PreparedDefinition& d = def_for("TS0601", "_TZE200_aqnazj70");
        check(bool_is(dp_bool(d, 4, true), "state_l4", true) && !dp_enum(d, 101, 0).find("action"),
              "aqnazj70 DP4 -> state_l4, no DP101 action");
        check(is_dp(encode(d, "state_l2", boolean(true)), 2, 0x01, 1), "state_l2 write");
    }
    {   // _TZE200_cirvgep4 is a T&H sensor (ZTH08-E), not a flood sensor.
        const PreparedDefinition& d = def_for("TS0601", "_TZE200_cirvgep4");
        check(approx(float_of(dp_num(d, 1, 215), "temperature"), 21.5f), "cirvgep4 DP1 temperature /10");
        check(float_of(dp_num(d, 2, 55), "humidity") == 55.0f && !dp_bool(d, 1, true).find("water_leak"),
              "cirvgep4 DP2 humidity, no water_leak");
    }
}

// ── 3. case-insensitive zigbeeModel pass ────────────────────────────

void model_case() {
    std::printf("case-insensitive model pass\n");
    resolves({"ms01", "eWeLink", "SNZB-03"});
    resolves({"MS01", "eWeLink", "SNZB-03"});
    const PreparedDefinition* m = find_definition_by_model("ms01", all());
    check(m && std::strcmp(m->model, "SNZB-03") == 0, "find_definition_by_model ms01 -> SNZB-03");
    // Every registered zigbeeModel still resolves to a def that lists it in
    // that exact case: the new pass only answers when no exact match exists.
    int bad = 0;
    for (const auto* d : registries()) {
        if (!d || d->manufacturer_name_prefix || d->manufacturer_names_count) continue;
        for (std::size_t i = 0; i < d->zigbee_models_count; ++i) {
            const char* zm = d->zigbee_models[i];
            if (!zm || !*zm) continue;
            const PreparedDefinition* r = find_definition(zm, "x", all());
            const PreparedDefinition* rm = find_definition_by_model(zm, all());
            if (!r || !has_model(*r, zm) || !rm || !has_model(*rm, zm)) {
                if (bad++ < 5) std::printf("  %s -> %s\n", zm, r ? r->model : "none");
            }
        }
    }
    check(bad == 0, "exact-case matches unchanged");
    // A differently-cased pair keeps each spelling on its own def.
    for (const char* zm : {"SLZB-06MG24", "SLZB-06Mg24", "DIMMER", "Dimmer"}) {
        const PreparedDefinition* r = find_definition(zm, "x", all());
        if (!r || !has_model(*r, zm)) { std::printf("  %s\n", zm); check(false, "case pair kept"); }
    }
}

// ── 4. dead controls ────────────────────────────────────────────────

void dead_controls() {
    std::printf("dead controls\n");
    {   // R037: Danfoss Ally 0x4059 is UINT8 upstream now; enum8 writes are rejected.
        const PreparedDefinition& d = def_for("eTRV0103", "Danfoss");
        check(is_write(encode(d, "heating_system_mode", str("boiler_heating")), 0x0201, 0x4059, 0x20, 1, 0x1246),
              "Ally heating_system_mode written as uint8");
    }
    // WP-P09D / HCXDD13LM power_on_behavior wait for W4 (L1 shared spec).
    // TRV601 (owner family) DP1: Raw published 0..3 against string exposes.
    for (const char* manu : {"_TZE204_rtrmfadk", "_TZE204_cvcu2p6e"}) {
        const PreparedDefinition& d = def_for("TS0601", manu);
        struct { std::uint8_t v; const char* sys; const char* preset; } const rows[] = {
            {0, "auto", "auto"}, {1, "heat", "manual"}, {2, "off", "off"}, {3, "heat", "on"} };
        for (const auto& r : rows) {
            const Out o = dp_enum(d, 1, r.v);
            if (!str_is(o, "system_mode", r.sys) || !str_is(o, "preset", r.preset)) {
                std::printf("  %s DP1 %u\n", manu, r.v); check(false, "TRV601 DP1 decode");
            }
        }
        check(is_dp(encode(d, "system_mode", str("auto")), 1, 0x04, 0) &&
              is_dp(encode(d, "system_mode", str("heat")), 1, 0x04, 1) &&
              is_dp(encode(d, "system_mode", str("off")), 1, 0x04, 2), "TRV601 system_mode toMap");
        check(is_dp(encode(d, "preset", str("manual")), 1, 0x04, 1) &&
              is_dp(encode(d, "preset", str("on")), 1, 0x04, 3), "TRV601 preset toMap");
    }
}

// ── 5. registry ─────────────────────────────────────────────────────

void registry_fixes() {
    std::printf("registry\n");
    resolves({"FNB54-WTS08ML1.0", "_TZ3000_x", "TS0207"});
    resolves({"AY222Z", "AOYAN", "TS0207"});
    resolves({"HS222Z", "HYSYIOT", "TS0207"});
    check(claims("_TZE284_1di7ujzp") == 1, "_TZE284_1di7ujzp registered once");
    for (const char* manu : {"_TZE284_1di7ujzp", "_TZE284_pfbtmgo1"}) {
        const PreparedDefinition& d = def_for("TS0601", manu);
        check(bool_is(dp_bool(d, 1, true), "water", true), "E13 DP1 water");
    }
    check(has_wl(def_for("TS0601", "_TZE284_pfbtmgo1"), "Sygonix", "W08-Z10T"), "Sygonix W08-Z10T");
}

// ── 6. SONOFF SNZB-02D / 02DR2 ──────────────────────────────────────

void snzb_02d() {
    std::printf("SNZB-02D / 02DR2 temperature + humidity\n");
    for (const char* zm : {"SNZB-02D", "SNZB-02DR2"}) {
        const PreparedDefinition& d = def_for(zm, "SONOFF");
        Report t; t.u16(0x0000, 0x29, 2150);
        check(approx(r_float(dispatch(d, 0x0402, 1, t.span()), "temperature"), 21.5f), "temperature");
        Report h; h.u16(0x0000, 0x21, 5500);
        check(approx(r_float(dispatch(d, 0x0405, 1, h.span()), "humidity"), 55.0f), "humidity");
        check(has_expose(d, "temperature") && has_expose(d, "humidity") &&
              has_binding(d, 0x0402) && has_binding(d, 0x0405), "exposes + binds");
    }
}

// ── 7. detections and one-liners ────────────────────────────────────

void detections() {
    std::printf("detections\n");
    resolves({"929004610902", "Signify Netherlands B.V.", "929004611002"});
    check(has_wl(def_for("929004611002", "Signify Netherlands B.V."), "Philips", "929004610902"), "Hue Flux 5 m WL");
    resolves({"M1P-E", "HEIMAN", "M1-PE"});
    resolves({"TS0601", "_TZE28C1000000_uqfph8ah", "BSEED_TS0601_cover"});
    resolves({"D726-ZG", "HZC", "4512788"});
    resolves({"Ysia 5 Zigbee Europe", "Somfy", "1871154"});
    check(has_wl(def_for("Ysia 5 HP Zigbee", "Somfy"), "Somfy", "1871158"), "Somfy 1871158 WL");
    resolves({"teelicht-nrf", "teelicht", "teelicht"});
    resolves({"3RCB1097Z", "Third Reality, Inc", "3RCB01057Z"});
    resolves({"3RCB1095Z", "Third Reality, Inc", "3RCB01057Z"});
    check(has_wl(def_for("3RCB01057Z", "Third Reality, Inc"), "Third Reality", "3RCB1097Z"), "3RCB1097Z WL");
    // R013 HYSYIOT batch, plus the bare zigbeeModels of the HOBEIAN originals.
    const Want hysy[] = {
        {"TS0601", "_TZD200_5ziwcyoe", "ZG-204ZX"}, {"HS204ZX", "HYSYIOT", "ZG-204ZX"}, {"ZG-204ZX", "HOBEIAN", "ZG-204ZX"},
        {"TS0601", "_TZD200_7khnomgh", "ZG-303Z"},  {"HS303Z", "HYSYIOT", "ZG-303Z"},
        {"TS0202", "_TZD200_vfh3tuo1", "ZG-204ZV"}, {"HS204ZV", "HYSYIOT", "ZG-204ZV"}, {"ZG-204ZV", "HOBEIAN", "ZG-204ZV"},
        {"TS0601", "_TZD200_gfng0whq", "ZG-204ZL"}, {"HS204ZL", "HYSYIOT", "ZG-204ZL"}, {"ZG-204ZL", "HOBEIAN", "ZG-204ZL"},
        {"HS308Z", "HYSYIOT", "ZG-308Z"},           {"TS0601", "_TZD200_y0rvjm11", "ZG-308Z"},
        {"TS0601", "_TZD200_gt1gge3x", "ZG-223Z"},
        {"HS807Z", "HS807Z", "CK-BL702-ROUTER-01(7018)"},
    };
    for (const auto& w : hysy) resolves(w);
    check(has_wl(def_for("TS0601", "_TZD200_5ziwcyoe"), "HYSYIOT", "HS204ZX") &&
          has_wl(def_for("TS0601", "_TZD200_7khnomgh"), "HYSYIOT", "HS303Z") &&
          has_wl(def_for("TS0202", "_TZD200_vfh3tuo1"), "HYSYIOT", "HS204ZV") &&
          has_wl(def_for("TS0601", "_TZD200_gfng0whq"), "HYSYIOT", "HS204ZL") &&
          has_wl(def_for("TS0601", "_TZD200_y0rvjm11"), "HYSYIOT", "HS308Z") &&
          has_wl(def_for("HS807Z", "HS807Z"), "HYSYIOT", "HS807Z"), "HYSYIOT white labels");
    check(has_wl(def_for("ESMLFzm_w6_Dimm", "AwoX"), "EGLO", "12224") &&
          has_wl(def_for("ESMLFzm_w6_Dimm", "AwoX"), "EGLO", "12226"), "AwoX 12226 EGLO white labels");
    check(has_wl(def_for("ESMLFzm_w6_TW", "EGLO"), "EGLO", "12245"), "EGLO 12245 white label");
    {
        const Expose* ct = find_expose(def_for("ZBEK-11", "ADEO"), "color_temp");
        check(ct && ct->value_min == 153 && ct->value_max == 370, "ADEO GU-10 colour temperature 153-370");
    }
    {   // R030: PP01 declares dontDividePercentage; kFzBattery halved it.
        const PreparedDefinition& d = def_for("TERNCY-PP01", "Xiaoyan");
        Report b; b.u8(0x0021, 0x20, 90);
        check(r_float(dispatch(d, 0x0001, 1, b.span()), "battery") == 90.0f, "PP01 battery not halved");
    }
    for (const char* manu : {"_TZE200_aoclfnxz", "_TZE200_5toc8efa"}) {   // R165, owner BHT-002
        const Expose* e = find_expose(def_for("TS0601", manu), "min_temperature_limit");
        check(e && e->value_min == 1 && e->value_max == 10, "BHT-002 min_temperature_limit 1-10");
    }
}

}  // namespace

int main() {
    std::printf("== z2m parity window v26.125.0 -> v26.133.0 ==\n");
    sp_p05d();
    gwa1221();
    sfd02_z_2();
    third_reality_repeater();
    fake_tuya_defs();
    model_case();
    dead_controls();
    registry_fixes();
    snzb_02d();
    detections();
    if (g_failures) { std::printf("FAILED: %d check(s)\n", g_failures); return 1; }
    std::printf("all parity-26133 checks passed\n");
    return 0;
}
