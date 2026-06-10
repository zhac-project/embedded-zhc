// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Feibit (Heiman-OEM) family. The auto-generated ports
// carried several classes of bug that this fixture pins after the fix:
//
//   IAS dead-key — six sensors wired the generic kFzIasZone (which emits the
//   raw "alarm"/"tamper"/... bits) against a semantic expose key the
//   converter never produced. z2m decodes each via a typed fz.ias_*_alarm_*
//   that publishes a semantic key:
//     * SBM01ZB  fz.ias_occupancy_alarm_1 (bit0)  -> kFzIasMotionAlarm   -> occupancy
//     * SSA01ZB  fz.ias_smoke_alarm_1     (bit0)  -> kFzIasSmokeAlarm    -> smoke
//     * SCA01ZB  fz.ias_carbon_monoxide_alarm_1   -> kFzIasCoAlarm       -> carbon_monoxide
//     * SGA01ZB  fz.ias_gas_alarm_2       (bit1)  -> kFzIasGasAlarm2     -> gas
//     * SWA01ZB  fz.ias_water_leak_alarm_1(bit0)  -> kFzIasWaterLeakAlarm-> water_leak
//     * SDM01ZB  fz.ias_contact_alarm_1   (bit0)  -> kFzIasContactAlarm  -> contact
//     * SEB01ZB  fz.ias_sos_alarm_2       (bit1)  -> kFzIasSosAlarm2     -> sos   (new generic)
//
//   Missing decoder —
//     * STH01ZB lowered only kFzBattery, dropping temperature + humidity.
//     * TSKT222W-H4 m.electricityMeter() defaults to cluster:"both" but the
//       port wired only kFzMetering (0x0702), dropping the 0x0B04 electrical
//       half (voltage/current). Added kFzElectricalMeasurement.
//
//   Missing tilt — SZT211_AW-P1 + TCUR218W-V1 are lift+tilt covers whose port
//   wired only kFzCoverPosition (attr 0x0008), dropping the tilt channel
//   (attr 0x0009). Added the new generic kFzCoverTilt. TCUR218W-V1 also had
//   the wrong endpoint IDs ({1,2} vs z2m {15,14}).
//
//   Wrong bundle — five action remotes/scene switches mis-modelled as settable
//   on/off switches:
//     * SZSN325W-Q : command_recall only           -> kFzCommandRecall, action
//     * NZRC106W-M2: command_arm + battery         -> kFzIasAceArm/Panic + battery
//     * TZSN408W-V1: commandsOnOff x4 EP           -> kFzCommandOn/Off/Toggle, action_<n>
//     * NZSN421W-Q : battery + commandsOnOff x4 EP -> + battery
//     * FMRC209W   : battery + commandsOnOff+Level -> + Move/Step/Stop
//     * SSS401ZB   : switch + command_recall       -> kept switch, ADDED action

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

namespace zhc::devices::feibit {
extern const PreparedDefinition kDef_SBM01ZB;       // motion  -> occupancy
extern const PreparedDefinition kDef_SSA01ZB;       // smoke
extern const PreparedDefinition kDef_SCA01ZB;       // carbon_monoxide
extern const PreparedDefinition kDef_SGA01ZB;       // gas (bit1)
extern const PreparedDefinition kDef_SWA01ZB;       // water_leak
extern const PreparedDefinition kDef_SDM01ZB;       // contact
extern const PreparedDefinition kDef_SEB01ZB;       // sos (bit1)
extern const PreparedDefinition kDef_STH01ZB;       // temperature + humidity
extern const PreparedDefinition kDef_TSKT222W_H4;   // metering + electrical
extern const PreparedDefinition kDef_SZT211_AW_P1;  // cover lift + tilt
extern const PreparedDefinition kDef_TCUR218W_V1;   // cover lift + tilt, EP15/14
extern const PreparedDefinition kDef_SZSN325W_Q;    // recall action
extern const PreparedDefinition kDef_NZRC106W_M2;   // ace arm/panic action
extern const PreparedDefinition kDef_TZSN408W_V1;   // commandsOnOff x4
extern const PreparedDefinition kDef_NZSN421W_Q;    // commandsOnOff x4 + battery
extern const PreparedDefinition kDef_FMRC209W;      // commandsOnOff + level
extern const PreparedDefinition kDef_SSS401ZB;      // switch + recall
}  // namespace zhc::devices::feibit

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
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// Keep `ctx` alive while the result is read: multi-endpoint suffixing and PIN
// copies allocate in RuntimeContext scratch, so result pointers alias `ctx`.
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

