// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Schwaiger family. Pins the real gaps the
// auto-generated ports carried against the z2m wire shape:
//
//   * HAL500 uses z2m m.light({colorTemp:{range:[153,370]}}) — the
//     lightingColorCtrl 0x0300 colour-temperature channel + a color_temp
//     expose. The auto-port wired only kFzOnOff + kFzBrightness, dropping
//     the entire CCT channel. Fixed by wiring kFzColorTemperature /
//     kTzColorTemp + the color_temp expose + the 0x0300 binding.
//
//   * HAL300 / HAL550 / HAL800 use z2m m.light({colorTemp:..., color:...})
//     — both the 0x0300 colour-temperature axis AND the XY/HS colour axis.
//     The auto-ports wired only kFzOnOff + kFzBrightness, dropping the
//     whole colour channel. Fixed by wiring kFzColor + kFzColorTemperature
//     / kTzColor + kTzColorTemp + the color_x/color_y/hue/saturation/
//     color_temp exposes + the 0x0300 binding.
//
//   * ZHS-15 (control, NOT changed) uses z2m fz.on_off +
//     fz.electrical_measurement and binds genOnOff + haElectricalMeasurement
//     only — NO seMetering. It exposes switch/power/voltage/current with NO
//     energy. The auto-port matched this exactly; this test pins that shape
//     so a future "metering" sweep doesn't wrongly bolt on 0x0702/energy.
//
// z2m-source: zigbee-herdsman-converters/src/devices/schwaiger.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::schwaiger {
extern const PreparedDefinition kDef_HAL300;   // full colour (CT + XY/HS)
extern const PreparedDefinition kDef_HAL500;   // CCT only
extern const PreparedDefinition kDef_HAL550;   // full colour (CT + XY/HS)
extern const PreparedDefinition kDef_HAL800;   // full colour (CT + XY/HS)
extern const PreparedDefinition kDef_ZHS_15;   // metering plug (0x0B04 only)
}  // namespace zhc::devices::schwaiger

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

DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster_id,
                        std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report (fc=0x18 non-mfg, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// ── CCT axis only: color_temp via lightingColorCtrl 0x0300 ──────────────
void check_cct(const PreparedDefinition& def, const char* tag) {
    char buf[96];
    check(def_exposes(def, "state"),      (std::snprintf(buf, sizeof(buf), "%s exposes state", tag), buf));
    check(def_exposes(def, "brightness"), (std::snprintf(buf, sizeof(buf), "%s exposes brightness", tag), buf));
    check(def_exposes(def, "color_temp"), (std::snprintf(buf, sizeof(buf), "%s exposes color_temp", tag), buf));
    check(def_binds(def, 0x0300),         (std::snprintf(buf, sizeof(buf), "%s binds lightingColorCtrl", tag), buf));

    // colorTemperature 0x0007 (u16) = 250 mireds → color_temp.
    const std::array<std::uint8_t, 2> ct = {0xFA, 0x00};
    auto rct = dispatch(def, 0x0300,
                        attr_report(0x0007, 0x21,
                                    std::span<const std::uint8_t>(ct.data(), ct.size())));
    check(has_key(rct, "color_temp"), (std::snprintf(buf, sizeof(buf), "%s decodes color_temp on 0x0300", tag), buf));

    // brightness still decodes (genLevelCtrl 0x0008).
    const std::array<std::uint8_t, 1> lvl = {0x7F};
    auto rb = dispatch(def, 0x0008,
                       attr_report(0x0000, 0x20,
                                   std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    check(has_key(rb, "brightness"), (std::snprintf(buf, sizeof(buf), "%s decodes brightness", tag), buf));
}

// ── full colour: CCT + XY/HS via lightingColorCtrl 0x0300 ───────────────
void check_full_color(const PreparedDefinition& def, const char* tag) {
    char buf[96];
    // CCT half (also asserts state/brightness/color_temp + 0x0300 bind).
    check_cct(def, tag);

    // Colour exposes present.
    check(def_exposes(def, "color_x"),    (std::snprintf(buf, sizeof(buf), "%s exposes color_x", tag), buf));
    check(def_exposes(def, "color_y"),    (std::snprintf(buf, sizeof(buf), "%s exposes color_y", tag), buf));
    check(def_exposes(def, "hue"),        (std::snprintf(buf, sizeof(buf), "%s exposes hue", tag), buf));
    check(def_exposes(def, "saturation"), (std::snprintf(buf, sizeof(buf), "%s exposes saturation", tag), buf));

    // currentX 0x0003 (u16) → color_x.
    const std::array<std::uint8_t, 2> cx = {0x00, 0x80};
    auto rcx = dispatch(def, 0x0300,
                        attr_report(0x0003, 0x21,
                                    std::span<const std::uint8_t>(cx.data(), cx.size())));
    check(has_key(rcx, "color_x"), (std::snprintf(buf, sizeof(buf), "%s decodes color_x on 0x0300", tag), buf));

    // currentHue 0x0000 (u8) → hue.
    const std::array<std::uint8_t, 1> hue = {0x80};
    auto rh = dispatch(def, 0x0300,
                       attr_report(0x0000, 0x20,
                                   std::span<const std::uint8_t>(hue.data(), hue.size())));
    check(has_key(rh, "hue"), (std::snprintf(buf, sizeof(buf), "%s decodes hue on 0x0300", tag), buf));
}

void test_lights() {
    check_cct(devices::schwaiger::kDef_HAL500, "HAL500");
    check_full_color(devices::schwaiger::kDef_HAL300, "HAL300");
    check_full_color(devices::schwaiger::kDef_HAL550, "HAL550");
    check_full_color(devices::schwaiger::kDef_HAL800, "HAL800");
}

// ── ZHS-15: electrical-measurement plug, 0x0B04 only (NO 0x0702) ────────
void test_zhs15_metering() {
    const auto& def = devices::schwaiger::kDef_ZHS_15;

    // z2m exposes: switch/power/voltage/current — NO energy.
    check(def_exposes(def, "state"),    "ZHS-15 exposes state");
    check(def_exposes(def, "power"),    "ZHS-15 exposes power");
    check(def_exposes(def, "voltage"),  "ZHS-15 exposes voltage");
    check(def_exposes(def, "current"),  "ZHS-15 exposes current");
    check(!def_exposes(def, "energy"),  "ZHS-15 has NO energy expose (z2m has no e.energy())");

    // z2m binds genOnOff + haElectricalMeasurement only — NOT seMetering.
    check(def_binds(def, 0x0006),       "ZHS-15 binds genOnOff");
    check(def_binds(def, 0x0B04),       "ZHS-15 binds haElectricalMeasurement");
    check(!def_binds(def, 0x0702),      "ZHS-15 does NOT bind seMetering (z2m fz.electrical_measurement only)");

    // ActivePower 0x050B (s16) = 150 → power on 0x0B04.
    const std::array<std::uint8_t, 2> p = {0x96, 0x00};
    auto rp = dispatch(def, 0x0B04,
                       attr_report(0x050B, 0x29,
                                   std::span<const std::uint8_t>(p.data(), p.size())));
    check(has_key(rp, "power"), "ZHS-15 decodes power on 0x0B04");

    // RMSVoltage 0x0505 (u16) = 230 → voltage.
    const std::array<std::uint8_t, 2> u = {0xE6, 0x00};
    auto ru = dispatch(def, 0x0B04,
                       attr_report(0x0505, 0x21,
                                   std::span<const std::uint8_t>(u.data(), u.size())));
    check(has_key(ru, "voltage"), "ZHS-15 decodes voltage on 0x0B04");

    // RMSCurrent 0x0508 (u16) = 5 → current.
    const std::array<std::uint8_t, 2> c = {0x05, 0x00};
    auto rc = dispatch(def, 0x0B04,
                       attr_report(0x0508, 0x21,
                                   std::span<const std::uint8_t>(c.data(), c.size())));
    check(has_key(rc, "current"), "ZHS-15 decodes current on 0x0B04");
}

}  // namespace

int main() {
    std::printf("test_schwaiger_parity\n");
    test_lights();
    test_zhs15_metering();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
