// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// TS011F smart-plug power monitoring. z2m-source: tuya.ts `TS011F_plug_1`
// (tuyaOnOff electricalMeasurements + tuya.fz.TS011F_electrical_measurement
// + fz.metering; configure saves acCurrentDivisor 1000 and seMetering
// divisor 100, nothing for voltage/power → factor 1).
//
// Pins:
//   * `_TZ3000_okaz9tjs` (Elivco LSPA9) resolves to its own def, any other
//     TS011F manufacturer still falls back to the generic kDefTS011F;
//   * both defs expose power/voltage/current/energy (read-only) and decode
//     a real haElectricalMeasurement / seMetering report with z2m's scaling;
//   * the okaz9tjs def binds 0x0006/0x0B04/0x0702, sends the Tuya magic
//     packet (genBasic read) and configures NO reporting (z2m #29034).

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS011F;
extern const PreparedDefinition kDefTS011F_okaz9tjs;
}  // namespace zhc::devices::tuya

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> tuya_reg() {
    return {devices::tuya::kTuyaRegistry, devices::tuya::kTuyaRegistryCount};
}

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return &def.exposes[i];
    return nullptr;
}

void check_measure_expose(const PreparedDefinition& def, const char* key,
                          const char* unit) {
    const Expose* e = find_expose(def, key);
    assert(e);
    assert(e->type == ExposeType::Numeric);
    assert(e->access == Access::State);
    assert(e->unit && std::strcmp(e->unit, unit) == 0);
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name,
                            const std::vector<std::uint8_t>& bytes) {
    InboundApsFrame raw{};
    raw.cluster_id   = cluster_id;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = std::span<const std::uint8_t>(bytes.data(), bytes.size());
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

double num(const DispatchResult& r, const char* key) {
    const Value* v = r.merged.find(key);
    assert(v);
    if (v->type == ValueType::Float) return v->f;
    if (v->type == ValueType::Int) return static_cast<double>(v->i);
    if (v->type == ValueType::Uint) return static_cast<double>(v->u);
    assert(false && "non-numeric value");
    return 0;
}

void check_decode(const PreparedDefinition& def) {
    // One haElectricalMeasurement report, three records (as TS011F sends):
    //   rmsVoltage   0x0505 u16 = 232   → 232 V
    //   rmsCurrent   0x0508 u16 = 111   → 0.111 A  (acCurrentDivisor 1000)
    //   activePower  0x050B s16 = 18    → 18 W
    const std::vector<std::uint8_t> em = {
        0x18, 0x42, 0x0A,
        0x05, 0x05, 0x21, 0xE8, 0x00,
        0x08, 0x05, 0x21, 0x6F, 0x00,
        0x0B, 0x05, 0x29, 0x12, 0x00,
    };
    auto r = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", em);
    assert(r.any_matched);
    assert(std::fabs(num(r, "voltage") - 232.0) < 1e-6);
    assert(std::fabs(num(r, "current") - 0.111) < 1e-4);
    assert(std::fabs(num(r, "power") - 18.0) < 1e-6);

    // seMetering currentSummDelivered 0x0000 u48 = 1234 → 12.34 kWh
    // (seMetering divisor 100).
    const std::vector<std::uint8_t> se = {
        0x18, 0x43, 0x0A,
        0x00, 0x00, 0x25, 0xD2, 0x04, 0x00, 0x00, 0x00, 0x00,
    };
    auto re = dispatch_zcl(def, 0x0702, "seMetering", se);
    assert(re.any_matched);
    assert(std::fabs(num(re, "energy") - 12.34) < 1e-4);
}

void check_shape(const PreparedDefinition& def) {
    assert(find_expose(def, "state"));
    assert(find_expose(def, "power_on_behavior"));
    check_measure_expose(def, "power",   "W");
    check_measure_expose(def, "voltage", "V");
    check_measure_expose(def, "current", "A");
    check_measure_expose(def, "energy",  "kWh");
    assert(def_binds(def, 0x0006));
    assert(def_binds(def, 0x0B04));
    assert(def_binds(def, 0x0702));
}

// ── configure capture ────────────────────────────────────────────────
int g_binds = 0, g_reports = 0, g_basic_reads = 0;
bool g_magic_attrs_ok = false;

bool cap_bind(std::uint16_t, std::uint8_t, std::uint16_t) { ++g_binds; return true; }
bool cap_report(std::uint16_t, std::uint8_t, std::uint16_t, std::uint16_t,
                std::uint8_t, std::uint16_t, std::uint16_t, std::uint32_t,
                std::uint16_t) { ++g_reports; return true; }
bool cap_read(std::uint16_t, std::uint8_t ep, std::uint16_t cluster,
              const std::uint8_t* attrs, std::uint8_t n, std::uint16_t manu) {
    if (cluster == 0x0000 && ep == 1 && manu == 0) {
        ++g_basic_reads;
        // z2m configureMagicPacket: manufacturerName, zclVersion,
        // appVersion, modelId, powerSource, 0xFFFE.
        const std::uint8_t want[] = {0x04, 0x00, 0x00, 0x00, 0x01, 0x00,
                                     0x05, 0x00, 0x07, 0x00, 0xFE, 0xFF};
        g_magic_attrs_ok = n == 6 && std::memcmp(attrs, want, sizeof(want)) == 0;
    }
    return true;
}

void check_okaz_configure() {
    const auto& def = devices::tuya::kDefTS011F_okaz9tjs;
    assert(def.reports_count == 0);
    RuntimeContext ctx{};
    ctx.configure_bind   = &cap_bind;
    ctx.configure_report = &cap_report;
    ctx.configure_read   = &cap_read;
    assert(run_configure(def, ctx));
    assert(g_binds == 3);
    assert(g_reports == 0);
    assert(g_basic_reads == 1);
    assert(g_magic_attrs_ok);
}

// z2m tuya.fz/tz power_on_behavior_1: genOnOff 0x8002 (moesStartUpOnOff)
// off=0 / on=1 / previous=2 — three values, no "toggle".
void check_power_on_behavior(const PreparedDefinition& def) {
    const Expose* e = find_expose(def, "power_on_behavior");
    assert(e && e->type == ExposeType::Enum && e->access == Access::StateSet);
    assert(e->enum_count == 3);
    const char* want[] = {"off", "on", "previous"};
    for (int i = 0; i < 3; ++i) assert(std::strcmp(e->enum_values[i], want[i]) == 0);

    // Report 0x8002 enum8 = 2 → "previous".
    const std::vector<std::uint8_t> rep = {0x18, 0x44, 0x0A, 0x02, 0x80, 0x30, 0x02};
    auto r = dispatch_zcl(def, 0x0006, "genOnOff", rep);
    const Value* v = r.merged.find("power_on_behavior");
    assert(v && v->type == ValueType::StringRef && std::strcmp(v->str, "previous") == 0);

    // Write "previous" → writeAttributes 0x8002 enum8 = 2.
    std::uint8_t frame[32]{};
    RuntimeContext ctx{};
    Value in{}; in.type = ValueType::StringRef; in.str = "previous";
    auto w = dispatch_to_zigbee(def, "power_on_behavior", in, ctx, frame);
    assert(w.ok && w.cluster_id == 0x0006 && w.frame_size == 7);
    const std::uint8_t exp[] = {0x10, 0x00, 0x02, 0x02, 0x80, 0x30, 0x02};
    assert(std::memcmp(frame, exp, sizeof(exp)) == 0);

    // "toggle" is not a TS011F value.
    in.str = "toggle";
    assert(!dispatch_to_zigbee(def, "power_on_behavior", in, ctx, frame).ok);
}

}  // namespace

