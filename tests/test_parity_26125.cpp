// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// z2m v26.115.1 → v26.125.0 (zigbee2mqtt 2.14.3) parity window. Pins what
// was hand-written this window; R9 triage in
// zhac-tools/parity/catchup_26115_26125/R9_TRIAGE.md.
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
#include "zhc/devices/candeo_registry.hpp"
#include "zhc/devices/develco_registry.hpp"
#include "zhc/devices/efekta_registry.hpp"
#include "zhc/devices/heiman_registry.hpp"
#include "zhc/devices/immax_registry.hpp"
#include "zhc/devices/ledvance_registry.hpp"
#include "zhc/devices/legrand_registry.hpp"
#include "zhc/devices/lincukoo_registry.hpp"
#include "zhc/devices/lumi_registry.hpp"
#include "zhc/devices/moes_registry.hpp"
#include "zhc/devices/namron_registry.hpp"
#include "zhc/devices/owon_registry.hpp"
#include "zhc/devices/philips_registry.hpp"
#include "zhc/devices/qa_registry.hpp"
#include "zhc/devices/saswell_registry.hpp"
#include "zhc/devices/schneider_registry.hpp"
#include "zhc/devices/sinope_registry.hpp"
#include "zhc/devices/sonoff_registry.hpp"
#include "zhc/devices/third_reality_registry.hpp"
#include "zhc/devices/tier_e_registries.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/devices/zemismart_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"
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
    return dispatch_from_zigbee(msg, {}, def, raw, ctx_in ? *ctx_in : test_ctx());
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


// The registries the new devices land in, plus the ones whose definitions
// claim the same zigbeeModels (generic TS0001 / TS0202 / TLSR82xx), in the
// adapter's order: vendor registries first, tier E last.
const std::vector<const PreparedDefinition*>& registries() {
    static std::vector<const PreparedDefinition*> reg;
    if (!reg.empty()) return reg;
    auto add = [&](const PreparedDefinition* const* r, std::size_t n) { reg.insert(reg.end(), r, r + n); };
    add(devices::tuya::kTuyaRegistry, devices::tuya::kTuyaRegistryCount);
    add(devices::immax::kImmaxRegistry, devices::immax::kImmaxRegistryCount);
    add(devices::qa::kQaRegistry, devices::qa::kQaRegistryCount);
    add(devices::owon::kOwonRegistry, devices::owon::kOwonRegistryCount);
    add(devices::candeo::kCandeoRegistry, devices::candeo::kCandeoRegistryCount);
    add(devices::sonoff::kSonoffRegistry, devices::sonoff::kSonoffRegistryCount);
    add(devices::lumi::kLumiRegistry, devices::lumi::kLumiRegistryCount);
    add(devices::zemismart::kZemismartRegistry, devices::zemismart::kZemismartRegistryCount);
    add(devices::legrand::kLegrandRegistry, devices::legrand::kLegrandRegistryCount);
    add(devices::sinope::kSinopeRegistry, devices::sinope::kSinopeRegistryCount);
    add(devices::philips::kPhilipsRegistry, devices::philips::kPhilipsRegistryCount);
    add(devices::moes::kMoesRegistry, devices::moes::kMoesRegistryCount);
    add(devices::lincukoo::kLincukooRegistry, devices::lincukoo::kLincukooRegistryCount);
    add(devices::efekta::kEfektaRegistry, devices::efekta::kEfektaRegistryCount);
    add(devices::ledvance::kLedvanceRegistry, devices::ledvance::kLedvanceRegistryCount);
    add(devices::schneider::kSchneiderRegistry, devices::schneider::kSchneiderRegistryCount);
    add(devices::third_reality::kThirdRealityRegistry, devices::third_reality::kThirdRealityRegistryCount);
    add(devices::develco::kDevelcoRegistry, devices::develco::kDevelcoRegistryCount);
    add(devices::namron::kNamronRegistry, devices::namron::kNamronRegistryCount);
    add(devices::heiman::kHeimanRegistry, devices::heiman::kHeimanRegistryCount);
    add(devices::saswell::kSaswellRegistry, devices::saswell::kSaswellRegistryCount);
    for (std::size_t i = 0; i < devices::tier_e::kTierERegistriesCount; ++i)
        add(devices::tier_e::kTierERegistries[i].reg, devices::tier_e::kTierERegistries[i].count);
    return reg;
}
struct Want { const char* zm; const char* manu; const char* model; };
void resolves(std::span<const PreparedDefinition* const> all, const Want& w) {
    const PreparedDefinition* d = find_definition(w.zm, w.manu, all);
    if (!d || std::strcmp(d->model, w.model) != 0) {
        std::printf("  %s/%s -> %s (want %s)\n", w.zm, w.manu, d ? d->model : "none", w.model);
        check(false, "new device resolves");
    }
}
const PreparedDefinition& def_for(const char* zm, const char* manu) {
    static const PreparedDefinition kNone{};
    const PreparedDefinition* d = find_definition(zm, manu, registries());
    if (!d) { std::printf("  %s/%s resolves to nothing\n", zm, manu); check(false, "definition found"); }
    return d ? *d : kNone;
}

