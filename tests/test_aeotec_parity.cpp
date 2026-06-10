// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Aeotec (Pico switch / shutter / range extender)
// family. The auto-generated ports carried four real gaps pinned here
// after the fix:
//
//   WG001 phantom on/off —
//     z2m: fromZigbee=[fz.linkquality_from_basic], toZigbee=[], exposes=[].
//     It is a mains range extender (router), not a switch. The port wired a
//     controllable kFzOnOff/kTzOnOff + a `state` expose + a 0x0006 bind.
//     Stripped to an exposeless def.
//
//   electricityMeter cluster:"both" — 0x0B04 electrical half dropped —
//     ZGA002 / ZGA003: m.electricityMeter() defaults to cluster:"both",
//     decoding seMetering (0x0702: energy) AND haElectricalMeasurement
//     (0x0B04: power/voltage/current). The port wired only kFzMetering,
//     dropping voltage/current. Added kFzElectricalMeasurement + V/I
//     exposes + the 0x0B04 bind.
//
//   windowCovering tilt 0x0009 dropped —
//     ZGA004: m.windowCovering({controls:["lift","tilt"]}) exposes position
//     AND tilt. The port wired only kFzCoverPosition (lift). Added
//     kFzCoverTilt + kTzCoverPositionTilt + a `tilt` expose, and dropped the
//     phantom on/off `state` bundle (a shutter has no controllable on/off).
//
//   m.deviceTemperature() + command (scene-button) endpoints dropped —
//     all three ZGA*: m.deviceTemperature() (genDeviceTempCfg 0x0002) and
//     m.commandsOnOff/LevelCtrl/WindowCovering on the secondary endpoints
//     were dropped. Added kFzDeviceTemperature + device_temperature expose,
//     and kFzCommand* + an `action` enum + endpoint_action_suffix so each
//     button endpoint surfaces as action_<n>.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::aeotec {
extern const PreparedDefinition kDef_WG001;   // range extender (router)
extern const PreparedDefinition kDef_ZGA002;  // switch + metering + electrical + actions
extern const PreparedDefinition kDef_ZGA003;  // 2x switch + metering + electrical + actions
extern const PreparedDefinition kDef_ZGA004;  // shutter lift + tilt + actions
}  // namespace zhc::devices::aeotec

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

bool def_uses_fz(const PreparedDefinition& def, const FzConverter* conv) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i)
        if (def.from_zigbee[i] == conv) return true;
    return false;
}

// Keep `ctx` alive while the result is read: multi-endpoint suffixing
// allocates in RuntimeContext scratch, so result pointers alias `ctx`.
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

// Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn, cmd,
// then payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> payload = {}) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : payload) v.push_back(b);
    return v;
}

// ZCL attribute report (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool action_is(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

// ── WG001: router — exposeless, no controllable bundle. ─────────────────────
void test_wg001_router_exposeless() {
    using namespace zhc::devices::aeotec;
    const auto& def = kDef_WG001;

    assert(def.exposes_count == 0);           // phantom `state` removed
    assert(!def_exposes(def, "state"));
    assert(def.from_zigbee_count == 0);       // kFzOnOff removed
    assert(def.to_zigbee_count == 0);         // kTzOnOff removed (no write path)
    assert(def.bindings_count == 0);          // phantom 0x0006 bind removed
    assert(!def_binds(def, 1, 0x0006));
}

// ── ZGA002: metering + electrical halves both live + device temp + actions. ─
void test_zga002_electrical_and_actions() {
    using namespace zhc::devices::aeotec;
    const auto& def = kDef_ZGA002;

    assert(def_exposes(def, "state"));          // controllable relay kept (ep1)
    assert(def_exposes(def, "energy"));         // metering half kept
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));        // electrical half added
    assert(def_exposes(def, "current"));        // electrical half added
    assert(def_exposes(def, "device_temperature"));
    assert(def_exposes(def, "action"));         // command endpoints added
    assert(def.to_zigbee_count == 1);           // tz.on_off kept (relay write)
    assert(def_binds(def, 1, 0x0702));
    assert(def_binds(def, 1, 0x0B04));          // electrical-measurement bind added
    assert(def_binds(def, 1, 0x0002));          // device-temperature bind added
    assert(def_binds(def, 2, 0x0006));          // command endpoints bound
    assert(def_binds(def, 3, 0x0006));
    assert(def.endpoint_action_suffix);

    // rmsVoltage (0x0505, u16) on haElectricalMeasurement -> "voltage".
    // ep1 is NOT in the {2,3} endpoint_map, so the key stays bare.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> v = {0xE6, 0x00};   // 230 LE
    auto vr = dispatch_zcl_ep(c1, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x0505, 0x21, std::span<const std::uint8_t>(v.data(), v.size())));
    assert(vr.any_matched);
    assert(vr.merged.find("voltage") != nullptr);

    // rmsCurrent (0x0508, u16) -> "current".
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> i = {0xF4, 0x01};   // 500 LE
    auto ir = dispatch_zcl_ep(c2, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x0508, 0x21, std::span<const std::uint8_t>(i.data(), i.size())));
    assert(ir.any_matched);
    assert(ir.merged.find("current") != nullptr);

    // genDeviceTempCfg currentTemperature (0x0000, s16) -> "device_temperature".
    RuntimeContext c3{};
    const std::array<std::uint8_t, 2> t = {0x19, 0x00};   // 25 LE
    auto tr = dispatch_zcl_ep(c3, def, 0x0002, 1, "genDeviceTempCfg",
                              attr_report(0x0000, 0x29, std::span<const std::uint8_t>(t.data(), t.size())));
    assert(tr.any_matched);
    assert(tr.merged.find("device_temperature") != nullptr);

    // genOnOff On (cmd 0x01) on ep2 -> action_2 (suffixed via endpoint_map).
    RuntimeContext c4{};
    auto a2 = dispatch_zcl_ep(c4, def, 0x0006, 2, "genOnOff", cmd_frame(0x01));
    assert(a2.any_matched);
    assert(action_is(a2, "action_2", "on"));
    assert(a2.merged.find("action") == nullptr);   // suffixed, not bare
}

