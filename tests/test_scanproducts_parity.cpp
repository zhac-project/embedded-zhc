// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Scan Products family (1 push dimmer + 1 switch).
// Pins the real gap the auto-generated ports carried against the z2m
// wire shape:
//
//   * 12501 "Zigbee push dimmer" uses z2m m.light({configureReporting:true}).
//     m.light() defaults powerOnBehavior=true, wiring fz.power_on_behavior
//     (genOnOff 0x4003 startUpOnOff enum8) + tz.power_on_behavior + a
//     power_on_behavior enum expose. The Tier-1 auto-port wired only
//     kFzOnOff + kFzBrightness, dropping the entire power_on_behavior axis.
//     Restored via kFzPowerOnBehavior / kTzPowerOnBehavior1 + the expose.
//
//   * 12502 "Zigbee 3.0 switch" uses z2m m.onOff(), which also defaults
//     powerOnBehavior=true. The Tier-1 auto-port wired only kFzOnOff,
//     dropping power_on_behavior. Restored the same way.
//
// Also pins the FALSE suspect ("dropped color/CT on the light"): the bare
// m.light({configureReporting:true}) call passes no color/colorTemp args, so
// z2m never adds the lightingColorCtrl 0x0300 axis (modernExtend.ts light()
// only pushes fz.color_colortemp + colour exposes inside `if (colorTemp ||
// argsColor)`). This test asserts NO color_temp / color_x / hue exposes so a
// future colour sweep does not wrongly bolt one on.
//
// z2m-source: zigbee-herdsman-converters/src/devices/scanproducts.ts.

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

namespace zhc::devices::scanproducts {
extern const PreparedDefinition kDef_D12501;  // push dimmer (m.light())
extern const PreparedDefinition kDef_D12502;  // switch      (m.onOff())
}  // namespace zhc::devices::scanproducts

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

// genOnOff 0x4003 startUpOnOff (enum8): 0x01 → power_on_behavior="on".
void check_power_on_behavior(const PreparedDefinition& def, const char* tag) {
    char buf[96];
    check(def_exposes(def, "power_on_behavior"),
          (std::snprintf(buf, sizeof(buf), "%s exposes power_on_behavior", tag), buf));
    check(def_binds(def, 0x0006),
          (std::snprintf(buf, sizeof(buf), "%s binds genOnOff", tag), buf));

    const std::array<std::uint8_t, 1> on = {0x01};
    auto r = dispatch(def, 0x0006,
                      attr_report(0x4003, 0x30,
                                  std::span<const std::uint8_t>(on.data(), on.size())));
    check(has_key(r, "power_on_behavior"),
          (std::snprintf(buf, sizeof(buf), "%s decodes power_on_behavior on 0x0006", tag), buf));
}

// No colour axis: m.light() with no color/colorTemp args → no 0x0300.
void check_no_color(const PreparedDefinition& def, const char* tag) {
    char buf[96];
    check(!def_exposes(def, "color_temp"),
          (std::snprintf(buf, sizeof(buf), "%s has NO color_temp (z2m m.light() no colorTemp arg)", tag), buf));
    check(!def_exposes(def, "color_x"),
          (std::snprintf(buf, sizeof(buf), "%s has NO color_x", tag), buf));
    check(!def_exposes(def, "color_y"),
          (std::snprintf(buf, sizeof(buf), "%s has NO color_y", tag), buf));
    check(!def_exposes(def, "hue"),
          (std::snprintf(buf, sizeof(buf), "%s has NO hue", tag), buf));
    check(!def_binds(def, 0x0300),
          (std::snprintf(buf, sizeof(buf), "%s does NOT bind lightingColorCtrl", tag), buf));
}

void test_dimmer() {
    const auto& def = devices::scanproducts::kDef_D12501;
    check(def_exposes(def, "state"),      "12501 exposes state");
    check(def_exposes(def, "brightness"), "12501 exposes brightness");

    // on/off decodes (genOnOff 0x0006 onOff 0x0000 bool).
    const std::array<std::uint8_t, 1> on = {0x01};
    auto rs = dispatch(def, 0x0006,
                       attr_report(0x0000, 0x10,
                                   std::span<const std::uint8_t>(on.data(), on.size())));
    check(has_key(rs, "state"), "12501 decodes state");

    // brightness decodes (genLevelCtrl 0x0008 currentLevel 0x0000).
    const std::array<std::uint8_t, 1> lvl = {0x7F};
    auto rb = dispatch(def, 0x0008,
                       attr_report(0x0000, 0x20,
                                   std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    check(has_key(rb, "brightness"), "12501 decodes brightness");

    check_power_on_behavior(def, "12501");
    check_no_color(def, "12501");
}

void test_switch() {
    const auto& def = devices::scanproducts::kDef_D12502;
    check(def_exposes(def, "state"),       "12502 exposes state");
    check(!def_exposes(def, "brightness"), "12502 has NO brightness (m.onOff(), not a dimmer)");

    // on/off decodes (genOnOff 0x0006 onOff 0x0000 bool).
    const std::array<std::uint8_t, 1> on = {0x01};
    auto rs = dispatch(def, 0x0006,
                       attr_report(0x0000, 0x10,
                                   std::span<const std::uint8_t>(on.data(), on.size())));
    check(has_key(rs, "state"), "12502 decodes state");

    check_power_on_behavior(def, "12502");
    check_no_color(def, "12502");
}

}  // namespace

int main() {
    std::printf("test_scanproducts_parity\n");
    test_dimmer();
    test_switch();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
