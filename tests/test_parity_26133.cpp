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
#include <initializer_list>
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

// ════════════════════════════════════════════════════════════════════
// Wave 2 (Tuya): triage_tuya.md ports on existing defs + W1-ledgered items.
// ════════════════════════════════════════════════════════════════════

std::span<const std::uint8_t> bytes(std::initializer_list<std::uint8_t> l) {
    static std::vector<std::uint8_t> keep[16];
    static int slot = 0;
    auto& v = keep[slot++ & 15];
    v.assign(l.begin(), l.end());
    return std::span<const std::uint8_t>(v.data(), v.size());
}

// C8 / W01 / D03: TS0601_switch_6_gang.
void w2_switch_6_gang() {
    std::printf("W2 TS0601_switch_6_gang\n");
    for (const char* manu : {"_TZE200_mwvfvw8g", "_TZE200_wnp4d4va", "_TZE200_cduqh1l0", "_TZE204_cduqh1l0",
                             "_TZE200_emxxanvi", "_TZE204_g4au0afs", "_TZE204_w1wwxoja", "_TZE204_lmgrbuwf",
                             "_TZE204_wskr3up8", "_TZE204_gxbdnfrh", "_TZE284_g1enhdsi", "_TZE284_znkkcauq",
                             "_TZE284_1iii6x0y", "_TZE284_r731zlxk"}) {
        const PreparedDefinition& d = def_for("TS0601", manu);
        if (!str_is(dp_enum(d, 14, 2), "power_on_behavior", "previous") ||
            !is_dp(encode(d, "power_on_behavior", str("previous")), 14, 0x04, 2)) {
            std::printf("  %s\n", manu); check(false, "DP14 powerOnBehaviorEnum 2 <-> previous");
        }
    }
    const PreparedDefinition& z = def_for("TS0601", "_TZE284_znkkcauq");
    check(&def_for("TS0601", "_TZE284_1iii6x0y") == &z && &def_for("TS0601", "_TZE284_r731zlxk") == &z,
          "1iii6x0y / r731zlxk on the switch_6_gang def");
    check(has_wl(z, "JWCOM", "SA-ZB417F-BR"), "JWCOM SA-ZB417F-BR white label");
    const PreparedDefinition& g = def_for("TS0601", "_TZE204_gxbdnfrh");
    check(float_of(dp_num(g, 7, 30), "countdown_l1") == 30.0f && float_of(dp_num(g, 12, 43200), "countdown_l6") == 43200.0f,
          "gxbdnfrh DP7 / DP12 -> countdown_l1 / countdown_l6 raw s");
    check(is_dp(encode(g, "countdown_l3", num(60)), 9, 0x02, 60), "countdown_l3 written to DP9");
    const Expose* c = find_expose(g, "countdown_l4");
    check(c && c->value_max == 43200 && c->unit && std::strcmp(c->unit, "s") == 0, "countdown_l4 expose 0-43200 s");
}

// F06 / X14 / D06 / D10 / D18 / X04 / X11: small rows.
void w2_small_rows() {
    std::printf("W2 small Tuya rows\n");
    {   // F06: ZG-109TDS DP1 is `temperature_probe` (a reading).
        const PreparedDefinition& d = def_for("Excellux", "DTS1XM9");
        check(approx(float_of(dp_num(d, 1, 215), "temperature_probe"), 21.5f) && !dp_num(d, 1, 215).find("probe_temperature"),
              "ZG-109TDS DP1 -> temperature_probe /10");
        const Expose* e = find_expose(d, "temperature_probe");
        check(e && e->access == Access::State && !has_expose(d, "probe_temperature"), "temperature_probe expose State");
    }
    // X14: MiBoxer E3-ZR joins TS0505B_1's moveToLevelWithOnOffDisable list.
    check(&def_for("TS0505B", "_TZB210_yatkpuha") == &def_for("TS0505B", "_TZB210_uoiqhjqe"),
          "_TZB210_yatkpuha on the moveToLevel TS0505B def");
    {   // D06: ZTH13 on TS0601_temperature_humidity_sensor_2.
        const PreparedDefinition& d = def_for("TS0601", "_TZE204_kwi6bbk4");
        check(approx(float_of(dp_num(d, 1, 215), "temperature"), 21.5f) && float_of(dp_num(d, 2, 40), "humidity") == 40.0f,
              "kwi6bbk4 DP1 temperature /10, DP2 humidity");
    }
    // D10: UZ-05T reports manufacturer "Wing" on the zigbeeModel-only TS0201.
    resolves({"TS0201", "Wing", "TS0201"});
    check(has_wl(def_for("TS0201", "Wing"), "Shenzhen Forever Young Technology", "UZ-05T"), "TS0201 white label UZ-05T");
    resolves({"TS0201", "_TZ3000_bguser20", "WSD500A"});   // fingerprinted TS0201s keep their def
    {   // D18: ME201WZ `_TZE2841000000_7uqwvoua` has no legacy twin.
        const PreparedDefinition& d = def_for("TS0601", "_TZE2841000000_7uqwvoua");
        check(&d == &def_for("TS0601", "_TZE284_kyyu8rbj"), "7uqwvoua on the ME201WZ def");
    }
    // X04 + F01: timeStart "1970".
    check(::zhc::tuya::time_start(def_for("TS0601", "_TZE204_wc2w9t1s"), "_TZE204_wc2w9t1s", "TS0601") == 1,
          "BOT-R9V-ZB timeStart 1970");
    // X11: Tongou is now Chayo, as vendor and as white-label vendor.
    int tongou = 0;
    for (const auto* d : registries()) {
        if (!d) continue;
        if (d->vendor && std::strcmp(d->vendor, "Tongou") == 0) ++tongou;
        for (std::size_t i = 0; i < d->white_labels_count; ++i)
            if (std::strcmp(d->white_labels[i].vendor, "Tongou") == 0) ++tongou;
    }
    check(tongou == 0, "no Tongou vendor or white label left (Chayo)");
    check(std::strcmp(def_for("TS0601", "_TZE284_6ocnqlhn").vendor, "Chayo") == 0, "TO-Q-SYS-JZT vendor Chayo");
}

// F01: HeatHUB HHST001.
void w2_hhst001() {
    std::printf("W2 HHST001\n");
    for (const char* manu : {"_TZE204_q12rv9gj", "_TZE284_zgisddrg", "_TZE284_6l44ypoc", "_TZE284_ksqviprv", "_TZE284_qvmmpbye"}) {
        const PreparedDefinition& d = def_for("TS0601", manu);
        if (&d != &def_for("TS0601", "_TZE204_q12rv9gj") || ::zhc::tuya::time_start(d, manu, "TS0601") != 1) {
            std::printf("  %s\n", manu); check(false, "HHST001 fingerprint + timeStart 1970");
        }
    }
    const PreparedDefinition& d = def_for("TS0601", "_TZE204_q12rv9gj");
    check(str_is(dp_enum(d, 46, 1), "temperature_scale", "fahrenheit") &&
          is_dp(encode(d, "temperature_scale", str("celsius")), 46, 0x04, 0), "DP46 temperatureUnitEnum");
    check(str_is(dp_enum(d, 101, 2), "manual_mode", "Temporary"), "DP101 2 -> Temporary");
    const Expose* m = find_expose(d, "manual_mode");
    check(m && m->type == ExposeType::Enum && m->enum_count == 3, "manual_mode expose Enum [Auto, Manual, Temporary]");
    check(bool_is(dp_bool(d, 114, true), "water_drop_switch", true), "DP114 water_drop_switch");
    check(float_of(dp_num(d, 115, 5), "day_standby_brightness") == 5.0f &&
          float_of(dp_num(d, 116, 2), "night_standby_brightness") == 2.0f, "DP115 / DP116 standby brightness raw");
    check(float_of(dp_num(d, 117, -3), "humidity_calibration") == -3.0f, "DP117 humidity_calibration raw (signed)");
    check(str_is(dp_enum(d, 119, 1), "lock_mode", "full_locked") &&
          is_dp(encode(d, "lock_mode", str("semi_locked")), 119, 0x04, 0), "DP119 lock_mode enum");
    const Expose* t = find_expose(d, "temperature_scale");
    check(t && t->type == ExposeType::Enum && has_expose(d, "lock_mode") && has_expose(d, "water_drop_switch"),
          "HHST001 new exposes");
}

