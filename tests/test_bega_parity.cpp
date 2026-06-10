// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Bega LED-luminaire family. Pins the real gap the
// auto-generated ports carried against the z2m wire shape:
//
//   * 13557 (E27 RGBW bulb) + 85000 (Garden Spotlight RGBW) are both
//     z2m m.light({colorTemp:{...}, color:...}) — full white-and-colour
//     luminaires — but the auto-port dropped the 0x0300 lightingColorCtrl
//     axis: color_temp + color_xy exposes and the cluster bind were
//     missing, so colour and colour-temperature reports decoded to
//     nothing. Fixed by wiring the generic kFzColorTemperature/kFzColor
//     (+ kTzColorTemp/kTzColor) and restoring the color_temp/color_xy
//     exposes and the {1,0x0300} bind.
//
// z2m-source: zigbee-herdsman-converters/src/devices/bega.ts.
//
// (70049 DALI control module is z2m fingerprint-only — manufacturerName
// "BEGA Gantenbrink-Leuchten KG" + empty modelID + endpoint signature —
// which the PreparedDefinition schema cannot express, INFRA defer.)

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::bega {
extern const PreparedDefinition kDef_D13557;  // E27 RGBW bulb
extern const PreparedDefinition kDef_D85000;  // Garden Spotlight RGBW
}  // namespace zhc::devices::bega

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("  FAIL: %s\n", what);
        ++g_failures;
    }
}

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t src_ep,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// ZCL attribute report (cluster-specific=false, Report=0x0A).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
}
bool def_exposes(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, name) == 0) return true;
    return false;
}
bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// ── white-and-colour luminaire: 0x0300 axis restored ────────────────
void check_colour_light(const PreparedDefinition& def, const char* tag) {
    check(def_exposes(def, "state"),      "light exposes state");
    check(def_exposes(def, "brightness"), "light exposes brightness");
    check(def_exposes(def, "color_temp"), "light exposes color_temp");
    check(def_exposes(def, "color_xy"),   "light exposes color_xy");
    check(def_binds(def, 0x0300),         "light binds lightingColorCtrl 0x0300");

    // lightingColorCtrl attr 0x0007 colorTemperature (u16 LE) -> color_temp.
    std::array<std::uint8_t, 2> ct{0x9A, 0x01};   // 410 mireds
    auto ctv = attr_report(0x0007, 0x21,
                           std::span<const std::uint8_t>(ct.data(), ct.size()));
    auto rct = dispatch_zcl(def, 0x0300, 1,
                            std::span<const std::uint8_t>(ctv.data(), ctv.size()));
    check(rct.any_matched,            "colorTemp report matched");
    check(has_key(rct, "color_temp"), "colorTemp report decodes color_temp");

    // lightingColorCtrl attr 0x0003 currentX (u16 LE) -> color_x.
    std::array<std::uint8_t, 2> cx{0x34, 0x12};
    auto cxv = attr_report(0x0003, 0x21,
                           std::span<const std::uint8_t>(cx.data(), cx.size()));
    auto rcx = dispatch_zcl(def, 0x0300, 1,
                            std::span<const std::uint8_t>(cxv.data(), cxv.size()));
    check(rcx.any_matched,         "currentX report matched");
    check(has_key(rcx, "color_x"), "currentX report decodes color_x");

    if (tag) { /* keep tag referenced for readability */ }
}

void test_13557_colour() { check_colour_light(devices::bega::kDef_D13557, "13557"); }
void test_85000_colour() { check_colour_light(devices::bega::kDef_D85000, "85000"); }

}  // namespace

int main() {
    test_13557_colour();
    test_85000_colour();
    if (g_failures) {
        std::printf("bega parity: %d FAILED\n", g_failures);
        return 1;
    }
    std::printf("bega parity: all checks passed\n");
    return 0;
}
