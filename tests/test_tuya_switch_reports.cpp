// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Phase-2b Batch 1: generic Tuya switch family genOnOff Configure-Reporting.
// Phase-2b Batch 1b: genOnOff bindings on every gang endpoint.
//
// The generic Tuya switch parents (TS0001/2/3/4, TS0011/12/13/14, TS000F)
// already BIND genOnOff via kAutoBindings but historically set up no
// attribute reporting (.reports = nullptr). z2m's `configure:` for each
// calls `reporting.onOff(endpoint)` on every gang's endpoint. This test
// asserts each def now carries an onOff (cluster 0x0006 / attr 0x0000)
// ReportingSpec for each expected endpoint (per the gang count), and that
// the values mirror z2m's `reporting.onOff` (min=0, max=3600, change=0).
//
// Batch 1b additionally asserts each def BINDS genOnOff (0x0006) on every
// gang endpoint EP1..N. Batch 1 added .reports on EP1..N but the multi-gang
// defs bound only EP1, so EP2..N reports had no route to the coordinator
// (the firmware's run_configure walks .bindings[] and .reports[] as
// INDEPENDENT loops — no auto-bind). z2m binds every gang endpoint; this
// guard keeps bindings and reports in lockstep so the gap can't regress.

#include <cassert>
#include <cstdint>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS0001;
extern const PreparedDefinition kDefTS0002;
extern const PreparedDefinition kDefTS0003;
extern const PreparedDefinition kDefTS0004;
extern const PreparedDefinition kDefTS0011;
extern const PreparedDefinition kDefTS0012;
extern const PreparedDefinition kDefTS0013;
extern const PreparedDefinition kDefTS0014;
extern const PreparedDefinition kDefTS000F;
}  // namespace zhc::devices::tuya

namespace {

using ::zhc::BindingSpec;
using ::zhc::PreparedDefinition;
using ::zhc::ReportingSpec;

// Returns true iff `def` has an onOff (genOnOff 0x0006 / onOff attr 0x0000)
// ReportingSpec on `endpoint` whose values match z2m's reporting.onOff.
bool has_onoff_report_on(const PreparedDefinition& def, std::uint8_t endpoint) {
    if (def.reports == nullptr) return false;
    for (std::uint8_t i = 0; i < def.reports_count; ++i) {
        const ReportingSpec& r = def.reports[i];
        if (r.endpoint != endpoint) continue;
        if (r.cluster_id != 0x0006) continue;
        if (r.attr_id != 0x0000) continue;
        // bool data type for genOnOff onOff.
        assert(r.attr_type == 0x10);
        // z2m reporting.onOff = payload("onOff", 0, repInterval.HOUR, 0).
        assert(r.min_interval == 0);
        assert(r.max_interval == 3600);
        assert(r.reportable_change == 0);
        assert(r.manufacturer_code == 0);
        return true;
    }
    return false;
}

// Returns true iff `def` has a genOnOff (0x0006) BindingSpec on `endpoint`.
// A bound endpoint is required for that endpoint's attribute reports to have
// a route to the coordinator (run_configure walks .bindings[] / .reports[]
// independently — no implicit bind on the report's endpoint).
bool has_onoff_binding_on(const PreparedDefinition& def, std::uint8_t endpoint) {
    if (def.bindings == nullptr) return false;
    for (std::uint8_t i = 0; i < def.bindings_count; ++i) {
        const BindingSpec& b = def.bindings[i];
        if (b.endpoint == endpoint && b.cluster_id == 0x0006) return true;
    }
    return false;
}

// Assert def is non-null reports with exactly `gang` onOff entries, one per
// endpoint 1..gang, and no stray entries beyond the gang count. Also assert
// genOnOff is BOUND on every gang endpoint 1..gang (Batch 1b), so each gang's
// reports have a route to the coordinator.
void check_switch(const PreparedDefinition& def, std::uint8_t gang) {
    assert(def.reports != nullptr);
    assert(def.reports_count == gang);
    for (std::uint8_t ep = 1; ep <= gang; ++ep) {
        assert(has_onoff_report_on(def, ep));
        // Every gang endpoint that reports onOff must also bind onOff.
        assert(has_onoff_binding_on(def, ep));
    }
    // No reporting on a non-existent (gang+1) endpoint.
    assert(!has_onoff_report_on(def, static_cast<std::uint8_t>(gang + 1)));
    // No onOff binding on a non-existent (gang+1) endpoint.
    assert(!has_onoff_binding_on(def, static_cast<std::uint8_t>(gang + 1)));
}

}  // namespace

int main() {
    using namespace zhc::devices::tuya;

    // 1-gang → EP1 only.
    check_switch(kDefTS0001, 1);
    check_switch(kDefTS0011, 1);
    check_switch(kDefTS000F, 1);

    // 2-gang → EP1+EP2.
    check_switch(kDefTS0002, 2);
    check_switch(kDefTS0012, 2);

    // 3-gang → EP1..EP3.
    check_switch(kDefTS0003, 3);
    check_switch(kDefTS0013, 3);

    // 4-gang → EP1..EP4.
    check_switch(kDefTS0004, 4);
    check_switch(kDefTS0014, 4);

    return 0;
}