// ── ZGA003: per-endpoint relays/metering on ep1+ep2 + actions on ep3+ep4. ───
void test_zga003_electrical_and_actions() {
    using namespace zhc::devices::aeotec;
    const auto& def = kDef_ZGA003;

    assert(def_exposes(def, "voltage"));        // electrical half added
    assert(def_exposes(def, "current"));
    assert(def_exposes(def, "device_temperature"));
    assert(def_exposes(def, "action"));
    assert(def.to_zigbee_count == 1);
    assert(def_binds(def, 1, 0x0B04));          // electrical bind on ep1
    assert(def_binds(def, 2, 0x0B04));          // and ep2
    assert(def_binds(def, 3, 0x0006));          // command endpoints
    assert(def_binds(def, 4, 0x0006));
    assert(def.endpoint_action_suffix);

    // rmsCurrent on ep1 -> current_1 (suffixed; ep1 is in the {1,2,3,4} map).
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> i = {0xF4, 0x01};
    auto ir = dispatch_zcl_ep(c1, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x0508, 0x21, std::span<const std::uint8_t>(i.data(), i.size())));
    assert(ir.any_matched);
    assert(ir.merged.find("current_1") != nullptr);
    assert(ir.merged.find("current") == nullptr);   // bare key suffixed away

    // genOnOff Off (cmd 0x00) on ep4 -> action_4.
    RuntimeContext c2{};
    auto a4 = dispatch_zcl_ep(c2, def, 0x0006, 4, "genOnOff", cmd_frame(0x00));
    assert(a4.any_matched);
    assert(action_is(a4, "action_4", "off"));
}

// ── ZGA004: cover lift + tilt both live + actions; no phantom on/off. ───────
void test_zga004_tilt_and_actions() {
    using namespace zhc::devices::aeotec;
    const auto& def = kDef_ZGA004;

    assert(def_exposes(def, "position"));       // lift kept
    assert(def_exposes(def, "tilt"));           // tilt added
    assert(def_exposes(def, "device_temperature"));
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));         // phantom on/off removed
    assert(!def_binds(def, 1, 0x0006));         // phantom genOnOff bind removed
    assert(def_binds(def, 1, 0x0102));          // window-covering kept
    assert(def_uses_fz(def, &::zhc::generic::kFzCoverTilt));
    assert(def.endpoint_action_suffix);

    // currentPositionTiltPercentage (0x0009, u8) on closuresWindowCovering
    // ep1 -> "tilt" (ep1 not in the {3,4,5} map, so bare).
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> t = {0x32};   // 50 %
    auto tr = dispatch_zcl_ep(c1, def, 0x0102, 1, "closuresWindowCovering",
                              attr_report(0x0009, 0x20, std::span<const std::uint8_t>(t.data(), t.size())));
    assert(tr.any_matched);
    assert(tr.merged.find("tilt") != nullptr);

    // currentPositionLiftPercentage (0x0008, u8) -> "position" still works.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> p = {0x14};   // 20 %
    auto pr = dispatch_zcl_ep(c2, def, 0x0102, 1, "closuresWindowCovering",
                              attr_report(0x0008, 0x20, std::span<const std::uint8_t>(p.data(), p.size())));
    assert(pr.any_matched);
    assert(pr.merged.find("position") != nullptr);

    // commandsWindowCovering: upOpen (cmd 0x00) on ep3 -> action_3 = "open".
    RuntimeContext c3{};
    auto o3 = dispatch_zcl_ep(c3, def, 0x0102, 3, "closuresWindowCovering", cmd_frame(0x00));
    assert(o3.any_matched);
    assert(action_is(o3, "action_3", "open"));

    // genOnOff Toggle (cmd 0x02) on ep4 -> action_4 = "toggle".
    RuntimeContext c4{};
    auto a4 = dispatch_zcl_ep(c4, def, 0x0006, 4, "genOnOff", cmd_frame(0x02));
    assert(a4.any_matched);
    assert(action_is(a4, "action_4", "toggle"));
}

}  // namespace

int main() {
    test_wg001_router_exposeless();
    test_zga002_electrical_and_actions();
    test_zga003_electrical_and_actions();
    test_zga004_tilt_and_actions();
    return 0;
}