// F05: SPM01V2.5 and SPM02.
void w2_spm() {
    std::printf("W2 SPM01V2.5 / SPM02\n");
    for (const char* manu : {"_TZE200_iwn0gpzz", "_TZE204_iwn0gpzz", "_TZE284_iwn0gpzz"}) resolves({"TS0601", manu, "SPM01V2.5"});
    for (const char* manu : {"_TZE200_ves1ycwx", "_TZE204_ves1ycwx", "_TZE284_ves1ycwx"}) resolves({"TS0601", manu, "SPM02"});
    {
        const PreparedDefinition& d = def_for("TS0601", "_TZE204_iwn0gpzz");
        const Out ph = dp_bytes(d, 6, 0x00, bytes({0x08, 0xFC, 0x00, 0x27, 0x10, 0x00, 0x08, 0xFC}));
        check(approx(float_of(ph, "voltage"), 230.0f) && approx(float_of(ph, "current"), 10.0f) && float_of(ph, "power") == 2300.0f,
              "V2.5 DP6 phaseVariant4");
        check(str_is(dp_bytes(d, 9, 0x05, bytes({0x00, 0x00, 0x01, 0x00})), "faults", "over_current"), "V2.5 DP9 faults bit 8");
        check(is_dp(encode(d, "energy_reset", boolean(false)), 16, 0x01, 0), "V2.5 DP16 energy_reset (RESET -> false)");
        check(float_of(dp_num(d, 29, 1500), "power") == 1500.0f, "V2.5 DP29 power raw");
        const Out t7 = dp_bytes(d, 17, 0x00, bytes({0x07, 0x01, 0x00, 0x14}));
        check(bool_is(t7, "high_power_alarm", true) && float_of(t7, "high_power_threshold") == 20.0f, "V2.5 DP17 threshold_7 id 7");
        const Out t8 = dp_bytes(d, 18, 0x00, bytes({0x01, 0x01, 0x00, 0x3F, 0x03, 0x00, 0x01, 0x03,
                                                    0x07, 0x01, 0x00, 0x00, 0x08, 0x01, 0x00, 0x3C, 0x09, 0x00, 0x00, 0x00}));
        check(bool_is(t8, "over_current_alarm", true) && float_of(t8, "over_current_threshold") == 63.0f &&
              bool_is(t8, "over_voltage_alarm", false) && float_of(t8, "over_voltage_threshold") == 259.0f &&
              bool_is(t8, "reverse_energy_alarm", true) && float_of(t8, "data_report_duration") == 60.0f &&
              bool_is(t8, "device_locating", false) && !t8.find("unbalanced_load_alarm"), "V2.5 DP18 spm01v25Thresholds");
        const Expose* oc = find_expose(d, "over_current_threshold");
        check(oc && oc->access == Access::State && has_expose(d, "faults") && has_expose(d, "energy_reset"),
              "V2.5 thresholds read-only, faults + energy_reset exposed");
    }
    {
        const PreparedDefinition& d = def_for("TS0601", "_TZE200_ves1ycwx");
        const Out y = dp_bytes(d, 7, 0x00, bytes({0x08, 0xFC, 0x00, 0x27, 0x10, 0x00, 0x08, 0xFC}));
        check(approx(float_of(y, "voltage_Y"), 230.0f) && approx(float_of(y, "current_Y"), 10.0f) && float_of(y, "power_Y") == 2300.0f,
              "SPM02 DP7 phaseVariant2WithPhase(Y)");
        check(dp_bytes(d, 6, 0x00, bytes({0x08, 0xFC, 0, 0, 0, 0, 0, 0})).find("voltage_X") &&
              dp_bytes(d, 8, 0x00, bytes({0x08, 0xFC, 0, 0, 0, 0, 0, 0})).find("voltage_Z"), "SPM02 DP6 / DP8 phases X / Z");
        check(is_dp(encode(d, "energy_reset", boolean(false)), 12, 0x01, 0), "SPM02 DP12 energy_reset");
        const Out t = dp_bytes(d, 18, 0x00, bytes({0x02, 0x01, 0x00, 0x0A, 0x05, 0x01, 0x00, 0x00}));
        check(bool_is(t, "unbalanced_load_alarm", true) && float_of(t, "unbalanced_load_threshold") == 10.0f &&
              bool_is(t, "phase_loss_alarm", true), "SPM02 DP18 spm02Thresholds ids 2 / 5");
        check(str_is(dp_bytes(d, 9, 0x05, bytes({0x00, 0x00, 0x40, 0x00})), "faults", "negative_power"), "SPM02 DP9 faults");
    }
}

// F07 / C2 / X08 / R056: DP rows on existing defs.
void w2_dp_rows() {
    std::printf("W2 DP rows\n");
    {   // F07: ZM25TQ, one def for both manus.
        resolves({"TS0601", "_TZE284_fzo2pocs", "ZM25TQ"});
        check(claims("_TZE284_fzo2pocs") == 1, "_TZE284_fzo2pocs registered once");
        const PreparedDefinition& d = def_for("TS0601", "_TZE200_fzo2pocs");
        check(str_is(dp_enum(d, 7, 1), "work_state", "closing"), "ZM25TQ DP7 1 -> work_state closing");
        check(is_dp(encode(d, "remote_pairing", str("PAIR")), 101, 0x01, 1), "ZM25TQ remote_pairing PAIR -> DP101 true");
        const Expose* w = find_expose(d, "work_state");
        check(w && w->access == Access::State && has_expose(d, "remote_pairing"), "ZM25TQ exposes");
    }
    for (const char* manu : {"_TZE284_kyyu8rbj", "_TZE284_mxujdmxo"}) {   // C2: ME201Z.x firmware DPs
        const PreparedDefinition& d = def_for("TS0601", manu);
        if (!approx(float_of(dp_num(d, 114, 1234), "liquid_depth"), 12.34f) ||
            !approx(float_of(dp_num(d, 115, 250), "installation_height"), 2.5f) ||
            !approx(float_of(dp_num(d, 116, 200), "liquid_depth_max"), 2.0f) ||
            !approx(float_of(dp_num(d, 19, 250), "installation_height"), 2.5f)) {
            std::printf("  %s\n", manu); check(false, "ME201WZ DP114 / 115 / 116 / 19 /100");
        }
    }
    for (const char* manu : {"_TZE200_ugekduaj", "_TZE204_ugekduaj"}) {   // X08: SDM01 V2-only rows
        const PreparedDefinition& d = def_for("TS0601", manu);
        const bool ok =
            approx(float_of(dp_num(d, 23, 1234), "produced_energy"), 12.34f) && float_of(dp_num(d, 29, 900), "power") == 900.0f &&
            approx(float_of(dp_num(d, 32, 5000), "ac_frequency"), 50.0f) && float_of(dp_num(d, 50, 98), "power_factor") == 98.0f &&
            approx(float_of(dp_num(d, 121, 2301), "voltage_c"), 230.1f) && approx(float_of(dp_num(d, 122, 1500), "current_c"), 1.5f) &&
            float_of(dp_num(d, 123, 345), "power_c") == 345.0f && float_of(dp_num(d, 126, 97), "power_factor_c") == 97.0f &&
            approx(float_of(dp_num(d, 127, 4321), "energy_c"), 43.21f) &&
            approx(float_of(dp_num(d, 128, 1111), "energy_produced_c"), 11.11f);
        if (!ok) { std::printf("  %s\n", manu); check(false, "SDM01 V2-only DPs 23/29/32/50/121-123/126-128"); }
    }
    for (const char* manu : {"_TZE200_nlrfgpny", "_TZE204_nlrfgpny", "_TZE284_nlrfgpny"}) {   // R056
        const PreparedDefinition& d = def_for("TS0601", manu);
        if (float_of(dp_num(d, 15, 0), "battery") != 100.0f || float_of(dp_num(d, 15, 55), "battery") != 55.0f) {
            std::printf("  %s\n", manu); check(false, "NAS-AB06B2 DP15 battery 0 -> 100");
        }
    }
}

// W1-ledgered: TRV601 DP6 / DP7 (owner family), SFD02-Z DP4 / DP14 value DPs.
void w2_ledgered() {
    std::printf("W2 TRV601 DP6/DP7, SFD02-Z DP4/DP14\n");
    for (const char* manu : {"_TZE204_rtrmfadk", "_TZE204_cvcu2p6e"}) {
        const PreparedDefinition& d = def_for("TS0601", manu);
        // Upstream lookups are plain numbers: enum or value DPs decode alike.
        const bool ok = str_is(dp_enum(d, 6, 1), "running_state", "heat") && str_is(dp_num(d, 6, 0), "running_state", "idle") &&
                        str_is(dp_enum(d, 7, 1), "window", "OPEN") && str_is(dp_num(d, 7, 0), "window", "CLOSE");
        if (!ok) { std::printf("  %s\n", manu); check(false, "TRV601 DP6 running_state {idle 0, heat 1}, DP7 window {CLOSE 0, OPEN 1}"); }
        check(str_is(dp_enum(d, 1, 1), "preset", "manual"), "TRV601 DP1 unchanged");
    }
    const PreparedDefinition& s = def_for("TS0601", "_TZE284_t88bjhfu");
    check(is_dp(encode(s, "light_type", str("halogen")), 4, 0x02, 2) &&
          is_dp(encode(s, "power_on_behavior", str("previous")), 14, 0x02, 2), "SFD02-Z DP4 / DP14 written as value DPs");
    check(str_is(dp_num(s, 4, 1), "light_type", "incandescent") && str_is(dp_num(s, 14, 0), "power_on_behavior", "off"),
          "SFD02-Z DP4 / DP14 decode value DPs");
    // Owner device: the new TS011F_5 def must not take the owner's plug.
    check(has_manu(def_for("TS011F", "_TZ3000_okaz9tjs"), "_TZ3000_okaz9tjs") && claims("_TZ3000_okaz9tjs") == 1,
          "owner TS011F _TZ3000_okaz9tjs keeps its dedicated def");
}

// ── W2 group a ──
// W2 tag `a`: N21 G08A-Z20T, N22 N02-Z10T, N28 SA-WK 4X4, N29 TS0601_ND_4Gang,
// N58 ZMD-208-2, N11 TS0601_GIRIER.

void w2_a_g08a_z20t() {
    std::printf("W2a Lincukoo G08A-Z20T\n");
    for (const char* manu : {"_TZE204_euszleja", "_TZE200_euszleja", "_TZE28C1000000_euszleja"}) {
        resolves({"TS0601", manu, "G08A-Z20T"});
        check(claims(manu) == 1, "G08A-Z20T manu listed by exactly one def");
    }
    const PreparedDefinition& d = def_for("TS0601", "_TZE204_euszleja");
    check(str_is(dp_enum(d, 1, 0), "alarm_state", "alarm"), "G08A DP1 enum 0 -> alarm_state alarm");
    check(str_is(dp_enum(d, 104, 31), "play_ringtone", "curtain_closed"), "G08A DP104 enum 31 -> play_ringtone curtain_closed");
    check(str_is(dp_enum(d, 23, 1), "night_light_mode", "auto"), "G08A DP23 enum 1 -> night_light_mode auto");
    check(bool_is(dp_bool(d, 16, true), "mute", true), "G08A DP16 onOff true -> mute ON");
    check(float_of(dp_num(d, 103, 30), "presence_hold_time") == 30.0f, "G08A DP103 raw 30 -> presence_hold_time 30 s");
    check(is_dp(encode(d, "alarm_mode", str("disable_alarm")), 19, 0x04, 3), "G08A alarm_mode disable_alarm -> DP19 enum 3");
    check(is_dp(encode(d, "night_light_mode", str("off")), 23, 0x04, 0), "G08A night_light_mode off -> DP23 enum 0");
    const Expose* x = find_expose(d, "presence_hold_time");
    check(x && x->value_min == 5 && x->value_max == 60 && x->value_step == 5, "G08A presence_hold_time expose 5-60 step 5");
}

