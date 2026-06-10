// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Nordtronic (Danish in-wall dimmer/relay) family.
// z2m-source: zigbee-herdsman-converters/src/devices/nordtronic.ts
//
// Two real z2m<->ZHC gaps were fixed:
//
//   * The four metering dimmers/relays — 98424072 (rotary), 98425271
//     (Box Dimmer G2), 98425033 (ceiling micro), 98425034 (DIN rail) —
//     use m.light() + m.electricityMeter(). z2m's electricityMeter()
//     defaults to cluster:"both" (electricalMeasurementType "ac"): it
//     layers haElectricalMeasurement 0x0B04 (power/voltage/current) on
//     top of seMetering 0x0702 (energy), binding both clusters. The
//     auto-port wired only kFzMetering (0x0702) and exposed only
//     energy+power — dropping the entire 0x0B04 half (voltage/current
//     decoder + exposes + bind). Fixed by adding kFzElectricalMeasurement
//     + voltage/current exposes + the {1,0x0B04} bind to each def.
//
//   * 98426061 "Remote Control" is, in z2m, a command client:
//     m.battery + m.commandsOnOff + m.commandsLevelCtrl +
//     m.commandsColorCtrl — it publishes `action` events from the
//     genOnOff / genLevelCtrl / genColorCtrl commands it sends. The
//     auto-port mis-ported it as a controllable on/off switch
//     (kFzOnOff + StateSet `state` + kTzOnOff) — a dead state with no
//     action stream. Fixed by wiring the generic kFzCommand* converters
//     (action) + an `action` String expose; battery kept (correct).
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * haElectricalMeasurement (0x0B04) reports yield power/voltage/current,
//   * seMetering (0x0702) still yields energy + power (regression),
//   * the metering defs declare the full expose set + 0x0702 & 0x0B04 binds,
//   * the remote emits z2m action verbs from cluster commands, declares an
//     `action` expose and NO `state`, and still decodes battery.

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

namespace zhc::devices::nordtronic {
extern const PreparedDefinition kDef_D98424072;  // rotary dimmer + meter
extern const PreparedDefinition kDef_D98425271;  // Box Dimmer G2 + meter
extern const PreparedDefinition kDef_D98425033;  // ceiling micro + meter
extern const PreparedDefinition kDef_D98425034;  // DIN rail + meter
extern const PreparedDefinition kDef_D98426061;  // action remote
extern const PreparedDefinition kDef_D98423051;  // plain relay (clean)
extern const PreparedDefinition kDef_D98425031;  // plain dimmer (clean)
}  // namespace zhc::devices::nordtronic

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;  // dispatch maps id->name; stamp for clarity
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A=ReportAttributes,
// then one record: <attr_id LE><type><value LE>.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// 3-byte cluster-specific command, client->server, no body:
//   fc(1)=0x01 | tsn(1) | cmd(1)
std::array<std::uint8_t, 3> cmd_frame(std::uint8_t cmd, std::uint8_t tsn = 0x42) {
    return {0x01, tsn, cmd};
}

bool action_is(const DispatchResult& r, const char* expected) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

