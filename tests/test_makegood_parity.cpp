// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the MakeGood (Tuya-OEM) device family.
// z2m-source: makegood.ts.
//
// Bug fixed (gap class b — MISSING decoder/channel): MG-AUZG01
// (_TZ3000_dd8wwzcy, TS011F) is a metering double power point. In z2m it
// uses tuya.modernExtend.tuyaOnOff({endpoints:["l1","l2"],
// electricalMeasurements:true}); the electricalMeasurements branch adds
// fz.electrical_measurement + fz.metering and exposes power/current/
// voltage/energy. The auto-port did not expand that modernExtend, so the
// generated def carried on/off ONLY — the entire metering surface (0x0702
// energy + 0x0B04 power/voltage/current) was dropped. The def was graduated
// to a Tier-2 parent override wiring the generic seMetering +
// haElectricalMeasurement decoders, the 4 exposes, and the 0x0702/0x0B04
// (+EP2 genOnOff) bindings z2m configure() establishes.
//
// DEFERRED (INFRA, gap class f): MG-AUZG01 declares two switch endpoints
// (l1/l2) with meta.multiEndpointSkip keeping metering global. The runtime
// suffix rewrite has no per-def metering-skip hook (only `voltage` is in
// kAlwaysGlobalKeys[]), so an endpoint_map would wrongly suffix
// power/current/energy. Same defer as Honyar U86Z223A10-ZJU01(GD). The def
// ships a single bare `state` + single (EP1) metering surface.
//
// Untouched (verified correct): MG-GPO01 (_TZ3210_bep7ccew) already declares
// fz.on_off + fz.electrical_measurement + fz.metering with the right/left
// endpoint_map in z2m; the generated def matched it exactly.

#include <cassert>
#include <cstddef>
#include <cstring>

#include "zhc/devices/makegood_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::makegood {
extern const PreparedDefinition kDef_MG_AUZG01;   // metering plug (FIX)
extern const PreparedDefinition kDef_MG_GPO01;    // dual-outlet metering plug
}  // namespace zhc::devices::makegood

using namespace zhc;

namespace {

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_has_fz(const PreparedDefinition& def, const FzConverter* conv) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i)
        if (def.from_zigbee[i] == conv) return true;
    return false;
}

bool def_binds_on(const PreparedDefinition& def, std::uint16_t cluster, std::uint8_t ep) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster && def.bindings[i].endpoint == ep) return true;
    return false;
}

bool def_has_endpoint(const PreparedDefinition& def, const char* label, std::uint8_t ep) {
    for (std::size_t i = 0; i < def.endpoint_map_count; ++i)
        if (def.endpoint_map[i].label &&
            std::strcmp(def.endpoint_map[i].label, label) == 0 &&
            def.endpoint_map[i].endpoint == ep)
            return true;
    return false;
}

bool registry_has_model(const char* model) {
    for (std::size_t i = 0; i < devices::makegood::kMakegoodRegistryCount; ++i) {
        const auto* d = devices::makegood::kMakegoodRegistry[i];
        if (d && d->model && std::strcmp(d->model, model) == 0) return true;
    }
    return false;
}

}  // namespace

// ── MG-AUZG01 — metering surface restored (was on/off-only) ───────────
static void test_auzg01_metering_surface() {
    const auto& def = devices::makegood::kDef_MG_AUZG01;
    // Metering exposes added by the electricalMeasurements branch.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power") && "power was dropped before the fix");
    assert(def_exposes(def, "energy") && "energy was dropped before the fix");
    assert(def_exposes(def, "voltage") && "voltage was dropped before the fix");
    assert(def_exposes(def, "current") && "current was dropped before the fix");
    // Generic seMetering + haElectricalMeasurement decoders are wired.
    assert(def_has_fz(def, &::zhc::generic::kFzMetering) && "seMetering decoder missing");
    assert(def_has_fz(def, &::zhc::generic::kFzElectricalMeasurement) &&
           "haElectricalMeasurement decoder missing");
    // Bindings: genOnOff + seMetering(0x0702) + haElectricalMeasurement(0x0B04)
    // on EP1 (z2m configure), plus genOnOff on EP2 (second outlet).
    assert(def_binds_on(def, 0x0006, 1));
    assert(def_binds_on(def, 0x0702, 1) && "seMetering bind was missing before the fix");
    assert(def_binds_on(def, 0x0B04, 1) && "haElectricalMeasurement bind was missing before the fix");
    assert(def_binds_on(def, 0x0006, 2) && "EP2 genOnOff was missing before the fix");
    // INFRA defer: no endpoint_map (would mis-suffix metering); single bare state.
    assert(def.endpoint_map_count == 0 && "dual-endpoint split is the documented INFRA defer");
}

// ── MG-GPO01 — already-correct dual-outlet metering plug ──────────────
static void test_gpo01_full_surface() {
    const auto& def = devices::makegood::kDef_MG_GPO01;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    assert(def_has_fz(def, &::zhc::generic::kFzMetering));
    assert(def_has_fz(def, &::zhc::generic::kFzElectricalMeasurement));
    // z2m endpoint:{right:1, left:2}.
    assert(def.endpoint_map_count == 2);
    assert(def_has_endpoint(def, "right", 1));
    assert(def_has_endpoint(def, "left", 2));
    assert(def_binds_on(def, 0x0006, 1));
    assert(def_binds_on(def, 0x0702, 1));
    assert(def_binds_on(def, 0x0B04, 1));
}

// ── registry sanity — both models resolve ─────────────────────────────
static void test_registry_models_present() {
    assert(registry_has_model("MG-AUZG01"));
    assert(registry_has_model("MG-GPO01"));
}

int main() {
    test_auzg01_metering_surface();
    test_gpo01_full_surface();
    test_registry_models_present();
    return 0;
}
