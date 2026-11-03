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
#include <map>
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
#include "zhc/devices/ikea_registry.hpp"
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
#include "zhc/devices/robb_registry.hpp"
#include "zhc/devices/saswell_registry.hpp"
#include "zhc/devices/schneider_registry.hpp"
#include "zhc/devices/shelly_registry.hpp"
#include "zhc/devices/sinope_registry.hpp"
#include "zhc/devices/sonoff_registry.hpp"
#include "zhc/devices/sunricher_registry.hpp"
#include "zhc/devices/third_reality_registry.hpp"
#include "zhc/devices/tier_e_registries.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/devices/yale_registry.hpp"
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
    add(devices::ikea::kIkeaRegistry, devices::ikea::kIkeaRegistryCount);
    add(devices::yale::kYaleRegistry, devices::yale::kYaleRegistryCount);
    add(devices::robb::kRobbRegistry, devices::robb::kRobbRegistryCount);
    add(devices::sunricher::kSunricherRegistry, devices::sunricher::kSunricherRegistryCount);
    add(devices::shelly::kShellyRegistry, devices::shelly::kShellyRegistryCount);
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
DispatchResult feed_ctx(const PreparedDefinition& def, std::uint16_t cluster, std::uint8_t ep,
                        const std::vector<std::uint8_t>& f, RuntimeContext& ctx) {
    return dispatch(def, cluster, ep, std::span<const std::uint8_t>(f.data(), f.size()), &ctx);
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

// ── wave 2: detections ─────────────────────────────────────────────

void detections() {
    std::printf("wave 2 detections\n");
    const Want w[] = {
        {"TS0225", "_TZ3210_ekjc2rzh", "TS0225_EKAZA"},
        {"TS0601", "_TZE284_hkdl5fmv", "TS0601_din_hoch"},
        {"TS0601", "_TZE28C1000000_19jwhi8c", "ZN-USC1U-HT"},
        {"TS0601", "_TZE28C1000000_yxun3xvr", "TS0601_1gang_switch"},
        {"TS0601", "_TZE204_zpvusbtv", "TS0601__TZE284_zpvusbtv"},
        {"TS0601", "_TZE204_rfpyqax9", "Pro Line X10"},
        {"TS0601", "_TZE284_goecjd1t", "ZWPM16"},
        {"TS0601", "_TZE284_adlblwab", "TS0601__TZE204_adlblwab"},
        {"TS0601", "_TZE284_xc7dve2g", "TS0601__TZE284_68utemio"},
        {"TS0601", "_TZE2841000000_8se38w3c", "TZ-ZT01_GA4"},
        {"929004276603", "Signify Netherlands B.V.", "929004276602"},
        {"LCX048", "Signify Netherlands B.V.", "929004284702"},
        {"929003811401_01", "Signify Netherlands B.V.", "929003046601"},
        {"929003811401_02", "Signify Netherlands B.V.", "929003046601"},
        {"LCE006", "Signify Netherlands B.V.", "929002294203"},
        {"YRD410-PB", "Yale", "YRD410-BLE"},
        {"ZTM2-EN", "IMOU", "ZTM1-EN"},
        {"TRADFRI bulb E17 CWS opal 600lm", "IKEA of Sweden", "LED1624G9"},
        {"SDM01G2W", "BITUO TECHNIK", "SDM01W-U01"},
        {"ZBM-MG21", "Inswift", "Silabs series 2 router"},
        {"SLZB-07MG26", "SMLIGHT", "Silabs series 2 router"},
    };
    for (const Want& x : w) resolves(registries(), x);
}

// TZ-ZT01_GA4 was a min stub; now decodes its four datapoints.
void tz_zt01_ga4() {
    std::printf("TZ-ZT01_GA4 datapoints\n");
    const PreparedDefinition& d = def_for("TS0601", "_TZE284_8se38w3c");
    RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    check(dp_num(d, 1, 215, ctx, out) && approx(float_of(out, "temperature"), 21.5f), "DP1 temperature /10");
    check(dp_num(d, 2, 48, ctx, out) && float_of(out, "humidity") == 48.0f, "DP2 humidity");
    check(dp_enum(d, 3, 1, ctx, out) && str_is(out, "battery_state", "medium"), "DP3 battery_state");
    check(dp_num(d, 38, -52, ctx, out) && approx(float_of(out, "temperature_probe"), -5.2f), "DP38 probe /10");
}

// ZM25TQ is a TS0601_cover_8 white-label now: inverted position, BOOL stroke limits.
void zm25tq_cover_8() {
    std::printf("ZM25TQ / cover_8\n");
    const PreparedDefinition& d = def_for("TS0601", "_TZE200_fzo2pocs");
    RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    check(dp_num(d, 3, 30, ctx, out) && float_of(out, "position") == 70.0f, "DP3 30 -> position 70");
    check(dp_bool(d, 103, true, ctx, out) && str_is(out, "upper_stroke_limit", "SET"), "DP103 true -> SET");
    check(dp_bool(d, 12, true, ctx, out) && bool_is(out, "motor_fault", true), "DP12 motor_fault");
    check(d.power_source_override == 0x01, "forcePowerSource mains");
    for (const char* m : {"_TZE200_fzo2pocs", "_TZE284_fzo2pocs", "_TZE204_r0jdjrvi", "_TZE200_g5xqosu7",
                          "_TZE204_g5xqosu7", "_TZE200_9vpe3fl1"}) {
        const Encoded e = encode(def_for("TS0601", m), "lower_stroke_limit", str("RESET"));
        check(e.ok && e.frame.size() == 10 && e.frame[5] == 105 && e.frame[6] == 0x01 && e.frame[9] == 0,
              "lower_stroke_limit RESET -> BOOL DP105 false");
    }
}

// Legrand WNP10: binds genIdentify / genOnOff / genBinaryInput, reports onOff.
void wnp10_configure() {
    std::printf("WNP10 configure\n");
    const PreparedDefinition& d = def_for("Hospitality on off plug", "Legrand");
    bool bin = false;
    for (std::size_t i = 0; i < d.bindings_count; ++i) bin |= d.bindings[i].cluster_id == 0x000F;
    check(bin && d.bindings_count == 3, "binds 0x0003 / 0x0006 / 0x000F");
    check(d.reports_count == 1 && d.reports[0].cluster_id == 0x0006 && d.reports[0].max_interval == 3600,
          "reports onOff 0..3600");
}

// ── wave 3: new devices ────────────────────────────────────────────

bool has_white_label(const PreparedDefinition& d, const char* model) {
    for (std::size_t i = 0; i < d.white_labels_count; ++i)
        if (std::strcmp(d.white_labels[i].model, model) == 0) return true;
    return false;
}

void new_devices() {
    std::printf("wave 3 new devices resolve\n");
    const Want w[] = {
        {"929004295101", "Signify Netherlands B.V.", "929004295101"},
        {"LCA022", "Signify Netherlands B.V.", "929003853601"},
        {"929004320901", "Signify Netherlands B.V.", "929004320901"},
        {"TOFSMYGGA plug outdoor", "IKEA of Sweden", "E2435-NA"},
        {"ROB_200-012-0", "ROBB smarrt", "ROB_200-012-0"},
        {"FH9127", "Futurehome AS", "FH9127"},
        {"3ROR0194Z", "Third Reality, Inc", "3ROR0194Z"},
        {"ZGRC-KEY-008", "Sunricher", "SR-ZG9001K8-CCT"},
        {"S2100-1001", "SIMON", "S2100-1001"},
        {"70E839TY", "SIMON", "70E839TY"},
        {"S2100-1002", "_TZ2000_mqovtiqe8m6av6po", "70E839TY"},
        {"S2100-1003", "_TZ2000_p4vghfngci5te7ol", "70E852TY"},
        {"ZG-204ZV-z", "HOBEIAN", "ZG-204ZV-z"},
        {"ZG-204ZV2-TH-z", "HOBEIAN", "ZG-204ZV2-TH-z"},
        {"ZG-204ZM-z", "HOBEIAN", "ZG-204ZS-z"},
        {"ZG-204Z", "HOBEIAN", "ZG-204Z"},
        {"4512797_98", "Namron AS", "4512797"},
        {"TRV602WZ", "Topband", "TRV602WZ"},
        {"Rem4 Heater", "Ariston", "Rem4 Heater"},
        {"teelicht-c6", "Teelicht", "teelicht"},
        {"BLU DoorWindow Mini ZB", "Shelly", "SBDW-203C"},
        {"BLU DoorWindow Outdoor ZB", "Shelly", "SBDW-302C"},
        {"TS0505B", "_TZ3210_ffuna0nr", "TS0505B_hs"},
        {"TS0503B", "_TZ3210_w7ge4ldo", "TS0503B_hs"},
        {"HS118Z", "HOBEIAN", "ZG-223Z"},
        {"TS0601", "_TZE284_tdhnhhiy", "SYZB-6W"},
        {"TS0601", "_TZE204_nvxorhcj", "ZTS-8W-B"},
        {"TS0601", "_TZE284_kow4ok3t", "ZTS-8B"},
        {"TS0601", "_TZE284_48ye65rk", "HYT001ZB"},
        {"TS0601", "_TZE284_fziifcxj", "APRM-04-005"},
        {"TS0601", "_TZE284_xr5yfele", "119710"},
        {"TS0601", "_TZE284_tdg4ckyh", "ZRF01"},
        {"TS0601", "_TZE28C1000000_f5efvtbv", "OXT-4CH-RELAY-AC"},
        {"TS0601", "_TZE204_m9dzckna", "OXT-4CH-RELAY-AC"},
        {"TS0601", "_TZE284_y7hpvgyp", "ZM6LT1"},
        {"TS0601", "_TZE200_xnwxmj8z", "MG-AU01ZLMV01-XX"},
        {"TS030F", "_TZ3210_hs01bacp", "ZC301"},
    };
    for (const Want& x : w) resolves(registries(), x);
    check(has_white_label(def_for("TS0601", "_TZE284_y7hpvgyp"), "ZM7LT1"), "ZM7LT1 white label");
    check(has_white_label(def_for("TS0601", "_TZE204_m9dzckna"), "OXT-4CH-RELAY-DC"), "OXT DC white label");
    check(std::strcmp(def_for("TS0601", "_TZE204_f5efvtbv").model, "OXT-4CH-RELAY-AC") != 0,
          "plain _TZE204_f5efvtbv stays on its own def");
}

void wave3_tuya_dps() {
    std::printf("wave 3 Tuya datapoints\n");
    {   // HYT001ZB
        const PreparedDefinition& d = def_for("TS0601", "_TZE284_48ye65rk");
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_num(d, 16, 215, ctx, out) && approx(float_of(out, "local_temperature"), 21.5f), "HYT DP16 /10");
        check(dp_bool(d, 102, true, ctx, out) && str_is(out, "running_state", "heat"), "HYT DP102 heat");
        check(dp_enum(d, 128, 2, ctx, out) && str_is(out, "preset", "holiday"), "HYT DP128 holiday");
        check(tz_value(encode(d, "system_mode", str("off"))) == 0, "HYT system_mode off -> DP125 false");
    }
    {   // APRM-04-005: DP2 carries preset and system_mode
        const PreparedDefinition& d = def_for("TS0601", "_TZE284_fziifcxj");
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_enum(d, 2, 5, ctx, out) && str_is(out, "preset", "antifrost") && str_is(out, "system_mode", "off"),
              "APRM DP2 5 -> antifrost / off");
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o2{};
        check(dp_enum(d, 2, 0, ctx, o2) && str_is(o2, "preset", "schedule") && str_is(o2, "system_mode", "heat"),
              "APRM DP2 0 -> schedule / heat");
        check(dp_enum(d, 15, 1, ctx, out) && bool_is(out, "window_open", true), "APRM DP15 1 -> window_open");
        Encoded e = encode(d, "system_mode", str("heat"));
        check(e.ok && e.frame[5] == 2 && tz_value(e) == 3, "APRM heat -> DP2 3");
        e = encode(d, "preset", str("antifrost"));
        check(e.ok && e.frame[5] == 2 && tz_value(e) == 5, "APRM antifrost -> DP2 5");
    }
    {   // Tervix 119710
        const PreparedDefinition& d = def_for("TS0601", "_TZE284_xr5yfele");
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(d.tuya_time_start == 1, "119710 time base 1970");
        check(dp_num(d, 27, -3, ctx, out) && float_of(out, "local_temperature_calibration") == -3.0f, "119710 DP27 signed");
        check(dp_enum(d, 118, 1, ctx, out) && str_is(out, "running_state", "heat"), "119710 DP118 heat");
        check(dp_num(d, 107, 30, ctx, out) && approx(float_of(out, "window_open_detection_temperature"), 3.0f),
              "119710 DP107 /10");
    }
    {   // ZRF01
        const PreparedDefinition& d = def_for("TS0601", "_TZE284_tdg4ckyh");
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_enum(d, 104, 1, ctx, out) && str_is(out, "button_4", "Saved"), "ZRF01 DP104 1 -> button_4 Saved");
        const Encoded e = encode(d, "start_3", str("Transmit"));
        check(e.ok && e.frame[5] == 111 && e.frame[6] == 0x04 && tz_value(e) == 0, "ZRF01 start_3 -> DP111 enum 0");
    }
    {   // OXT-4CH-RELAY-AC
        const PreparedDefinition& d = def_for("TS0601", "_TZE204_m9dzckna");
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_enum(d, 17, 2, ctx, out) && str_is(out, "power_on_behavior_l4", "previous"), "OXT DP17 -> l4 previous");
        check(dp_num(d, 8, 60, ctx, out) && float_of(out, "countdown_l2") == 60.0f, "OXT DP8 countdown_l2");
    }
    {   // Nova Digital ZTS-8B / ZTS-8W-B / SYZB-6W
        const PreparedDefinition& d = def_for("TS0601", "_TZE284_kow4ok3t");
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_bool(d, 102, true, ctx, out) && bool_is(out, "state_l8", true), "ZTS-8B DP102 -> state_l8");
        check(dp_bool(d, 16, true, ctx, out) && bool_is(out, "backlight_switch", true), "ZTS-8B DP16 backlight");
        const PreparedDefinition& w = def_for("TS0601", "_TZE204_nvxorhcj");
        check(dp_enum(w, 14, 2, ctx, out) && str_is(out, "power_on_behavior", "previous"), "ZTS-8W-B DP14 previous");
        const PreparedDefinition& s6 = def_for("TS0601", "_TZE284_tdhnhhiy");
        check(dp_bool(s6, 16, true, ctx, out) && bool_is(out, "backlight_switch", true), "SYZB-6W DP16 backlight");
        check(s6.power_source_override == 0x01 && s6.white_labels_count == 0, "SYZB-6W mains, no white labels");
    }
    {   // MakeGood MG-AU01ZLMV01-XX
        const PreparedDefinition& d = def_for("TS0601", "_TZE200_xnwxmj8z");
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(d.tuya_time_start == 1 && has_white_label(d, "SWS1TZ"), "MG-AU01 1970 + SWS1TZ");
        check(dp_num(d, 21, 1234, ctx, out) && approx(float_of(out, "current"), 1.234f, 0.0005f), "MG-AU01 DP21 /1000");
    }
}