// ssIasZone commandStatusChangeNotification carrying `status`.
std::vector<std::uint8_t> ias_cmd(std::uint16_t status) {
    return {0x19, 0x01, 0x00, std::uint8_t(status & 0xFF), std::uint8_t(status >> 8), 0x00, 0x01, 0x00, 0x00};
}
DispatchResult feed(const PreparedDefinition& def, std::uint16_t cluster, std::uint8_t ep,
                    const std::vector<std::uint8_t>& f) {
    return dispatch(def, cluster, ep, std::span<const std::uint8_t>(f.data(), f.size()));
}
// Tuya setData frame: [fc tsn cmd][seq:2][dp][type][len:2][value].
std::int32_t tz_value(const Encoded& e) {
    if (!e.ok || e.frame.size() < 10) return INT32_MIN;
    std::uint32_t v = 0;
    for (std::size_t i = 9; i < e.frame.size(); ++i) v = (v << 8) | e.frame[i];
    return static_cast<std::int32_t>(v);
}

// ── wave 1: owner devices and small real bugs ──────────────────────

// herdsman #1924: newer _TZE284_ devices report datapoints with EF00 command
// 0x0A (`dataReportAlt`), same payload as dataReport.
void herdsman_data_report_alt() {
    std::printf("EF00 cmd 0x0A dataReportAlt\n");
    for (std::uint8_t cmd : {std::uint8_t(0x02), std::uint8_t(0x0A), std::uint8_t(0x0B)}) {
        const std::uint8_t f[] = {0x09, 0x01, cmd, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01};
        InboundApsFrame raw{};
        raw.cluster_id = 0xEF00; raw.src_endpoint = 1; raw.dst_endpoint = 1;
        raw.data = std::span<const std::uint8_t>(f, sizeof(f));
        DecodedMessage msg{};
        check(decode_frame(raw, {}, msg), "decodes");
        const bool dp = msg.family == FrameFamily::TuyaDp;
        check(cmd == 0x0B ? !dp : dp, cmd == 0x0B ? "0x0B stays Zcl" : "0x02 / 0x0A are TuyaDp");
    }
}

