// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the HFH Solutions family. Pins the real gap the
// auto-generated port carried against the z2m wire shape:
//
//   * 303-0136 (On-Air Combi CTW) uses z2m m.light({colorTemp:{range:
//     [155,495]}}), which adds the lightingColorCtrl 0x0300 colour-temp
//     channel + a color_temp expose (mireds). The auto-port wired only
//     kFzOnOff + kFzBrightness, dropping the entire CCT channel. Fixed by
//     wiring kFzColorTemperature / kTzColorTemp + the color_temp expose +
//     the 0x0300 binding.
//
// z2m-source: zigbee-herdsman-converters/src/devices/hfh.ts.

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

namespace zhc::devices::hfh {
extern const PreparedDefinition kDef_D303_0136;  // CCT LED controller
}  // namespace zhc::devices::hfh

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

// ── 303-0136: tunable-white light with color_temp via 0x0300 ────────────
void test_303_0136() {
    const auto& def = devices::hfh::kDef_D303_0136;

    // Exposes: state + brightness + color_temp (the restored CCT axis).
    check(def_exposes(def, "state"),      "303-0136 exposes state");
    check(def_exposes(def, "brightness"), "303-0136 exposes brightness");
    check(def_exposes(def, "color_temp"), "303-0136 exposes color_temp");
    check(def_binds(def, 0x0300),         "303-0136 binds lightingColorCtrl");

    // colorTemperature 0x0007 (u16) = 300 mireds → color_temp.
    const std::array<std::uint8_t, 2> ct = {0x2C, 0x01};
    auto rct = dispatch(def, 0x0300,
                        attr_report(0x0007, 0x21,
                                    std::span<const std::uint8_t>(ct.data(), ct.size())));
    check(has_key(rct, "color_temp"), "303-0136 decodes color_temp on 0x0300");

    // onOff 0x0000 (bool) = 1 → state.
    const std::array<std::uint8_t, 1> on = {0x01};
    auto rs = dispatch(def, 0x0006,
                       attr_report(0x0000, 0x10,
                                   std::span<const std::uint8_t>(on.data(), on.size())));
    check(has_key(rs, "state"), "303-0136 decodes state on 0x0006");

    // brightness still decodes (genLevelCtrl 0x0008).
    const std::array<std::uint8_t, 1> lvl = {0x7F};
    auto rb = dispatch(def, 0x0008,
                       attr_report(0x0000, 0x20,
                                   std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    check(has_key(rb, "brightness"), "303-0136 decodes brightness on 0x0008");
}

}  // namespace

int main() {
    std::printf("test_hfh_parity\n");
    test_303_0136();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