// Moes ZC301: datapoints in, closuresWindowCovering out.
void zc301() {
    std::printf("ZC301 curtain motor\n");
    const PreparedDefinition& d = def_for("TS030F", "_TZ3210_hs01bacp");
    RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    check(dp_num(d, 3, 30, ctx, out) && float_of(out, "position") == 70.0f, "DP3 30 -> position 70");
    check(dp_enum(d, 7, 1, ctx, out) && str_is(out, "work_state", "opening") && str_is(out, "state", "OPEN"),
          "DP7 1 -> opening / OPEN");
    check(dp_enum(d, 5, 3, ctx, out) && bool_is(out, "calibration_completed", true), "DP5 3 -> calibrated");
    check(dp_num(d, 10, 12345, ctx, out) && approx(float_of(out, "time_total"), 12.345f, 0.0005f), "DP10 ms -> s");
    check(dp_bool(d, 107, true, ctx, out) && str_is(out, "curtain_close_direction", "DOWN"), "DP107 true -> DOWN");
    Encoded e = encode(d, "position", num(30));
    check(e.ok && e.cluster == 0x0102 && e.frame.size() == 4 && e.frame[2] == 0x05 && e.frame[3] == 70,
          "position 30 -> goToLiftPercentage 70");
    e = encode(d, "state", str("STOP"));
    check(e.ok && e.cluster == 0x0102 && e.frame[2] == 0x02, "state STOP -> stop");
    e = encode(d, "calibration", str("save_limit"));
    check(e.ok && e.cluster == 0x0102 && e.frame.size() == 3 && e.frame[2] == 0x02, "save_limit -> stop");
    e = encode(d, "calibration", str("up_start"));
    check(e.ok && e.cluster == 0x0102 && e.frame[2] == 0x02 && e.frame.size() == 7 &&
          e.frame[3] == 0x02 && e.frame[4] == 0xF0 && e.frame[5] == 0x30 && e.frame[6] == 1,
          "up_start -> write 0xF002 enum8 1");
    e = encode(d, "curtain_close_direction", str("DOWN"));
    check(e.ok && e.frame[5] == 107 && tz_value(e) == 1, "DOWN -> DP107 true");
    Report rep; rep.u8(0xF002, 0x30, 3);
    check(r_bool(dispatch(d, 0x0102, 1, rep.span()), "calibration_completed", true), "0xF002 3 -> calibrated");
}

void wave3_zcl() {
    std::printf("wave 3 ZCL decoders\n");
    {   // pvvx ZG-204ZS-z occupancy bitmap
        Report rep; rep.u8(0x0000, 0x18, 0x05);
        const auto r = dispatch(def_for("ZG-204ZS-z", "HOBEIAN"), 0x0406, 1, rep.span());
        check(r_bool(r, "occupancy", true) && r_bool(r, "motion", false) && r_bool(r, "presence", true),
              "ZG-204ZS-z bitmap 0b101");
    }
    {   // Namron 4512797 buttonEvent: code 8 -> double
        const std::vector<std::uint8_t> f{0x19, 0x01, 0x00, 0x01, 0x08};
        check(r_str(feed(def_for("4512797", "Namron AS"), 0xE004, 1, f), "action", "double"), "4512797 double");
        const std::vector<std::uint8_t> g{0x19, 0x01, 0x00, 0x01, 0x02};
        check(!r_find(feed(def_for("4512797", "Namron AS"), 0xE004, 1, g), "action"), "4512797 code 2 ignored");
    }
    {   // Shelly BLU DoorWindow: alarm1 only -> tilted
        const auto r = feed(def_for("BLU DoorWindow Mini ZB", "Shelly"), 0x0500, 1, ias_cmd(0x0001));
        check(r_str(r, "handle_position", "tilted") && r_bool(r, "contact", false), "SBDW alarm1 -> tilted");
        Report rep; rep.u16(0x0002, 0x19, 0x000B);
        const auto r2 = dispatch(def_for("BLU DoorWindow Outdoor ZB", "Shelly"), 0x0500, 1, rep.span());
        check(r_str(r2, "handle_position", "open") && r_bool(r2, "battery_low", true), "SBDW report 0x0B -> open");
    }
    {   // Topband TRV602WZ running state, Ariston outdoor temperature, teelicht speed
        Report rep; rep.u16(0x0029, 0x19, 0x0001);
        check(r_str(dispatch(def_for("TRV602WZ", "Topband"), 0x0201, 1, rep.span()), "running_state", "heat"),
              "TRV602WZ 0x0029 1 -> heat");
        Report out; out.u16(0x0001, 0x29, 1234);
        check(approx(r_float(dispatch(def_for("Rem4 Heater", "Ariston"), 0x0201, 1, out.span()), "outdoor_temperature"),
                     12.34f), "Rem4 outdoor_temperature /100");
        Report sp; sp.u8(0x0001, 0x20, 25);
        check(approx(r_float(dispatch(def_for("teelicht-c6", "Teelicht"), 0xFC00, 1, sp.span()), "flicker_speed"), 2.5f),
              "teelicht flicker_speed /10");
    }
}