void w2_a_n02_z10t() {
    std::printf("W2a Lincukoo N02-Z10T\n");
    for (const char* manu : {"_TZE2841000000_yfttaycl", "_TZE284_yfttaycl"}) {
        resolves({"TS0601", manu, "N02-Z10T"});
        check(claims(manu) == 1, "N02-Z10T manu listed by exactly one def");
    }
    const PreparedDefinition& d = def_for("TS0601", "_TZE284_yfttaycl");
    check(float_of(dp_num(d, 1, 85), "sound_value") == 85.0f, "N02 DP1 raw 85 -> sound_value 85 dB");
    check(str_is(dp_enum(d, 8, 1), "action", "high"), "N02 DP8 enum 1 -> action high");
    check(float_of(dp_num(d, 12, 77), "battery") == 77.0f, "N02 DP12 raw 77 -> battery 77");
    check(bool_is(dp_bool(d, 23, false), "indicator", false), "N02 DP23 onOff false -> indicator OFF");
    check(is_dp(encode(d, "noise_report_threshold", num(100)), 14, 0x02, 100), "N02 noise_report_threshold 100 -> DP14 value 100");
    check(is_dp(encode(d, "indicator", boolean(true)), 23, 0x01, 1), "N02 indicator ON -> DP23 bool 1");
    const Expose* x = find_expose(d, "noise_hold_time");
    check(x && x->value_min == 10 && x->value_max == 300, "N02 noise_hold_time expose 10-300 s");
}

void w2_a_sa_wk_4x4() {
    std::printf("W2a Nova Digital SA-WK 4X4\n");
    resolves({"TS0601", "_TZE204_bb5xlia1", "SA-WK 4X4"});
    check(claims("_TZE204_bb5xlia1") == 1, "bb5xlia1 listed by exactly one def");
    const PreparedDefinition& d = def_for("TS0601", "_TZE204_bb5xlia1");
    check(bool_is(dp_bool(d, 101, true), "state_master", true), "SA-WK DP101 onOff -> state_master ON");
    check(str_is(dp_enum(d, 14, 2), "relay_status", "memory"), "SA-WK DP14 enum 2 -> relay_status memory");
    check(float_of(dp_num(d, 12, 43200), "countdown_l6") == 43200.0f, "SA-WK DP12 raw -> countdown_l6 43200 s");
    check(str_is(dp_enum(d, 15, 2), "light_mode", "pos"), "SA-WK DP15 enum 2 -> light_mode pos");
    check(is_dp(encode(d, "state_l5", boolean(true)), 5, 0x01, 1), "SA-WK state_l5 ON -> DP5 bool 1");
    check(is_dp(encode(d, "countdown_l1", num(60)), 7, 0x02, 60), "SA-WK countdown_l1 60 -> DP7 value 60");
    const Expose* x = find_expose(d, "countdown_l6");
    check(has_expose(d, "state_master") && x && x->value_max == 43200, "SA-WK state_master + countdown_l6 0-43200 exposes");
}

void w2_a_nd_4gang() {
    std::printf("W2a Nova Digital TS0601_ND_4Gang\n");
    resolves({"TS0601", "_TZE204_tayx2ud6", "TS0601_ND_4Gang"});
    check(claims("_TZE204_tayx2ud6") == 1, "tayx2ud6 listed by exactly one def");
    const PreparedDefinition& d = def_for("TS0601", "_TZE204_tayx2ud6");
    check(d.tuya_time_start == 2, "ND_4Gang tuyaBase timeStart 2000");
    check(str_is(dp_enum(d, 14, 2), "power_on_behavior", "previous"), "ND_4Gang DP14 powerOnBehaviorEnum 2 -> previous");
    check(str_is(dp_enum(d, 15, 2), "backlight_mode", "position"), "ND_4Gang DP15 enum 2 -> backlight_mode position");
    check(bool_is(dp_bool(d, 16, true), "backlight", true), "ND_4Gang DP16 onOff -> backlight ON");
    check(is_dp(encode(d, "backlight_mode", str("relay")), 15, 0x04, 1), "ND_4Gang backlight_mode relay -> DP15 enum 1");
    check(is_dp(encode(d, "state_l4", boolean(false)), 4, 0x01, 0), "ND_4Gang state_l4 OFF -> DP4 bool 0");
    check(has_expose(d, "backlight_mode") && has_expose(d, "state_l4"), "ND_4Gang backlight_mode + state_l4 exposes");
}

void w2_a_zmd_208_2() {
    std::printf("W2a Zemismart ZMD-208-2\n");
    resolves({"TS0601", "_TZE28C1000000_gheewd2t", "ZMD-208-2"});
    check(claims("_TZE28C1000000_gheewd2t") == 1, "gheewd2t listed by exactly one def");
    const PreparedDefinition& d = def_for("TS0601", "_TZE28C1000000_gheewd2t");
    check(approx(float_of(dp_num(d, 2, 1000), "brightness_l1"), 254.0f), "ZMD-208-2 DP2 1000 -> brightness_l1 254 (scale 0-254/0-1000)");
    check(approx(float_of(dp_num(d, 8, 500), "brightness_l2"), 127.0f), "ZMD-208-2 DP8 500 -> brightness_l2 127");
    check(bool_is(dp_bool(d, 7, true), "state_l2", true), "ZMD-208-2 DP7 bool -> state_l2 ON");
    check(is_dp(encode(d, "brightness_l1", num(254)), 2, 0x02, 1000), "ZMD-208-2 brightness_l1 254 -> DP2 value 1000");
    check(is_dp(encode(d, "state_l1", boolean(true)), 1, 0x01, 1), "ZMD-208-2 state_l1 ON -> DP1 bool 1");
    check(d.config_steps_count == 1, "ZMD-208-2 tuyaMagicPacket configure step");
    const Expose* x = find_expose(d, "brightness_l2");
    check(x && x->value_max == 254 && has_expose(d, "state_l1"), "ZMD-208-2 state_l1 + brightness_l2 0-254 exposes");
}

void w2_a_girier() {
    std::printf("W2a Girier TS0601_GIRIER\n");
    resolves({"TS0601", "_TZE28C1000000_ntqql63p", "TS0601_GIRIER"});
    check(claims("_TZE28C1000000_ntqql63p") == 1, "ntqql63p listed by exactly one def");
    const PreparedDefinition& d = def_for("TS0601", "_TZE28C1000000_ntqql63p");
    const Out sc = dp_enum(d, 1, 2);
    check(bool_is(sc, "presence", true) && bool_is(sc, "occupancy", true) &&
          str_is(sc, "presence_state", "sensor_close"), "GIRIER DP1 2 -> presence/occupancy true, presence_state sensor_close");
    const Out ab = dp_enum(d, 1, 0);
    check(bool_is(ab, "presence", false) && bool_is(ab, "occupancy", false) &&
          str_is(ab, "presence_state", "absence"), "GIRIER DP1 0 -> presence/occupancy false, presence_state absence");
    check(float_of(dp_num(d, 106, 120), "presence_delay") == 120.0f, "GIRIER DP106 raw 120 -> presence_delay 120 s");
    check(is_dp(encode(d, "presence_delay", num(300)), 106, 0x02, 300), "GIRIER presence_delay 300 -> DP106 value 300");
    const Expose* x = find_expose(d, "presence_delay");
    check(x && x->value_max == 1800 && has_expose(d, "presence_state"), "GIRIER presence_state + presence_delay 0-1800 exposes");
}

// Call order: w2_a_g08a_z20t, w2_a_n02_z10t, w2_a_sa_wk_4x4, w2_a_nd_4gang, w2_a_zmd_208_2, w2_a_girier

// ── W2 group b1 ──
// ── W2 b1: HYSYIOT HS206Z / HS207Z / HS209Z / HS300TF, Jikaida JKD-839Z, Xenon Smart X7633 ──

// ssIasZone commandStatusChangeNotification frame carrying `status`.
DispatchResult w2_b1_ias(const PreparedDefinition& d, std::uint16_t status) {
    const std::vector<std::uint8_t> f{0x09, 0x10, 0x00, std::uint8_t(status & 0xFF), std::uint8_t(status >> 8),
                                      0x00, 0x01, 0x00, 0x00};
    return dispatch(d, 0x0500, 1, std::span<const std::uint8_t>(f.data(), f.size()));
}

// N44: HS206Z — TS0202 _TZD200_rdbronta + bare zigbeeModel.
void w2_b1_hs206z() {
    std::printf("W2 b1 HS206Z\n");
    resolves({"TS0202", "_TZD200_rdbronta", "HS206Z"});
    resolves({"HS206Z", "HYSYIOT", "HS206Z"});
    check(claims("_TZD200_rdbronta") == 1, "_TZD200_rdbronta claimed once");
    const PreparedDefinition& d = def_for("TS0202", "_TZD200_rdbronta");
    check(bool_is(dp_enum(d, 1, 0), "occupancy", true) && bool_is(dp_enum(d, 1, 1), "occupancy", false),
          "HS206Z DP1 trueFalse0: 0 -> occupancy true, 1 -> false");
    check(str_is(dp_enum(d, 3, 2), "battery_state", "high"), "HS206Z DP3 2 -> battery_state high");
    check(float_of(dp_num(d, 4, 87), "battery") == 87.0f, "HS206Z DP4 battery raw");
    check(r_bool(w2_b1_ias(d, 0x0009), "occupancy", true), "HS206Z ias_occupancy_alarm_1 bit 0 -> occupancy");
    check(is_dp(encode(d, "pir_sensitivity", str("middle")), 9, 0x04, 1), "HS206Z pir_sensitivity middle -> DP9 enum 1");
    Value v{}; v.type = ValueType::Int; v.i = 60;
    check(is_dp(encode(d, "pir_delay", v), 12, 0x02, 60), "HS206Z pir_delay 60 -> DP12 value 60");
    const Expose* e = find_expose(d, "pir_delay");
    check(e && e->access == Access::StateSet && e->value_min == 30 && e->value_max == 120 && has_expose(d, "battery_low"),
          "HS206Z pir_delay expose StateSet 30..120 + battery_low");
}