// HOBEIAN ZG-102ZA (owner): tamper + battery_low from zoneStatus, command and report.
void zg_102za() {
    std::printf("ZG-102ZA tamper / battery_low\n");
    const PreparedDefinition& d = def_for("ZG-102Z", "HOBEIAN");
    check(has_expose(d, "tamper") && has_expose(d, "battery_low"), "exposes tamper + battery_low");
    auto r = feed(d, 0x0500, 1, ias_cmd(0x000D));
    check(r_bool(r, "contact", false) && r_bool(r, "tamper", true) && r_bool(r, "battery_low", true),
          "cmd 0x000D -> open, tamper, battery_low");
    Report rep; rep.u16(0x0002, 0x19, 0x0004);
    r = dispatch(d, 0x0500, 1, rep.span());
    check(r_bool(r, "contact", true) && r_bool(r, "tamper", true) && r_bool(r, "battery_low", false),
          "report 0x0004 -> closed, tamper");
}

// Saswell SEA801 (owner def): Nous H1Z 3ymoslep fingerprints.
void sea801_nous() {
    std::printf("SEA801 Nous H1Z 3ymoslep\n");
    for (const char* m : {"_TZE200_3ymoslep", "_TZE204_3ymoslep", "_TZE284_3ymoslep", "_TZE2841000000_3ymoslep"})
        resolves(registries(), {"TS0601", m, "SEA801-Zigbee/SEA802-Zigbee"});
}

// phaseVariant2WithPhase on the clamp meters: DP6/7/8 were missing; two's
// complement negative power (upstream capture 0xFFFFF7 = -9 W).
void clamp_meter_phases() {
    std::printf("clamp meter DP6/7/8\n");
    const std::uint8_t neg9[] = {9, 38, 0, 0, 29, 255, 255, 247};
    const std::span<const std::uint8_t> b(neg9, sizeof(neg9));
    for (const char* m : {"_TZE200_nslr42tt", "_TZE200_wbhaespm", "_TZE204_wbhaespm", "_TZE284_wbhaespm"}) {
        const PreparedDefinition& d = def_for("TS0601", m);
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_raw(d, 8, b, ctx, out) && float_of(out, "power_c") == -9.0f &&
              approx(float_of(out, "voltage_c"), 234.2f) && approx(float_of(out, "current_c"), 0.029f, 0.0005f),
              "DP8 -> phase c, -9 W");
    }
    const PreparedDefinition& d = def_for("TS0601", "_TZE204_m64smti7");
    RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    check(dp_raw(d, 6, b, ctx, out) && float_of(out, "power_a") == -9.0f, "m64smti7 DP6 -> phase a");
}

// SONOFF SNZB-03 / SNZB-05P (and Besisglas MS01 / WL01): typed IAS keys.
void sonoff_ias_keys() {
    std::printf("SNZB-03 occupancy / SNZB-05P water_leak\n");
    const PreparedDefinition& m = def_for("SNZB-03", "eWeLink");
    auto r = feed(m, 0x0500, 1, ias_cmd(0x0001));
    check(r_bool(r, "occupancy", true) && !r_find(r, "alarm"), "SNZB-03 -> occupancy");
    check(has_expose(m, "occupancy") && !has_expose(m, "alarm"), "SNZB-03 exposes occupancy");
    check(&def_for("MS01", "zbeacon") == &m, "Besisglas MS01 rides on SNZB-03");
    const PreparedDefinition& w = def_for("SNZB-05P", "SONOFF");
    r = feed(w, 0x0500, 1, ias_cmd(0x0009));
    check(r_bool(r, "water_leak", true) && r_bool(r, "battery_low", true) && !r_find(r, "alarm"),
          "SNZB-05P -> water_leak");
    check(has_expose(w, "water_leak") && !has_expose(w, "alarm"), "SNZB-05P exposes water_leak");
}

// Nimly locks: batteryPercentageRemaining is not halved (dontDividePercentage).
void nimly_battery() {
    std::printf("Nimly battery\n");
    Report rep; rep.u8(0x0021, 0x20, 80);
    const auto r = dispatch(def_for("NimlyPRO", "Onesti Products AS"), 0x0001, 11, rep.span());
    check(r_float(r, "battery") == 80.0f, "battery 80 -> 80 %");
}

