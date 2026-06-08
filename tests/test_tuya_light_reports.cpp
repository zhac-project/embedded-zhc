// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Phase-2b Batch 3: generic Tuya ZCL-light Configure-Reporting + bindings.
//
// The generic standard-ZCL Tuya lights bound genOnOff / genLevelCtrl
// [/ lightingColorCtrl] and decoded onOff / brightness [/ colorTemp] but
// historically set up no attribute reporting (.reports = nullptr). z2m's
// profile for these is the tuyaLight extend wrapping `lib/modernExtend.ts`
// `light()` with `configureReporting: true`, which Configure-Reports:
//
//   onOff            (genOnOff 0x0006 / 0x0000, bool)        — ALWAYS
//   currentLevel     (genLevelCtrl 0x0008 / 0x0000, u8)      — ALWAYS
//   colorTemperature (lightingColorCtrl 0x0300 / 0x0007, u16)— iff CCT
//   currentX         (lightingColorCtrl 0x0300 / 0x0003, u16)— iff RGB
//   currentY         (lightingColorCtrl 0x0300 / 0x0004, u16)— iff RGB
//
// Values mirror z2m EXACTLY:
//   onOff            reporting.ts `onOff`             min=0  max=3600 change=0
//   currentLevel     reporting.ts `brightness`        min=1  max=3600 change=1
//   colorTemperature reporting.ts `colorTemperature`  min=0  max=3600 change=1
//   currentX/currentY modernExtend light() color block min=10 max=65535 change=1
//
// colorMode is intentionally NOT reported — z2m's `lib/light.ts` only
// READS colorMode; it never enters the configureReporting *report* set.
// This test pins that (asserts no colorMode report on 0x0300 / 0x0008).
//
// Per-def capability is matched to the actual def shape:
//   TS110E  — dimmer (no colour Fz)           → onOff + currentLevel
//   TS0502A — CCT (colorTemp Fz, no x/y)      → + colorTemperature
//   TS0505A — RGB+CCT (colorTemp Fz)          → + colorTemperature + x + y
//   TS0505B — RGB+CCT (colorTemp Fz)          → + colorTemperature + x + y
//
// Additionally asserts each def BINDS every cluster it reports on its
// endpoint. run_configure walks .bindings[] and .reports[] as INDEPENDENT
// loops — a report on an unbound cluster has no route to the coordinator —
// so this guard keeps bindings and reports in lockstep.

#include <cassert>
#include <cstdint>
#include <cstring>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS110E;
extern const PreparedDefinition kDefTS0502A;
extern const PreparedDefinition kDefTS0505A;
extern const PreparedDefinition kDefTS0505B;
}  // namespace zhc::devices::tuya