// N45: HS207Z — {TS0601, TS0202} x {_TZD200_,_TZE200_,_TZE204_}8sql5jcj + bare zigbeeModel.
void w2_b1_hs207z() {
    std::printf("W2 b1 HS207Z\n");
    for (const char* zm : {"TS0601", "TS0202"})
        for (const char* manu : {"_TZD200_8sql5jcj", "_TZE200_8sql5jcj", "_TZE204_8sql5jcj"})
            resolves({zm, manu, "HS207Z"});
    resolves({"HS207Z", "HYSYIOT", "HS207Z"});
    check(claims("_TZE204_8sql5jcj") == 1, "_TZE204_8sql5jcj claimed once");
    const PreparedDefinition& d = def_for("TS0601", "_TZE200_8sql5jcj");
    check(approx(float_of(dp_num(d, 110, 235), "temperature"), 23.5f), "HS207Z DP110 temperature /10");
    check(approx(float_of(dp_num(d, 106, -15), "temperature_calibration"), -1.5f), "HS207Z DP106 temperature_calibration /10 signed");
    check(str_is(dp_enum(d, 109, 1), "temperature_unit", "fahrenheit"), "HS207Z DP109 1 -> fahrenheit");
    check(bool_is(dp_bool(d, 102, true), "vibration", true), "HS207Z DP102 vibration");
    check(float_of(dp_num(d, 11, 320), "illuminance") == 320.0f, "HS207Z DP11 illuminance raw");
    check(is_dp(encode(d, "temperature_calibration", num(-1.5)), 106, 0x02, static_cast<std::uint32_t>(-15)),
          "HS207Z temperature_calibration -1.5 -> DP106 -15");
    check(is_dp(encode(d, "indicator", boolean(true)), 107, 0x01, 1), "HS207Z indicator -> DP107 bool");
    const Expose* e = find_expose(d, "vibration_delay");
    check(e && e->access == Access::StateSet && e->value_max == 1440 && has_expose(d, "temperature_unit"),
          "HS207Z vibration_delay expose 1..1440");
}

// N46: HS209Z — {TS0601, TS0202} x {_TZD200_,_TZE200_}bgjlfsuy + bare zigbeeModel.
void w2_b1_hs209z() {
    std::printf("W2 b1 HS209Z\n");
    for (const char* zm : {"TS0601", "TS0202"})
        for (const char* manu : {"_TZD200_bgjlfsuy", "_TZE200_bgjlfsuy"})
            resolves({zm, manu, "HS209Z"});
    resolves({"HS209Z", "HYSYIOT", "HS209Z"});
    check(claims("_TZE200_bgjlfsuy") == 1, "_TZE200_bgjlfsuy claimed once");
    const PreparedDefinition& d = def_for("TS0202", "_TZD200_bgjlfsuy");
    const DispatchResult z = w2_b1_ias(d, 0x0009);
    check(r_bool(z, "occupancy", true) && r_bool(z, "battery_low", true),
          "HS209Z iasZoneAlarm occupancy {alarm_1, battery_low}");
    check(str_is(dp_enum(d, 3, 1), "battery_state", "medium"), "HS209Z dpBatteryState DP3 1 -> medium");
    check(str_is(dp_bool(d, 107, true), "indicator", "ON"), "HS209Z dpBinary DP107 true -> indicator ON");
    check(float_of(dp_num(d, 103, 250), "detection_distance") == 250.0f, "HS209Z DP103 detection_distance raw");
    check(is_dp(encode(d, "indicator", str("OFF")), 107, 0x01, 0), "HS209Z indicator OFF -> DP107 false");
    Value v{}; v.type = ValueType::Int; v.i = 300;
    check(is_dp(encode(d, "pir_delay", v), 12, 0x02, 300), "HS209Z pir_delay 300 -> DP12");
    const Expose* e = find_expose(d, "pir_delay");
    check(e && e->value_max == 28800 && e->value_step == 30 && has_binding(d, 0x0001),
          "HS209Z pir_delay 30..28800 step 30 + genPowerCfg bind (m.battery)");
}

// N47: HS300TF — TS0601 x {_TZD200_,_TZE200_}{7sa1ogeny,sa1ogeny} + bare zigbeeModel.
void w2_b1_hs300tf() {
    std::printf("W2 b1 HS300TF\n");
    for (const char* manu : {"_TZD200_7sa1ogeny", "_TZE200_7sa1ogeny", "_TZD200_sa1ogeny", "_TZE200_sa1ogeny"})
        resolves({"TS0601", manu, "HS300TF"});
    resolves({"HS300TF", "HYSYIOT", "HS300TF"});
    check(claims("_TZE200_7sa1ogeny") == 1, "_TZE200_7sa1ogeny claimed once");
    const PreparedDefinition& d = def_for("TS0601", "_TZE200_7sa1ogeny");
    check(approx(float_of(dp_num(d, 5, 215), "temperature"), 21.5f), "HS300TF DP5 temperature /10");
    check(float_of(dp_num(d, 3, 42), "soil_moisture") == 42.0f, "HS300TF DP3 soil_moisture raw");
    check(str_is(dp_enum(d, 111, 1), "water_warning", "alarm"), "HS300TF DP111 1 -> water_warning alarm");
    check(str_is(dp_enum(d, 116, 2), "soil_fertility_warning", "high"), "HS300TF DP116 2 -> soil_fertility_warning high");
    check(str_is(dp_enum(d, 9, 0), "temperature_unit", "celsius"), "HS300TF DP9 temperatureUnitEnum 0 -> celsius");
    check(is_dp(encode(d, "temperature_calibration", num(1.5)), 107, 0x02, 15), "HS300TF temperature_calibration 1.5 -> DP107 15");
    check(is_dp(encode(d, "temperature_unit", str("fahrenheit")), 9, 0x04, 1), "HS300TF temperature_unit -> DP9 enum 1");
    const Expose* e = find_expose(d, "report_period");
    check(e && e->access == Access::StateSet && e->value_min == 5 && e->value_max == 1200 && has_expose(d, "soil_fertility_set_v1"),
          "HS300TF report_period 5..1200 + fertility thresholds");
}

// N48: Jikaida JKD-839Z gas sensor.
void w2_b1_jkd839z() {
    std::printf("W2 b1 JKD-839Z\n");
    resolves({"TS0601", "_TZE204_mqij5jas", "JKD-839Z"});
    check(claims("_TZE204_mqij5jas") == 1, "_TZE204_mqij5jas claimed once");
    const PreparedDefinition& d = def_for("TS0601", "_TZE204_mqij5jas");
    check(bool_is(dp_enum(d, 1, 0), "gas", true) && bool_is(dp_enum(d, 1, 1), "gas", false),
          "JKD-839Z DP1 trueFalse0: 0 -> gas true");
    check(float_of(dp_num(d, 2, 120), "gas_value") == 120.0f, "JKD-839Z DP2 gas_value raw");
    check(str_is(dp_enum(d, 9, 2), "self_test_result", "failure"), "JKD-839Z DP9 selfTestResult 2 -> failure");
    check(is_dp(encode(d, "silence", boolean(true)), 16, 0x01, 1), "JKD-839Z silence -> DP16 bool");
    const Expose* e = find_expose(d, "silence");
    check(e && e->access == Access::StateSet && has_expose(d, "self_test_result"), "JKD-839Z silence expose StateSet");
}

// N54: Xenon Smart X7633 gas detector (+ WL Zorro Alert JT-ZL2010Z).
void w2_b1_x7633() {
    std::printf("W2 b1 X7633\n");
    resolves({"TS0601", "_TZE204_0corc0ph", "X7633"});
    check(claims("_TZE204_0corc0ph") == 1, "_TZE204_0corc0ph claimed once");
    const PreparedDefinition& d = def_for("TS0601", "_TZE204_0corc0ph");
    check(has_wl(d, "Zorro Alert", "JT-ZL2010Z"), "X7633 white label Zorro Alert JT-ZL2010Z");
    check(bool_is(dp_enum(d, 1, 0), "gas", true) && bool_is(dp_enum(d, 1, 1), "gas", false),
          "X7633 DP1 trueFalseEnum0: enum 0 -> gas true");
    check(bool_is(dp_bool(d, 12, true), "end_of_life", false) && bool_is(dp_bool(d, 12, false), "end_of_life", true),
          "X7633 DP12 trueFalseInvert -> end_of_life");
    const std::uint8_t bits[] = { 0x05 };
    const Out f = dp_bytes(d, 11, 0x05, std::span<const std::uint8_t>(bits, 1));
    check(bool_is(f, "fault_alarm", true) && bool_is(f, "fault", true) && bool_is(f, "serious_fault", false) &&
          bool_is(f, "sensor_fault", true) && bool_is(f, "probe_fault", false) && bool_is(f, "power_fault", false),
          "X7633 DP11 bitmap 0x05 -> fault_alarm + fault + sensor_fault");
    const std::uint8_t none[] = { 0x00 };
    check(bool_is(dp_bytes(d, 11, 0x05, std::span<const std::uint8_t>(none, 1)), "fault_alarm", false),
          "X7633 DP11 bitmap 0 -> fault_alarm false");
    check(bool_is(dp_bool(d, 10, true), "preheat", true), "X7633 DP10 preheat raw");
    check(!encode(d, "fault_alarm", boolean(true)).ok, "X7633 DP11 has no write path (no `to` upstream)");
    const Expose* e = find_expose(d, "power_fault");
    check(e && e->access == Access::State && has_expose(d, "fault_alarm") && has_expose(d, "end_of_life"),
          "X7633 per-bit fault exposes");
}

// Call order: w2_b1_hs206z, w2_b1_hs207z, w2_b1_hs209z, w2_b1_hs300tf, w2_b1_jkd839z, w2_b1_x7633
// (w2_b1_ias is a helper, not a test).

// ── W2 group b2 ──
// ── W2 b2: TS0601_lqhqj73n, T3E_mua6ucdj, HY607W-3A (X01), TS0601_cover_4 (F03) ──