// ZCL attribute report builder (fc=0x18 S->C, tsn, cmd=0x0A, one record).
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

// ── IAS: a 9-byte Zone Status Change Notification (ssIasZone cmd 0x00). ─────
struct IasFrame { std::array<std::uint8_t, 9> bytes; };

IasFrame ias_notif(std::uint16_t zone_status) {
    IasFrame f{};
    f.bytes = { 0x09, 0x42, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

DispatchResult dispatch_ias(RuntimeContext& ctx, const PreparedDefinition& def,
                            const IasFrame& f) {
    return dispatch_zcl_ep(ctx, def, 0x0500, 1, "ssIasZone",
                           std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
}

// Typed IAS sensor: the semantic key is exposed (and bare "alarm" is gone),
// an alarm on `alarm_bit` decodes to it, and a clear report drops it. tamper +
// battery_low (status bits 2/3) always co-decode.
void check_ias(const PreparedDefinition& def, const char* sem, unsigned alarm_bit) {
    assert(def_exposes(def, sem));
    assert(!def_exposes(def, "alarm"));

    RuntimeContext c1{};
    auto on = dispatch_ias(c1, def, ias_notif(static_cast<std::uint16_t>(1u << alarm_bit)));
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);   // legacy bare key must be gone

    RuntimeContext c2{};
    auto off = dispatch_ias(c2, def, ias_notif(0x0000));
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    RuntimeContext c3{};
    auto tb = dispatch_ias(c3, def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

void test_ias_sensors() {
    using namespace zhc::devices::feibit;
    check_ias(kDef_SBM01ZB, "occupancy",       0);
    check_ias(kDef_SSA01ZB, "smoke",           0);
    check_ias(kDef_SCA01ZB, "carbon_monoxide", 0);
    check_ias(kDef_SGA01ZB, "gas",             1);   // alarm_2 -> bit 1
    check_ias(kDef_SWA01ZB, "water_leak",      0);
    check_ias(kDef_SDM01ZB, "contact",         0);
    check_ias(kDef_SEB01ZB, "sos",             1);   // sos_alarm_2 -> bit 1

    // SGA01ZB on bit 0 must NOT fire (z2m reads alarm_2 only).
    RuntimeContext cg{};
    auto bit0 = dispatch_ias(cg, kDef_SGA01ZB, ias_notif(0x0001));
    assert(b_false(bit0.merged.find("gas")));
}

// ── STH01ZB: temperature (0x0402) + humidity (0x0405) regained. ────────────
void test_sth01zb() {
    using namespace zhc::devices::feibit;
    const auto& def = kDef_STH01ZB;
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "battery"));
    assert(def_binds(def, 1, 0x0402));
    assert(def_binds(def, 1, 0x0405));

    // 21.50 °C -> 2150 (s16) on msTemperatureMeasurement measuredValue 0x0000.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> t = {0x66, 0x08};   // 2150 LE
    auto tr = dispatch_zcl_ep(c1, def, 0x0402, 1, "msTemperatureMeasurement",
                              attr_report(0x0000, 0x29, std::span<const std::uint8_t>(t.data(), t.size())));
    assert(tr.any_matched);
    // fz_temperature scales raw/100 -> Float 21.5.
    const Value* tv = tr.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);

    // 55.00 % -> 5500 (u16) on msRelativeHumidity measuredValue 0x0000.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> h = {0x7C, 0x15};   // 5500 LE
    auto hr = dispatch_zcl_ep(c2, def, 0x0405, 1, "msRelativeHumidity",
                              attr_report(0x0000, 0x21, std::span<const std::uint8_t>(h.data(), h.size())));
    assert(hr.any_matched);
    assert(hr.merged.find("humidity") != nullptr);
}

// ── TSKT222W-H4: electrical-measurement half regained (voltage/current). ───
void test_tskt222w() {
    using namespace zhc::devices::feibit;
    const auto& def = kDef_TSKT222W_H4;
    assert(def_exposes(def, "energy"));    // metering half (kept)
    assert(def_exposes(def, "voltage"));   // electrical half (added)
    assert(def_exposes(def, "current"));
    assert(def_binds(def, 1, 0x0702));
    assert(def_binds(def, 1, 0x0B04));

    // rmsVoltage (0x0505, u16) on haElectricalMeasurement -> "voltage".
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
}

// ── Covers: tilt (attr 0x0009) decodes to "tilt"; lift still works. ────────
void check_cover_tilt(const PreparedDefinition& def, std::uint8_t lift_ep) {
    assert(def_exposes(def, "position"));
    assert(def_exposes(def, "tilt"));

    // currentPositionLiftPercentage (0x0008, u8) -> position.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> p = {40};
    auto pr = dispatch_zcl_ep(c1, def, 0x0102, lift_ep, "closuresWindowCovering",
                              attr_report(0x0008, 0x20, std::span<const std::uint8_t>(p.data(), p.size())));
    assert(pr.any_matched);
    // currentPositionTiltPercentage (0x0009, u8) -> tilt.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> t = {70};
    auto tr = dispatch_zcl_ep(c2, def, 0x0102, lift_ep, "closuresWindowCovering",
                              attr_report(0x0009, 0x20, std::span<const std::uint8_t>(t.data(), t.size())));
    assert(tr.any_matched);
    // single-endpoint cover (SZT211): bare "tilt"; multi-EP (TCUR218) suffixes.
    assert(tr.merged.find("tilt") != nullptr || tr.merged.find("tilt_top") != nullptr ||
           tr.merged.find("tilt_bottom") != nullptr);
}

void test_covers() {
    using namespace zhc::devices::feibit;
    check_cover_tilt(kDef_SZT211_AW_P1, 1);

    // TCUR218W-V1: endpoint IDs restored to z2m {top:15, bottom:14}.
    const auto& dual = kDef_TCUR218W_V1;
    assert(dual.endpoint_map && dual.endpoint_map_count == 2);
    bool has15 = false, has14 = false;
    for (std::size_t i = 0; i < dual.endpoint_map_count; ++i) {
        if (dual.endpoint_map[i].endpoint == 15) has15 = true;
        if (dual.endpoint_map[i].endpoint == 14) has14 = true;
    }
    assert(has15 && has14);
    assert(def_binds(dual, 15, 0x0102));
    assert(def_binds(dual, 14, 0x0102));
    check_cover_tilt(dual, 15);
}

// ── Wrong-bundle remotes: action, no settable state, no write path. ────────
void check_remote_shape(const PreparedDefinition& def) {
    assert(def_exposes(def, "action"));
    assert(!def_exposes(def, "state"));
    assert(def.to_zigbee_count == 0);
    assert(def.from_zigbee_count > 0);
}

void test_scene_recall() {
    using namespace zhc::devices::feibit;
    const auto& def = kDef_SZSN325W_Q;
    check_remote_shape(def);
    // genScenes Recall (cmd 0x05): group(2) + scene(1)=3 -> action recall_3.
    RuntimeContext c{};
    const std::array<std::uint8_t, 3> pl = {0x00, 0x00, 0x03};
    auto r = dispatch_zcl_ep(c, def, 0x0005, 1, "genScenes",
                             cmd_frame(0x05, std::span<const std::uint8_t>(pl.data(), pl.size())));
    assert(r.any_matched);
    assert(action_is(r, "action", "recall_3"));
    assert(def_binds(def, 1, 0x0005));
}

void test_security_remote() {
    using namespace zhc::devices::feibit;
    const auto& def = kDef_NZRC106W_M2;
    check_remote_shape(def);
    assert(def_exposes(def, "battery"));

    // ssIasAce Arm (cmd 0x00): armMode=3 (arm_all_zones), codeLen=0, zoneId=0.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 3> arm = {0x03, 0x00, 0x00};
    auto ar = dispatch_zcl_ep(c1, def, 0x0501, 1, "ssIasAce",
                              cmd_frame(0x00, std::span<const std::uint8_t>(arm.data(), arm.size())));
    assert(ar.any_matched);
    assert(action_is(ar, "action", "arm_all_zones"));

    // ssIasAce Panic (cmd 0x04): no payload -> action "panic".
    RuntimeContext c2{};
    auto pr = dispatch_zcl_ep(c2, def, 0x0501, 1, "ssIasAce", cmd_frame(0x04));
    assert(pr.any_matched);
    assert(action_is(pr, "action", "panic"));
}

void test_multibutton_onoff() {
    using namespace zhc::devices::feibit;
    for (const PreparedDefinition* defp : {&kDef_TZSN408W_V1, &kDef_NZSN421W_Q}) {
        const auto& def = *defp;
        check_remote_shape(def);
        assert(def.endpoint_action_suffix);
        assert(def.endpoint_map && def.endpoint_map_count == 4);

        // genOnOff On (cmd 0x01) on EP3 -> action_3 = "on".
        RuntimeContext c1{};
        auto on3 = dispatch_zcl_ep(c1, def, 0x0006, 3, "genOnOff", cmd_frame(0x01));
        assert(on3.any_matched);
        assert(action_is(on3, "action_3", "on"));
        assert(on3.merged.find("action") == nullptr);   // suffixed, not bare

        // genOnOff Off on EP1 -> action_1 = "off".
        RuntimeContext c2{};
        auto off1 = dispatch_zcl_ep(c2, def, 0x0006, 1, "genOnOff", cmd_frame(0x00));
        assert(action_is(off1, "action_1", "off"));

        assert(def_binds(def, 1, 0x0006));
        assert(def_binds(def, 4, 0x0006));
    }
    // NZSN421W-Q keeps battery; TZSN408W-V1 does not.
    assert(def_exposes(kDef_NZSN421W_Q, "battery"));
    assert(def_binds(kDef_NZSN421W_Q, 1, 0x0001));
}

void test_dimmer_remote() {
    using namespace zhc::devices::feibit;
    const auto& def = kDef_FMRC209W;
    check_remote_shape(def);
    assert(!def.endpoint_action_suffix);   // single endpoint -> no suffix
    assert(def_exposes(def, "battery"));

    // genOnOff Toggle (cmd 0x02) -> bare action "toggle".
    RuntimeContext c1{};
    auto tg = dispatch_zcl_ep(c1, def, 0x0006, 1, "genOnOff", cmd_frame(0x02));
    assert(tg.any_matched);
    assert(action_is(tg, "action", "toggle"));
    assert(tg.merged.find("action_1") == nullptr);

    // genLevelCtrl Stop (cmd 0x03) -> action "brightness_stop".
    RuntimeContext c2{};
    auto st = dispatch_zcl_ep(c2, def, 0x0008, 1, "genLevelCtrl", cmd_frame(0x03));
    assert(st.any_matched);
    assert(action_is(st, "action", "brightness_stop"));

    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0008));
}