namespace {

using ::zhc::PreparedDefinition;
using ::zhc::ReportingSpec;

// ── reporting / binding helpers ─────────────────────────────────────
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

// ── per-attribute z2m-value asserts (all on EP1) ────────────────────
void assert_onoff(const PreparedDefinition& def) {
    const ReportingSpec* r = find_report(def, 1, 0x0006, 0x0000);
    assert(r != nullptr);
    assert(r->attr_type == 0x10);       // bool
    assert(r->min_interval == 0);
    assert(r->max_interval == 3600);
    assert(r->reportable_change == 0);
    assert(r->manufacturer_code == 0);
}

void assert_level(const PreparedDefinition& def) {
    const ReportingSpec* r = find_report(def, 1, 0x0008, 0x0000);
    assert(r != nullptr);
    assert(r->attr_type == 0x20);       // u8
    assert(r->min_interval == 1);
    assert(r->max_interval == 3600);
    assert(r->reportable_change == 1);
    assert(r->manufacturer_code == 0);
}

void assert_color_temp(const PreparedDefinition& def) {
    const ReportingSpec* r = find_report(def, 1, 0x0300, 0x0007);
    assert(r != nullptr);
    assert(r->attr_type == 0x21);       // u16
    assert(r->min_interval == 0);
    assert(r->max_interval == 3600);
    assert(r->reportable_change == 1);
    assert(r->manufacturer_code == 0);
}

void assert_current_xy(const PreparedDefinition& def) {
    const ReportingSpec* x = find_report(def, 1, 0x0300, 0x0003);
    assert(x != nullptr);
    assert(x->attr_type == 0x21);       // u16
    assert(x->min_interval == 10);
    assert(x->max_interval == 65535);
    assert(x->reportable_change == 1);
    assert(x->manufacturer_code == 0);

    const ReportingSpec* y = find_report(def, 1, 0x0300, 0x0004);
    assert(y != nullptr);
    assert(y->attr_type == 0x21);       // u16
    assert(y->min_interval == 10);
    assert(y->max_interval == 65535);
    assert(y->reportable_change == 1);
    assert(y->manufacturer_code == 0);
}

// colorMode (0x0300/0x0008) must NOT be reported — z2m only reads it.
void assert_no_color_mode(const PreparedDefinition& def) {
    assert(find_report(def, 1, 0x0300, 0x0008) == nullptr);
}

// ── capability introspection from the Fz set ────────────────────────
// A colorTemperature Fz ⇒ the def decodes lightingColorCtrl colour.
// We key on the converter cluster string to avoid hard-coding which
// generic Fz instance the def references.
bool decodes_color(const PreparedDefinition& def) {
    for (std::uint8_t i = 0; i < def.from_zigbee_count; ++i) {
        const auto* fz = def.from_zigbee[i];
        if (fz != nullptr && fz->cluster != nullptr &&
            std::strcmp(fz->cluster, "lightingColorCtrl") == 0) {
            return true;
        }
    }
    return false;
}

// Cross-check shared by every light: bindings/reports lockstep + no
// phantom-EP bleed.
void check_common(const PreparedDefinition& def) {
    assert(def.reports != nullptr);
    assert(def.reports_count > 0);

    // onOff + currentLevel are ALWAYS present, z2m values exact.
    assert_onoff(def);
    assert_level(def);
    assert_no_color_mode(def);

    // Every ReportingSpec entry's cluster must be bound on its endpoint.
    for (std::uint8_t i = 0; i < def.reports_count; ++i) {
        const ReportingSpec& r = def.reports[i];
        assert(has_binding(def, r.endpoint, r.cluster_id));
    }

    // No reporting bleeds onto a phantom second endpoint.
    assert(find_report(def, 2, 0x0006, 0x0000) == nullptr);
    assert(find_report(def, 2, 0x0008, 0x0000) == nullptr);
    assert(find_report(def, 2, 0x0300, 0x0007) == nullptr);
}

// A plain dimmer: exactly onOff + currentLevel, no colour reports.
void check_dimmer(const PreparedDefinition& def) {
    check_common(def);
    assert(!decodes_color(def));                       // capability gate
    assert(def.reports_count == 2);
    // No colour reports at all.
    assert(find_report(def, 1, 0x0300, 0x0007) == nullptr);
    assert(find_report(def, 1, 0x0300, 0x0003) == nullptr);
    assert(find_report(def, 1, 0x0300, 0x0004) == nullptr);
    // genOnOff + genLevelCtrl bound; lightingColorCtrl not expected.
    assert(has_binding(def, 1, 0x0006));
    assert(has_binding(def, 1, 0x0008));
}

// A CCT light: onOff + currentLevel + colorTemperature, no x/y.
void check_cct(const PreparedDefinition& def) {
    check_common(def);
    assert(decodes_color(def));                        // capability gate
    assert(def.reports_count == 3);
    assert_color_temp(def);
    // No x/y on a CCT-only light.
    assert(find_report(def, 1, 0x0300, 0x0003) == nullptr);
    assert(find_report(def, 1, 0x0300, 0x0004) == nullptr);
    assert(has_binding(def, 1, 0x0006));
    assert(has_binding(def, 1, 0x0008));
    assert(has_binding(def, 1, 0x0300));
}

// An RGB+CCT light: onOff + currentLevel + colorTemperature + x + y.
void check_rgbcct(const PreparedDefinition& def) {
    check_common(def);
    assert(decodes_color(def));                        // capability gate
    assert(def.reports_count == 5);
    assert_color_temp(def);
    assert_current_xy(def);
    assert(has_binding(def, 1, 0x0006));
    assert(has_binding(def, 1, 0x0008));
    assert(has_binding(def, 1, 0x0300));
}

}  // namespace

int main() {
    using namespace zhc::devices::tuya;

    // TS110E — dimmer, no colour: onOff + currentLevel only.
    check_dimmer(kDefTS110E);

    // TS0502A — tunable-white (CCT): onOff + currentLevel + colorTemp.
    check_cct(kDefTS0502A);

    // TS0505A / TS0505B — RGB+CCT: + colorTemp + currentX + currentY.
    check_rgbcct(kDefTS0505A);
    check_rgbcct(kDefTS0505B);

    return 0;
}
