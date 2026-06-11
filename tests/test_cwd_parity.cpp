// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the CWD HLL6948V1 "Collingwood H2 pro" CCT downlight.
//
// Pins the real gap the auto-generated port carried against the z2m wire
// shape: HLL6948V1 uses z2m m.light({colorTemp: {range: [153, 370]}}).
// z2m m.light() with a colorTemp arg wires tz.light_colortemp +
// e.withColorTemp(range) + a reportable lightingColorCtrl 0x0300
// colorTemperature attribute (modernExtend.ts light() lines 1217-1233 +
// the configureReporting block ~1296-1298). The Tier-1 auto-port wired
// only kFzOnOff + kFzBrightness with exposes state/brightness and bindings
// 0x0006/0x0008, dropping the entire color_temp axis. Restored via the
// generic kFzColorTemperature (0x0300 attr 0x0007 colorTemperature →
// color_temp) / kTzColorTemp (moveToColorTemp 0x0A) + the color_temp
// expose + the 0x0300 binding.
//
// Also pins the FALSE over-reach: the z2m call passes colorTemp ONLY (no
// `color` arg), so there is NO xy/hs color axis. This test asserts NO
// color_x/color_y/hue/saturation exposes so a future colour sweep does not
// wrongly bolt one on.
//
// z2m-source: zigbee-herdsman-converters/src/devices/cwd.ts #HLL6948V1.

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

namespace zhc::devices::cwd {
extern const PreparedDefinition kDef_HLL6948V1;  // CCT light (m.light({colorTemp}))
}  // namespace zhc::devices::cwd

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

void check_cct_light(const PreparedDefinition& def, const char* tag) {
    char buf[96];

    // Base on/off + brightness surface survives.
    check(def_exposes(def, "state"),
          (std::snprintf(buf, sizeof(buf), "%s exposes state", tag), buf));
    check(def_exposes(def, "brightness"),
          (std::snprintf(buf, sizeof(buf), "%s exposes brightness", tag), buf));
    check(def_binds(def, 0x0006),
          (std::snprintf(buf, sizeof(buf), "%s binds genOnOff", tag), buf));
    check(def_binds(def, 0x0008),
          (std::snprintf(buf, sizeof(buf), "%s binds genLevelCtrl", tag), buf));

    // on/off decodes (genOnOff 0x0006 onOff 0x0000 bool).
    const std::array<std::uint8_t, 1> on = {0x01};
    auto rs = dispatch(def, 0x0006,
                       attr_report(0x0000, 0x10,
                                   std::span<const std::uint8_t>(on.data(), on.size())));
    check(has_key(rs, "state"),
          (std::snprintf(buf, sizeof(buf), "%s decodes state", tag), buf));

    // brightness decodes (genLevelCtrl 0x0008 currentLevel 0x0000 u8).
    const std::array<std::uint8_t, 1> lvl = {0x7F};
    auto rb = dispatch(def, 0x0008,
                       attr_report(0x0000, 0x20,
                                   std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    check(has_key(rb, "brightness"),
          (std::snprintf(buf, sizeof(buf), "%s decodes brightness", tag), buf));

    // THE RESTORED AXIS: color_temp expose + 0x0300 binding + decoder.
    check(def_exposes(def, "color_temp"),
          (std::snprintf(buf, sizeof(buf), "%s exposes color_temp (z2m colorTemp arg)", tag), buf));
    check(def_binds(def, 0x0300),
          (std::snprintf(buf, sizeof(buf), "%s binds lightingColorCtrl", tag), buf));

    // lightingColorCtrl 0x0300 attr 0x0007 colorTemperature (u16, mireds=370).
    const std::array<std::uint8_t, 2> ct = {0x72, 0x01};  // 0x0172 = 370
    auto rc = dispatch(def, 0x0300,
                       attr_report(0x0007, 0x21,
                                   std::span<const std::uint8_t>(ct.data(), ct.size())));
    check(has_key(rc, "color_temp"),
          (std::snprintf(buf, sizeof(buf), "%s decodes color_temp on 0x0300", tag), buf));
    const Value* ctv = rc.merged.find("color_temp");
    check(ctv != nullptr && ctv->type == ValueType::Uint && ctv->u == 370u,
          (std::snprintf(buf, sizeof(buf), "%s color_temp==370 mireds", tag), buf));

    // FALSE over-reach guard: z2m has NO `color` arg → no xy/hs axis.
    check(!def_exposes(def, "color_x"),
          (std::snprintf(buf, sizeof(buf), "%s has NO color_x (z2m no color arg)", tag), buf));
    check(!def_exposes(def, "color_y"),
          (std::snprintf(buf, sizeof(buf), "%s has NO color_y", tag), buf));
    check(!def_exposes(def, "hue"),
          (std::snprintf(buf, sizeof(buf), "%s has NO hue", tag), buf));
    check(!def_exposes(def, "saturation"),
          (std::snprintf(buf, sizeof(buf), "%s has NO saturation", tag), buf));
}

void test_cct_light() {
    check_cct_light(devices::cwd::kDef_HLL6948V1, "HLL6948V1");
}

}  // namespace

int main() {
    std::printf("test_cwd_parity\n");
    test_cct_light();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