// ── wave 4: M items + remaining S rows ─────────────────────────────

Value uval(std::uint32_t n) { Value v{}; v.type = ValueType::Uint; v.u = n; return v; }
bool has_report(const PreparedDefinition& d, std::uint8_t ep, std::uint16_t cluster, std::uint16_t attr,
                std::uint32_t change) {
    for (std::size_t i = 0; i < d.reports_count; ++i) {
        const ReportingSpec& r = d.reports[i];
        if (r.endpoint == ep && r.cluster_id == cluster && r.attr_id == attr && r.reportable_change == change)
            return true;
    }
    return false;
}
bool has_binding(const PreparedDefinition& d, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < d.bindings_count; ++i)
        if (d.bindings[i].endpoint == ep && d.bindings[i].cluster_id == cluster) return true;
    return false;
}
// A context with a one-device runtime store, for scratch-cached decoders.
struct StoreCtx {
    RuntimeStore<1> store{};
    RuntimeContext ctx{};
    StoreCtx() { ctx.store = &store; ctx.store_get = &RuntimeStore<1>::get; ctx.device_index = 0; }
};

void wave4_tuya() {
    std::printf("wave 4 Tuya\n");
    for (const char* m : {"_TZE200_bjzrowv2", "_TZE204_bjzrowv2", "_TZE284_bjzrowv2"})
        resolves(registries(), {"TS0601", m, "TS0601_cover_15"});
    {   // TS0601_cover_15
        const PreparedDefinition& d = def_for("TS0601", "_TZE200_bjzrowv2");
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_enum(d, 1, 2, ctx, out) && str_is(out, "state", "CLOSE"), "cover_15 DP1 2 -> CLOSE");
        check(dp_num(d, 3, 40, ctx, out) && float_of(out, "position") == 40.0f, "cover_15 DP3 -> position");
        check(dp_enum(d, 7, 2, ctx, out) && str_is(out, "motor_state", "stopped"), "cover_15 DP7 2 -> stopped");
        Encoded e = encode(d, "position", num(60));
        check(e.ok && e.frame[5] == 2 && tz_value(e) == 60, "cover_15 position -> DP2");
        e = encode(d, "motor_direction", str("reversed"));
        check(e.ok && e.frame[5] == 5 && tz_value(e) == 1, "cover_15 reversed -> DP5 1");
    }
    {   // genOnOff countdown read-back (LZWSM16-1, HG06620)
        Report on; on.u16(0x4001, 0x21, 120);
        check(r_float(dispatch(def_for("TS0011", "_TZ3000_hbxsdd6k"), 0x0006, 1, on.span()), "countdown") == 120.0f,
              "LZWSM16-1 onTime -> countdown");
        const PreparedDefinition& hg = def_for("TS0101", "_TZ3000_br3laukf");
        Report off; off.u16(0x4002, 0x21, 300);
        check(r_float(dispatch(hg, 0x0006, 1, off.span()), "countdown") == 300.0f, "HG06620 offWaitTime -> countdown");
        Report both; both.u16(0x4001, 0x21, 10).u16(0x4002, 0x21, 20);
        check(r_float(dispatch(hg, 0x0006, 1, both.span()), "countdown") == 20.0f, "offWaitTime wins over onTime");
        Encoded e = encode(hg, "countdown", uval(600));
        check(e.ok && e.cluster == 0x0006 && e.frame[2] == 0x42 && e.frame[4] == 0x58 && e.frame[5] == 0x02,
              "HG06620 countdown -> onWithTimedOff 600");
        check(has_expose(hg, "countdown") && has_report(hg, 1, 0x0006, 0x0000, 0), "HG06620 countdown expose + onOff report");
    }
    for (const char* m : {"_TZE204_clrdrnya", "_TZE204_dtzziy1e"}) {   // MTG distance_report
        const PreparedDefinition& d = def_for("TS0601", m);
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_enum(d, 116, 1, ctx, out) && str_is(out, "distance_report", "ON"), "MTG DP116 1 -> ON");
        const Encoded e = encode(d, "distance_report", str("OFF"));
        check(e.ok && e.frame[5] == 116 && tz_value(e) == 0, "MTG distance_report OFF -> DP116 0");
        check(has_expose(d, "distance_report"), "MTG distance_report exposed");
    }
    {   // HOBEIAN ZG-204ZX: temperature in the display unit; calibration divisors
        const PreparedDefinition& d = def_for("ZG-204ZX", "_TZE200_w0ap83qu");
        StoreCtx s; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_num(d, 111, 225, s.ctx, out) && approx(float_of(out, "temperature"), 22.5f), "ZG-204ZX 22.5 C");
        out = {};
        check(dp_enum(d, 109, 1, s.ctx, out) && str_is(out, "temperature_unit", "fahrenheit"), "ZG-204ZX unit F");
        out = {};
        check(dp_num(d, 111, 725, s.ctx, out) && approx(float_of(out, "temperature"), 22.5f), "ZG-204ZX 72.5 F -> 22.5 C");
        out = {};
        check(dp_num(d, 105, 15, s.ctx, out) && approx(float_of(out, "temperature_calibration"), 1.5f), "DP105 /10");
        check(dp_num(d, 104, 5, s.ctx, out) && float_of(out, "humidity_calibration") == 5.0f, "DP104 raw");
        check(has_expose(d, "temperature") && !has_expose(d, "action"), "ZG-204ZX exposes");
    }
    {   // HYSYIOT HS208Z: occupancy + battery from ZCL
        for (const PreparedDefinition* d : {&def_for("TS0202", "_TZD200_sjjp9bti"), &def_for("HS208Z", "_TZ3000_any")}) {
            check(r_bool(feed(*d, 0x0500, 1, ias_cmd(0x0001)), "occupancy", true), "HS208Z IAS alarm_1 -> occupancy");
            Report b; b.u8(0x0021, 0x20, 200);
            check(r_float(dispatch(*d, 0x0001, 1, b.span()), "battery") == 100.0f, "HS208Z genPowerCfg battery");
            RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
            dp_bool(*d, 1, true, ctx, out);
            check(!out.find("occupancy"), "HS208Z DP1 no longer occupancy");
            check(has_binding(*d, 1, 0x0001) && has_report(*d, 1, 0x0001, 0x0021, 10), "HS208Z battery bind + report");
        }
    }
    {   // Mercator SPP04G configure
        const PreparedDefinition& d = def_for("TS011F", "_TZ3210_yvxjawlt");
        check(d.config_steps_count == 1 && d.config_steps[0].op == ConfigStepOp::Read &&
              d.config_steps[0].cluster_id == 0x0000, "SPP04G magic packet");
        check(has_report(d, 2, 0x0006, 0x0000, 0) && has_report(d, 1, 0x0B04, 0x0505, 5) &&
              has_report(d, 1, 0x0B04, 0x0508, 50) && has_report(d, 1, 0x0B04, 0x050B, 1), "SPP04G reports");
    }
    resolves(registries(), {"TS130F", "_TZE20C_xbexmf8h", "TS130F_xbexmf8h"});
    {   // TS130F_xbexmf8h datapoints
        const PreparedDefinition& d = def_for("TS130F", "_TZE20C_xbexmf8h");
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(dp_enum(d, 8, 1, ctx, out) && str_is(out, "motor_direction", "reversed"), "xbexmf8h DP8 -> reversed");
        check(dp_enum(d, 3, 0, ctx, out) && str_is(out, "calibration", "ON"), "xbexmf8h DP3 0 -> ON");
        check(dp_num(d, 10, 123, ctx, out) && approx(float_of(out, "calibration_time"), 12.3f), "DP10 /10");
        check(dp_enum(d, 14, 1, ctx, out) && str_is(out, "backlight_mode", "OFF"), "DP14 1 -> OFF");
        const Encoded e = encode(d, "calibration", str("OFF"));
        check(e.ok && e.frame[5] == 3 && tz_value(e) == 1, "calibration OFF -> DP3 1");
    }
    for (const char* m : {"_TZE204_6a4vxfnv", "_TZE204_0hhou4yj"})
        resolves(registries(), {"TS0601", m, "TS0601_floor_thermostat_006"});
    {   // TS0601_floor_thermostat_006: local_temperature follows the active sensor
        const PreparedDefinition& d = def_for("TS0601", "_TZE204_0hhou4yj");
        check(d.tuya_time_start == 1, "floor_006 time base 1970");
        StoreCtx s;
        auto run = [&](auto fn) { FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o{}; fn(o); return o; };
        auto o = run([&](auto& p) { dp_num(d, 24, 215, s.ctx, p); });
        check(approx(float_of(o, "internal_temperature"), 21.5f) && approx(float_of(o, "local_temperature"), 21.5f),
              "DP24 -> internal + local (sensor IN)");
        o = run([&](auto& p) { dp_num(d, 102, 250, s.ctx, p); });
        check(approx(float_of(o, "external_temperature_input"), 25.0f) && !o.find("local_temperature"),
              "DP102 -> external only (sensor IN)");
        o = run([&](auto& p) { dp_enum(d, 43, 2, s.ctx, p); });
        check(str_is(o, "sensor", "OU"), "DP43 2 -> OU");
        o = run([&](auto& p) { dp_num(d, 102, 260, s.ctx, p); });
        check(approx(float_of(o, "local_temperature"), 26.0f), "DP102 -> local (sensor OU)");
        o = run([&](auto& p) { dp_num(d, 24, 220, s.ctx, p); });
        check(approx(float_of(o, "internal_temperature"), 22.0f) && !o.find("local_temperature"),
              "DP24 -> internal only (sensor OU)");
        o = run([&](auto& p) { dp_num(d, 27, 3, s.ctx, p); dp_bool(d, 40, true, s.ctx, p); dp_bool(d, 1, true, s.ctx, p);
                               dp_enum(d, 2, 0, s.ctx, p); dp_enum(d, 36, 1, s.ctx, p); dp_num(d, 103, 15, s.ctx, p); });
        check(float_of(o, "local_temperature_calibration") == 3.0f && bool_is(o, "eco_mode", true) &&
              str_is(o, "system_mode", "heat") && str_is(o, "preset", "auto") && str_is(o, "running_state", "idle") &&
              approx(float_of(o, "deadzone_temperature"), 1.5f), "floor_006 DPs 27/40/1/2/36/103");
        const Encoded e = encode(d, "sensor", str("OU"));
        check(e.ok && e.frame[5] == 43 && tz_value(e) == 2, "sensor OU -> DP43 2");
    }
}

