// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Iluminize (German lighting) non-trivial ports.
//
// Iluminize is overwhelmingly m.light()/m.onOff() over generic clusters —
// those bundles are exercised by the generic suites. The remotes
// (511.324/.524/.541/.544/.557, 5144.*) are correctly ported as action
// devices (kFzCommand* + action expose, empty toolbox). The real z2m<->ZHC
// gaps were on the roller-shutter relay and the metering dimmers:
//
//   * 5128.10 (roller shutter) — z2m drives it as a brightness cover:
//       fz.cover_position_via_brightness (genLevelCtrl currentLevel → position
//       + state) + fz.cover_state_via_onoff (genOnOff → state) +
//       fz.cover_position_tilt (closuresWindowCovering lift). The auto-gen port
//       wired only the generic kFzCoverPosition (windowCovering attr 0x0008)
//       and exposed `position` alone — a real device reporting on genLevelCtrl
//       / genOnOff decoded to NOTHING and never surfaced state. Fixed with the
//       vendor cover-via-brightness/onoff converters + a `state` expose.
//   * 5715 / 5717 (smart dimmers) — z2m m.electricityMeter() defaults to
//       cluster:"both" ⇒ power+voltage+current (haElectricalMeasurement 0x0B04)
//       AND energy (seMetering 0x0702). The auto-gen port wired only kFzMetering
//       (0x0702) and dropped the whole 0x0B04 half — current/voltage never
//       decoded. Fixed with kFzElectricalMeasurement + current/voltage exposes
//       + the 0x0B04 binding.
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * 5128.10 exposes state + position (no tilt), decodes position from
//     genLevelCtrl currentLevel (0..255 → 0..100), OPEN/CLOSE from genOnOff,
//     and lift from closuresWindowCovering; binds 0x0006/0x0008/0x0102,
//   * 5715/5717 expose energy/power/voltage/current and decode voltage +
//     current off haElectricalMeasurement; bind 0x0B04.
//
// z2m-source: zigbee-herdsman-converters/src/devices/iluminize.ts
//             #5128.10 / #5715 / #5717 + converters/fromZigbee.ts
//             cover_position_via_brightness / cover_state_via_onoff /
//             cover_position_tilt / electrical_measurement + metering +
//             lib/modernExtend.ts electricityMeter (cluster:"both").

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::iluminize {
extern const PreparedDefinition kDef_D5128_10;
extern const PreparedDefinition kDef_D5715;
extern const PreparedDefinition kDef_D5717;
}  // namespace zhc::devices::iluminize

using namespace zhc;

