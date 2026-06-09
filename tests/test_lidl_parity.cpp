// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Lidl Silvercrest / Livarno / Parkside family
// (Tuya-OEM). z2m-source: lidl.ts.
//
// Bugs fixed by the lidl parity pass (all on real wire shapes):
//
//  1. IAS sensors lowered the generic kFzIasZone (which emits the bare
//     key "alarm") while their zoneType implies a semantic key:
//       - HG06335/HG07310 motion → z2m m.iasZoneAlarm({zoneType:"occupancy"})
//       - HG06336 contact        → z2m m.iasZoneAlarm({zoneType:"contact"})
//     Swapped to the typed kFzIasMotionAlarm / kFzIasContactAlarm
//     converters, which emit "occupancy" / "contact" directly.
//
//  2. Command remotes emitted a controllable on/off `state` (kFzOnOff +
//     kTzOnOff) instead of an `action` stream:
//       - FB20-002 / FB21-001 (Livarno dimmer remotes)
//       - HG08164 (Silvercrest smart button)
//     z2m wires fz.command_on/off/step/move/stop → action. Swapped to the
//     generic kFzCommand* converters (each emits action="…").
//
//  3. HG08673 power plug dropped its entire metering surface — only
//     kFzOnOff was wired. z2m builds it with electricalMeasurements +
//     metering polling. Added kFzElectricalMeasurement (power/voltage/
//     current) + kFzMetering (energy/power) + the matching exposes.
//
//  4. PSBZS-A1 carried a duplicate battery+onoff stub (kDef_PSBZS_A1) for
//     the SAME TS0601 / _TZE200_htnnfasr fingerprint as the full Tuya-DP
//     def (kDefLid__TZE200_htnnfasr). The stub was removed; this test pins
//     that the registry now holds exactly one def for that fingerprint and
//     that it carries the full DP-driven expose set (timer / time_left /
//     frost_lock).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "zhc/devices/lidl_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lidl {
extern const PreparedDefinition kDef_HG06335_HG07310;   // motion  -> occupancy
extern const PreparedDefinition kDef_HG06336;           // contact
extern const PreparedDefinition kDef_FB20_002;          // dimmer remote (action)
extern const PreparedDefinition kDef_FB21_001;          // dimmer remote (action)
extern const PreparedDefinition kDef_HG08164;           // smart button (action+battery)
extern const PreparedDefinition kDef_HG08673;           // metering plug
extern const PreparedDefinition kDefLid__TZE200_htnnfasr;  // PSBZS A1 full DP def
}  // namespace zhc::devices::lidl

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool action_is(const DispatchResult& r, const char* expected) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

// ── IAS Zone Status Change Notification (cluster 0x0500) ─────────────
// 9 bytes: fc(1)=0x09 tsn(1) cmd(1)=0x00 zoneStatus_le(2) extStatus(1)
//          zoneId(1) delay_le(2). bit 0 = alarm_1, bit 2 = tamper,
//          bit 3 = battery_low.
struct IasFrame { std::array<std::uint8_t, 9> bytes; };

IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = {0x09, tsn, 0x00,
               static_cast<std::uint8_t>(zone_status & 0xFF),
               static_cast<std::uint8_t>(zone_status >> 8),
               0x00, 0x00, 0x00, 0x00};
    return f;
}

DispatchResult dispatch_ias(const PreparedDefinition& def, const IasFrame& f) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(),
                                                        f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Assert: a typed IAS converter surfaces the semantic key from alarm_1