void w2_b2_lqhqj73n() {
    std::printf("W2 b2: TS0601_lqhqj73n roller motor (_TZE284_lqhqj73n)\n");
    resolves({"TS0601", "_TZE284_lqhqj73n", "TS0601_lqhqj73n"});
    const PreparedDefinition& d = def_for("TS0601", "_TZE284_lqhqj73n");
    check(str_is(dp_enum(d, 1, 2), "state", "CLOSE"), "lqhqj73n DP1 2 -> state CLOSE (coverAction)");
    {
        const Out o = dp_num(d, 2, 30);
        check(float_of(o, "position") == 70.0f && str_is(o, "state", "OPEN"),
              "lqhqj73n DP2 30 -> position 70 + state OPEN (coverPositionInverted)");
    }
    {
        const Out o = dp_num(d, 3, 100);
        check(float_of(o, "position") == 0.0f && str_is(o, "state", "CLOSE"),
              "lqhqj73n DP3 100 -> position 0 + state CLOSE (coverPositionInverted)");
    }
    check(str_is(dp_enum(d, 7, 1), "motor_state", "closing"), "lqhqj73n DP7 1 -> motor_state closing");
    check(bool_is(dp_num(d, 12, 3), "motor_fault", true) && bool_is(dp_num(d, 12, 2), "motor_fault", false),
          "lqhqj73n DP12 (v & 1) === 1 -> motor_fault");
    check(str_is(dp_enum(d, 104, 2), "obstacle_sensitivity", "medium"), "lqhqj73n DP104 2 -> obstacle_sensitivity medium");
    check(is_dp(encode(d, "position", num(30)), 2, 0x02, 70), "lqhqj73n position 30 -> DP2 value 70 (inverted)");
    check(is_dp(encode(d, "obstacle_sensitivity", str("low")), 104, 0x04, 3), "lqhqj73n obstacle_sensitivity low -> DP104 enum 3");
    check(has_expose(d, "motor_fault") && has_expose(d, "obstacle_sensitivity") && has_expose(d, "battery"),
          "lqhqj73n exposes motor_fault, obstacle_sensitivity, battery");
}

void w2_b2_t3e() {
    std::printf("W2 b2: T3E_mua6ucdj touch panel (_TZE200_mua6ucdj)\n");
    resolves({"TS0601", "_TZE200_mua6ucdj", "T3E_mua6ucdj"});
    const PreparedDefinition& d = def_for("TS0601", "_TZE200_mua6ucdj");
    check(bool_is(dp_bool(d, 25, true), "state_l2", true), "T3E DP25 true -> state_l2 ON");
    {
        const Out o = dp_bool(d, 135, false);
        check(bool_is(o, "scene_switch_2_2", false) && str_is(o, "action", "scene_switch_2_2_off"),
              "T3E DP135 false -> scene_switch_2_2 OFF + action scene_switch_2_2_off");
    }
    check(str_is(dp_enum(d, 106, 0), "action", "smart_curtain_close"), "T3E DP106 0 -> action smart_curtain_close (t3eAction)");
    check(str_is(dp_num(d, 5, 0), "action", "scene_1") && dp_enum(d, 5, 1).find("action") == nullptr,
          "T3E DP5 0 -> action scene_1; non-zero -> no action (t3eAction)");
    check(is_dp(encode(d, "scene_switch_3_4", str("ON")), 141, 0x01, 1), "T3E scene_switch_3_4 ON -> DP141 bool true");
    check(is_dp(encode(d, "state_l1", str("OFF")), 24, 0x01, 0), "T3E state_l1 OFF -> DP24 bool false");
    const Expose* a = find_expose(d, "action");
    check(a && a->enum_count == 52 && has_expose(d, "scene_switch_3_4") && has_expose(d, "state_l3"),
          "T3E exposes state_l1..l3, 12 scene switches, 52 actions");
}

void w2_b2_hy607w() {
    std::printf("W2 b2: HY607W-3A gas boiler thermostat (_TZE200_khah2lkr, X01)\n");
    resolves({"TS0601", "_TZE200_khah2lkr", "HY607W-3A"});
    const PreparedDefinition& d = def_for("TS0601", "_TZE200_khah2lkr");
    bool sync = false;
    for (std::size_t i = 0; i < d.from_zigbee_count; ++i) sync = sync || d.from_zigbee[i] == &tuya::kFzTuyaMcuSyncTime;
    check(sync && d.tuya_time_start == 1, "HY607W-3A tuyaBase timeStart 1970 + mcuSyncTime (#13676)");
    check(approx(float_of(dp_num(d, 16, 215), "local_temperature"), 21.5f), "HY607W-3A DP16 215 -> local_temperature 21.5");
    check(approx(float_of(dp_num(d, 50, 205), "occupied_heating_setpoint"), 20.5f),
          "HY607W-3A DP50 205 -> occupied_heating_setpoint 20.5");
    {
        const std::uint8_t b[] = { 3 };
        check(str_is(dp_bytes(d, 128, 0x00, b), "mode_state", "temp_override"),
              "HY607W-3A DP128 raw [3] -> mode_state temp_override (first byte)");
    }
    check(str_is(dp_bool(d, 102, true), "running_state", "heat") && str_is(dp_bool(d, 102, false), "running_state", "idle"),
          "HY607W-3A DP102 true/false -> running_state heat/idle");
    check(is_dp(encode(d, "occupied_heating_setpoint", num(21.5)), 50, 0x02, 215),
          "HY607W-3A occupied_heating_setpoint 21.5 -> DP50 value 215");
    check(is_dp(encode(d, "state", str("OFF")), 125, 0x01, 0), "HY607W-3A state OFF -> DP125 bool false");
    check(has_expose(d, "mode_state") && has_expose(d, "running_state") && !has_expose(d, "system_mode"),
          "HY607W-3A exposes upstream DP keys, not the old ZCL stub");
}

void w2_b2_cover4() {
    std::printf("W2 b2: TS0601_cover_4 / Moes AM43 (_TZE200_icka1clh, _TZE200_zah67ekd, F03)\n");
    resolves({"TS0601", "_TZE200_icka1clh", "TS0601__TZE200_icka1clh"});
    resolves({"TS0601", "_TZE200_zah67ekd", "TS0601__TZE200_icka1clh"});
    check(claims("_TZE200_zah67ekd") == 1, "_TZE200_zah67ekd listed once (shares TS0601_cover_4 upstream)");
    const PreparedDefinition& d = def_for("TS0601", "_TZE200_icka1clh");
    check(has_wl(d, "Moes", "AM43-0.45/40-ES-EB"), "cover_4 white label Moes AM43-0.45/40-ES-EB");
    check(dp_enum(d, 1, 2).find("state") == nullptr, "cover_4 DP1 decode ignored (last command, #13597)");
    {
        const Out o = dp_num(d, 2, 0);
        check(float_of(o, "position") == 0.0f && str_is(o, "state", "CLOSE"), "cover_4 DP2 0 -> position 0 + state CLOSE (am43Position)");
    }
    {
        const Out o = dp_num(d, 3, 40);
        check(float_of(o, "position") == 40.0f && str_is(o, "state", "OPEN"), "cover_4 DP3 40 -> position 40 + state OPEN (am43Position)");
    }
    check(dp_enum(d, 7, 1).count == 0, "cover_4 DP7 work_state ignored");
    check(str_is(dp_enum(d, 5, 1), "motor_direction", "reversed"), "cover_4 DP5 1 -> motor_direction reversed (tubularMotorDirection)");
    check(is_dp(encode(d, "state", str("STOP")), 1, 0x04, 1), "cover_4 state STOP -> DP1 enum 1 (write kept)");
    check(is_dp(encode(d, "position", num(60)), 2, 0x02, 60), "cover_4 position 60 -> DP2 value 60");
    check(is_dp(encode(d, "factory_reset", boolean(true)), 102, 0x01, 1), "cover_4 factory_reset true -> DP102 bool (raw true)");
    check(has_expose(d, "opening_mode") && has_expose(d, "motor_speed") && !has_expose(d, "tilt"),
          "cover_4 lift-mode exposes (tilt deferred)");
}

// Call order: w2_b2_lqhqj73n, w2_b2_t3e, w2_b2_hy607w, w2_b2_cover4

// ── W2 group d1 ──
// ── W2 d1: fontastic _TYST11_yfmeg60f, ME173, SP107E-Zigbee ─────────────

void w2_d1_fontastic() {
    std::printf("w2 d1: Fontastic _TYST11_yfmeg60f TRV\n");
    resolves({"fmeg60f", "_TYST11_yfmeg60f", "_TYST11_yfmeg60f"});
    check(claims("_TYST11_yfmeg60f") == 1, "_TYST11_yfmeg60f listed by exactly one def");
    const PreparedDefinition& d = def_for("fmeg60f", "_TYST11_yfmeg60f");
    check(str_is(dp_bool(d, 101, true), "system_mode", "heat"), "fontastic DP101 true -> system_mode heat (lookup heat:true)");
    check(str_is(dp_bool(d, 40, true), "child_lock", "LOCK"), "fontastic DP40 true -> child_lock LOCK (lockUnlock)");
    check(str_is(dp_bool(d, 130, true), "anti_scaling", "ON"), "fontastic DP130 true -> anti_scaling ON (onOff)");
    check(approx(float_of(dp_num(d, 102, 215), "local_temperature"), 21.5f), "fontastic DP102 215 -> local_temperature 21.5 (divideBy10)");
    check(approx(float_of(dp_num(d, 27, -3), "local_temperature_calibration"), -3.0f), "fontastic DP27 -3 -> local_temperature_calibration -3 (raw)");
    const std::uint8_t odd[] = { 0x03 };
    const std::uint8_t even[] = { 0x02 };
    check(bool_is(dp_bytes(d, 105, 0x05, odd), "battery_low", true), "fontastic DP105 bitmap 0x03 -> battery_low true (v & 1)");
    check(bool_is(dp_bytes(d, 105, 0x05, even), "battery_low", false), "fontastic DP105 bitmap 0x02 -> battery_low false (v & 1)");
    check(bool_is(dp_bool(d, 106, true), "away_mode", true), "fontastic DP106 true -> away_mode true (raw)");
    check(is_dp(encode(d, "system_mode", str("off")), 101, 0x01, 0), "fontastic system_mode off -> DP101 bool false");
    check(is_dp(encode(d, "current_heating_setpoint", num(21.5)), 103, 0x02, 215), "fontastic current_heating_setpoint 21.5 -> DP103 215");
    const Expose* x = find_expose(d, "local_temperature_calibration");
    check(x && x->value_min == -6 && x->value_max == 6, "fontastic local_temperature_calibration expose -6..6");
    check(!has_expose(d, "schedule_monday"), "fontastic schedule DPs 123-129 deferred");
    check(has_binding(d, 0x0000), "fontastic tuyaBase bindBasicOnConfigure -> genBasic binding");
    check(d.tuya_time_start == 2, "fontastic tuyaBase timeStart 2000");
}

