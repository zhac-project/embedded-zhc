// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Envilar family. Pins the real gaps the
// auto-generated ports carried against the z2m wire shape:
//
//   * 1CH-HP-RELAY-7853 + 7859 use z2m m.electricityMeter(), which
//     defaults to cluster:"both" + ac — decoding BOTH seMetering 0x0702
//     (energy) AND haElectricalMeasurement 0x0B04 (power/voltage/current).
//     The auto-ports wired only kFzMetering and exposed energy+power,
//     dropping the 0x0B04 half (voltage + current). Fixed by wiring
//     kFzElectricalMeasurement + the voltage/current exposes + the 0x0B04
//     binding.
//
//   * ZGR904-S is a battery scene/dimmer remote (a SENDER). z2m wires
//     fz.command_recall/on/off/move/stop + fz.battery and exposes an
//     action() enum with NO toZigbee. The auto-port mis-ported it as a
//     controllable on/off load (kFzOnOff + kTzOnOff + phantom `state`),
//     so every physical press was dropped. Fixed by wiring the generic
//     command converters → `action`, dropping `state`/kTzOnOff.
//
//   * ZG50CC-CCT-DRIVER + 5463 + 5491 use z2m m.light({colorTemp:{range:
//     [160,450]}}), which adds the lightingColorCtrl 0x0300 colour-temp
//     channel + a color_temp expose. The auto-ports wired only
//     kFzOnOff + kFzBrightness, dropping the entire CCT channel. Fixed by
//     wiring kFzColorTemperature / kTzColorTemp + the color_temp expose +
//     the 0x0300 binding.
//
// z2m-source: zigbee-herdsman-converters/src/devices/envilar.ts.

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

namespace zhc::devices::envilar {
extern const PreparedDefinition kDef_D1CH_HP_RELAY_7853;   // relay + metering
extern const PreparedDefinition kDef_D7859;                // dimmer + metering
extern const PreparedDefinition kDef_ZGR904_S;             // scene remote
extern const PreparedDefinition kDef_ZG50CC_CCT_DRIVER;    // CCT driver
extern const PreparedDefinition kDef_D5463;                // CCT driver
extern const PreparedDefinition kDef_D5491;                // CCT driver
}  // namespace zhc::devices::envilar

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

// Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn, cmd,
// then the command payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> payload = {}) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : payload) v.push_back(b);
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

bool def_lists_action(const PreparedDefinition& def, const char* action) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        const Expose& e = def.exposes[i];
        if (!e.name || std::strcmp(e.name, "action") != 0) continue;
        for (std::size_t j = 0; j < e.enum_count; ++j)
            if (e.enum_values[j] && std::strcmp(e.enum_values[j], action) == 0) return true;
    }
    return false;
}

// ── metering devices: 0x0702 energy + 0x0B04 power/voltage/current ──────
void check_metering_both(const PreparedDefinition& def, const char* tag) {
    char buf[96];

    // Exposes: energy (0x0702) + power/voltage/current (0x0B04).
    check(def_exposes(def, "energy"),  (std::snprintf(buf, sizeof(buf), "%s exposes energy", tag), buf));
    check(def_exposes(def, "power"),   (std::snprintf(buf, sizeof(buf), "%s exposes power", tag), buf));
    check(def_exposes(def, "voltage"), (std::snprintf(buf, sizeof(buf), "%s exposes voltage", tag), buf));
    check(def_exposes(def, "current"), (std::snprintf(buf, sizeof(buf), "%s exposes current", tag), buf));
    check(def_binds(def, 0x0702),      (std::snprintf(buf, sizeof(buf), "%s binds seMetering", tag), buf));
    check(def_binds(def, 0x0B04),      (std::snprintf(buf, sizeof(buf), "%s binds haElectricalMeasurement", tag), buf));

    // ActivePower 0x050B (s16) = 150 → power on 0x0B04.
    const std::array<std::uint8_t, 2> p = {0x96, 0x00};
    auto rp = dispatch(def, 0x0B04,
                       attr_report(0x050B, 0x29,
                                   std::span<const std::uint8_t>(p.data(), p.size())));
    check(has_key(rp, "power"), (std::snprintf(buf, sizeof(buf), "%s decodes power on 0x0B04", tag), buf));

    // RMSVoltage 0x0505 (u16) = 230 → voltage.
    const std::array<std::uint8_t, 2> u = {0xE6, 0x00};
    auto ru = dispatch(def, 0x0B04,
                       attr_report(0x0505, 0x21,
                                   std::span<const std::uint8_t>(u.data(), u.size())));
    check(has_key(ru, "voltage"), (std::snprintf(buf, sizeof(buf), "%s decodes voltage on 0x0B04", tag), buf));

    // RMSCurrent 0x0508 (u16) = 5 → current.
    const std::array<std::uint8_t, 2> c = {0x05, 0x00};
    auto rc = dispatch(def, 0x0B04,
                       attr_report(0x0508, 0x21,
                                   std::span<const std::uint8_t>(c.data(), c.size())));
    check(has_key(rc, "current"), (std::snprintf(buf, sizeof(buf), "%s decodes current on 0x0B04", tag), buf));

    // seMetering 0x0702: CurrentSummDelivered 0x0000 → energy still decodes.
    const std::array<std::uint8_t, 6> e = {0x10, 0x27, 0x00, 0x00, 0x00, 0x00};  // u48 10000
    auto re = dispatch(def, 0x0702,
                       attr_report(0x0000, 0x25,
                                   std::span<const std::uint8_t>(e.data(), e.size())));
    check(has_key(re, "energy"), (std::snprintf(buf, sizeof(buf), "%s decodes energy on 0x0702", tag), buf));
}

