// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for BTicino (Legrand-family) devices:
//   * F20T60A — DIN power module. z2m m.electricityMeter({cluster:
//     "electrical"}) measures via haElectricalMeasurement (0x0B04) only:
//     power/voltage/current, NO energy. The generated port wrongly wired
//     kFzMetering (seMetering 0x0702) + exposed a phantom `energy` and
//     dropped voltage+current. Fixed → kFzElectricalMeasurement.
//   * K4003C/... — light switch. z2m fz.K4003C_binary_input decodes
//     genBinaryInput (0x000F) presentValue into action ("on"/"off") +
//     state. The generated port wired only kFzOnOff and dropped the
//     action surface. Fixed → added kFzK4003CBinaryInput + action expose.
//
// z2m-source: zigbee-herdsman-converters/src/devices/bticino.ts
//             #F20T60A (electricityMeter cluster:"electrical"),
//             #K4003C/... (fz.K4003C_binary_input).

#include <array>
#include <cassert>
#include <cmath>
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

namespace zhc::devices::bticino {
extern const PreparedDefinition kDef_F20T60A;
extern const PreparedDefinition kDef_K4003C_L4003C_N4003C_NT4003C;
}  // namespace zhc::devices::bticino

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

bool action_is(const DispatchResult& r, const char* expected) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
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

// ── F20T60A: haElectricalMeasurement, not seMetering ────────────────────
void test_f20t60a_electrical() {
    const auto& def = devices::bticino::kDef_F20T60A;

    // ActivePower 0x050B (s16) = 150 → power 150.
    const std::array<std::uint8_t, 2> p = {0x96, 0x00};  // 150
    auto rp = dispatch(def, 0x0B04,
                       attr_report(0x050B, 0x29,
                                   std::span<const std::uint8_t>(p.data(), p.size())));
    const Value* pv = rp.merged.find("power");
    check(pv != nullptr, "F20T60A decodes power on 0x0B04");

    // RMSVoltage 0x0505 (u16) = 230 → voltage.
    const std::array<std::uint8_t, 2> u = {0xE6, 0x00};  // 230
    auto ru = dispatch(def, 0x0B04,
                       attr_report(0x0505, 0x21,
                                   std::span<const std::uint8_t>(u.data(), u.size())));
    check(has_key(ru, "voltage"), "F20T60A decodes voltage on 0x0B04");

    // RMSCurrent 0x0508 (u16) = 5 → current.
    const std::array<std::uint8_t, 2> c = {0x05, 0x00};
    auto rc = dispatch(def, 0x0B04,
                       attr_report(0x0508, 0x21,
                                   std::span<const std::uint8_t>(c.data(), c.size())));
    check(has_key(rc, "current"), "F20T60A decodes current on 0x0B04");

    // Exposes match z2m electricityMeter(cluster:"electrical").
    check(def_exposes(def, "power"),   "F20T60A exposes power");
    check(def_exposes(def, "voltage"), "F20T60A exposes voltage");
    check(def_exposes(def, "current"), "F20T60A exposes current");
    check(!def_exposes(def, "energy"), "F20T60A NO phantom energy expose");
    check(def_binds(def, 0x0B04), "F20T60A binds haElectricalMeasurement");
    check(!def_binds(def, 0x0702), "F20T60A does NOT bind seMetering");

    // onOff relay still works (m.onOff()).
    const std::array<std::uint8_t, 1> on = {0x01};
    auto rs = dispatch(def, 0x0006,
                       attr_report(0x0000, 0x10,
                                   std::span<const std::uint8_t>(on.data(), on.size())));
    check(has_key(rs, "state"), "F20T60A onOff decodes state");
}

// ── K4003C: genBinaryInput presentValue → action + state ────────────────
void test_k4003c_binary_input() {
    const auto& def = devices::bticino::kDef_K4003C_L4003C_N4003C_NT4003C;

    // presentValue 0x0055 (bool) = 1 → action "on", state on.
    const std::array<std::uint8_t, 1> on = {0x01};
    auto ron = dispatch(def, 0x000F,
                        attr_report(0x0055, 0x10,
                                    std::span<const std::uint8_t>(on.data(), on.size())));
    check(action_is(ron, "on"), "K4003C presentValue 1 → action on");
    const Value* son = ron.merged.find("state");
    check(son && son->type == ValueType::Bool && son->b, "K4003C presentValue 1 → state on");

    // presentValue = 0 → action "off", state off.
    const std::array<std::uint8_t, 1> off = {0x00};
    auto roff = dispatch(def, 0x000F,
                         attr_report(0x0055, 0x10,
                                     std::span<const std::uint8_t>(off.data(), off.size())));
    check(action_is(roff, "off"), "K4003C presentValue 0 → action off");
    const Value* soff = roff.merged.find("state");
    check(soff && soff->type == ValueType::Bool && !soff->b, "K4003C presentValue 0 → state off");

    // genOnOff relay state still decodes (kFzOnOff retained).
    auto rs = dispatch(def, 0x0006,
                       attr_report(0x0000, 0x10,
                                   std::span<const std::uint8_t>(on.data(), on.size())));
    check(has_key(rs, "state"), "K4003C onOff decodes state");

    // Surface + bindings.
    check(def_exposes(def, "action"), "K4003C exposes action");
    check(def_exposes(def, "state"),  "K4003C exposes state");
    check(def_binds(def, 0x000F),     "K4003C binds genBinaryInput");
}

}  // namespace

int main() {
    std::printf("test_bticino_parity\n");
    test_f20t60a_electrical();
    test_k4003c_binary_input();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
