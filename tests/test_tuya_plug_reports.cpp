// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Phase-2b Batch 2: generic Tuya smart-plug Configure-Reporting + bindings.
//
// The generic Tuya plug parents (TS011F `TS011F_plug_1`, TS0121
// `TS0121_plug`) bound + decoded genOnOff / haElectricalMeasurement /
// seMetering but historically set up no attribute reporting
// (.reports = nullptr). z2m's profile for both is
// `tuya.modernExtend.tuyaOnOff({electricalMeasurements: true})` +
// `electricityMeasurementPoll`, exposing state + voltage + current + power
// + energy. This test asserts each def now carries the matching .reports:
//
//   onOff   (genOnOff 0x0006 / onOff 0x0000, bool)        — ALWAYS
//   rmsVoltage  (haElectricalMeasurement 0x0B04 / 0x0505) — iff V/I/P exposed
//   rmsCurrent  (haElectricalMeasurement 0x0B04 / 0x0508) — iff V/I/P exposed
//   activePower (haElectricalMeasurement 0x0B04 / 0x050B) — iff V/I/P exposed
//   currentSummationDelivered (seMetering 0x0702 / 0x0000) — iff energy exposed
//
// Values mirror z2m's lib/reporting.ts EXACTLY:
//   onOff                 payload("onOff", 0, HOUR, 0)         min=0 max=3600 rc=0
//   rmsVoltage/rmsCurrent payload(..., 5, HOUR, 1)            min=5 max=3600 rc=1
//   activePower           payload("activePower", 5, HOUR, 1)  min=5 max=3600 rc=1
//   currentSummDelivered  payload(..., 5, HOUR, 257)          min=5 max=3600 rc=257
//
// Additionally asserts each def BINDS every cluster it reports on EP1
// (genOnOff + haElectricalMeasurement + seMetering). run_configure walks
// .bindings[] and .reports[] as INDEPENDENT loops — a report on an unbound
// endpoint has no route to the coordinator — so this guard keeps bindings
// and reports in lockstep. We do NOT report a cluster the def does not
// expose, so the metering report is gated on an actual energy expose.

#include <cassert>
#include <cstdint>
#include <cstring>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS011F;
extern const PreparedDefinition kDefTS0121;
}  // namespace zhc::devices::tuya