// Plain writeAttributes frame: [fc tsn 0x02][attr:2][type][value...]; with a
// manufacturer code the code sits after fc, so everything shifts by two.
bool is_write(const Encoded& e, std::uint16_t cluster, std::uint16_t attr, std::uint8_t type, std::uint32_t v,
              std::uint16_t manu = 0) {
    const std::size_t o = manu ? 2 : 0;
    if (!e.ok || e.cluster != cluster || e.frame.size() < 7 + o || e.frame[2 + o] != 0x02) return false;
    if (manu && (e.frame[1] != (manu & 0xFF) || e.frame[2] != (manu >> 8))) return false;
    std::uint32_t got = 0;
    for (std::size_t i = 6 + o; i < e.frame.size(); ++i) got |= std::uint32_t(e.frame[i]) << (8 * (i - 6 - o));
    return e.frame[3 + o] == (attr & 0xFF) && e.frame[4 + o] == (attr >> 8) && e.frame[5 + o] == type && got == v;
}
bool has_step(const PreparedDefinition& d, ConfigStepOp op, std::uint8_t ep, std::uint16_t cluster,
              std::uint16_t manu = 0) {
    for (std::size_t i = 0; i < d.config_steps_count; ++i) {
        const ConfigStep& s = d.config_steps[i];
        if (s.op == op && (s.endpoint ? s.endpoint : 1) == ep && s.cluster_id == cluster && s.manu_code == manu)
            return true;
    }
    return false;
}

void wave4_zcl_s() {
    std::printf("wave 4 ZCL S rows\n");
    for (const char* zm : {"AC201", "AC221", "IRB-4-1-00"}) {   // F046 acLouverPosition
        const PreparedDefinition& d = def_for(zm, "OWON");
        check(is_write(encode(d, "ac_louver_position", str("fully_closed")), 0x0201, 0x0045, 0x30, 1),
              "louver fully_closed writes 1");
        check(is_write(encode(d, "ac_louver_position", str("half_open")), 0x0201, 0x0045, 0x30, 4),
              "louver half_open writes 4");
        Report z; z.u8(0x0045, 0x30, 0);
        check(r_str(dispatch(d, 0x0201, 1, z.span()), "ac_louver_position", "fully_closed"), "louver 0 reads fully_closed");
        check(has_expose(d, "ac_louver_position"), "louver exposed");
    }
    {   // I16 Schneider MEG5779 window_open
        const PreparedDefinition& d = def_for("MEG5779", "Schneider Electric");
        Report w(0x105E); w.u8(0xE012, 0x10, 1);
        check(r_bool(dispatch(d, 0x0201, 1, w.span()), "window_open", true), "MEG5779 0xE012 -> window_open");
        check(has_expose(d, "window_open") && has_step(d, ConfigStepOp::Read, 1, 0x0201, 0x105E), "MEG5779 read step");
    }
    {   // I33 Schneider EKO07259 regulator_level
        const PreparedDefinition& d = def_for("EKO07259", "Schneider Electric");
        Report r(0x105E); r.u8(0x0000, 0x20, 60);
        check(r_float(dispatch(d, 0xFF16, 1, r.span()), "regulator_level") == 60.0f, "EKO07259 regulator_level read");
        check(is_write(encode(d, "regulator_level", uval(70)), 0xFF16, 0x0000, 0x20, 70, 0x105E),
              "EKO07259 regulator_level write");
    }
    check(def_for("eMotion Air", "LinknLink").power_source_override == 0x03, "eMotion Air battery powered");   // F088
    {   // I17 Amina S hardware_current_limit
        const PreparedDefinition& d = def_for("amina S", "Amina Distribution AS");
        Report r; r.u8(0x0011, 0x20, 16);
        check(r_float(dispatch(d, 0x0008, 10, r.span()), "hardware_current_limit") == 16.0f, "amina maxLevel");
        check(has_expose(d, "hardware_current_limit") && has_step(d, ConfigStepOp::Read, 10, 0x0008), "amina read step");
    }
    {   // F028 Schneider WV704R0A0902 (iTRV)
        const PreparedDefinition& d = def_for("iTRV", "Schneider Electric");
        check(d.bindings_count == 2 && d.bindings[0].cluster_id == 0x0201 && d.bindings[1].cluster_id == 0x0001,
              "iTRV binds hvacThermostat first");
        check(has_report(d, 1, 0x0201, 0x0000, 10) && has_report(d, 1, 0x0201, 0x0012, 10) &&
              has_report(d, 1, 0x0201, 0x0008, 10) && has_report(d, 1, 0x0001, 0x0020, 0) &&
              has_report(d, 1, 0x0204, 0x0001, 1), "iTRV reports");
        Report p; p.u8(0x0008, 0x20, 42);
        check(r_float(dispatch(d, 0x0201, 1, p.span()), "pi_heating_demand") == 42.0f, "iTRV pi_heating_demand");
        Report k; k.u8(0x0001, 0x30, 1);
        check(r_find(dispatch(d, 0x0204, 1, k.span()), "keypad_lockout") != nullptr, "iTRV keypad_lockout");
    }
    {   // I04 Danfoss Ally heating_system_mode
        const PreparedDefinition& d = def_for("eTRV0103", "Danfoss");
        Report r(0x1246); r.u8(0x4059, 0x30, 1);
        check(r_str(dispatch(d, 0x0201, 1, r.span()), "heating_system_mode", "boiler_heating"), "Ally 0x4059 -> boiler_heating");
        check(is_write(encode(d, "heating_system_mode", str("default_heating")), 0x0201, 0x4059, 0x30, 2, 0x1246),
              "Ally heating_system_mode write");
        check(has_expose(d, "heating_system_mode"), "Ally heating_system_mode exposed");
    }
    {   // F063 Philips LST001 colour temperature
        const PreparedDefinition& d = def_for("LST001", "Philips");
        check(has_expose(d, "color_temp") && encode(d, "color_temp", uval(300)).ok, "LST001 color_temp");
    }
    {   // F053 Sunricher SR-ZG9095B fan
        const PreparedDefinition& d = def_for("ZG9095B", "Sunricher");
        Report f; f.u8(0x0000, 0x30, 3);
        const auto r = dispatch(d, 0x0202, 1, f.span());
        check(r_str(r, "fan_mode", "high") && r_bool(r, "fan_state", true), "ZG9095B fan high");
        check(is_write(encode(d, "fan_mode", str("auto")), 0x0202, 0x0000, 0x30, 5), "ZG9095B fan_mode auto -> 5");
        check(has_binding(d, 1, 0x0202) && has_expose(d, "fan_mode"), "ZG9095B fan bind + expose");
    }
    for (const char* zm : {"XHS2-SE", "URC4460BC0-X-R"}) {   // F050/F079 poll control
        const PreparedDefinition& d = def_for(zm, "Sercomm");
        bool ok = d.config_steps_count == 2;
        if (ok) {
            const ConfigStep& c = d.config_steps[0];
            const ConfigStep& w = d.config_steps[1];
            ok = c.op == ConfigStepOp::Cmd && c.cluster_id == 0x0020 && c.cmd_id == 0x02 && c.payload_len == 4 &&
                 c.payload[0] == 24 && w.op == ConfigStepOp::Write && w.cluster_id == 0x0020 && w.attr_id == 0x0000 &&
                 w.attr_type == 0x23 && w.payload_len == 4 && w.payload[0] == 0x90 && w.payload[1] == 0x33;
        }
        check(ok, "XHS2 long poll 24 qs + checkin 13200");
    }
    {   const PreparedDefinition& d = def_for("XHS2-SE", "Sercomm");
        check(d.bindings_count == 1 && has_binding(d, 1, 0x0402) && has_report(d, 1, 0x0402, 0x0000, 100),
              "XHS2-SE binds temperature only, with a report");
    }
}

