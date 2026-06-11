// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Micro Matic Norge AS ZB250 (zigbeeModel SZ1000),
// a "Zigbee dimmer for LED". The z2m def is:
//
//     m.light({configureReporting:true}) + m.electricityMeter()
//
// Two real gaps the Tier-1 auto-port carried against the z2m wire shape:
//
//   * metering 0x0B04 half. m.electricityMeter() defaults cluster:"both"
//     + electricalMeasurementType:"ac" (modernExtend.ts electricityMeter()
//     → genericMeter cluster "both"), wiring BOTH fz.metering (seMetering
//     0x0702) AND fz.electrical_measurement (haElectricalMeasurement 0x0B04)
//     and exposing power/voltage/current/energy. power/voltage/current come
//     from 0x0B04, energy from 0x0702. The Tier-1 port wired only kFzMetering
//     (0x0702), dropping the whole 0x0B04 half (no voltage, no current).
//     Restored kFzElectricalMeasurement + the voltage/current exposes + the
//     0x0B04 binding.
//
//   * power_on_behavior. m.light() defaults powerOnBehavior=true (genOnOff
//     0x4003 startUpOnOff enum8). The Tier-1 port wired only kFzOnOff +
//     kFzBrightness. Restored via kFzPowerOnBehavior / kTzPowerOnBehavior1.
//
// Also pins the FALSE colour suspect: m.light() is called with no color and
// no colorTemp arg, so there is NO lightingColorCtrl 0x0300 axis. Asserts NO
// color_temp / color_x / color_y / hue expose so a future colour sweep does
// not wrongly bolt one on.
//
// z2m-source: zigbee-herdsman-converters/src/devices/micromatic.ts.

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

namespace zhc::devices::micromatic {
extern const PreparedDefinition kDef_ZB250;  // dimmer + electricityMeter
}  // namespace zhc::devices::micromatic

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

void test_light_axis() {
    const auto& def = devices::micromatic::kDef_ZB250;

    check(def_exposes(def, "state"),      "ZB250 exposes state");
    check(def_exposes(def, "brightness"), "ZB250 exposes brightness");

    // on/off decodes (genOnOff 0x0006 onOff 0x0000 bool).
    const std::array<std::uint8_t, 1> on = {0x01};
    auto rs = dispatch(def, 0x0006,
                       attr_report(0x0000, 0x10,
                                   std::span<const std::uint8_t>(on.data(), on.size())));
    check(has_key(rs, "state"), "ZB250 decodes state on genOnOff");

    // brightness decodes (genLevelCtrl 0x0008 currentLevel 0x0000 u8).
    const std::array<std::uint8_t, 1> lvl = {0x7F};
    auto rb = dispatch(def, 0x0008,
                       attr_report(0x0000, 0x20,
                                   std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    check(has_key(rb, "brightness"), "ZB250 decodes brightness on genLevelCtrl");
}

void test_power_on_behavior() {
    const auto& def = devices::micromatic::kDef_ZB250;

    check(def_exposes(def, "power_on_behavior"), "ZB250 exposes power_on_behavior");
    check(def_binds(def, 0x0006),                "ZB250 binds genOnOff");

    // genOnOff 0x4003 startUpOnOff (enum8): 0x01 → power_on_behavior="on".
    const std::array<std::uint8_t, 1> v = {0x01};
    auto r = dispatch(def, 0x0006,
                      attr_report(0x4003, 0x30,
                                  std::span<const std::uint8_t>(v.data(), v.size())));
    check(has_key(r, "power_on_behavior"),
          "ZB250 decodes power_on_behavior on genOnOff 0x4003");
}

void test_metering_0702() {
    const auto& def = devices::micromatic::kDef_ZB250;

    check(def_exposes(def, "energy"), "ZB250 exposes energy");
    check(def_binds(def, 0x0702),     "ZB250 binds seMetering");

    // seMetering 0x0702 currentSummDelivered 0x0000 (u48). energy comes from
    // the metering cluster.
    const std::array<std::uint8_t, 6> summ = {0x10, 0x27, 0x00, 0x00, 0x00, 0x00};
    auto r = dispatch(def, 0x0702,
                      attr_report(0x0000, 0x25,
                                  std::span<const std::uint8_t>(summ.data(), summ.size())));
    check(has_key(r, "energy"), "ZB250 decodes energy on seMetering 0x0000");
}

void test_electrical_0b04() {
    const auto& def = devices::micromatic::kDef_ZB250;

    // The 0x0B04 half: power/voltage/current + the binding (the SUSPECT gap).
    check(def_exposes(def, "power"),   "ZB250 exposes power");
    check(def_exposes(def, "voltage"), "ZB250 exposes voltage");
    check(def_exposes(def, "current"), "ZB250 exposes current");
    check(def_binds(def, 0x0B04),      "ZB250 binds haElectricalMeasurement");

    // haElectricalMeasurement 0x0B04: rmsVoltage 0x0505 (u16).
    const std::array<std::uint8_t, 2> volt = {0xE6, 0x00};  // 230
    auto rv = dispatch(def, 0x0B04,
                       attr_report(0x0505, 0x21,
                                   std::span<const std::uint8_t>(volt.data(), volt.size())));
    check(has_key(rv, "voltage"), "ZB250 decodes voltage on 0x0B04 0x0505");

    // rmsCurrent 0x0508 (u16).
    const std::array<std::uint8_t, 2> cur = {0x64, 0x00};  // 100
    auto rc = dispatch(def, 0x0B04,
                       attr_report(0x0508, 0x21,
                                   std::span<const std::uint8_t>(cur.data(), cur.size())));
    check(has_key(rc, "current"), "ZB250 decodes current on 0x0B04 0x0508");

    // activePower 0x050B (s16). z2m sources power from 0x0B04.
    const std::array<std::uint8_t, 2> pow = {0x32, 0x00};  // 50
    auto rp = dispatch(def, 0x0B04,
                       attr_report(0x050B, 0x29,
                                   std::span<const std::uint8_t>(pow.data(), pow.size())));
    check(has_key(rp, "power"), "ZB250 decodes power on 0x0B04 0x050B");
}

void test_no_color() {
    const auto& def = devices::micromatic::kDef_ZB250;

    // FALSE suspect: m.light() with no color/colorTemp args → no 0x0300 axis.
    check(!def_exposes(def, "color_temp"),
          "ZB250 has NO color_temp (z2m m.light() no colorTemp arg)");
    check(!def_exposes(def, "color_x"), "ZB250 has NO color_x");
    check(!def_exposes(def, "color_y"), "ZB250 has NO color_y");
    check(!def_exposes(def, "hue"),     "ZB250 has NO hue");
    check(!def_binds(def, 0x0300),      "ZB250 does NOT bind lightingColorCtrl");
}

}  // namespace

int main() {
    std::printf("test_micromatic_parity\n");
    test_light_axis();
    test_power_on_behavior();
    test_metering_0702();
    test_electrical_0b04();
    test_no_color();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