namespace {

using ::zhc::PreparedDefinition;
using ::zhc::ReportingSpec;

// ── expose introspection ────────────────────────────────────────────
// These generic plug parents carry the lumi power/meter converters whose
// runtime states are voltage/current/power (electrical) and energy
// (metering). The current auto-generated exposes are minimal (`state`,
// optionally `power_on_behavior`), so we infer the profile from the
// from_zigbee converter set: an haElectricalMeasurement Fz ⇒ V/I/P, and
// an seMetering Fz ⇒ energy. This mirrors what the def actually decodes,
// which is exactly the set z2m configures reporting for.
bool decodes_cluster(const PreparedDefinition& def, const char* cluster) {
    for (std::uint8_t i = 0; i < def.from_zigbee_count; ++i) {
        const auto* fz = def.from_zigbee[i];
        if (fz != nullptr && fz->cluster != nullptr &&
            std::strcmp(fz->cluster, cluster) == 0) {
            return true;
        }
    }
    return false;
}
bool exposes_electrical(const PreparedDefinition& def) {
    return decodes_cluster(def, "haElectricalMeasurement");
}
bool exposes_energy(const PreparedDefinition& def) {
    return decodes_cluster(def, "seMetering");
}

// ── reporting helpers ───────────────────────────────────────────────
const ReportingSpec* find_report(const PreparedDefinition& def,
                                 std::uint8_t endpoint,
                                 std::uint16_t cluster_id,
                                 std::uint16_t attr_id) {
    if (def.reports == nullptr) return nullptr;
    for (std::uint8_t i = 0; i < def.reports_count; ++i) {
        const ReportingSpec& r = def.reports[i];
        if (r.endpoint == endpoint && r.cluster_id == cluster_id &&
            r.attr_id == attr_id) {
            return &r;
        }
    }
    return nullptr;
}

bool has_binding(const PreparedDefinition& def, std::uint8_t endpoint,
                 std::uint16_t cluster_id) {
    if (def.bindings == nullptr) return false;
    for (std::uint8_t i = 0; i < def.bindings_count; ++i) {
        if (def.bindings[i].endpoint == endpoint &&
            def.bindings[i].cluster_id == cluster_id) {
            return true;
        }
    }
    return false;
}

// Assert an onOff report on EP1 matching z2m reporting.onOff exactly.
void assert_onoff(const PreparedDefinition& def) {
    const ReportingSpec* r = find_report(def, 1, 0x0006, 0x0000);
    assert(r != nullptr);
    assert(r->attr_type == 0x10);       // bool
    assert(r->min_interval == 0);
    assert(r->max_interval == 3600);
    assert(r->reportable_change == 0);
    assert(r->manufacturer_code == 0);
}

// Assert the three haElectricalMeasurement reports on EP1, z2m values.
void assert_electrical(const PreparedDefinition& def) {
    // rmsVoltage 0x0505 — u16, payload(...,5,HOUR,1).
    const ReportingSpec* v = find_report(def, 1, 0x0B04, 0x0505);
    assert(v != nullptr);
    assert(v->attr_type == 0x21);       // u16
    assert(v->min_interval == 5);
    assert(v->max_interval == 3600);
    assert(v->reportable_change == 1);
    assert(v->manufacturer_code == 0);

    // rmsCurrent 0x0508 — u16, payload(...,5,HOUR,1).
    const ReportingSpec* c = find_report(def, 1, 0x0B04, 0x0508);
    assert(c != nullptr);
    assert(c->attr_type == 0x21);       // u16
    assert(c->min_interval == 5);
    assert(c->max_interval == 3600);
    assert(c->reportable_change == 1);
    assert(c->manufacturer_code == 0);

    // activePower 0x050B — s16, payload(...,5,HOUR,1).
    const ReportingSpec* p = find_report(def, 1, 0x0B04, 0x050B);
    assert(p != nullptr);
    assert(p->attr_type == 0x29);       // s16
    assert(p->min_interval == 5);
    assert(p->max_interval == 3600);
    assert(p->reportable_change == 1);
    assert(p->manufacturer_code == 0);
}

// Assert the seMetering currentSummationDelivered report on EP1, z2m values.
void assert_metering(const PreparedDefinition& def) {
    const ReportingSpec* e = find_report(def, 1, 0x0702, 0x0000);
    assert(e != nullptr);
    assert(e->attr_type == 0x25);       // u48
    assert(e->min_interval == 5);
    assert(e->max_interval == 3600);
    assert(e->reportable_change == 257);
    assert(e->manufacturer_code == 0);
}

// Full per-def check: reports match the def's actual exposed clusters, and
// every reported cluster is bound on EP1. No report for a non-exposed
// cluster, and no report on a phantom EP2.
void check_plug(const PreparedDefinition& def) {
    assert(def.reports != nullptr);
    assert(def.reports_count > 0);

    // onOff is always present + always bound.
    assert_onoff(def);
    assert(has_binding(def, 1, 0x0006));

    const bool elec = exposes_electrical(def);
    const bool ener = exposes_energy(def);

    if (elec) {
        assert_electrical(def);
        // Every reported cluster must be bound.
        assert(has_binding(def, 1, 0x0B04));
    } else {
        // No electrical reports if not exposed.
        assert(find_report(def, 1, 0x0B04, 0x0505) == nullptr);
        assert(find_report(def, 1, 0x0B04, 0x0508) == nullptr);
        assert(find_report(def, 1, 0x0B04, 0x050B) == nullptr);
    }

    if (ener) {
        assert_metering(def);
        assert(has_binding(def, 1, 0x0702));
    } else {
        assert(find_report(def, 1, 0x0702, 0x0000) == nullptr);
    }

    // Cross-check: every ReportingSpec entry's cluster is bound on its
    // endpoint (bindings/reports lockstep — no orphan report).
    for (std::uint8_t i = 0; i < def.reports_count; ++i) {
        const ReportingSpec& r = def.reports[i];
        assert(has_binding(def, r.endpoint, r.cluster_id));
    }

    // No reporting bleeds onto a phantom second endpoint.
    assert(find_report(def, 2, 0x0006, 0x0000) == nullptr);
    assert(find_report(def, 2, 0x0B04, 0x0505) == nullptr);
    assert(find_report(def, 2, 0x0702, 0x0000) == nullptr);
}

}  // namespace

int main() {
    using namespace zhc::devices::tuya;

    // Both plugs expose state + voltage + current + power + energy, so each
    // carries onOff + 3 electrical + 1 metering = 5 reports on EP1, with
    // genOnOff + haElectricalMeasurement + seMetering all bound on EP1.
    check_plug(kDefTS011F);
    check_plug(kDefTS0121);

    // Explicit profile assertion: both are full V/I/P + energy plugs.
    assert(exposes_electrical(kDefTS011F));
    assert(exposes_energy(kDefTS011F));
    assert(exposes_electrical(kDefTS0121));
    assert(exposes_energy(kDefTS0121));

    // Exactly five reports each (onOff + rmsV + rmsI + activeP + energy).
    assert(kDefTS011F.reports_count == 5);
    assert(kDefTS0121.reports_count == 5);

    return 0;
}