int main() {
    using devices::tuya::kDefTS011F;
    using devices::tuya::kDefTS011F_okaz9tjs;

    // Matcher: manufacturer-specific def wins; others keep the generic.
    assert(find_definition("TS011F", "_TZ3000_okaz9tjs", tuya_reg()) == &kDefTS011F_okaz9tjs);
    assert(find_definition("TS011F", "_TZ3000_typdpbpg", tuya_reg()) == &kDefTS011F);
    assert(find_definition("TS011F", nullptr, tuya_reg()) == &kDefTS011F);

    check_shape(kDefTS011F_okaz9tjs);
    check_decode(kDefTS011F_okaz9tjs);
    check_okaz_configure();
    check_power_on_behavior(kDefTS011F_okaz9tjs);

    check_shape(kDefTS011F);
    check_decode(kDefTS011F);
    check_power_on_behavior(kDefTS011F);
    // Generic keeps z2m's default reporting path.
    assert(kDefTS011F.reports_count > 0);

    // Hub-side meter polling (z2m electricityMeasurementPoll shape): both
    // TS011F defs ask for haElectricalMeasurement AND seMetering reads —
    // _TZ3000_okaz9tjs never reports either one on its own.
    constexpr std::uint8_t kBoth = kMeterPollElectrical | kMeterPollMetering;
    assert(kDefTS011F_okaz9tjs.meter_poll == kBoth);
    assert(kDefTS011F.meter_poll == kBoth);
    // Opt-in only: nothing else in the Tuya registry (generated defs
    // included) polls.
    std::size_t polled = 0;
    for (const auto* d : tuya_reg()) polled += (d && d->meter_poll) ? 1 : 0;
    assert(polled == 2);
    return 0;
}
