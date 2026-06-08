// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// lumi (Aqara) parity Batch 1: mains-powered switch / plug
// Configure-Reporting + bindings.
//
// A worklist parity pass flagged 113 lumi defs as "missing configure":
// z2m sets up attribute reporting in each SKU's
// `configure(device, coordinatorEndpoint)` (via reporting.bind + the
// reporting.* helpers), but the ZHC lumi defs carried `.reports =
// nullptr`. The dominant *in-tree-doable* z2m pattern for the
// mains-powered relay family is a plain
// `reporting.bind(genOnOff[, ...])` + `reporting.onOff(endpoint)` (some
// add deviceTemperature or currentSummDelivered). That maps 1:1 onto a
// `ReportingSpec[]` on `PreparedDefinition.reports`, walked by
// run_configure alongside `.bindings` (same mechanism as the ZNCZ15LM
// metering-plug precedent and the ikea / Tuya-plug reporting batches).
//
// This batch wires four PARENT defs that already bound + decoded
// genOnOff but set up no reporting:
//
//   SSM-U01  (lumi.switch.n0acn2) — onOff + deviceTemperature
//   SSM-U02  (lumi.switch.l0acn2) — onOff
//   ZNCZ02LM (lumi.plug)          — onOff
//   SP-EUC01 (lumi.plug.maeu01)   — onOff + currentSummDelivered (energy)
//
// z2m exact values (lib/reporting.ts defaults):
//   reporting.onOff               payload("onOff", 0, HOUR, 0)
//                                 → 0x0006/0x0000 bool  min 0   max 3600 rc 0
//   reporting.deviceTemperature   payload("currentTemperature", 300, HOUR, 1)
//                                 → 0x0002/0x0000 s16   min 300 max 3600 rc 1
//   reporting.currentSummDelivered payload(..., 5, HOUR, 257)
//                                 → 0x0702/0x0000 u48   min 5   max 3600 rc 257
//
// Also asserts each def BINDS every cluster it reports on EP1.
// run_configure walks .bindings[] and .reports[] as INDEPENDENT loops —
// a report on an unbound endpoint has no route to the coordinator — so
// this guard keeps bindings and reports in lockstep. Per z2m, V/I/P
// (rmsVoltage/rmsCurrent/activePower) are intentionally NOT reported for
// these SKUs, so we assert their absence too.

#include <cassert>
#include <cstdint>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefSSMU01;
extern const PreparedDefinition kDefSSMU02;
extern const PreparedDefinition kDefZNCZ02LM;
extern const PreparedDefinition kDefSPEUC01;
}  // namespace zhc::devices::lumi

using ::zhc::PreparedDefinition;
using ::zhc::ReportingSpec;

namespace {

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

// genOnOff.onOff on EP1, matching z2m reporting.onOff exactly.
void assert_onoff(const PreparedDefinition& def) {
    const ReportingSpec* r = find_report(def, 1, 0x0006, 0x0000);
    assert(r != nullptr);
    assert(r->attr_type == 0x10);          // bool
    assert(r->min_interval == 0);
    assert(r->max_interval == 3600);
    assert(r->reportable_change == 0);
    assert(r->manufacturer_code == 0);
    // Reported cluster must be bound (route to coordinator).
    assert(has_binding(def, 1, 0x0006));
}

// genDeviceTempCfg.currentTemperature on EP1, z2m reporting.deviceTemperature.
void assert_device_temperature(const PreparedDefinition& def) {
    const ReportingSpec* r = find_report(def, 1, 0x0002, 0x0000);
    assert(r != nullptr);
    assert(r->attr_type == 0x29);          // s16
    assert(r->min_interval == 300);
    assert(r->max_interval == 3600);
    assert(r->reportable_change == 1);
    assert(r->manufacturer_code == 0);
    assert(has_binding(def, 1, 0x0002));
}

// seMetering.currentSummDelivered on EP1, z2m reporting.currentSummDelivered.
void assert_energy(const PreparedDefinition& def) {
    const ReportingSpec* r = find_report(def, 1, 0x0702, 0x0000);
    assert(r != nullptr);
    assert(r->attr_type == 0x25);          // u48
    assert(r->min_interval == 5);
    assert(r->max_interval == 3600);
    assert(r->reportable_change == 257);
    assert(r->manufacturer_code == 0);
    assert(has_binding(def, 1, 0x0702));
}

// z2m intentionally does NOT configureReporting V/I/P for these SKUs.
void assert_no_electrical(const PreparedDefinition& def) {
    assert(find_report(def, 1, 0x0B04, 0x0505) == nullptr);  // rmsVoltage
    assert(find_report(def, 1, 0x0B04, 0x0508) == nullptr);  // rmsCurrent
    assert(find_report(def, 1, 0x0B04, 0x050B) == nullptr);  // activePower
}

void test_ssm_u01() {
    const auto& def = zhc::devices::lumi::kDefSSMU01;
    assert(def.reports != nullptr);
    assert(def.reports_count == 2);
    assert_onoff(def);
    assert_device_temperature(def);
    assert_no_electrical(def);
}

void test_ssm_u02() {
    const auto& def = zhc::devices::lumi::kDefSSMU02;
    assert(def.reports != nullptr);
    assert(def.reports_count == 1);
    assert_onoff(def);
    // No device-temperature report for the 2-ch variant (z2m only does onOff).
    assert(find_report(def, 1, 0x0002, 0x0000) == nullptr);
    assert_no_electrical(def);
}

void test_zncz02lm() {
    const auto& def = zhc::devices::lumi::kDefZNCZ02LM;
    assert(def.reports != nullptr);
    assert(def.reports_count == 1);
    assert_onoff(def);
    // Decodes power/energy but z2m does not configureReporting them here.
    assert(find_report(def, 1, 0x0702, 0x0000) == nullptr);
    assert_no_electrical(def);
}

void test_sp_euc01() {
    const auto& def = zhc::devices::lumi::kDefSPEUC01;
    assert(def.reports != nullptr);
    assert(def.reports_count == 2);
    assert_onoff(def);
    assert_energy(def);
    // z2m: "Voltage and current are not supported" — no V/I/P reporting.
    assert_no_electrical(def);
}

}  // namespace

int main() {
    test_ssm_u01();
    test_ssm_u02();
    test_zncz02lm();
    test_sp_euc01();
    return 0;
}