// ── SSS401ZB: switch half preserved AND scene recall action added. ─────────
void test_switch_plus_recall() {
    using namespace zhc::devices::feibit;
    const auto& def = kDef_SSS401ZB;
    assert(def_exposes(def, "state"));     // controllable switch kept
    assert(def_exposes(def, "action"));    // recall action added
    assert(def.to_zigbee_count == 1);      // tz.on_off kept

    // genScenes Recall scene 2 -> action recall_2.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 3> pl = {0x00, 0x00, 0x02};
    auto r = dispatch_zcl_ep(c1, def, 0x0005, 1, "genScenes",
                             cmd_frame(0x05, std::span<const std::uint8_t>(pl.data(), pl.size())));
    assert(r.any_matched);
    assert(action_is(r, "action", "recall_2"));

    // genOnOff On report still decodes the relay state.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto sr = dispatch_zcl_ep(c2, def, 0x0006, 1, "genOnOff",
                              attr_report(0x0000, 0x10, std::span<const std::uint8_t>(on.data(), on.size())));
    assert(sr.any_matched);
    assert(b_true(sr.merged.find("state")));
    assert(def_binds(def, 1, 0x0005));
}

}  // namespace

int main() {
    test_ias_sensors();
    test_sth01zb();
    test_tskt222w();
    test_covers();
    test_scene_recall();
    test_security_remote();
    test_multibutton_onoff();
    test_dimmer_remote();
    test_switch_plus_recall();
    return 0;
}
