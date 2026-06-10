// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the GE (Jasco / Enbrighten) light + switch + metering
// family. The one real z2m<->ez gap caught here:
//
//   * PTAPT-WH02 ("Quirky smart switch") declares z2m
//     `endpoint: () => ({default: 2})` — its genOnOff cluster lives on
//     endpoint 2, not endpoint 1. The auto-port bound genOnOff on EP1 and
//     left default_endpoint=0, so outbound on/off frames route to the
//     wrong endpoint (dead toggle) and reporting binds the wrong endpoint.
//     Fixed by re-endpointing the binding ep1->ep2 and setting
//     default_endpoint=2.
//
//   * Regression guards: the GE metering devices use z2m
//     fz.metering / m.electricityMeter({cluster:"metering"}), which is
//     seMetering 0x0702 only (energy + power). They must NOT carry a
//     0x0B04 voltage/current half (that would be cluster:"both" /
//     "electrical"). The bulbs/dimmers are plain m.light() = on/off +
//     brightness with NO color_temp (z2m light() defaults color/colorTemp
//     undefined).
//
// z2m-source: zigbee-herdsman-converters/src/devices/ge.ts +
//             lib/modernExtend.ts light() / electricityMeter().

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::ge {
extern const PreparedDefinition kDef_PSB19_SW27;   // bulb (SoftWhite)
extern const PreparedDefinition kDef_D45852GE;     // plug-in dimmer
extern const PreparedDefinition kDef_D45853GE;     // plug-in switch + metering
extern const PreparedDefinition kDef_D45856GE;     // in-wall switch + metering
extern const PreparedDefinition kDef_D45857GE;     // in-wall dimmer + metering
extern const PreparedDefinition kDef_PTAPT_WH02;   // quirky switch (EP2)
extern const PreparedDefinition kDef_POTLK_WH02;   // smart plug (EP1)
}  // namespace zhc::devices::ge

using namespace zhc;

namespace {

constexpr std::uint16_t GEN_ON_OFF       = 0x0006;
constexpr std::uint16_t SE_METERING      = 0x0702;
constexpr std::uint16_t HA_ELECTRICAL    = 0x0B04;

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep &&
            def.bindings[i].cluster_id == cluster_id)
            return true;
    return false;
}

bool binds_any_ep(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

// ── PTAPT-WH02: genOnOff on EP2, default_endpoint override ───────────
void check_ptapt_wh02() {
    const auto& def = devices::ge::kDef_PTAPT_WH02;
    // controllable on/off surface kept
    assert(def_exposes(def, "state"));
    // z2m endpoint default:2 — outbound frames must route to EP2.
    assert(def.default_endpoint == 2);
    // reporting binding must target EP2, NOT EP1.
    assert(binds(def, 2, GEN_ON_OFF));
    assert(!binds(def, 1, GEN_ON_OFF));
    // plain switch — no light/metering surface.
    assert(!def_exposes(def, "brightness"));
    assert(!def_exposes(def, "energy"));
}

// ── POTLK-WH02: plain m.onOff() on EP1 (no override) ─────────────────
void check_potlk_wh02() {
    const auto& def = devices::ge::kDef_POTLK_WH02;
    assert(def_exposes(def, "state"));
    assert(def.default_endpoint == 0);           // EP1 default preserved
    assert(binds(def, 1, GEN_ON_OFF));
    assert(!def_exposes(def, "brightness"));
}

// ── Bulbs / dimmers: m.light() = on/off + brightness, NO color_temp ──
void check_lights() {
    for (const auto* defp : {&devices::ge::kDef_PSB19_SW27,
                             &devices::ge::kDef_D45852GE}) {
        const auto& def = *defp;
        assert(def_exposes(def, "state"));
        assert(def_exposes(def, "brightness"));
        // plain light() leaves color/colorTemp undefined.
        assert(!def_exposes(def, "color_temp"));
        assert(!def_exposes(def, "color"));
        // dimmers/bulbs carry no metering half.
        assert(!def_exposes(def, "energy"));
        assert(!def_exposes(def, "voltage"));
    }
}

// ── Metering switches/dimmers: seMetering 0x0702 only, NO 0x0B04 ─────
void check_metering() {
    // 45853GE / 45856GE: switch + metering (energy + power).
    for (const auto* defp : {&devices::ge::kDef_D45853GE,
                             &devices::ge::kDef_D45856GE}) {
        const auto& def = *defp;
        assert(def_exposes(def, "state"));
        assert(def_exposes(def, "energy"));
        assert(def_exposes(def, "power"));
        assert(!def_exposes(def, "brightness"));
        // cluster:"metering" / fz.metering is 0x0702 only.
        assert(binds_any_ep(def, SE_METERING));
        assert(!binds_any_ep(def, HA_ELECTRICAL));
        assert(!def_exposes(def, "voltage"));
        assert(!def_exposes(def, "current"));
    }
    // 45857GE: dimmer + metering(cluster:"metering").
    const auto& d57 = devices::ge::kDef_D45857GE;
    assert(def_exposes(d57, "state"));
    assert(def_exposes(d57, "brightness"));
    assert(def_exposes(d57, "energy"));
    assert(def_exposes(d57, "power"));
    assert(binds_any_ep(d57, SE_METERING));
    assert(!binds_any_ep(d57, HA_ELECTRICAL));
    assert(!def_exposes(d57, "voltage"));
    assert(!def_exposes(d57, "current"));
}

}  // namespace

int main() {
    check_ptapt_wh02();
    check_potlk_wh02();
    check_lights();
    check_metering();
    return 0;
}