namespace {

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

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report (fc=0x18, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool uint_is(const DispatchResult& r, const char* key, std::uint64_t expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::Uint) return false;
    return v->u == expected;
}

bool str_is(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

// ── 5128.10 — roller shutter, brightness cover. ──────────────────────────
void check_cover() {
    using namespace zhc::devices::iluminize;
    const auto& def = kDef_D5128_10;

    // e.cover_position() ⇒ state + position; never a tilt key.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "position"));
    assert(!def_exposes(def, "tilt"));
    assert(!def_exposes(def, "brightness"));   // not a light

    // Position via genLevelCtrl currentLevel (attr 0x0000, u8). 0xFF ≈ 100%.
    RuntimeContext ctx1{};
    const std::array<std::uint8_t, 1> lvl = {0xFF};
    auto pos = dispatch_zcl_ep(ctx1, def, 0x0008, 1, "genLevelCtrl",
                               attr_report(0x0000, 0x20,
                                           std::span<const std::uint8_t>(lvl.data(), lvl.size())));
    assert(pos.any_matched);
    assert(uint_is(pos, "position", 100));   // 255 → 100
    assert(str_is(pos, "state", "OPEN"));    // >0 ⇒ OPEN

    // currentLevel 0 → position 0, state CLOSE.
    RuntimeContext ctx2{};
    const std::array<std::uint8_t, 1> zero = {0x00};
    auto closed = dispatch_zcl_ep(ctx2, def, 0x0008, 1, "genLevelCtrl",
                                  attr_report(0x0000, 0x20,
                                              std::span<const std::uint8_t>(zero.data(), zero.size())));
    assert(closed.any_matched);
    assert(uint_is(closed, "position", 0));
    assert(str_is(closed, "state", "CLOSE"));

    // State via genOnOff onOff (attr 0x0000, bool). 1 → OPEN.
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto st = dispatch_zcl_ep(ctx3, def, 0x0006, 1, "genOnOff",
                              attr_report(0x0000, 0x10,
                                          std::span<const std::uint8_t>(on.data(), on.size())));
    assert(st.any_matched);
    assert(str_is(st, "state", "OPEN"));

    // genOnOff 0 → CLOSE.
    RuntimeContext ctx4{};
    const std::array<std::uint8_t, 1> off = {0x00};
    auto stc = dispatch_zcl_ep(ctx4, def, 0x0006, 1, "genOnOff",
                               attr_report(0x0000, 0x10,
                                           std::span<const std::uint8_t>(off.data(), off.size())));
    assert(stc.any_matched);
    assert(str_is(stc, "state", "CLOSE"));

    // Lift fallback via closuresWindowCovering attr 0x0008 (u8) = 42.
    RuntimeContext ctx5{};
    const std::array<std::uint8_t, 1> lift = {42};
    auto lf = dispatch_zcl_ep(ctx5, def, 0x0102, 1, "closuresWindowCovering",
                              attr_report(0x0008, 0x20,
                                          std::span<const std::uint8_t>(lift.data(), lift.size())));
    assert(lf.any_matched);
    assert(uint_is(lf, "position", 42));

    // Binds: genOnOff + genLevelCtrl + closuresWindowCovering.
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
    assert(def_binds(def, 1, 0x0102));

    // A real write path exists (cover is controllable).
    assert(def.to_zigbee_count == 2);
}

// ── 5715 / 5717 — metering dimmers (both clusters). ──────────────────────
void check_meter(const PreparedDefinition& def) {
    // electricityMeter(cluster:"both") ⇒ energy + power + voltage + current.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));

    // Voltage via haElectricalMeasurement RMSVoltage (attr 0x0505, u16) = 230.
    RuntimeContext ctx1{};
    const std::array<std::uint8_t, 2> volt = {0xE6, 0x00};   // 230
    auto v = dispatch_zcl_ep(ctx1, def, 0x0B04, 1, "haElectricalMeasurement",
                             attr_report(0x0505, 0x21,
                                         std::span<const std::uint8_t>(volt.data(), volt.size())));
    assert(v.any_matched);
    assert(uint_is(v, "voltage", 230));

    // Current via haElectricalMeasurement RMSCurrent (attr 0x0508, u16) = 1500.
    RuntimeContext ctx2{};
    const std::array<std::uint8_t, 2> cur = {0xDC, 0x05};    // 1500
    auto c = dispatch_zcl_ep(ctx2, def, 0x0B04, 1, "haElectricalMeasurement",
                             attr_report(0x0508, 0x21,
                                         std::span<const std::uint8_t>(cur.data(), cur.size())));
    assert(c.any_matched);
    assert(uint_is(c, "current", 1500));

    // Energy via seMetering CurrentSummDelivered (attr 0x0000, u48) = 1234.
    RuntimeContext ctx3{};
    const std::array<std::uint8_t, 6> en = {0xD2, 0x04, 0x00, 0x00, 0x00, 0x00};
    auto e = dispatch_zcl_ep(ctx3, def, 0x0702, 1, "seMetering",
                             attr_report(0x0000, 0x25,
                                         std::span<const std::uint8_t>(en.data(), en.size())));
    assert(e.any_matched);
    assert(uint_is(e, "energy", 1234));

    // Binds: both metering clusters.
    assert(def_binds(def, 1, 0x0702));
    assert(def_binds(def, 1, 0x0B04));
}

}  // namespace

int main() {
    using namespace zhc::devices::iluminize;
    check_cover();
    check_meter(kDef_D5715);
    check_meter(kDef_D5717);
    return 0;
}