void w2_d1_me173() {
    std::printf("w2 d1: Ecobuy Well ME173 TRV _TZE200_nhxzhga5\n");
    resolves({"TS0601", "_TZE200_nhxzhga5", "ME173"});
    check(claims("_TZE200_nhxzhga5") == 1, "_TZE200_nhxzhga5 listed by exactly one def");
    const PreparedDefinition& d = def_for("TS0601", "_TZE200_nhxzhga5");
    const Out m = dp_enum(d, 2, 3);
    check(str_is(m, "preset", "eco") && str_is(m, "system_mode", "heat"), "ME173 DP2 3 -> preset eco + system_mode heat (fromMap)");
    check(str_is(dp_enum(d, 2, 0), "system_mode", "auto"), "ME173 DP2 0 -> system_mode auto");
    check(str_is(dp_enum(d, 3, 1), "running_state", "idle"), "ME173 DP3 1 -> running_state idle");
    check(approx(float_of(dp_num(d, 105, 175), "eco_temperature"), 17.5f), "ME173 DP105 175 -> eco_temperature 17.5 (divideBy10)");
    check(approx(float_of(dp_num(d, 47, 253), "local_temperature_calibration"), -3.0f), "ME173 DP47 253 (int8) -> local_temperature_calibration -3");
    check(approx(float_of(dp_num(d, 47, -3), "local_temperature_calibration"), -3.0f), "ME173 DP47 0xFFFFFFFD (int32) -> local_temperature_calibration -3");
    check(str_is(dp_bool(d, 7, true), "child_lock", "LOCK"), "ME173 DP7 true -> child_lock LOCK (lockUnlock)");
    check(str_is(dp_enum(d, 15, 0), "window_open", "OPEN"), "ME173 DP15 0 -> window_open OPEN");
    check(is_dp(encode(d, "system_mode", str("heat")), 2, 0x04, 1), "ME173 system_mode heat -> DP2 enum 1 (toMap)");
    check(is_dp(encode(d, "preset", str("boost")), 2, 0x04, 5), "ME173 preset boost -> DP2 enum 5 (toMap)");
    Value cal{}; cal.type = ValueType::Int; cal.i = -3;
    check(is_dp(encode(d, "local_temperature_calibration", cal), 47, 0x02, 0xFFFFFFFDu),
          "ME173 local_temperature_calibration -3 -> DP47 s32 0xFFFFFFFD (to: v => v)");
    check(has_expose(d, "boost_timeset_countdown") && !has_expose(d, "schedule_monday"),
          "ME173 exposes boost_timeset_countdown; schedule DPs 28-34 deferred");
    check(d.tuya_time_start == 1, "ME173 tuyaBase timeStart 1970");
}

void w2_d1_sp107e() {
    std::printf("w2 d1: Tuya SP107E-Zigbee LED music controller _TZE204_zajeikkt\n");
    resolves({"TS0601", "_TZE204_zajeikkt", "SP107E-Zigbee"});
    check(claims("_TZE204_zajeikkt") == 1, "_TZE204_zajeikkt listed by exactly one def");
    const PreparedDefinition& d = def_for("TS0601", "_TZE204_zajeikkt");
    check(bool_is(dp_bool(d, 1, true), "state", true), "SP107E DP1 true -> state on (tvc.onOff, Bool as MS032Z)");
    check(str_is(dp_enum(d, 103, 2), "rgb_order", "GRB"), "SP107E DP103 2 -> rgb_order GRB (ms032zColorOrder)");
    check(str_is(dp_enum(d, 104, 4), "mode", "music_screen"), "SP107E DP104 4 -> mode music_screen");
    check(str_is(dp_enum(d, 112, 9), "led_chip", "SK6812"), "SP107E DP112 9 -> led_chip SK6812 (sp107eLedChips)");
    check(approx(float_of(dp_num(d, 102, 300), "segment_pixels"), 300.0f), "SP107E DP102 300 -> segment_pixels 300 (raw)");
    check(is_dp(encode(d, "mode", str("effects")), 104, 0x04, 2), "SP107E mode effects -> DP104 enum 2");
    Value b{}; b.type = ValueType::Int; b.i = 80;
    check(is_dp(encode(d, "brightness_percent", b), 106, 0x02, 80), "SP107E brightness_percent 80 -> DP106 value 80");
    const Expose* x = find_expose(d, "segment_pixels");
    check(x && x->value_min == 1 && x->value_max == 300 && x->category == ExposeCategory::Config,
          "SP107E segment_pixels expose 1..300, config");
    check(!has_expose(d, "falling_dot_color"), "SP107E DP113 falling_dot_color (HSV string) deferred");
}

// Call order: w2_d1_fontastic(); w2_d1_me173(); w2_d1_sp107e();

// ── W2 group d2 ──
// W2 tag `d2`: N27 Nous D1Z, N43 EARU EAYCBM-Z-1P (+ WL EAYCBM-Z-2P), N42 Tuya 07P4.

void w2_d2_d1z() {
    std::printf("W2d2 Nous D1Z\n");
    resolves({"TS0601", "_TZE204_vfnf7zhq", "D1Z"});
    check(claims("_TZE204_vfnf7zhq") == 1, "D1Z manu listed by exactly one def");
    const PreparedDefinition& d = def_for("TS0601", "_TZE204_vfnf7zhq");

    const std::uint8_t ph[] = {0x09, 0x01, 0x01, 0x11, 0x70, 0x00, 0x3F, 0x07};   // 230.5 V, 70 A, 16135 W
    const Out o = dp_bytes(d, 6, 0x00, std::span<const std::uint8_t>(ph, sizeof(ph)));
    check(approx(float_of(o, "voltage"), 230.5f) && approx(float_of(o, "current"), 70.0f) &&
          float_of(o, "power") == 16135.0f, "D1Z DP6 phaseVariant4 -> voltage /10, 24-bit current /1000, 24-bit power");

    const std::uint8_t f[] = {0x00, 0x00, 0x40, 0x01};
    check(str_is(dp_bytes(d, 9, 0x05, std::span<const std::uint8_t>(f, 4)), "faults", "short_circuit,magnetism"),
          "D1Z DP9 circuitBreakerFaults bits 0+14 -> short_circuit,magnetism");

    const std::uint8_t a2[] = {0x01, 0x01, 0x02, 0x76, 0x03, 0x00, 0x01, 0x13};   // id1 ON 630, id3 OFF 275
    const Out t2 = dp_bytes(d, 18, 0x00, std::span<const std::uint8_t>(a2, sizeof(a2)));
    check(bool_is(t2, "over_current_alarm", true) && approx(float_of(t2, "over_current_threshold"), 63.0f),
          "D1Z DP18 threshold_5 id1 -> over_current_alarm ON, over_current_threshold 630/10 = 63 A");
    check(bool_is(t2, "over_voltage_alarm", false) && float_of(t2, "over_voltage_threshold") == 275.0f,
          "D1Z DP18 threshold_5 id3 -> over_voltage_alarm OFF, over_voltage_threshold 275 V");
    const std::uint8_t a3[] = {0x03, 0x01, 0x00, 0x32};
    check(approx(float_of(dp_bytes(d, 126, 0x00, std::span<const std::uint8_t>(a3, 4)), "lost_flow_threshold"), 5.0f),
          "D1Z DP126 threshold_6 id3 50 -> lost_flow_threshold 5.0");
    const std::uint8_t a1[] = {0x04, 0x01, 0x00, 0x1E};
    check(float_of(dp_bytes(d, 17, 0x00, std::span<const std::uint8_t>(a1, 4)), "leakage_current_threshold") == 30.0f,
          "D1Z DP17 threshold_4 id4 -> leakage_current_threshold 30 mA");

    check(str_is(dp_bool(d, 12, false), "energy_balance_reset", "idle"), "D1Z DP12 reset.from -> energy_balance_reset idle");
    check(str_is(dp_enum(d, 127, 1), "status", "consumption"), "D1Z DP127 enum 1 -> status consumption");
    check(approx(float_of(dp_num(d, 13, 4321), "energy_balance"), 43.21f), "D1Z DP13 divideBy100 -> energy_balance 43.21");

    check(is_dp(encode(d, "energy_balance_reset", str("RESET")), 12, 0x01, 0), "D1Z energy_balance_reset RESET -> DP12 bool false");
    check(is_dp(encode(d, "energy_balance_add", num(12.5)), 14, 0x02, 1250), "D1Z energy_balance_add 12.5 -> DP14 value 1250");
    check(is_dp(encode(d, "power_on_behavior", str("previous")), 134, 0x04, 2), "D1Z power_on_behavior previous -> DP134 enum 2");
    check(!encode(d, "over_current_threshold", num(40)).ok, "D1Z alarm thresholds are read-only (RMW write deferred)");

    const Expose* x = find_expose(d, "countdown");
    check(x && x->value_max == 86400 && has_expose(d, "lost_flow_threshold") && !has_expose(d, "inching"),
          "D1Z exposes: countdown max 86400, flat alarm_set_3, no inching");
}