// (bit 0) and NEVER the bare "alarm" key the generic kFzIasZone emits.
void check_ias_alarm1(const PreparedDefinition& def, const char* sem) {
    assert(def_exposes(def, sem));
    assert(!def_exposes(def, "alarm"));   // bare key must be gone from exposes

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);       // bare key never emitted

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper + battery_low
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// ── genOnOff cluster-specific command (cluster 0x0006) ───────────────
// 3 bytes: fc(1)=0x01 (cluster-specific, C->S, not manu) | tsn | cmd.
DispatchResult on_off_cmd(const PreparedDefinition& def, std::uint8_t cmd) {
    const std::array<std::uint8_t, 3> b = {0x01, 0x42, cmd};
    auto raw = make_frame(0x0006, 1,
                          std::span<const std::uint8_t>(b.data(), b.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// genLevelCtrl Step (cmd 0x02): [mode, size, tt_lo, tt_hi]. mode 0 = up.
DispatchResult level_step(const PreparedDefinition& def, std::uint8_t mode) {
    const std::array<std::uint8_t, 7> b = {0x01, 0x42, 0x02, mode, 0x40, 0x00, 0x00};
    auto raw = make_frame(0x0008, 1,
                          std::span<const std::uint8_t>(b.data(), b.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// genLevelCtrl Move (cmd 0x01): [mode, rate]. mode 0 = up.
DispatchResult level_move(const PreparedDefinition& def, std::uint8_t mode) {
    const std::array<std::uint8_t, 5> b = {0x01, 0x42, 0x01, mode, 0x32};
    auto raw = make_frame(0x0008, 1,
                          std::span<const std::uint8_t>(b.data(), b.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// genLevelCtrl Stop (cmd 0x03): no body.
DispatchResult level_stop(const PreparedDefinition& def) {
    const std::array<std::uint8_t, 3> b = {0x01, 0x42, 0x03};
    auto raw = make_frame(0x0008, 1,
                          std::span<const std::uint8_t>(b.data(), b.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// A command remote: exposes "action" (never "state"), and the standard
// command verbs decode to the z2m action literals.
void check_command_remote(const PreparedDefinition& def) {
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));   // not a controllable on/off endpoint

    auto on = on_off_cmd(def, 0x01);
    assert(on.any_matched && action_is(on, "on"));

    auto off = on_off_cmd(def, 0x00);
    assert(off.any_matched && action_is(off, "off"));

    auto step_up = level_step(def, 0x00);
    assert(step_up.any_matched && action_is(step_up, "brightness_step_up"));

    auto step_dn = level_step(def, 0x01);
    assert(step_dn.any_matched && action_is(step_dn, "brightness_step_down"));

    auto stop = level_stop(def);
    assert(stop.any_matched && action_is(stop, "brightness_stop"));
}

// ── Energy / power attribute reports ─────────────────────────────────
// fc=0x18 (profile-wide, S->C) | tsn | cmd=0x0A (Report Attributes) | body.
DispatchResult attr_report(const PreparedDefinition& def, std::uint16_t cluster,
                           std::span<const std::uint8_t> body) {
    std::array<std::uint8_t, 32> buf{};
    buf[0] = 0x18; buf[1] = 0x42; buf[2] = 0x0A;
    std::size_t n = 3;
    for (auto x : body) buf[n++] = x;
    auto raw = make_frame(cluster, 1,
                          std::span<const std::uint8_t>(buf.data(), n));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

void check_metering_plug(const PreparedDefinition& def) {
    // The full power-monitoring surface must be declared.
    assert(def_exposes(def, "state"));    // controllable plug — state is real
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    assert(def_exposes(def, "energy"));

    // haElectricalMeasurement (0x0B04): RMSVoltage attr 0x0505 (u16) = 0x0E10
    // (3600). type 0x21 = uint16.
    const std::array<std::uint8_t, 5> volt = {0x05, 0x05, 0x21, 0x10, 0x0E};
    auto rv = attr_report(def, 0x0B04, std::span<const std::uint8_t>(volt.data(),
                                                                     volt.size()));
    assert(rv.any_matched);
    const Value* v = rv.merged.find("voltage");
    assert(v && v->type == ValueType::Uint && v->u == 3600);

    // seMetering (0x0702): CurrentSummDelivered attr 0x0000 (u48) = 1234.
    // type 0x25 = uint48 (6 bytes LE).
    const std::array<std::uint8_t, 9> eng = {0x00, 0x00, 0x25,
                                             0xD2, 0x04, 0x00, 0x00, 0x00, 0x00};
    auto re = attr_report(def, 0x0702, std::span<const std::uint8_t>(eng.data(),
                                                                     eng.size()));
    assert(re.any_matched);
    const Value* e = re.merged.find("energy");
    assert(e && e->type == ValueType::Uint && e->u == 1234);
}

// PSBZS-A1 duplicate-fingerprint guard: the registry must hold exactly one
// def matching TS0601 / _TZE200_htnnfasr, and it must be the full Tuya-DP
// def (carrying timer / time_left / frost_lock), not the battery+onoff stub.
void check_psbzs_single_def() {
    int matches = 0;
    for (std::size_t i = 0; i < devices::lidl::kLidlRegistryCount; ++i) {
        const PreparedDefinition* d = devices::lidl::kLidlRegistry[i];
        bool model_ok = false;
        for (std::size_t m = 0; m < d->zigbee_models_count; ++m)
            if (d->zigbee_models[m] && std::strcmp(d->zigbee_models[m], "TS0601") == 0)
                model_ok = true;
        bool manu_ok = false;
        for (std::size_t m = 0; m < d->manufacturer_names_count; ++m)
            if (d->manufacturer_names[m] &&
                std::strcmp(d->manufacturer_names[m], "_TZE200_htnnfasr") == 0)
                manu_ok = true;
        if (model_ok && manu_ok) ++matches;
    }
    assert(matches == 1);   // the duplicate stub is gone

    // The surviving def is the full DP def with the real watering-timer keys.
    const auto& def = devices::lidl::kDefLid__TZE200_htnnfasr;
    assert(def_exposes(def, "timer"));
    assert(def_exposes(def, "time_left"));
    assert(def_exposes(def, "frost_lock"));
}

}  // namespace

int main() {
    using namespace zhc::devices::lidl;

    // 1. IAS sensors decode their semantic key (not the bare "alarm").
    check_ias_alarm1(kDef_HG06335_HG07310, "occupancy");
    check_ias_alarm1(kDef_HG06336, "contact");

    // 2. Command remotes emit `action`, not `state`.
    check_command_remote(kDef_FB20_002);
    check_command_remote(kDef_FB21_001);
    check_command_remote(kDef_HG08164);

    // HG08164 also keeps battery alongside the action stream.
    {
        const std::array<std::uint8_t, 7> rep = {0x18, 0x01, 0x0A,
                                                 0x21, 0x00, 0x20, 0xC8};
        auto raw = make_frame(0x0001 /* genPowerCfg */, 1,
                              std::span<const std::uint8_t>(rep.data(), rep.size()));
        DecodedMessage msg{};
        assert(decode_frame(raw, {}, msg));
        RuntimeContext ctx{};
        auto r = dispatch_from_zigbee(msg, {}, kDef_HG08164, raw, ctx);
        assert(r.any_matched);
        const Value* b = r.merged.find("battery");
        assert(b && b->type == ValueType::Uint && b->u == 100);
        assert(def_exposes(kDef_HG08164, "battery"));
    }
    // A move command also decodes (the dimmer remotes bind genLevelCtrl).
    {
        auto mv = level_move(kDef_FB20_002, 0x00);
        assert(mv.any_matched && action_is(mv, "brightness_move_up"));
    }

    // 3. HG08673 surfaces the full power-monitoring channel set.
    check_metering_plug(kDef_HG08673);

    // 4. PSBZS-A1 has exactly one (full DP) def — the dup stub is gone.
    check_psbzs_single_def();

    std::printf("lidl parity tests passed\n");
    return 0;
}