// Attribute report carrying one 48-bit unsigned (type 0x25) value.
std::vector<std::uint8_t> u48_report(std::uint16_t attr, std::uint64_t val) {
    std::vector<std::uint8_t> f{0x18, 0x01, 0x0A, std::uint8_t(attr & 0xFF), std::uint8_t(attr >> 8), 0x25};
    for (int i = 0; i < 6; ++i) f.push_back(std::uint8_t(val >> (8 * i)));
    return f;
}
int g_reads = 0;
std::uint16_t g_read_cluster = 0, g_read_manu = 0;

void wave4_zcl_m() {
    std::printf("wave 4 ZCL M items\n");
    {   // F098 EGLO 901471: CT panel on EP1 (white), RGB ring on EP3 (rgb)
        const PreparedDefinition& d = def_for("EBF-RGB-ZMB-CLB", "EGLO");
        Report on; on.u8(0x0000, 0x10, 1);
        const auto r1 = dispatch(d, 0x0006, 1, on.span());
        const auto r3 = dispatch(d, 0x0006, 3, on.span());
        check(r_find(r1, "state_white") && r_find(r3, "state_rgb") && !r_find(r3, "state"), "901471 EP1 white / EP3 rgb");
        check(has_expose(d, "color_temp_white") && has_expose(d, "color_x_rgb") && !has_expose(d, "color_temp_rgb") &&
              !has_expose(d, "state"), "901471 split exposes");
        check(has_binding(d, 1, 0x0300) && has_binding(d, 3, 0x0300) && has_binding(d, 3, 0x0006), "901471 EP3 binds");
    }
    {   // F044 Develco EMIZB-132 on EP2
        const PreparedDefinition& d = def_for("EMIZB-132", "Develco Products A/S");
        StoreCtx s;
        Report p; p.u32(0x0304, 0x2B, 1500).u32(0x0305, 0x2B, 200);
        auto r = dispatch(d, 0x0B04, 2, p.span(), &s.ctx);
        check(r_float(r, "power") == 1500.0f && r_float(r, "power_reactive") == 200.0f, "EMIZB-132 total power");
        Report v; v.u16(0x0505, 0x21, 2301).u16(0x0508, 0x21, 52).u16(0x0908, 0x21, 10);
        r = dispatch(d, 0x0B04, 2, v.span(), &s.ctx);
        check(approx(r_float(r, "voltage"), 230.1f) && approx(r_float(r, "current"), 5.2f) &&
              approx(r_float(r, "current_phase_b"), 1.0f), "EMIZB-132 current /10 by default");
        Report m(0x1015); m.u16(0x0302, 0x31, 0x0203);
        check(r_str(dispatch(d, 0x0702, 2, m.span(), &s.ctx), "interface_mode", "kaifa_and_kamstrup"), "EMIZB-132 interface_mode");
        r = dispatch(d, 0x0B04, 2, v.span(), &s.ctx);
        check(approx(r_float(r, "current"), 0.052f, 0.0001f), "EMIZB-132 kaifa_and_kamstrup current /1000");
        const auto e1 = u48_report(0x0000, 123456), e0 = u48_report(0x0000, 0), ex = u48_report(0x0000, 0xFFFFFFFFFFFFull);
        check(approx(r_float(feed(d, 0x0702, 2, e1), "energy"), 123.456f), "EMIZB-132 energy /1000");
        check(!r_find(feed(d, 0x0702, 2, e0), "energy") && !r_find(feed(d, 0x0702, 2, ex), "energy"),
              "EMIZB-132 zero / invalid energy dropped");
        const Encoded w = encode(d, "interface_mode", str("aidon_meter"));
        check(is_write(w, 0x0702, 0x0302, 0x31, 0x0202, 0x1015) && w.ep == 2, "EMIZB-132 interface_mode write on EP2");
        check(has_binding(d, 2, 0x0702) && has_binding(d, 2, 0x0B04) && !has_binding(d, 1, 0x0702) &&
              has_report(d, 2, 0x0B04, 0x0304, 1) && has_report(d, 2, 0x0B04, 0x0305, 1), "EMIZB-132 EP2 binds + reports");
    }
    {   // F029 Sinopé TH1400ZB / TH1300ZB: a localTemp report re-reads floor/room temperature
        RuntimeContext ctx{};
        ctx.configure_read = [](std::uint16_t, std::uint8_t, std::uint16_t c, const std::uint8_t* ids, std::uint8_t n,
                                std::uint16_t manu) {
            ++g_reads; g_read_cluster = c; g_read_manu = manu;
            return n == 2 && ids[0] == 0x07 && ids[1] == 0x01 && ids[2] == 0x0D && ids[3] == 0x01;
        };
        for (const char* zm : {"TH1400ZB", "TH1300ZB"}) {
            const PreparedDefinition& d = def_for(zm, "Sinope Technologies");
            g_reads = 0;
            Report t; t.u16(0x0000, 0x29, 2150);
            dispatch(d, 0x0201, 1, t.span(), &ctx);
            check(g_reads == 1 && g_read_cluster == 0xFF01 && g_read_manu == 0x119C, "Sinopé localTemp triggers floor/room read");
            check(has_step(d, ConfigStepOp::Read, 1, 0xFF01, 0x119C), "Sinopé initial floor/room read");
        }
    }
    {   // F051 Shelly WS90 drops all-zero frames
        const PreparedDefinition& d = def_for("Ecowitt WS90", "Shelly");
        StoreCtx s;
        Report h0; h0.u16(0x0000, 0x21, 0);
        check(r_find(dispatch(d, 0x0405, 1, h0.span(), &s.ctx), "humidity") != nullptr, "WS90 humidity 0 with no history kept");
        Report h; h.u16(0x0000, 0x21, 6000);
        dispatch(d, 0x0405, 1, h.span(), &s.ctx);
        check(!r_find(dispatch(d, 0x0405, 1, h0.span(), &s.ctx), "humidity"), "WS90 humidity 0 after 60 % dropped");
        Report t; t.u16(0x0000, 0x29, 150);
        Report t0; t0.u16(0x0000, 0x29, 0);
        dispatch(d, 0x0402, 1, t.span(), &s.ctx);
        check(r_find(dispatch(d, 0x0402, 1, t0.span(), &s.ctx), "temperature") != nullptr, "WS90 0 C after 1.5 C kept");
        Report tw; tw.u16(0x0000, 0x29, 2500);
        dispatch(d, 0x0402, 1, tw.span(), &s.ctx);
        check(!r_find(dispatch(d, 0x0402, 1, t0.span(), &s.ctx), "temperature"), "WS90 0 C after 25 C dropped");
        Report p0; p0.u16(0x0000, 0x29, 0);
        check(!r_find(dispatch(d, 0x0403, 1, p0.span(), &s.ctx), "pressure"), "WS90 pressure 0 dropped");
        Report l; l.u16(0x0000, 0x21, 30000);
        Report l0; l0.u16(0x0000, 0x21, 0);
        dispatch(d, 0x0400, 1, l.span(), &s.ctx);
        check(!r_find(dispatch(d, 0x0400, 1, l0.span(), &s.ctx), "illuminance"), "WS90 illuminance 0 after daylight dropped");
    }
    {   // F078b Namron Edge custom attributes
        const PreparedDefinition& d = def_for("4512783", "Namron AS");
        StoreCtx s;
        Report a; a.u16(0x0029, 0x19, 0x0001).u8(0x0025, 0x18, 0x04).u8(0x8004, 0x30, 6).u16(0x801d, 0x29, 40)
                   .u8(0x8023, 0x20, 6).u8(0x8024, 0x20, 200).u8(0x8006, 0x18, 0x48).u16(0x8025, 0x29, 285)
                   .u8(0x8000, 0x10, 1);
        const auto r = dispatch(d, 0x0201, 1, a.span(), &s.ctx);
        check(r_str(r, "running_state", "heat") && r_str(r, "programming_operation_mode", "eco") &&
              r_str(r, "sensor_mode", "regulator") && r_float(r, "regulator_percentage") == 40.0f &&
              r_float(r, "countdown_set") == 30.0f && r_float(r, "countdown_left") == 0.0f &&
              r_str(r, "fault", "internal_sensor_error,overheat_error") && approx(r_float(r, "max_heat_temp"), 28.5f) &&
              r_bool(r, "window_open_check", true), "Namron Edge custom attrs");
        Report f0; f0.u8(0x8006, 0x18, 0);
        check(r_str(dispatch(d, 0x0201, 1, f0.span(), &s.ctx), "fault", "none"), "Namron Edge fault 0 -> none");
        Encoded e = encode(d, "sensor_mode", str("floor"));
        check(is_write(e, 0x0201, 0x8004, 0x30, 1) && e.frame[0] == 0x00, "Edge write asks for a default response");
        check(is_write(encode(d, "holiday_temp_set", num(12.5)), 0x0201, 0x8013, 0x29, 1250), "Edge holiday_temp_set x100");
        check(is_write(encode(d, "frost", boolean(true)), 0x0201, 0x8001, 0x10, 1), "Edge frost write");
        check(d.power_source_override == 0x01 && has_binding(d, 1, 0x0405) && has_report(d, 1, 0x0405, 0x0000, 100) &&
              has_step(d, ConfigStepOp::Read, 1, 0x0201), "Edge configure");
    }
    for (const char* zm : {"AUX-1-ZB-S", "DK-1-ZB-S", "GR-3-ZB-S", "HR-1-ZB-S", "HS-3-ZB-S", "HT-1-ZB-S", "MD-1-ZB-S",
                           "MD-3-ZB-S", "ME-1-ZB-S", "TCL-1-ZB-S"}) {   // I01b Onokom external sensor
        const PreparedDefinition& d = def_for(zm, "ONOKOM");
        check(is_write(encode(d, "external_air_temperature", num(21.5)), 0x0201, 0x4751, 0x29, 2150, 0x4703),
              "onokom external_air_temperature x100");
        check(has_expose(d, "allow_the_use_of_an_external_temperature_sensor") &&
              has_expose(d, "external_temperature_sensor_validity_period"), "onokom external sensor exposes");
    }
    {   const PreparedDefinition& d = def_for("HT-1-ZB-S", "ONOKOM");
        Report r(0x4703); r.u8(0x4750, 0x30, 1).u16(0x4751, 0x29, 2150).u16(0x4752, 0x21, 30);
        const auto x = dispatch(d, 0x0201, 1, r.span());
        check(r_str(x, "allow_the_use_of_an_external_temperature_sensor", "Yes") &&
              approx(r_float(x, "external_air_temperature"), 21.5f) &&
              r_float(x, "external_temperature_sensor_validity_period") == 30.0f, "onokom external sensor decode");
        check(is_write(encode(d, "allow_the_use_of_an_external_temperature_sensor", str("Yes")), 0x0201, 0x4750, 0x30, 1,
                       0x4703), "onokom allow write");
        check(!has_expose(def_for("GR-1-ZB-S", "ONOKOM"), "external_air_temperature"), "GR-1 has no external sensor");
    }
}