void w2_d2_eaycbm() {
    std::printf("W2d2 EARU EAYCBM-Z-1P\n");
    resolves({"TS0601", "_TZE204_davzgqq0", "EAYCBM-Z-1P"});
    check(claims("_TZE204_davzgqq0") == 1, "EAYCBM manu listed by exactly one def");
    const PreparedDefinition& d = def_for("TS0601", "_TZE204_davzgqq0");
    check(has_wl(d, "EARU", "EAYCBM-Z-2P"), "EAYCBM white label EARU EAYCBM-Z-2P");

    check(approx(float_of(dp_num(d, 1, 12345), "energy"), 123.45f), "EAYCBM DP1 divideBy100 -> energy 123.45");
    const std::uint8_t ph[] = {0x09, 0x01, 0x01, 0x27, 0x10, 0x00, 0x03, 0xE8};
    const Out o = dp_bytes(d, 6, 0x00, std::span<const std::uint8_t>(ph, sizeof(ph)));
    check(approx(float_of(o, "voltage"), 230.5f) && approx(float_of(o, "current"), 10.0f) && float_of(o, "power") == 1000.0f,
          "EAYCBM DP6 phaseVariant2 narrow read -> 230.5 V, current bytes 3-4 only (10 A), power 1000 W");

    const std::uint8_t lk[] = {0x04, 0x01, 0x00, 0x1E};
    const Out l = dp_bytes(d, 17, 0x00, std::span<const std::uint8_t>(lk, 4));
    check(bool_is(l, "leakage_breaker", true) && float_of(l, "leakage_threshold") == 30.0f,
          "EAYCBM DP17 thresholdBlocks code 4 -> leakage_breaker ON, leakage_threshold 30 mA");
    const std::uint8_t th[] = {0x01, 0x01, 0x00, 0x28, 0x03, 0x00, 0x01, 0x0E, 0x04, 0x01, 0x00, 0xB4};
    const Out t = dp_bytes(d, 18, 0x00, std::span<const std::uint8_t>(th, sizeof(th)));
    check(bool_is(t, "over_current_breaker", true) && float_of(t, "over_current_threshold") == 40.0f,
          "EAYCBM DP18 code 1 -> over_current_breaker ON, over_current_threshold 40 A");
    check(bool_is(t, "over_voltage_breaker", false) && float_of(t, "over_voltage_threshold") == 270.0f &&
          float_of(t, "under_voltage_threshold") == 180.0f,
          "EAYCBM DP18 codes 3/4 -> over_voltage_breaker OFF 270 V, under_voltage_threshold 180 V");

    check(is_dp(encode(d, "state", boolean(true)), 16, 0x01, 1), "EAYCBM state ON -> DP16 bool true");
    check(!encode(d, "over_current_threshold", num(32)).ok, "EAYCBM thresholds read-only (thresholdBlocks RMW deferred)");
    check(has_expose(d, "leakage_threshold") && !has_expose(d, "breaker_id"),
          "EAYCBM exposes leakage_threshold, no breaker_id (string DP19 deferred)");
}

void w2_d2_07p4() {
    std::printf("W2d2 Tuya 07P4\n");
    resolves({"TS0601", "_TZE284_0or4eirj", "07P4"});
    check(claims("_TZE284_0or4eirj") == 1, "07P4 manu listed by exactly one def");
    const PreparedDefinition& d = def_for("TS0601", "_TZE284_0or4eirj");
    check(d.tuya_time_start == 1, "07P4 tuyaBase timeStart 1970 -> tuya_time_start 1");

    check(str_is(dp_enum(d, 3, 0), "action", "scene_3"), "07P4 DP3 static -> action scene_3");
    check(str_is(dp_bool(d, 8, true), "action", "scene_8"), "07P4 DP8 static (any value) -> action scene_8");
    check(bool_is(dp_bool(d, 25, true), "state_l2", true), "07P4 DP25 onOff true -> state_l2 ON");
    check(str_is(dp_enum(d, 41, 2), "power_on_behavior_l3", "previous"), "07P4 DP41 enum 2 -> power_on_behavior_l3 previous");
    check(str_is(dp_enum(d, 121, 2), "cover_state_l3", "close"), "07P4 DP121 enum 2 -> cover_state_l3 close");
    check(float_of(dp_num(d, 138, 42), "cover_position_l4") == 42.0f, "07P4 DP138 raw 42 -> cover_position_l4 42");

    check(is_dp(encode(d, "state_l1", boolean(true)), 24, 0x01, 1), "07P4 state_l1 ON -> DP24 bool true");
    check(is_dp(encode(d, "cover_state_l2", str("stop")), 120, 0x04, 1), "07P4 cover_state_l2 stop -> DP120 enum 1");
    check(is_dp(encode(d, "led_warm_l3", num(55)), 133, 0x02, 55), "07P4 led_warm_l3 55 -> DP133 value 55");

    const Expose* x = find_expose(d, "cover_position_l1");
    check(x && x->unit && std::strcmp(x->unit, "%") == 0 && has_expose(d, "action") && !has_expose(d, "switch_name_l1"),
          "07P4 exposes cover_position_l1 %, action; no utf8 names (deferred)");
}

// Call order: w2_d2_d1z(); w2_d2_eaycbm(); w2_d2_07p4();

// ── W2 group c ──
// ── W2 tag c: ZCL Tuya switches / plugs / ZP01 ──────────────────────

Value w2_c_int(std::int64_t i) { Value v{}; v.type = ValueType::Int; v.i = i; return v; }
// tuyaTz.on_off_countdown: genOnOff onWithTimedOff (cmd 0x42), ontime = offwaittime = s.
bool w2_c_is_countdown(const Encoded& e, std::uint16_t s) {
    return e.ok && e.cluster == 0x0006 && e.frame.size() == 8 && e.frame[2] == 0x42 &&
           e.frame[4] == (s & 0xFF) && e.frame[5] == (s >> 8) && e.frame[6] == (s & 0xFF);
}
bool w2_c_tz_on_ep(const PreparedDefinition& d, const char* key, std::uint8_t ep) {
    for (std::size_t i = 0; i < d.to_zigbee_count; ++i)
        if (d.to_zigbee[i] && d.to_zigbee[i]->key && std::strcmp(d.to_zigbee[i]->key, key) == 0)
            return d.to_zigbee[i]->endpoint == ep;
    return false;
}

void w2_c_ls12136() {
    std::printf("W2c Lupus LS12136 (TS011F _TZ3000_9dm33lp8)\n");
    resolves({"TS011F", "_TZ3000_9dm33lp8", "LS12136"});
    check(claims("_TZ3000_9dm33lp8") == 1, "_TZ3000_9dm33lp8 claimed by one def");
    const PreparedDefinition& d = def_for("TS011F", "_TZ3000_9dm33lp8");
    Report s; s.u8(0x0000, 0x10, 1);
    check(r_bool(dispatch(d, 0x0006, 2, s.span()), "state_right", true), "LS12136 EP2 onOff 1 -> state_right ON (endpoint right: 2)");
    Report p; p.u8(0x8002, 0x30, 2);
    check(r_str(dispatch(d, 0x0006, 1, p.span()), "power_outage_memory_left", "restore"),
          "LS12136 genOnOff 0x8002 2 -> power_outage_memory restore (tuyaFz.power_outage_memory)");
    Report c; c.u8(0x8000, 0x10, 1);
    check(r_bool(dispatch(d, 0x0006, 1, c.span()), "child_lock_left", true), "LS12136 genOnOff 0x8000 1 -> child_lock LOCK");
    Report m; m.u8(0x8001, 0x30, 1);
    check(r_str(dispatch(d, 0x0006, 1, m.span()), "indicator_mode_left", "off/on"), "LS12136 0x8001 1 -> indicator_mode off/on");
    check(is_write(encode(d, "power_outage_memory", str("restore")), 0x0006, 0x8002, 0x30, 2),
          "LS12136 power_outage_memory restore -> genOnOff 0x8002 enum8 2");
    check(is_write(encode(d, "child_lock", boolean(true)), 0x0006, 0x8000, 0x10, 1), "LS12136 child_lock -> genOnOff 0x8000 bool");
    check(has_expose(d, "state_left") && has_expose(d, "power_outage_memory") && has_expose(d, "child_lock") &&
          has_expose(d, "indicator_mode") && !has_expose(d, "power_on_behavior") && !has_expose(d, "power"),
          "LS12136 exposes: switch left/right, power_outage_memory, indicator_mode, child_lock; no POB, no metering");
}

void w2_c_sa_t2() {
    std::printf("W2c Nova Digital SA-T2 (TS011F _TZ3210_z1kba38n)\n");
    resolves({"TS011F", "_TZ3210_z1kba38n", "SA-T2"});
    const PreparedDefinition& d = def_for("TS011F", "_TZ3210_z1kba38n");
    Report t; t.u16(0x4001, 0x21, 120);
    check(r_float(dispatch(d, 0x0006, 2, t.span()), "countdown_l2") == 120.0f, "SA-T2 EP2 onTime 120 -> countdown_l2 120 (tuyaFz.on_off_countdown)");
    Report p; p.u8(0x8002, 0x30, 0);
    check(r_str(dispatch(d, 0x0006, 1, p.span()), "power_outage_memory_l1", "off"), "SA-T2 0x8002 0 -> power_outage_memory off");
    check(w2_c_is_countdown(encode(d, "countdown", w2_c_int(60)), 60), "SA-T2 countdown 60 -> onWithTimedOff 60 s");
    check(is_write(encode(d, "indicator_mode", str("on")), 0x0006, 0x8001, 0x30, 3), "SA-T2 indicator_mode on -> 0x8001 3 (backlight_indicator_mode_1)");
    const Expose* c = find_expose(d, "countdown_l2");
    check(c && c->value_max == 43200 && has_expose(d, "child_lock") && has_expose(d, "power_outage_memory") &&
          !has_expose(d, "power_on_behavior"), "SA-T2 exposes countdown_l1/l2 0-43200 s, child_lock, power_outage_memory");
}

void w2_c_zp01() {
    std::printf("W2c Tuya ZP01 (TS0202 _TZ3000_lf56vpxj / _TZ3040_o4mkahkc)\n");
    resolves({"TS0202", "_TZ3000_lf56vpxj", "ZP01"});
    resolves({"TS0202", "_TZ3040_o4mkahkc", "ZP01"});
    const PreparedDefinition& d = def_for("TS0202", "_TZ3000_lf56vpxj");
    const std::uint8_t ias[] = {0x19, 0x01, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00};
    const DispatchResult a = dispatch(d, 0x0500, 1, std::span<const std::uint8_t>(ias, sizeof(ias)));
    check(r_bool(a, "occupancy", true) && r_bool(a, "tamper", true) && r_bool(a, "battery_low", false),
          "ZP01 zoneStatus 0x0005 notification -> occupancy + tamper (fz.ias_occupancy_alarm_1)");
    Report z; z.u16(0x0002, 0x19, 0x0008);
    const DispatchResult b = dispatch(d, 0x0500, 1, z.span());
    check(r_bool(b, "occupancy", false) && r_bool(b, "battery_low", true),
          "ZP01 zoneStatus attr 0x0008 -> battery_low, no occupancy (fz.ias_occupancy_alarm_1_report)");
    Report k; k.u8(0x0013, 0x20, 2).u8(0xF001, 0x20, 1);
    const DispatchResult r = dispatch(d, 0x0500, 1, k.span());
    check(r_str(r, "sensitivity", "high") && r_str(r, "keep_time", "60"), "ZP01 0x0013 2 / 0xF001 1 -> sensitivity high, keep_time 60 (ZM35HQ_attr)");
    check(is_write(encode(d, "keep_time", str("120")), 0x0500, 0xF001, 0x20, 2), "ZP01 keep_time 120 -> ssIasZone 0xF001 uint8 2");
    check(is_write(encode(d, "sensitivity", str("low")), 0x0500, 0x0013, 0x20, 0), "ZP01 sensitivity low -> ssIasZone 0x0013 0");
    check(has_expose(d, "sensitivity") && has_expose(d, "keep_time") && has_expose(d, "tamper") && has_binding(d, 0x0001),
          "ZP01 exposes sensitivity / keep_time / tamper, binds genPowerCfg");
}

