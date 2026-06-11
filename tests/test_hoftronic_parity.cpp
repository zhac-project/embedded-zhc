// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Hoftronic HD300W-ZB "Rotary smart zigbee LED dimmer".
//
// Pins the real gap the auto-generated port carried against the z2m wire
// shape: HD300W-ZB uses z2m m.light({powerOnBehavior:true, ...}). z2m
// m.light() with powerOnBehavior=true wires fz.power_on_behavior (genOnOff
// 0x4003 startUpOnOff enum8) + tz.power_on_behavior + a power_on_behavior
// enum expose (modernExtend.ts light() lines 1273-1277). The Tier-1
// auto-port wired only kFzOnOff + kFzBrightness, dropping the entire
// power_on_behavior axis. Restored via kFzPowerOnBehavior /
// kTzPowerOnBehavior1 (both genOnOff 0x4003) + the expose.
//
// Also pins the FALSE suspect: HD300W-ZB does NOT declare color/colorTemp.
// z2m m.light() adds the lightingColorCtrl 0x0300 axis ONLY when called with
// color/colorTemp args (modernExtend.ts light() lines 1217-1252); the
// hoftronic call passes neither (only configureReporting/powerOnBehavior/
// turnsOffAtBrightness1/ota/effect:false), so there is no colour axis. This
// test asserts NO color_temp / color_x / color_y / hue exposes so a future
// colour sweep does not wrongly bolt one on.
//
// z2m-source: zigbee-herdsman-converters/src/devices/hoftronic.ts.

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

namespace zhc::devices::hoftronic {
extern const PreparedDefinition kDef_HD300W_ZB;  // dimmer (m.light())
}  // namespace zhc::devices::hoftronic

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
    const Value* pob = r.merged.find("power_on_behavior");
    check(pob != nullptr && pob->type == ValueType::StringRef && pob->str != nullptr &&
              std::strcmp(pob->str, "on") == 0,
          (std::snprintf(buf, sizeof(buf), "%s power_on_behavior=on for startUpOnOff=1", tag), buf));
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

    // on/off still decodes (genOnOff 0x0006 onOff 0x0000 bool).
    const std::array<std::uint8_t, 1> on = {0x01};
    auto rs = dispatch(def, 0x0006,
                       attr_report(0x0000, 0x10,
                                   std::span<const std::uint8_t>(on.data(), on.size())));
    check(has_key(rs, "state"),
          (std::snprintf(buf, sizeof(buf), "%s decodes state", tag), buf));

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

void test_dimmer() {
    check_dimmer(devices::hoftronic::kDef_HD300W_ZB, "HD300W-ZB");
}

}  // namespace

int main() {
    std::printf("test_hoftronic_parity\n");
    test_dimmer();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
