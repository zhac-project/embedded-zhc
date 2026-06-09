// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Lonsonho (Tuya-OEM) device family.
// z2m-source: lonsonho.ts.
//
// Bug fixed: X701A (1 gang switch with backlight) declared the Tuya
// fingerprint tuya.fingerprint("TS0001", ["_TZ3000_t3s9qmmg",
// "_TZ3000_ehgouyvu"]) in z2m, but the generated def carried only the
// first manufacturer name. The "_TZ3000_ehgouyvu" hardware variant never
// matched any definition. The def was graduated to a Tier-2 parent
// override that restores both manufacturer names.
//
// The remaining tests are structural parity guards over representative
// devices of each class, pinning the shapes that prior Tuya-OEM vendors
// (zemismart / lidl / siglis / perenio) regressed on:
//   * multi-gang switches carry an endpoint_map + per-gang suffixed
//     exposes (the siglis/perenio "missing endpoint_map" bug);
//   * the attribute-based TS130F covers expose "position" on the standard
//     closuresWindowCovering cluster 0x0102 (NOT a Tuya-DP curtain), and
//     the dual module suffixes position per endpoint;
//   * the RGBCW bulb carries the colour surface.
//
// NOTE (deferred, INFRA): X711A / X712A / X713A use z2m
// legacy.fz.tuya_switch and VM-Zigbee-S02-0-10V uses a tuyaDatapoints
// map — both are Tuya-DP (0xEF00) devices whose runtime decode needs the
// Tuya-DP dispatch layer that is not yet wired in ZHC. Their exposes and
// endpoint_maps mirror z2m so the UI surfaces the right controls; the
// tests below only pin that surface, not DP decode.

#include <cassert>
#include <cstddef>
#include <cstring>
#include <span>

#include "zhc/devices/lonsonho_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lonsonho {
extern const PreparedDefinition kDef_X701A;        // 1-gang switch (fingerprint fix)
extern const PreparedDefinition kDef_QS_Zigbee_C01;     // single TS130F cover
extern const PreparedDefinition kDef_TS130F_dual;       // dual TS130F cover
extern const PreparedDefinition kDef_QS_Zigbee_S04_2C_LN;  // 2-gang switch (real EPs)
extern const PreparedDefinition kDef_X712A;        // 2-gang switch (DP-dispatched)
extern const PreparedDefinition kDef_VM_Zigbee_S02_0_10V;  // 0-10V dimmer (DP)
extern const PreparedDefinition kDef_ZB_RGBCW;     // RGBCW bulb
}  // namespace zhc::devices::lonsonho

using namespace zhc;

namespace {

bool def_has_manu(const PreparedDefinition& def, const char* manu) {
    for (std::size_t i = 0; i < def.manufacturer_names_count; ++i)
        if (def.manufacturer_names[i] && std::strcmp(def.manufacturer_names[i], manu) == 0)
            return true;
    return false;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

const Expose* def_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return &def.exposes[i];
    return nullptr;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool def_has_endpoint(const PreparedDefinition& def, const char* label) {
    for (std::size_t i = 0; i < def.endpoint_map_count; ++i)
        if (def.endpoint_map[i].label && std::strcmp(def.endpoint_map[i].label, label) == 0)
            return true;
    return false;
}

// The whole registry must contain a def that claims `manu` for `model`.
bool registry_resolves(const char* model, const char* manu) {
    for (std::size_t i = 0; i < devices::lonsonho::kLonsonhoRegistryCount; ++i) {
        const auto* d = devices::lonsonho::kLonsonhoRegistry[i];
        if (!d || !d->model || std::strcmp(d->model, model) != 0) continue;
        if (def_has_manu(*d, manu)) return true;
    }
    return false;
}

}  // namespace

// ── X701A fingerprint fix — both manufacturer names present ───────────
static void test_x701a_both_manufacturers() {
    const auto& def = devices::lonsonho::kDef_X701A;
    // The previously-present name still resolves.
    assert(def_has_manu(def, "_TZ3000_t3s9qmmg"));
    // The dropped name is restored — this is the fix.
    assert(def_has_manu(def, "_TZ3000_ehgouyvu") &&
           "_TZ3000_ehgouyvu must match X701A (was missing from generated def)");
    // And it resolves through the registry, not just the symbol.
    assert(registry_resolves("X701A", "_TZ3000_ehgouyvu"));
    assert(registry_resolves("X701A", "_TZ3000_t3s9qmmg"));
    // Plain on/off switch surface intact.
    assert(def_exposes(def, "state"));
    assert(def_binds(def, 0x0006));
}

// ── attribute-based TS130F single cover — position on 0x0102 ──────────
// z2m: fz.cover_position_tilt on closuresWindowCovering (the
// addTuyaClosuresWindowCoveringCluster extend is attribute-based, NOT the
// 0xEF00 Tuya-DP curtain). Guard against the zemismart DP-curtain trap.
static void test_c01_cover_is_standard_cluster() {
    const auto& def = devices::lonsonho::kDef_QS_Zigbee_C01;
    const Expose* pos = def_expose(def, "position");
    assert(pos && "cover must expose \"position\"");
    assert(pos->type == ExposeType::Numeric);
    assert(def_binds(def, 0x0102) && "attribute cover binds closuresWindowCovering");
    // Single-gang carries NO endpoint_map (project rule).
    assert(def.endpoint_map_count == 0);
}

// ── dual TS130F cover — per-endpoint position + endpoint_map ──────────
static void test_dual_cover_per_endpoint() {
    const auto& def = devices::lonsonho::kDef_TS130F_dual;
    assert(def.endpoint_map_count == 2);
    assert(def_has_endpoint(def, "left"));
    assert(def_has_endpoint(def, "right"));
    // Per-endpoint runtime keys are suffixed <key>_<label>.
    assert(def_exposes(def, "position_left"));
    assert(def_exposes(def, "position_right"));
    assert(!def_exposes(def, "position") && "dual cover must not carry bare position");
    // Both endpoints bind the cover cluster.
    bool ep1 = false, ep2 = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i) {
        if (def.bindings[i].cluster_id == 0x0102 && def.bindings[i].endpoint == 1) ep1 = true;
        if (def.bindings[i].cluster_id == 0x0102 && def.bindings[i].endpoint == 2) ep2 = true;
    }
    assert(ep1 && ep2);
}