// ── wave 4 Lumi ─────────────────────────────────────────────────────

// genBasic attribute report of modelId (0x0005, char string) with `tsn`.
std::vector<std::uint8_t> model_id_report(std::uint8_t tsn, const char* model) {
    const std::size_t n = std::strlen(model);
    std::vector<std::uint8_t> f{0x18, tsn, 0x0A, 0x05, 0x00, 0x42, std::uint8_t(n)};
    f.insert(f.end(), model, model + n);
    return f;
}
std::uint32_t f32_bits(float f) { std::uint32_t u = 0; std::memcpy(&u, &f, sizeof(u)); return u; }

void wave4_lumi() {
    std::printf("wave 4 Lumi\n");
    {   // I12b / I12d / I12e MCCGQ13LM
        const PreparedDefinition& d = def_for("lumi.magnet.ac01", "LUMI");
        StoreCtx s;
        const auto b1 = model_id_report(0x21, "lumi.magnet.ac01");
        check(r_str(feed_ctx(d, 0x0000, 1, b1, s.ctx), "action", "single"), "MCCGQ13LM button press -> action single");
        check(!r_find(feed_ctx(d, 0x0000, 1, b1, s.ctx), "action"), "MCCGQ13LM same-tsn repeat dropped");
        const auto b2 = model_id_report(0x22, "lumi.magnet.agl01");
        check(!r_find(feed_ctx(d, 0x0000, 1, b2, s.ctx), "action"), "MCCGQ13LM other modelId: no action");
        Report p; p.u8(0x0020, 0x20, 29);
        const auto r = dispatch(d, 0x0001, 1, p.span());
        check(r_float(r, "voltage") == 2900.0f && r_float(r, "battery") == 33.0f, "MCCGQ13LM batteryVoltage x100 on the 2850-3000 curve");
        Report bad; bad.u8(0x0020, 0x20, 0xFF);
        check(!r_find(dispatch(d, 0x0001, 1, bad.span()), "voltage"), "MCCGQ13LM batteryVoltage 255 ignored");
        check(is_write(encode(d, "detection_distance", str("20mm")), 0xFCC0, 0x010C, 0x20, 2, 0x115F),
              "MCCGQ13LM detection_distance write");
        Report dd(0x115F); dd.u8(0x010C, 0x20, 3);
        check(r_str(dispatch(d, 0xFCC0, 1, dd.span()), "detection_distance", "30mm"), "MCCGQ13LM detection_distance read");
        Report d0(0x115F); d0.u8(0x010C, 0x20, 0);
        check(!r_find(dispatch(d, 0xFCC0, 1, d0.span()), "detection_distance"), "MCCGQ13LM detection_distance 0 ignored");
        check(has_expose(d, "detection_distance") && has_expose(d, "action"), "MCCGQ13LM exposes");
    }
    {   // I18a / I18b / I18c KD-R01D + knob rotation on ZNXNKG01LM / ZNXNKG02LM
        const PreparedDefinition& d = def_for("lumi.switch.agl011", "LUMI");
        check(is_write(encode(d, "event_mode", str("event")), 0xFCC0, 0x0009, 0x20, 1, 0x115F), "KD-R01D event_mode write");
        Report em(0x115F); em.u8(0x0009, 0x20, 0);
        check(r_str(dispatch(d, 0xFCC0, 1, em.span()), "event_mode", "command"), "KD-R01D event_mode read");
        check(is_write(encode(d, "mode_switch", str("quick_mode")), 0xFCC0, 0x0004, 0x21, 1, 0x115F), "KD-R01D mode_switch write");
        Report ms(0x115F); ms.u16(0x0004, 0x21, 4);
        check(r_str(dispatch(d, 0xFCC0, 1, ms.span()), "mode_switch", "anti_flicker_mode"), "KD-R01D mode_switch read");
        Report rot(0x115F); rot.u8(570, 0x20, 2).u16(558, 0x29, 30).u16(563, 0x29, 8);
        auto r = dispatch(d, 0xFCC0, 1, rot.span());
        check(r_str(r, "action", "rotation") && r_str(r, "action_rotation_button_state", "released") &&
              r_float(r, "action_rotation_angle") == 30.0f && r_float(r, "action_rotation_percent") == 8.0f,
              "KD-R01D knob rotation");
        check(r_str(dispatch(d, 0xFCC0, 72, rot.span()), "action_rotation_button_state", "pressed"), "KD-R01D EP72 = pressed");
        Report rp(0x115F); rp.u8(570, 0x20, 0x81);
        r = dispatch(d, 0xFCC0, 1, rp.span());
        check(r_str(r, "action", "start_rotating") && r_str(r, "action_rotation_button_state", "pressed"), "KD-R01D bit 7 = pressed");
        check(has_expose(d, "event_mode") && has_expose(d, "mode_switch") && has_expose(d, "action_rotation_angle") &&
              has_expose(d, "action_rotation_button_state"), "KD-R01D exposes");
        for (const char* zm : {"lumi.switch.rkna01", "lumi.remote.rkba01"}) {
            const PreparedDefinition& k = def_for(zm, "LUMI");
            r = dispatch(k, 0xFCC0, 72, rot.span());
            check(r_str(r, "action", "rotation") && r_str(r, "action_rotation_button_state", "released") &&
                  has_expose(k, "action_rotation_percent"), "ZNXNKG knob rotation (no pressed endpoint)");
        }
    }
    {   // F072 DS-K02D/E wireless buttons on EP3 / EP4
        const PreparedDefinition& d = def_for("lumi.switch.aeu003", "LUMI");
        Report a; a.u16(0x0055, 0x21, 1);
        check(r_str(dispatch(d, 0x0012, 3, a.span()), "action", "single_top_wireless_button"), "DS-K02D EP3 single_top");
        Report h; h.u16(0x0055, 0x21, 0);
        check(r_str(dispatch(d, 0x0012, 4, h.span()), "action", "hold_bottom_wireless_button"), "DS-K02D EP4 hold_bottom");
        check(!r_find(dispatch(d, 0x0012, 1, a.span()), "action"), "DS-K02D EP1: no action");
        Report t; t.u16(0x0055, 0x21, 3);
        check(!r_find(dispatch(d, 0x0012, 3, t.span()), "action"), "DS-K02D triple dropped");
        const std::vector<std::uint8_t> rr{0x18, 0x01, 0x01, 0x55, 0x00, 0x00, 0x21, 0x01, 0x00};
        check(!r_find(feed(d, 0x0012, 3, rr), "action"), "DS-K02D readResponse ignored");
        check(has_binding(d, 3, 0x0012) && has_binding(d, 4, 0x0012) && !has_binding(d, 3, 0xFCC0) &&
              !has_binding(d, 4, 0xFCC0) && !has_binding(d, 1, 0x0012),
              "DS-K02D binds EP3/EP4 multistate (no 0xFCC0 bind: house rule since 3b74e93)");
        bool off = d.reports_count == 2;
        for (std::size_t i = 0; i < d.reports_count; ++i)
            off = off && d.reports[i].cluster_id == 0x0012 && d.reports[i].attr_id == 0x0055 &&
                  d.reports[i].max_interval == 0xFFFF && d.reports[i].reportable_change == 1;
        check(off && has_report(d, 3, 0x0012, 0x0055, 1) && has_report(d, 4, 0x0012, 0x0055, 1),
              "DS-K02D presentValue reporting off (max 0xFFFF)");
    }
    {   // F073 WSDCGQ12LM scaledValue pressure + 500..2000 hPa filter
        const PreparedDefinition& d = def_for("lumi.sensor_ht.agl02", "LUMI");
        StoreCtx s;
        Report p; p.u16(0x0000, 0x29, 1013).u16(0x0010, 0x29, 10132).u8(0x0014, 0x28, 0xFF);
        check(approx(r_float(dispatch(d, 0x0403, 1, p.span(), &s.ctx), "pressure"), 1013.2f), "WSDCGQ12LM scaledValue pressure");
        Report q; q.u16(0x0010, 0x29, 10045);
        check(approx(r_float(dispatch(d, 0x0403, 1, q.span(), &s.ctx), "pressure"), 1004.5f), "WSDCGQ12LM cached scale");
        StoreCtx fresh;
        check(approx(r_float(dispatch(d, 0x0403, 1, q.span(), &fresh.ctx), "pressure"), 1004.5f), "WSDCGQ12LM default scale -1");
        Report m; m.u16(0x0000, 0x29, 1001);
        check(r_float(dispatch(d, 0x0403, 1, m.span(), &s.ctx), "pressure") == 1001.0f, "WSDCGQ12LM measuredValue fallback");
        Report junk; junk.u16(0x0000, 0x29, 0);
        check(!r_find(dispatch(d, 0x0403, 1, junk.span(), &s.ctx), "pressure"), "WSDCGQ12LM out-of-range pressure dropped");
    }
    {   // F083 FP310 presence (0xFCC0 0x0142)
        const PreparedDefinition& d = def_for("lumi.sensor_occupy.acn1", "LUMI");
        Report on(0x115F); on.u8(0x0142, 0x20, 1);
        check(r_bool(dispatch(d, 0xFCC0, 1, on.span()), "presence", true), "FP310 presence 1 -> true");
        Report off(0x115F); off.u8(0x0142, 0x20, 0);
        check(r_bool(dispatch(d, 0xFCC0, 1, off.span()), "presence", false), "FP310 presence 0 -> false");
        check(!has_binding(d, 1, 0xFCC0) && has_report(d, 1, 0xFCC0, 0x0142, 1) && has_expose(d, "presence"),
              "FP310 presence bind / report / expose");
        check(has_step(d, ConfigStepOp::Read, 1, 0xFCC0, 0x115F), "FP310 reads presence at configure");
    }
    {   // N20 SP-P06E Outdoor plug H2 EU
        const PreparedDefinition& d = def_for("lumi.plug.aeu006", "LUMI");
        check(d.model && std::strcmp(d.model, "SP-P06E") == 0, "lumi.plug.aeu006 is SP-P06E");
        Report on; on.u8(0x0000, 0x10, 1);
        check(r_find(dispatch(d, 0x0006, 1, on.span()), "state") != nullptr, "SP-P06E on/off");
        Report pw; pw.u16(0x050B, 0x29, 1200);
        check(r_float(dispatch(d, 0x0B04, 1, pw.span()), "power") == 1200.0f, "SP-P06E activePower W");
        check(is_write(encode(d, "led_indicator", boolean(true)), 0xFCC0, 0x0203, 0x10, 1, 0x115F), "SP-P06E led_indicator");
        check(is_write(encode(d, "child_lock", str("LOCK")), 0xFCC0, 0x0285, 0x20, 1, 0x115F), "SP-P06E child_lock");
        check(is_write(encode(d, "charging_protection", boolean(true)), 0xFCC0, 0x0202, 0x10, 1, 0x115F),
              "SP-P06E charging_protection");
        check(is_write(encode(d, "power_on_behavior", str("previous")), 0xFCC0, 0x0517, 0x20, 1, 0x115F),
              "SP-P06E power_on_behavior");
        check(is_write(encode(d, "flip_indicator_light", str("ON")), 0xFCC0, 0x00F0, 0x20, 1, 0x115F),
              "SP-P06E flip_indicator_light");
        Report st(0x115F); st.u8(0x0285, 0x20, 1).u8(0x0517, 0x20, 2).u8(0x0202, 0x10, 1).u8(0x0203, 0x10, 0);
        const auto r = dispatch(d, 0xFCC0, 1, st.span());
        check(r_bool(r, "child_lock", true) && r_str(r, "power_on_behavior", "off") && r_bool(r, "charging_protection", true) &&
              r_bool(r, "led_indicator", false), "SP-P06E settings read");
        check(has_expose(d, "power_on_behavior") && has_expose(d, "child_lock") && has_expose(d, "energy") &&
              !has_expose(d, "overload_protection") && has_report(d, 1, 0x0006, 0x0000, 0) &&
              has_report(d, 1, 0x0B04, 0x050B, 5), "SP-P06E exposes + reports");
    }
    {   // F043 ZNJLBL01LM roller shade driver E1 (lumi.curtain.acn002 off ZNCLDJ14LM)
        const PreparedDefinition& d = def_for("lumi.curtain.acn002", "LUMI");
        check(d.model && std::strcmp(d.model, "ZNJLBL01LM") == 0, "lumi.curtain.acn002 is ZNJLBL01LM");
        const PreparedDefinition& h = def_for("lumi.curtain.hagl07", "LUMI");
        check(h.model && std::strcmp(h.model, "ZNCLDJ14LM") == 0, "lumi.curtain.hagl07 stays ZNCLDJ14LM");
        check(is_write(encode(d, "position", num(40)), 0x000D, 0x0055, 0x39, f32_bits(40.0f)),
              "ZNJLBL01LM position -> genAnalogOutput presentValue");
        check(is_write(encode(d, "state", str("CLOSE")), 0x000D, 0x0055, 0x39, f32_bits(0.0f)), "ZNJLBL01LM CLOSE -> 0");
        check(is_write(encode(d, "state", str("OPEN")), 0x000D, 0x0055, 0x39, f32_bits(100.0f)), "ZNJLBL01LM OPEN -> 100");
        check(is_write(encode(d, "state", str("STOP")), 0x0013, 0x0055, 0x21, 2), "ZNJLBL01LM STOP -> genMultistateOutput 2");
        check(is_write(encode(d, "reverse_direction", boolean(true)), 0xFCC0, 0x0400, 0x10, 1, 0x115F),
              "ZNJLBL01LM reverse_direction write");
        check(is_write(encode(d, "motor_speed", str("high")), 0xFCC0, 0x0408, 0x20, 2, 0x115F), "ZNJLBL01LM motor_speed write");
        Report o(0x115F); o.u8(0x0400, 0x10, 1).u8(0x0409, 0x10, 1).u8(0x040A, 0x20, 87).u8(0x0408, 0x20, 1);
        auto r = dispatch(d, 0xFCC0, 1, o.span());
        check(r_bool(r, "reverse_direction", true) && r_bool(r, "charging_status", true) &&
              r_float(r, "battery") == 87.0f && r_str(r, "motor_speed", "medium"), "ZNJLBL01LM options / charging / battery");
        Report pos; pos.u32(0x0055, 0x39, f32_bits(55.0f));
        check(r_float(dispatch(d, 0x000D, 1, pos.span()), "position") == 55.0f, "ZNJLBL01LM position report");
        RuntimeContext ctx{};
        ctx.configure_read = [](std::uint16_t, std::uint8_t, std::uint16_t c, const std::uint8_t* ids, std::uint8_t n,
                                std::uint16_t manu) {
            ++g_reads; g_read_cluster = c; g_read_manu = manu;
            return n == 1 && ids[0] == 0x55 && ids[1] == 0x00;
        };
        g_reads = 0;
        Report mv; mv.u16(0x0055, 0x21, 1);
        r = dispatch(d, 0x0013, 1, mv.span(), &ctx);
        check(r_str(r, "motor_state", "opening") && r_bool(r, "running", true) && g_reads == 0, "ZNJLBL01LM opening");
        Report bl; bl.u16(0x0055, 0x21, 3);
        r = dispatch(d, 0x0013, 1, bl.span(), &ctx);
        check(r_str(r, "motor_state", "blocked") && r_bool(r, "running", false) && g_reads == 1 && g_read_cluster == 0x000D &&
              g_read_manu == 0, "ZNJLBL01LM stop re-reads the position");
        check(has_expose(d, "reverse_direction") && has_expose(d, "motor_state") && has_expose(d, "charging_status") &&
              has_expose(d, "motor_speed") && d.power_source_override == 0x03 &&
              has_step(d, ConfigStepOp::Read, 1, 0xFCC0, 0x115F), "ZNJLBL01LM exposes / battery / configure read");
    }
}

}  // namespace