void w2_c_a8mhmsol() {
    std::printf("W2c Tuya _TZ3000_a8mhmsol (TS0002)\n");
    resolves({"TS0002", "_TZ3000_a8mhmsol", "_TZ3000_a8mhmsol"});
    const PreparedDefinition& d = def_for("TS0002", "_TZ3000_a8mhmsol");
    Report st; st.u8(0xD030, 0x30, 2);
    check(r_str(dispatch(d, 0xE001, 1, st.span()), "switch_type_l1", "momentary"), "a8mhmsol 0xE001 0xD030 2 -> switch_type momentary");
    Report bl; bl.u8(0x5000, 0x30, 1);
    check(r_bool(dispatch(d, 0x0006, 1, bl.span()), "backlight_mode_l1", true), "a8mhmsol genOnOff 0x5000 1 -> backlight_mode ON (tuyaBacklightSwitch)");
    Report pb; pb.u8(0x8002, 0x30, 2);
    check(r_str(dispatch(d, 0x0006, 1, pb.span()), "power_on_behavior_l1", "previous"), "a8mhmsol 0x8002 2 -> power_on_behavior previous (moesStartUpOnOff)");
    Report cd; cd.u16(0x4001, 0x21, 30);
    check(r_float(dispatch(d, 0x0006, 2, cd.span()), "countdown_l2") == 30.0f, "a8mhmsol EP2 onTime 30 -> countdown_l2");
    check(is_write(encode(d, "backlight_mode", boolean(true)), 0x0006, 0x5000, 0x30, 1), "a8mhmsol backlight_mode ON -> genOnOff 0x5000 1");
    check(is_write(encode(d, "power_on_behavior", str("previous")), 0x0006, 0x8002, 0x30, 2) &&
          w2_c_tz_on_ep(d, "power_on_behavior", 1), "a8mhmsol power_on_behavior previous -> 0x8002 2 on EP1 (multiEndpointSkip)");
    check(has_expose(d, "power_on_behavior") && has_expose(d, "switch_type") && has_expose(d, "indicator_mode") &&
          has_expose(d, "countdown_l2") && has_binding(d, 0x0006), "a8mhmsol exposes POB, switch_type, indicator_mode, countdown_l1/l2");
}

void w2_c_ts011f_5() {
    std::printf("W2c Tuya TS011F_5 5-gang strip\n");
    for (const char* m : {"_TZ3000_cfnprab5", "_TZ3000_o005nuxx", "_TZ3000_gdyjfvgm", "_TZ3000_pl5v1yyy", "_TZ3000_djgzdba9"}) {
        resolves({"TS011F", m, "TS011F_5"});
        check(claims(m) == 1, "TS011F_5 manu claimed by one def only");
    }
    const PreparedDefinition& d = def_for("TS011F", "_TZ3000_cfnprab5");
    Report s; s.u8(0x0000, 0x10, 1);
    check(r_bool(dispatch(d, 0x0006, 5, s.span()), "state_l5", true), "TS011F_5 EP5 onOff -> state_l5 (endpoint l5: 5)");
    Report p; p.u8(0x8002, 0x30, 2);
    check(r_str(dispatch(d, 0x0006, 1, p.span()), "power_outage_memory_l1", "restore"), "TS011F_5 0x8002 2 -> power_outage_memory restore");
    Report c; c.u8(0x8000, 0x10, 0);
    check(r_bool(dispatch(d, 0x0006, 1, c.span()), "child_lock_l1", false), "TS011F_5 0x8000 0 -> child_lock UNLOCK");
    check(is_write(encode(d, "power_outage_memory", str("on")), 0x0006, 0x8002, 0x30, 1) &&
          w2_c_tz_on_ep(d, "power_outage_memory", 1), "TS011F_5 power_outage_memory on -> 0x8002 1 on EP1 only (#13611)");
    check(w2_c_is_countdown(encode(d, "countdown", w2_c_int(300)), 300), "TS011F_5 countdown 300 -> onWithTimedOff");
    check(has_expose(d, "state_l5") && has_expose(d, "countdown_l5") && has_expose(d, "child_lock") &&
          has_wl(d, "UseeLink", "SM-O301-AZ") && has_wl(d, "Lotus", "Ref 2117") && d.endpoint_map_count == 5,
          "TS011F_5 exposes l1..l5 state + countdown, child_lock; white labels UseeLink / Lotus");
}

void w2_c_udl7uyd2() {
    std::printf("W2c Nova Digital SA-1 (TS0001 _TZ3000_udl7uyd2)\n");
    resolves({"TS0001", "_TZ3000_udl7uyd2", "TS0001"});
    check(claims("_TZ3000_udl7uyd2") == 1, "_TZ3000_udl7uyd2 claimed by one def");
    const PreparedDefinition& d = def_for("TS0001", "_TZ3000_udl7uyd2");
    check(has_manu(d, "_TZ3000_udl7uyd2") && !has_manu(d, "_TZ3000_p26flek3"), "udl7uyd2 has its own def, not the p26flek3 one");
    Report bl; bl.u8(0x5000, 0x30, 0);
    check(r_bool(dispatch(d, 0x0006, 1, bl.span()), "backlight_mode", false), "udl7uyd2 0x5000 0 -> backlight_mode OFF (backlight_mode_off_on)");
    Report im; im.u8(0x8001, 0x30, 2);
    check(r_str(dispatch(d, 0x0006, 1, im.span()), "indicator_mode", "on/off"), "udl7uyd2 0x8001 2 -> indicator_mode on/off");
    Report cd; cd.u16(0x4001, 0x21, 90);
    check(r_float(dispatch(d, 0x0006, 1, cd.span()), "countdown") == 90.0f, "udl7uyd2 onTime 90 -> countdown");
    check(is_write(encode(d, "backlight_mode", boolean(true)), 0x0006, 0x5000, 0x30, 1), "udl7uyd2 backlight_mode ON -> tuyaBacklightSwitch 0x5000");
    check(w2_c_is_countdown(encode(d, "countdown", w2_c_int(30)), 30), "udl7uyd2 countdown 30 -> onWithTimedOff");
    check(has_expose(d, "switch_type") && has_expose(d, "indicator_mode") && has_expose(d, "countdown") &&
          !has_expose(d, "power_on_behavior") && has_wl(d, "Nova Digital", "SA-1"),
          "udl7uyd2 exposes switch_type / backlight / indicator / countdown, no power_on_behavior");
}

void w2_c_mw1pqqqt() {
    std::printf("W2c Tuya _TZ3000_mw1pqqqt 3-ch USB switch (+ _TZ3210_n0wbkysi)\n");
    resolves({"TS0003", "_TZ3000_mw1pqqqt", "_TZ3000_mw1pqqqt"});
    resolves({"TS0003", "_TZ3210_n0wbkysi", "_TZ3000_mw1pqqqt"});
    check(claims("_TZ3210_n0wbkysi") == 1, "_TZ3210_n0wbkysi claimed by one def");
    const PreparedDefinition& d = def_for("TS0003", "_TZ3210_n0wbkysi");
    Report s; s.u8(0x0000, 0x10, 1);
    check(r_bool(dispatch(d, 0x0006, 3, s.span()), "state_l3", true), "mw1pqqqt EP3 onOff -> state_l3 (endpoint l3: 3)");
    Report cd; cd.u16(0x4001, 0x21, 600);
    check(r_float(dispatch(d, 0x0006, 1, cd.span()), "countdown_l1") == 600.0f, "mw1pqqqt EP1 onTime 600 -> countdown_l1");
    Report pb; pb.u8(0x8002, 0x30, 1);
    check(r_str(dispatch(d, 0x0006, 2, pb.span()), "power_on_behavior_l2", "on"), "mw1pqqqt EP2 0x8002 1 -> power_on_behavior_l2 on");
    check(is_write(encode(d, "power_on_behavior", str("off")), 0x0006, 0x8002, 0x30, 0), "mw1pqqqt power_on_behavior off -> 0x8002 0");
    check(has_expose(d, "state_l3") && has_expose(d, "countdown_l3") && has_expose(d, "power_on_behavior") &&
          !has_expose(d, "state"), "mw1pqqqt exposes state_l1..l3, countdown_l1..l3, power_on_behavior");
}

// Call order: w2_c_ls12136, w2_c_sa_t2, w2_c_zp01, w2_c_a8mhmsol, w2_c_ts011f_5, w2_c_udl7uyd2, w2_c_mw1pqqqt

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
    // Wave 2 (Tuya)
    w2_switch_6_gang();
    w2_small_rows();
    w2_hhst001();
    w2_spm();
    w2_dp_rows();
    w2_ledgered();
    w2_a_g08a_z20t();
    w2_a_n02_z10t();
    w2_a_sa_wk_4x4();
    w2_a_nd_4gang();
    w2_a_zmd_208_2();
    w2_a_girier();
    w2_b1_hs206z();
    w2_b1_hs207z();
    w2_b1_hs209z();
    w2_b1_hs300tf();
    w2_b1_jkd839z();
    w2_b1_x7633();
    w2_b2_lqhqj73n();
    w2_b2_t3e();
    w2_b2_hy607w();
    w2_b2_cover4();
    w2_d1_fontastic();
    w2_d1_me173();
    w2_d1_sp107e();
    w2_d2_d1z();
    w2_d2_eaycbm();
    w2_d2_07p4();
    w2_c_ls12136();
    w2_c_sa_t2();
    w2_c_zp01();
    w2_c_a8mhmsol();
    w2_c_ts011f_5();
    w2_c_udl7uyd2();
    w2_c_mw1pqqqt();
    if (g_failures) { std::printf("FAILED: %d check(s)\n", g_failures); return 1; }
    std::printf("all parity-26133 checks passed\n");
    return 0;
}
