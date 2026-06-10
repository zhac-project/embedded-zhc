// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Ajax Online LED-bulb family. Pins the real gap the
// auto-generated ports carried against the z2m wire shape: the dropped
// colour / colour-temperature axes.
//
//   * Aj_Zigbee_Led_Strip / AJ_ZB_GU10 / AJ_ZIGPROA60 / ZB_A60_RGBCW use
//     z2m m.light({colorTemp:..., color:true}) — both the lightingColorCtrl
//     0x0300 colour-temperature axis AND the XY/HS colour axis. The
//     auto-ports wired only kFzOnOff + kFzBrightness, dropping the whole
//     colour channel. Fixed by wiring kFzColor + kFzColorTemperature /
//     kTzColor + kTzColorTemp + the color_x/color_y/hue/saturation/
//     color_temp exposes + the 0x0300 binding.
//
//   * ZB-CCT_Filament uses z2m m.light({colorTemp:{range:[153,454]}}) (no
//     color) — the 0x0300 colour-temperature axis only. The auto-port
//     dropped it; fixed by wiring kFzColorTemperature / kTzColorTemp + the
//     color_temp expose + the 0x0300 binding.
//
//   * AJ_RGBCCT_CTRL (TS0505B / _TZ3210_hzy4rjz3) uses z2m
//     tuya.modernExtend.tuyaLight({color:true, colorTemp:[153,500]}) — a
//     standard-cluster RGB+CCT light. The auto-port was badly wrong: a
//     phantom battery channel (kFzBattery + battery/voltage exposes) on a
//     mains light, with NO brightness / colour / colour-temp and NO
//     to_zigbee. Fixed to a full RGB+CCT light; this test also pins that the
//     phantom battery/voltage exposes and the genPowerCfg (0x0001) binding
//     are gone.
//
// z2m-source: zigbee-herdsman-converters/src/devices/ajax_online.ts.

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

namespace zhc::devices::ajax_online {
extern const PreparedDefinition kDef_Aj_Zigbee_Led_Strip;  // full colour (CT + XY/HS)
extern const PreparedDefinition kDef_AJ_ZB_GU10;           // full colour (CT + XY/HS)
extern const PreparedDefinition kDef_AJ_ZIGPROA60;         // full colour (CT + XY/HS)
extern const PreparedDefinition kDef_ZB_A60_RGBCW;         // full colour (CT + XY/HS)
extern const PreparedDefinition kDef_ZB_CCT_Filament;      // CCT only
extern const PreparedDefinition kDef_AJ_RGBCCT_CTRL;       // full colour (Tuya, mains)
}  // namespace zhc::devices::ajax_online

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
    check_full_color(devices::ajax_online::kDef_Aj_Zigbee_Led_Strip, "Aj_Zigbee_Led_Strip");
    check_full_color(devices::ajax_online::kDef_AJ_ZB_GU10,          "AJ_ZB_GU10");
    check_full_color(devices::ajax_online::kDef_AJ_ZIGPROA60,        "AJ_ZIGPROA60");
    check_full_color(devices::ajax_online::kDef_ZB_A60_RGBCW,        "ZB_A60_RGBCW");
    check_cct(devices::ajax_online::kDef_ZB_CCT_Filament,            "ZB-CCT_Filament");

    // ZB-CCT_Filament is CCT-only — z2m has NO color:true, so no XY/HS axis.
    check(!def_exposes(devices::ajax_online::kDef_ZB_CCT_Filament, "color_x"),
          "ZB-CCT_Filament has NO color_x (z2m colorTemp-only, no color)");
}

// ── AJ_RGBCCT_CTRL: full colour Tuya light, NOT a phantom-battery sensor ─
void test_rgbcct_ctrl() {
    const auto& def = devices::ajax_online::kDef_AJ_RGBCCT_CTRL;

    // Full RGB+CCT light shape.
    check_full_color(def, "AJ_RGBCCT_CTRL");

    // Phantom battery channel must be gone (z2m tuyaLight has no battery).
    check(!def_exposes(def, "battery"), "AJ_RGBCCT_CTRL has NO battery expose (phantom dropped)");
    check(!def_exposes(def, "voltage"), "AJ_RGBCCT_CTRL has NO voltage expose (phantom dropped)");
    check(!def_binds(def, 0x0001),      "AJ_RGBCCT_CTRL does NOT bind genPowerCfg (phantom battery dropped)");

    // to_zigbee must exist now (was nullptr) so the light is controllable.
    check(def.to_zigbee_count > 0, "AJ_RGBCCT_CTRL has to_zigbee converters (was nullptr)");

    // Stays mfr-gated to _TZ3210_hzy4rjz3 so it wins over the generic TS0505B.
    check(def.manufacturer_names_count == 1, "AJ_RGBCCT_CTRL keeps its manufacturer gate");
}

}  // namespace

int main() {
    std::printf("test_ajax_online_parity\n");
    test_lights();
    test_rgbcct_ctrl();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