// Final whole-branch review fixes.
void final_review() {
    std::printf("final review fixes\n");
    // I1 / M1: a kReportFlagOptional slipped into manufacturer_code turns a
    // standard report into a mandatory manufacturer-specific one (code 0x0001).
    for (const PreparedDefinition* d : registries()) {
        for (std::size_t i = 0; i < d->reports_count; ++i) {
            const std::uint16_t m = d->reports[i].manufacturer_code;
            if (m != 0 && m < 0x1000) {
                std::printf("  %s report 0x%04X/0x%04X manu 0x%04X\n", d->model, d->reports[i].cluster_id,
                            d->reports[i].attr_id, m);
                check(false, "report manufacturer_code is 0 or a real code");
            }
        }
    }
    {   // I2: EKO07259 0xFF16 (manu 0x105E) is not decoded as metering / thermostat
        const PreparedDefinition& d = def_for("EKO07259", "Schneider Electric");
        Report r(0x105E); r.u8(0x0000, 0x20, 50);
        const auto res = dispatch(d, 0xFF16, 1, r.span());
        check(r_float(res, "regulator_level") == 50.0f && !r_find(res, "local_temperature") &&
              !r_find(res, "energy") && !r_find(res, "state"), "EKO07259 regulator_level only");
    }
    {   // I3: Ariston Rem4 Heater binds and reports its zone thermostat on EP2
        const PreparedDefinition& d = def_for("Rem4 Heater", "Ariston");
        check(d.default_endpoint == 2 && has_binding(d, 2, 0x0006) && has_binding(d, 2, 0x0201) &&
              has_report(d, 2, 0x0006, 0x0000, 1) && has_report(d, 2, 0x0201, 0x0000, 10) &&
              has_report(d, 2, 0x0201, 0x0012, 10) && has_report(d, 2, 0x0201, 0x001C, 0),
              "Ariston EP2 binds / reports / default endpoint");
    }
    {   // I4: ZC301 DP107 / DP155 go out as Tuya sendData (0x04)
        const PreparedDefinition& d = def_for("TS030F", "_TZ3210_hs01bacp");
        Encoded e = encode(d, "curtain_close_direction", str("DOWN"));
        check(e.ok && e.cluster == 0xEF00 && e.frame[2] == 0x04 && e.frame[5] == 107 && tz_value(e) == 1,
              "ZC301 DP107 sendData");
        Value on{}; on.type = ValueType::Bool; on.b = true;
        e = encode(d, "charge_led_switch", on);
        check(e.ok && e.frame[2] == 0x04 && e.frame[5] == 155 && tz_value(e) == 1, "ZC301 DP155 sendData");
        e = encode(d, "position", num(30));
        check(e.ok && e.cluster == 0x0102, "ZC301 position still ZCL");
    }
    {   // M12 (re-graded): _TZ3210_ffuna0nr keeps the TS0505B light reporting it had
        const PreparedDefinition& d = def_for("TS0505B", "_TZ3210_ffuna0nr");
        check(std::strcmp(d.model, "TS0505B_hs") == 0 && has_report(d, 1, 0x0006, 0x0000, 0) &&
              has_report(d, 1, 0x0300, 0x0003, 1), "TS0505B_hs light reporting");
    }
    {   // M5 (re-graded): XHS2-UE temperature / batteryVoltage reporting (triage row)
        const PreparedDefinition& d = def_for("URC4460BC0-X-R", "Universal Electronics Inc");
        check(has_report(d, 1, 0x0402, 0x0000, 100) && has_report(d, 1, 0x0001, 0x0020, 0),
              "XHS2-UE temperature / battery reporting");
    }
}

