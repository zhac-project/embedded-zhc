// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the M-ELEC "Stitchy" family (3 dimmers + 1 relay).
// Pins the real gap the auto-generated ports carried against the z2m
// wire shape:
//
//   * ML-ST-D200 / ML-ST-D200-NF / ML-ST-BP-DIM use z2m m.light()
//     (BP-DIM = m.light({effect:false})). m.light() defaults
//     powerOnBehavior=true, wiring fz.power_on_behavior (genOnOff 0x4003
//     startUpOnOff enum8) + tz.power_on_behavior + a power_on_behavior
//     enum expose. The Tier-1 auto-ports wired only kFzOnOff +
//     kFzBrightness, dropping the entire power_on_behavior axis. Restored
//     via kFzPowerOnBehavior / kTzPowerOnBehavior1 + the expose.
//
//   * ML-ST-R200 uses z2m m.onOff(), which also defaults
//     powerOnBehavior=true. The Tier-1 auto-port wired only kFzOnOff,
//     dropping power_on_behavior. Restored the same way.
//
// Also pins the FALSE suspect: none of the four declare color/colorTemp.
// z2m m.light() adds the lightingColorCtrl 0x0300 axis ONLY when called
// with color/colorTemp args (modernExtend.ts light() lines 1217-1252);
// all four M-ELEC calls pass neither, so there is no colour axis. This
// test asserts NO color_temp / color_x / hue exposes so a future colour
// sweep does not wrongly bolt one on.
//
// z2m-source: zigbee-herdsman-converters/src/devices/m_elec.ts.

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

namespace zhc::devices::m_elec {
extern const PreparedDefinition kDef_ML_ST_D200;     // dimmer (m.light())
extern const PreparedDefinition kDef_ML_ST_D200_NF;  // dimmer (m.light())
extern const PreparedDefinition kDef_ML_ST_BP_DIM;   // dimmer (m.light({effect:false}))
extern const PreparedDefinition kDef_ML_ST_R200;     // relay  (m.onOff())
}  // namespace zhc::devices::m_elec

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

void check_dimmer(const PreparedDefinition& def, const char* tag) {
    char buf[96];
    check(def_exposes(def, "state"),
          (std::snprintf(buf, sizeof(buf), "%s exposes state", tag), buf));
    check(def_exposes(def, "brightness"),
          (std::snprintf(buf, sizeof(buf), "%s exposes brightness", tag), buf));

    // brightness still decodes (genLevelCtrl 0x0008 currentLevel 0x0000).
    const std::array<std::uint8_t, 1> lvl = {0x7F};
    auto rb = dispatch(def, 0x0008,
                       attr_report(0x0000, 0x20,
                                   std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    check(has_key(rb, "brightness"),
          (std::snprintf(buf, sizeof(buf), "%s decodes brightness", tag), buf));

    check_power_on_behavior(def, tag);
    check_no_color(def, tag);
}

void test_dimmers() {
    check_dimmer(devices::m_elec::kDef_ML_ST_D200,    "ML-ST-D200");
    check_dimmer(devices::m_elec::kDef_ML_ST_D200_NF, "ML-ST-D200-NF");
    check_dimmer(devices::m_elec::kDef_ML_ST_BP_DIM,  "ML-ST-BP-DIM");
}

void test_relay() {
    const auto& def = devices::m_elec::kDef_ML_ST_R200;
    check(def_exposes(def, "state"),        "ML-ST-R200 exposes state");
    check(!def_exposes(def, "brightness"),  "ML-ST-R200 has NO brightness (m.onOff(), not a dimmer)");

    // on/off still decodes (genOnOff 0x0006 onOff 0x0000 bool).
    const std::array<std::uint8_t, 1> on = {0x01};
    auto rs = dispatch(def, 0x0006,
                       attr_report(0x0000, 0x10,
                                   std::span<const std::uint8_t>(on.data(), on.size())));
    check(has_key(rs, "state"), "ML-ST-R200 decodes state");

    check_power_on_behavior(def, "ML-ST-R200");
    check_no_color(def, "ML-ST-R200");
}

}  // namespace

int main() {
    std::printf("test_m_elec_parity\n");
    test_dimmers();
    test_relay();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