void test_7853() { check_metering_both(devices::envilar::kDef_D1CH_HP_RELAY_7853, "7853"); }
void test_7859() {
    check_metering_both(devices::envilar::kDef_D7859, "7859");
    // 7859 is also a dimmer — brightness must still decode (genLevelCtrl 0x0008).
    const std::array<std::uint8_t, 1> lvl = {0x7F};
    auto rb = dispatch(devices::envilar::kDef_D7859, 0x0008,
                       attr_report(0x0000, 0x20,
                                   std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    check(has_key(rb, "brightness"), "7859 decodes brightness on 0x0008");
}

// ── ZGR904-S: scene/dimmer remote → action, no controllable state ───────
void test_zgr904s_remote() {
    const auto& def = devices::envilar::kDef_ZGR904_S;

    // Sender shape: action expose, NO settable state, NO toZigbee.
    check(def_exposes(def, "action"),   "ZGR904-S exposes action");
    check(!def_exposes(def, "state"),   "ZGR904-S has NO phantom state expose");
    check(def.to_zigbee_count == 0,     "ZGR904-S has NO toZigbee (sender)");
    check(def_exposes(def, "battery"),  "ZGR904-S exposes battery");

    // Action enum lists the z2m values.
    check(def_lists_action(def, "recall_1"),            "ZGR904-S lists recall_1");
    check(def_lists_action(def, "recall_2"),            "ZGR904-S lists recall_2");
    check(def_lists_action(def, "on"),                  "ZGR904-S lists on");
    check(def_lists_action(def, "off"),                 "ZGR904-S lists off");
    check(def_lists_action(def, "brightness_stop"),     "ZGR904-S lists brightness_stop");
    check(def_lists_action(def, "brightness_move_up"),  "ZGR904-S lists brightness_move_up");
    check(def_lists_action(def, "brightness_move_down"),"ZGR904-S lists brightness_move_down");

    // genOnOff On (cmd 0x01) → "on"; Off (cmd 0x00) → "off".
    check(action_is(dispatch(def, 0x0006, cmd_frame(0x01)), "on"),  "ZGR904-S On cmd → action on");
    check(action_is(dispatch(def, 0x0006, cmd_frame(0x00)), "off"), "ZGR904-S Off cmd → action off");

    // genScenes Recall (cmd 0x05): group u16le + scene u8. scene 1 → recall_1.
    {
        const std::uint8_t p1[] = {0x00, 0x00, 0x01};
        check(action_is(dispatch(def, 0x0005, cmd_frame(0x05,
                            std::span<const std::uint8_t>(p1, sizeof(p1)))), "recall_1"),
              "ZGR904-S Recall scene 1 → action recall_1");
        const std::uint8_t p2[] = {0x00, 0x00, 0x02};
        check(action_is(dispatch(def, 0x0005, cmd_frame(0x05,
                            std::span<const std::uint8_t>(p2, sizeof(p2)))), "recall_2"),
              "ZGR904-S Recall scene 2 → action recall_2");
    }

    // genLevelCtrl Move up (cmd 0x01, move_mode=0, rate=50) → brightness_move_up.
    {
        const std::uint8_t up[] = {0x00, 0x32};
        check(action_is(dispatch(def, 0x0008, cmd_frame(0x01,
                            std::span<const std::uint8_t>(up, sizeof(up)))), "brightness_move_up"),
              "ZGR904-S Move up → brightness_move_up");
        const std::uint8_t down[] = {0x01, 0x32};
        check(action_is(dispatch(def, 0x0008, cmd_frame(0x01,
                            std::span<const std::uint8_t>(down, sizeof(down)))), "brightness_move_down"),
              "ZGR904-S Move down → brightness_move_down");
    }

    // genLevelCtrl Stop (cmd 0x03) → brightness_stop.
    check(action_is(dispatch(def, 0x0008, cmd_frame(0x03)), "brightness_stop"),
          "ZGR904-S Stop → brightness_stop");

    // Battery still decodes (genPowerCfg 0x0001, attr 0x0021 batteryPercentage).
    const std::array<std::uint8_t, 1> bp = {0xC8};  // 200 → 100%
    auto rbat = dispatch(def, 0x0001,
                         attr_report(0x0021, 0x20,
                                     std::span<const std::uint8_t>(bp.data(), bp.size())));
    check(has_key(rbat, "battery"), "ZGR904-S decodes battery");
}

// ── CCT drivers: color_temp via lightingColorCtrl 0x0300 ────────────────
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

void test_cct_drivers() {
    check_cct(devices::envilar::kDef_ZG50CC_CCT_DRIVER, "ZG50CC-CCT-DRIVER");
    check_cct(devices::envilar::kDef_D5463, "5463");
    check_cct(devices::envilar::kDef_D5491, "5491");
}

}  // namespace

int main() {
    std::printf("test_envilar_parity\n");
    test_7853();
    test_7859();
    test_zgr904s_remote();
    test_cct_drivers();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