// F-04 (security review 2026-11-03): a setting that changes how a measurement
// reads survives a hub restart. The restart zeroes DeviceRuntimeState; until
// the device reports the setting again, the decoder takes the text the hub
// kept for it (its shadow, restored from flash: RuntimeContext::state_text).
namespace zhc::devices::tuya { extern const PreparedDefinition kDef_ZG_IR01; }
namespace {
std::map<std::string, std::string> g_kept;   // device 0's shadow, as restored at boot
bool kept_text(std::uint16_t, const char* key, char* out, std::size_t cap) {
    const auto it = g_kept.find(key);
    if (it == g_kept.end() || it->second.size() >= cap) return false;
    std::memcpy(out, it->second.c_str(), it->second.size() + 1);
    return true;
}
// The shadow keeps the text values a decode publishes.
void keep(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& p) {
    for (std::uint8_t i = 0; i < p.count; ++i)
        if (p.items[i].value.type == ValueType::StringRef && p.items[i].value.str)
            g_kept[p.items[i].key] = p.items[i].value.str;
}
struct Hub : StoreCtx {   // one device, slot 0
    Hub() { g_kept.clear(); ctx.state_text = &kept_text; }
    void restart() { store = RuntimeStore<1>{}; }   // the runtime store starts zeroed; the shadow is restored
};
using Out = FixedPayload<ZHC_FIXED_PAYLOAD_CAP>;
}  // namespace

void settings_survive_restart() {
    std::printf("F-04 settings survive a restart\n");
    const PreparedDefinition& ir = devices::tuya::kDef_ZG_IR01;
    {   // ZG-IR01: DP111 unit, DP109 temperature in that unit
        Hub h; Out u{}, t{};
        dp_enum(ir, 111, 1, h.ctx, u); keep(u);
        check(dp_num(ir, 109, 770, h.ctx, t) && approx(float_of(t, "temperature"), 25.0f), "ZG-IR01 77.0 F -> 25.0 C");
        h.restart(); t = {};
        check(dp_num(ir, 109, 770, h.ctx, t) && approx(float_of(t, "temperature"), 25.0f),
              "ZG-IR01 after a restart: 77.0 F still 25.0 C");
        u = {}; t = {};
        dp_enum(ir, 111, 0, h.ctx, u);   // the device's own report beats the kept text
        check(dp_num(ir, 109, 225, h.ctx, t) && approx(float_of(t, "temperature"), 22.5f),
              "ZG-IR01 a unit report after the restart wins over the kept unit");
    }
    {   // ZG-204ZX: DP109 unit, DP111 temperature (the same shared helpers)
        const PreparedDefinition& d = def_for("ZG-204ZX", "_TZE200_w0ap83qu");
        Hub h; Out u{}, t{};
        dp_enum(d, 109, 1, h.ctx, u); keep(u);
        h.restart();
        check(dp_num(d, 111, 770, h.ctx, t) && approx(float_of(t, "temperature"), 25.0f),
              "ZG-204ZX after a restart: 77.0 F still 25.0 C");
    }
    const PreparedDefinition& floor = def_for("TS0601", "_TZE204_0hhou4yj");
    {   // TS0601_floor_thermostat_006: DP43 picks the probe local_temperature follows
        Hub h; Out s{}, ext{}, in{};
        dp_enum(floor, 43, 2, h.ctx, s); keep(s);   // OU: the external floor probe
        h.restart();
        dp_num(floor, 102, 300, h.ctx, ext);
        dp_num(floor, 24, 180, h.ctx, in);
        check(approx(float_of(ext, "local_temperature"), 30.0f), "floor_006 after a restart: DP102 still local (OU)");
        check(approx(float_of(in, "internal_temperature"), 18.0f) && !in.find("local_temperature"),
              "floor_006 after a restart: DP24 internal only (OU)");
    }
    const PreparedDefinition& meter = def_for("EMIZB-132", "Develco Products A/S");
    Report cur; cur.u16(0x0508, 0x21, 52);
    {   // Develco EMIZB-132: interface_mode (read at configure, never reported) sets the current divisor
        Hub h;
        Report m(0x1015); m.u16(0x0302, 0x31, 0x0203);
        keep(dispatch(meter, 0x0702, 2, m.span(), &h.ctx).merged);
        h.restart();
        check(approx(r_float(dispatch(meter, 0x0B04, 2, cur.span(), &h.ctx), "current"), 0.052f, 0.0001f),
              "EMIZB-132 after a restart: kaifa_and_kamstrup current still /1000");
    }
    {   // Neither reported since boot nor kept: upstream's unset-state defaults
        // (Celsius, IN, norwegian_han), not cached, so a text the hub restores
        // after the first frame still counts.
        Hub h; Out t{}, in{};
        check(dp_num(ir, 109, 225, h.ctx, t) && approx(float_of(t, "temperature"), 22.5f), "ZG-IR01 unit unknown: Celsius");
        g_kept["temperature_unit"] = "fahrenheit"; t = {};
        check(dp_num(ir, 109, 770, h.ctx, t) && approx(float_of(t, "temperature"), 25.0f),
              "ZG-IR01 a unit kept after the first frame still counts");
        g_kept["temperature_unit"] = "kelvin"; h.restart(); t = {};
        check(dp_num(ir, 109, 225, h.ctx, t) && approx(float_of(t, "temperature"), 22.5f),
              "ZG-IR01 a kept text that names no unit: Celsius");
        Hub f;
        dp_num(floor, 24, 180, f.ctx, in);
        check(approx(float_of(in, "local_temperature"), 18.0f), "floor_006 sensor unknown: DP24 is local (IN)");
        Hub m;
        check(approx(r_float(dispatch(meter, 0x0B04, 2, cur.span(), &m.ctx), "current"), 5.2f),
              "EMIZB-132 interface_mode unknown: current /10 (norwegian_han)");
        RuntimeContext bare{}; t = {};
        check(dp_num(ir, 109, 300, bare, t) && approx(float_of(t, "temperature"), 30.0f), "no store, no hook: Celsius");
    }
    {   // WSDCGQ12LM: the pressure scale is no state the hub keeps (z2m reads it
        // from herdsman's attribute cache). Unknown since boot it is Aqara's -1,
        // and the 500..2000 hPa window drops what another scale would give:
        // a dropped frame, never a reading ten times off.
        const PreparedDefinition& d = def_for("lumi.sensor_ht.agl02", "LUMI");
        Hub h;
        Report a; a.u16(0x0010, 0x29, 10132).u8(0x0014, 0x28, 0xFF);   // scale -1
        check(approx(r_float(dispatch(d, 0x0403, 1, a.span(), &h.ctx), "pressure"), 1013.2f), "WSDCGQ12LM scale -1");
        h.restart();
        Report q; q.u16(0x0010, 0x29, 10045);
        check(approx(r_float(dispatch(d, 0x0403, 1, q.span(), &h.ctx), "pressure"), 1004.5f),
              "WSDCGQ12LM after a restart: Aqara's -1");
        Report b; b.u16(0x0010, 0x29, 1013).u8(0x0014, 0x28, 0xFE);    // scale -2
        check(approx(r_float(dispatch(d, 0x0403, 1, b.span(), &h.ctx), "pressure"), 1013.0f), "WSDCGQ12LM scale -2");
        h.restart();
        Report s; s.u16(0x0010, 0x29, 1004);
        check(!r_find(dispatch(d, 0x0403, 1, s.span(), &h.ctx), "pressure"),
              "WSDCGQ12LM scale -2 unknown after a restart: dropped, not 100.4 hPa");
    }
}

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
    detections();
    tz_zt01_ga4();
    zm25tq_cover_8();
    wnp10_configure();
    new_devices();
    wave3_tuya_dps();
    zc301();
    wave3_zcl();
    wave4_tuya();
    wave4_zcl_s();
    wave4_zcl_m();
    wave4_lumi();
    final_review();
    settings_survive_restart();
    if (g_failures) { std::printf("FAILED: %d check(s)\n", g_failures); return 1; }
    std::printf("all parity-26125 checks passed\n");
    return 0;
}