// ── 2-gang switch on real endpoints {l1:1, l2:2} ─────────────────────
static void test_s04_multigang_endpoints() {
    const auto& def = devices::lonsonho::kDef_QS_Zigbee_S04_2C_LN;
    assert(def.endpoint_map_count == 2);
    assert(def_has_endpoint(def, "l1"));
    assert(def_has_endpoint(def, "l2"));
    assert(def_exposes(def, "state_l1"));
    assert(def_exposes(def, "state_l2"));
    // Distinct physical endpoints -> both endpoints bind genOnOff.
    bool ep1 = false, ep2 = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i) {
        if (def.bindings[i].cluster_id == 0x0006 && def.bindings[i].endpoint == 1) ep1 = true;
        if (def.bindings[i].cluster_id == 0x0006 && def.bindings[i].endpoint == 2) ep2 = true;
    }
    assert(ep1 && ep2);
}

// ── DP-dispatched multi-gang (X712A) — surface + endpoint_map present ─
// INFRA-deferred (legacy.fz.tuya_switch). z2m maps both gangs to EP1 and
// dispatches by Tuya DP; the endpoint_map must still carry l1/l2 so keys
// suffix correctly, and both gang exposes must be present.
static void test_x712a_dp_switch_surface() {
    const auto& def = devices::lonsonho::kDef_X712A;
    assert(def.endpoint_map_count == 2);
    assert(def_has_endpoint(def, "l1"));
    assert(def_has_endpoint(def, "l2"));
    // z2m {l1:1, l2:1}: both gangs share the same physical endpoint.
    for (std::size_t i = 0; i < def.endpoint_map_count; ++i)
        assert(def.endpoint_map[i].endpoint == 1);
    assert(def_exposes(def, "state_l1"));
    assert(def_exposes(def, "state_l2"));
}

// ── 0-10V dimmer (VM-S02) — DP surface mirrors z2m datapoints ─────────
// INFRA-deferred (tuyaDatapoints). Pin that the per-channel DP exposes
// and endpoint_map are present so the UI is correct.
static void test_vm_s02_dp_surface() {
    const auto& def = devices::lonsonho::kDef_VM_Zigbee_S02_0_10V;
    assert(def.endpoint_map_count == 2);
    assert(def_has_endpoint(def, "l1"));
    assert(def_has_endpoint(def, "l2"));
    const char* keys[] = {
        "state_l1", "brightness_l1", "min_brightness_l1", "max_brightness_l1",
        "switch_type_l1", "countdown_l1",
        "state_l2", "brightness_l2", "min_brightness_l2", "max_brightness_l2",
        "switch_type_l2", "countdown_l2", "power_on_behavior",
    };
    for (const char* k : keys) {
        assert(def_exposes(def, k));
    }
}

// ── RGBCW bulb — full colour surface ─────────────────────────────────
static void test_rgbcw_colour_surface() {
    const auto& def = devices::lonsonho::kDef_ZB_RGBCW;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_xy"));
    assert(def_binds(def, 0x0300) && "colour bulb binds lightingColorCtrl");
    // It must resolve for an eWeLink ZB-CL01 variant (cross-product manu list).
    assert(registry_resolves("ZB-RGBCW", "eWeLink"));
}

int main() {
    test_x701a_both_manufacturers();
    test_c01_cover_is_standard_cluster();
    test_dual_cover_per_endpoint();
    test_s04_multigang_endpoints();
    test_x712a_dp_switch_surface();
    test_vm_s02_dp_surface();
    test_rgbcw_colour_surface();
    return 0;
}