// Heiman HS2VTD: contact on EP1 only, vibration (alarm_2) on EP2.
void hs2vtd_endpoints() {
    std::printf("HS2VTD EP1 contact / EP2 vibration\n");
    const PreparedDefinition& d = def_for("HS2VTD", "HEIMAN");
    auto r = feed(d, 0x0500, 2, ias_cmd(0x0002));
    check(r_bool(r, "vibration", true) && !r_find(r, "contact"), "EP2 alarm_2 -> vibration, no contact");
    r = feed(d, 0x0500, 1, ias_cmd(0x0009));
    check(r_bool(r, "contact", false) && r_bool(r, "battery_low", true) && !r_find(r, "vibration"),
          "EP1 alarm_1 -> open, battery_low");
    check(has_expose(d, "vibration"), "exposes vibration");
}

void namron_4566703() {
    std::printf("Namron 4566703\n");
    check(has_model(def_for("4566703", "NAMRON AS"), "4566703"), "4566703 recognised");
}

// Aqara MCCGQ13LM: tag 3 of the 0xF7 heartbeat is a constant 25 °C, ignored.
void mccgq13lm_no_temperature() {
    std::printf("MCCGQ13LM device_temperature\n");
    const std::vector<std::uint8_t> f{0x1C, 0x5F, 0x11, 0x31, 0x0A, 0xF7, 0x00, 0x41, 36,
        0x01, 0x21, 0xC5, 0x0B, 0x03, 0x28, 0x19, 0x04, 0x21, 0xA8, 0x13, 0x05,
        0x21, 0x11, 0x00, 0x06, 0x24, 0x02, 0x00, 0x00, 0x00, 0x00, 0x08, 0x21,
        0x1C, 0x01, 0x0A, 0x21, 0x00, 0x00, 0x0C, 0x20, 0x01, 0x64, 0x10, 0x00};
    const PreparedDefinition& d = def_for("lumi.magnet.ac01", "LUMI");
    check(std::strcmp(d.model, "MCCGQ13LM") == 0, "lumi.magnet.ac01 is MCCGQ13LM");
    const auto r = feed(d, 0xFCC0, 1, f);
    check(r_find(r, "battery") && !r_find(r, "device_temperature"), "battery, no device_temperature");
    check(!has_expose(d, "device_temperature"), "no device_temperature expose");
}

// AVATTO TS0601_thermostat_14: DP47 calibration /10 on the _TZE284_ copy, DP2 system_mode.
void vjpaih9f() {
    std::printf("vjpaih9f calibration / system_mode\n");
    for (const char* m : {"_TZE204_vjpaih9f", "_TZE284_vjpaih9f"}) {
        const PreparedDefinition& d = def_for("TS0601", m);
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_num(d, 47, -15, ctx, out) && approx(float_of(out, "local_temperature_calibration"), -1.5f),
              "DP47 -15 -> -1.5");
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o2{};
        check(dp_enum(d, 2, 6, ctx, o2) && str_is(o2, "system_mode", "off") && str_is(o2, "preset", "off"),
              "DP2 6 -> off / off");
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o3{};
        check(dp_enum(d, 2, 3, ctx, o3) && str_is(o3, "system_mode", "heat") && str_is(o3, "preset", "comfort"),
              "DP2 3 -> heat / comfort");
        const Encoded e = encode(d, "system_mode", str("auto"));
        check(e.ok && e.frame.size() == 10 && e.frame[5] == 2 && e.frame[9] == 1, "write auto -> DP2 enum 1");
    }
}

// Moes ZHT-002: DP2 is schedule_mode, no longer clobbers system_mode.
void zht_002() {
    std::printf("ZHT-002 schedule_mode\n");
    const PreparedDefinition& d = def_for("TS0601", "_TZE204_xalsoe3m");
    RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    check(dp_enum(d, 2, 1, ctx, out) && str_is(out, "schedule_mode", "manual") && !out.find("system_mode"),
          "DP2 1 -> schedule_mode manual");
    check(has_expose(d, "schedule_mode"), "exposes schedule_mode");
}