// ── metering: 0x0B04 half restored, 0x0702 regression preserved. ──────
void check_meter(const PreparedDefinition& def) {
    // Static shape: full expose set + both meter clusters bound.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));   // 0x0B04 half (was dropped)
    assert(def_exposes(def, "current"));   // 0x0B04 half (was dropped)
    assert(def_exposes(def, "energy"));
    assert(def_binds(def, 0x0006));        // genOnOff
    assert(def_binds(def, 0x0008));        // genLevelCtrl
    assert(def_binds(def, 0x0702));        // seMetering
    assert(def_binds(def, 0x0B04));        // haElectricalMeasurement (was dropped)

    // haElectricalMeasurement 0x0B04: ActivePower 0x050B (s16) -> power.
    const std::uint8_t p_b[] = {0x4B, 0x00};   // 75 W
    auto rp = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x050B, 0x29, p_b));
    assert(rp.merged.find("power") != nullptr);

    // RMSVoltage 0x0505 (u16) -> voltage.
    const std::uint8_t v_b[] = {0xF5, 0x00};   // 245 V
    auto rv = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x0505, 0x21, v_b));
    const Value* vv = rv.merged.find("voltage");
    assert(vv != nullptr);

    // RMSCurrent 0x0508 (u16) -> current.
    const std::uint8_t c_b[] = {0x2C, 0x01};   // 300
    auto rc = dispatch_zcl(def, 0x0B04, "haElectricalMeasurement", 1,
                           attr_report(0x0508, 0x21, c_b));
    assert(rc.merged.find("current") != nullptr);

    // Regression: seMetering 0x0702 CurrentSummDelivered 0x0000 (u48) -> energy.
    const std::uint8_t e_b[] = {0xD2, 0x04, 0x00, 0x00, 0x00, 0x00};
    auto re = dispatch_zcl(def, 0x0702, "seMetering", 1,
                           attr_report(0x0000, 0x25, e_b));
    assert(re.merged.find("energy") != nullptr);

    // InstantaneousDemand 0x0400 (s24) -> power.
    const std::uint8_t d_b[] = {0x96, 0x00, 0x00};
    auto rd = dispatch_zcl(def, 0x0702, "seMetering", 1,
                           attr_report(0x0400, 0x2A, d_b));
    assert(rd.merged.find("power") != nullptr);
}

// ── remote: action stream restored, dead state removed. ───────────────
void check_remote() {
    using namespace zhc::devices::nordtronic;
    const PreparedDefinition& def = kDef_D98426061;

    // Static shape: action expose present, no dead `state`, battery kept.
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));     // the dead StateSet on/off is gone
    assert(def_exposes(def, "battery"));
    assert(def.to_zigbee_count == 0);       // no controllable tz path
    assert(def_binds(def, 0x0006));         // genOnOff (command source)

    // genOnOff commands -> action on/off/toggle.
    auto off = cmd_frame(0x00);
    auto roff = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                             std::span<const std::uint8_t>(off.data(), off.size()));
    assert(action_is(roff, "off"));

    auto on = cmd_frame(0x01);
    auto ron = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                            std::span<const std::uint8_t>(on.data(), on.size()));
    assert(action_is(ron, "on"));

    auto tog = cmd_frame(0x02);
    auto rtog = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                             std::span<const std::uint8_t>(tog.data(), tog.size()));
    assert(action_is(rtog, "toggle"));

    // genLevelCtrl Step (cmd 0x02): fc=0x01,tsn,cmd=0x02, then body
    // step_mode u8 (1=down), step_size u8, transition_time u16 LE
    // -> action brightness_step_down.
    std::array<std::uint8_t, 7> step = {0x01, 0x42, 0x02, 0x01, 0x1A, 0x00, 0x00};
    auto rstep = dispatch_zcl(def, 0x0008, "genLevelCtrl", 1,
                              std::span<const std::uint8_t>(step.data(), step.size()));
    assert(action_is(rstep, "brightness_step_down"));

    // genPowerCfg batteryPercentageRemaining 0x0021 (u8) -> battery.
    const std::uint8_t bat_b[] = {0xC8};  // 200 -> 100%
    auto rb = dispatch_zcl(def, 0x0001, "genPowerCfg", 1,
                           attr_report(0x0021, 0x20, bat_b));
    assert(rb.merged.find("battery") != nullptr);
}

// ── clean siblings keep their shape (no phantom meter/action). ────────
void check_clean() {
    using namespace zhc::devices::nordtronic;
    // Plain relay: state only, no meter, no action.
    assert(def_exposes(kDef_D98423051, "state"));
    assert(!def_exposes(kDef_D98423051, "energy"));
    assert(!def_exposes(kDef_D98423051, "action"));
    // Plain dimmer: state+brightness, no meter.
    assert(def_exposes(kDef_D98425031, "state"));
    assert(def_exposes(kDef_D98425031, "brightness"));
    assert(!def_exposes(kDef_D98425031, "energy"));
    assert(!def_binds(kDef_D98425031, 0x0702));
    assert(!def_binds(kDef_D98425031, 0x0B04));
}

}  // namespace

int main() {
    using namespace zhc::devices::nordtronic;
    check_meter(kDef_D98424072);
    check_meter(kDef_D98425271);
    check_meter(kDef_D98425033);
    check_meter(kDef_D98425034);
    check_remote();
    check_clean();
    return 0;
}
