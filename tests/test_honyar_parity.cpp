// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Honyar (Tuya-OEM) device family.
// z2m-source: honyar.ts.
//
// Bug fixed (gap class f — missing / incomplete endpoint_map): the three
// multi-gang switches each declare two or three switch endpoints in z2m
// (e.switch().withEndpoint(...) + endpoint:{...} + meta.multiEndpoint, or
// m.deviceEndpoints + m.onOff over named endpoints) and bind genOnOff on
// every endpoint. The generated defs carried a single bare `state` expose
// and bound genOnOff only on EP1, so every gang collided on the bare
// `state` key at runtime (last-writer-wins) and the upper endpoints were
// never bound. Each def was graduated to a Tier-2 parent override with
// per-gang suffixed exposes + per-endpoint genOnOff bindings + the
// endpoint_map (mirrors the lonsonho S04-2C-LN multi-gang fix shape):
//   * U2-86K21ND10-ZD (HY0096) — 2 gang {left:1, right:2}
//   * U2-86K31ND10-ZD (HY0097) — 3 gang {left:1, center:2, right:3}
//   * U86K31ND6        (00500c35) — 3 gang {left:1, center:2, right:3}
//     (already had the endpoint_map but only one bare expose + one bind)
//
// Untouched (verified correct): U2-86K11ND10-ZD (HY0095) single-gang
// switch carries NO endpoint_map (project rule) and U86Z13A16-ZJH(HA)
// (HY0043) single-endpoint metering plug (on_off + metering + electrical).
//
// NOTE (deferred, INFRA): U86Z223A10-ZJU01(GD) (HY0157) is a dual-socket
// (l1/l2) metering plug. z2m keeps power/current/voltage/energy global via
// meta.multiEndpointSkip while suffixing only the switch state per
// endpoint. The runtime suffix rewrite has no per-def "metering-skip" hook
// — adding power/energy/current to the shared kAlwaysGlobalKeys[] would
// wrongly de-suffix genuine per-socket metering on other devices — so the
// dual-endpoint split is left for that infra. The single-endpoint metering
// surface it ships today is unaffected.

#include <cassert>
#include <cstddef>
#include <cstring>

#include "zhc/devices/honyar_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::honyar {
extern const PreparedDefinition kDef_U2_86K11ND10_ZD;   // 1-gang switch
extern const PreparedDefinition kDef_U2_86K21ND10_ZD;   // 2-gang switch (FIX)
extern const PreparedDefinition kDef_U2_86K31ND10_ZD;   // 3-gang switch (FIX)
extern const PreparedDefinition kDef_U86K31ND6;         // 3-gang switch (FIX)
extern const PreparedDefinition kDef_U86Z13A16_ZJH_HA_; // metering plug
}  // namespace zhc::devices::honyar

using namespace zhc;

namespace {

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
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

bool def_binds_on(const PreparedDefinition& def, std::uint16_t cluster, std::uint8_t ep) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster && def.bindings[i].endpoint == ep) return true;
    return false;
}

bool registry_has_model(const char* model) {
    for (std::size_t i = 0; i < devices::honyar::kHonyarRegistryCount; ++i) {
        const auto* d = devices::honyar::kHonyarRegistry[i];
        if (d && d->model && std::strcmp(d->model, model) == 0) return true;
    }
    return false;
}

}  // namespace

// ── HY0096 2-gang — endpoint_map + per-gang suffixed exposes/bindings ──
static void test_2gang_left_right() {
    const auto& def = devices::honyar::kDef_U2_86K21ND10_ZD;
    // endpoint_map mirrors z2m endpoint:{left:1, right:2}.
    assert(def.endpoint_map_count == 2);
    assert(def_has_endpoint(def, "left", 1));
    assert(def_has_endpoint(def, "right", 2));
    // Per-gang suffixed exposes, no bare `state` (would collide).
    assert(def_exposes(def, "state_left"));
    assert(def_exposes(def, "state_right"));
    assert(!def_exposes(def, "state") && "2-gang must not carry a colliding bare state");
    // genOnOff bound on BOTH endpoints (z2m binds each).
    assert(def_binds_on(def, 0x0006, 1));
    assert(def_binds_on(def, 0x0006, 2) && "EP2 genOnOff was missing before the fix");
}

// ── HY0097 3-gang — {left:1, center:2, right:3} ───────────────────────
static void test_3gang_left_center_right() {
    const auto& def = devices::honyar::kDef_U2_86K31ND10_ZD;
    assert(def.endpoint_map_count == 3);
    assert(def_has_endpoint(def, "left", 1));
    assert(def_has_endpoint(def, "center", 2));
    assert(def_has_endpoint(def, "right", 3));
    assert(def_exposes(def, "state_left"));
    assert(def_exposes(def, "state_center"));
    assert(def_exposes(def, "state_right"));
    assert(!def_exposes(def, "state"));
    assert(def_binds_on(def, 0x0006, 1));
    assert(def_binds_on(def, 0x0006, 2));
    assert(def_binds_on(def, 0x0006, 3) && "EP2/EP3 genOnOff were missing before the fix");
}

// ── 00500c35 3-gang (deviceEndpoints + onOff) ─────────────────────────
static void test_u86k31nd6_full_surface() {
    const auto& def = devices::honyar::kDef_U86K31ND6;
    assert(def.endpoint_map_count == 3);
    assert(def_has_endpoint(def, "left", 1));
    assert(def_has_endpoint(def, "center", 2));
    assert(def_has_endpoint(def, "right", 3));
    // Was a single bare `state`; now per-gang.
    assert(def_exposes(def, "state_left"));
    assert(def_exposes(def, "state_center"));
    assert(def_exposes(def, "state_right"));
    assert(!def_exposes(def, "state"));
    assert(def_binds_on(def, 0x0006, 1));
    assert(def_binds_on(def, 0x0006, 2));
    assert(def_binds_on(def, 0x0006, 3) && "EP2/EP3 genOnOff were missing before the fix");
}

// ── HY0095 single-gang — NO endpoint_map (project rule) ───────────────
static void test_1gang_no_endpoint_map() {
    const auto& def = devices::honyar::kDef_U2_86K11ND10_ZD;
    assert(def.endpoint_map_count == 0 && "single-gang must carry no endpoint_map");
    assert(def_exposes(def, "state"));
    assert(!def_exposes(def, "state_left"));
    assert(def_binds_on(def, 0x0006, 1));
}

// ── HY0043 metering plug — single endpoint, metering surface intact ───
static void test_metering_plug_surface() {
    const auto& def = devices::honyar::kDef_U86Z13A16_ZJH_HA_;
    assert(def.endpoint_map_count == 0);
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    // z2m default electricityMeter uses both clusters: 0x0702 + 0x0B04.
    assert(def_binds_on(def, 0x0006, 1));
    assert(def_binds_on(def, 0x0702, 1));
    assert(def_binds_on(def, 0x0B04, 1));
}

// ── registry sanity — every model resolves ────────────────────────────
static void test_registry_models_present() {
    assert(registry_has_model("U2-86K11ND10-ZD"));
    assert(registry_has_model("U2-86K21ND10-ZD"));
    assert(registry_has_model("U2-86K31ND10-ZD"));
    assert(registry_has_model("U86K31ND6"));
    assert(registry_has_model("U86Z13A16-ZJH(HA)"));
    assert(registry_has_model("U86Z223A10-ZJU01(GD)"));
}

int main() {
    test_2gang_left_right();
    test_3gang_left_center_right();
    test_u86k31nd6_full_surface();
    test_1gang_no_endpoint_map();
    test_metering_plug_surface();
    test_registry_models_present();
    return 0;
}