// Tuya time base: TRV602Z and ENGO EONE-230W/BATB are 1970 since v26.125.0.
void time_base() {
    std::printf("Tuya time base\n");
    const PreparedDefinition blank{};
    for (const char* m : {"_TZE204_ltwbm23f", "_TZE204_qyr2m29i", "_TZE284_ltwbm23f", "_TZE200_awnadkan",
                          "_TZE204_ca3i8m8p", "_TZE200_gtouvmvl", "_TZE284_stvgmdjz", "_TZE28C1000000_pmbxyf97"})
        check(tuya::time_start(blank, m, "TS0601") == 1, m);
    check(has_manu(def_for("TS0601", "_TZE284_stvgmdjz"), "_TZE284_stvgmdjz"), "SFL02-Z-1 _TZE284_stvgmdjz");
}

// Signed 8-bit datapoints: ZS-300Z calibrations, ZG-103Z axes.
void signed_8bit() {
    std::printf("signed 8-bit DPs\n");
    for (const char* m : {"_TZE284_k7p2q5d9", "_TZE284_65gzcss7", "_TZE284_0ints6wl", "_TZE284_yzr43ayq"}) {
        const PreparedDefinition& d = def_for("TS0601", m);
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_num(d, 104, 250, ctx, out) && float_of(out, "soil_calibration") == -6.0f, "DP104 250 -> -6");
        check(dp_num(d, 105, 5, ctx, out) && float_of(out, "humidity_calibration") == 5.0f, "DP105 5 -> 5");
        check(dp_num(d, 107, 251, ctx, out) && approx(float_of(out, "temperature_calibration"), -0.5f), "DP107 251 -> -0.5");
        check(dp_num(d, 106, 5, ctx, out) && float_of(out, "illuminance_calibration") == 50.0f, "DP106 5 -> 50 lx");
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o2{};
        check(dp_num(d, 110, 1, ctx, o2) && bool_is(o2, "soil_moisture_warning", true) &&
              !o2.find("soil_moisture_warning_threshold") && !o2.find("soil_warning"), "DP110 -> warning state");
        check(tz_value(encode(d, "soil_calibration", num(-6))) == 250, "write -6 -> 250");
        check(tz_value(encode(d, "temperature_calibration", num(-0.5))) == 251, "write -0.5 -> 251");
        check(tz_value(encode(d, "illuminance_calibration", num(50))) == 5, "write 50 lx -> 5");
        const Encoded e = encode(d, "soil_moisture_warning_threshold", num(30));
        check(e.ok && e.frame[5] == 110 && tz_value(e) == 30, "threshold write -> DP110 30");
        check(has_expose(d, "soil_moisture_warning_threshold") && !has_expose(d, "soil_warning"), "DP110 exposes");
    }
    const PreparedDefinition& z = def_for("ZG-103Z", "HOBEIAN");
    RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    check(dp_num(z, 101, 200, ctx, out) && float_of(out, "x") == -56.0f, "ZG-103Z x 200 -> -56");
    check(dp_num(z, 103, 127, ctx, out) && float_of(out, "z") == 127.0f, "ZG-103Z z 127 stays");
}

}  // namespace

int main() {
    std::printf("== z2m parity window v26.115.1 -> v26.125.0 ==\n");
    herdsman_data_report_alt();
    zg_102za();
    sea801_nous();
    clamp_meter_phases();
    sonoff_ias_keys();
    nimly_battery();
    hs2vtd_endpoints();
    namron_4566703();
    mccgq13lm_no_temperature();
    vjpaih9f();
    zht_002();
    time_base();
    signed_8bit();
    if (g_failures) { std::printf("FAILED: %d check(s)\n", g_failures); return 1; }
    std::printf("all parity-26125 checks passed\n");
    return 0;
}
