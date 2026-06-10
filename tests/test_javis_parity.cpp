// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Javis family (z2m<->embedded-zhc parity pass over the 2
// existing javis defs). Two real bugs were fixed:
//
//   1. JS-MC-SENSOR-ZB (TS0601 microwave/presence sensor): the auto-port
//      mis-wired this Tuya 0xEF00 DP-stream sensor to the IAS-zone converter
//      (kFzIasZone) with phantom alarm/tamper/battery_low exposes and a 0x0500
//      bind. z2m decodes it via `legacy.fz.javis_microwave_sensor` over the DP
//      stream. Ported the real DP->key map (presence/occupancy/illuminance/
//      sensitivity/keep_time/led_enable/illuminance_calibration). The
//      `_TZE200_kagkgk0i` variant remaps DP102/106/107 and was split into its
//      own def so each manufacturer gets z2m's exact wiring.
//
//   2. JS-SLK2-ZB (biometric lock): the auto-port wired it as a controllable
//      on/off switch (kFzOnOff + kTzOnOff + StateSet `state` + genOnOff bind).
//      z2m's def has an EMPTY toZigbee and exposes `action(["unlock"])` +
//      battery. Dropped the phantom on/off control + dead `state` expose.
//
// z2m-source: zigbee-herdsman-converters/src/devices/javis.ts (+ lib/legacy.ts
// javis_microwave_sensor, converters/fromZigbee.ts javis_lock_report).

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/javis_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> javis_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::javis::kJavisRegistry,
        devices::javis::kJavisRegistryCount);
}

DecodedMessage make_msg() {
    DecodedMessage msg{};
    msg.family       = FrameFamily::TuyaDp;
    msg.type         = MessageType::Command;
    msg.cluster      = "manuSpecificTuya";
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    return msg;
}

InboundApsFrame make_raw() {
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    return raw;
}

DispatchResult dispatch_dp(const PreparedDefinition& def,
                           std::span<const TuyaDpRecord> recs) {
    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, recs, def, raw, ctx);
}

const PreparedDefinition& resolve(const char* manu) {
    const auto* d = find_definition("TS0601", manu, javis_registry());
    assert(d && "javis manufacturer name must resolve to a def");
    return *d;
}

bool has_expose(const PreparedDefinition& d, const char* name) {
    for (std::uint8_t i = 0; i < d.exposes_count; ++i)
        if (d.exposes[i].name && std::strcmp(d.exposes[i].name, name) == 0)
            return true;
    return false;
}

// Single Enum DP (dp_type 0x04, 1 wire byte). `store` points at the wire byte.
TuyaDpRecord enum_rec(std::uint8_t dp, const std::uint8_t* store) {
    return { dp, 0x04, std::span<const std::uint8_t>(store, 1) };
}

}  // namespace

// --- Bug 1a: DP1 presence enum -> presence_state string + occupancy bool ----
static void test_dp1_presence_and_occupancy() {
    const auto& d = resolve("_TZE200_lgstepha");

    // value 1 = big_motion -> occupied
    const std::uint8_t w1 = 1;
    const TuyaDpRecord r1[] = { enum_rec(1, &w1) };
    auto res1 = dispatch_dp(d, std::span<const TuyaDpRecord>(r1, 1));
    assert(res1.any_matched);
    const Value* ps = res1.merged.find("presence_state");
    assert(ps && ps->type == ValueType::StringRef &&
           std::strcmp(ps->str, "big_motion") == 0);
    const Value* occ = res1.merged.find("occupancy");
    assert(occ && occ->type == ValueType::Bool && occ->b == true);

    // value 0 = no_motion -> NOT occupied
    const std::uint8_t w0 = 0;
    const TuyaDpRecord r0[] = { enum_rec(1, &w0) };
    auto res0 = dispatch_dp(d, std::span<const TuyaDpRecord>(r0, 1));
    assert(res0.any_matched);
    const Value* ps0 = res0.merged.find("presence_state");
    assert(ps0 && std::strcmp(ps0->str, "no_motion") == 0);
    const Value* occ0 = res0.merged.find("occupancy");
    assert(occ0 && occ0->type == ValueType::Bool && occ0->b == false);

    // value 5 = initializing -> NOT occupied (boundary: 0 < v < 5)
    const std::uint8_t w5 = 5;
    const TuyaDpRecord r5[] = { enum_rec(1, &w5) };
    auto res5 = dispatch_dp(d, std::span<const TuyaDpRecord>(r5, 1));
    const Value* occ5 = res5.merged.find("occupancy");
    assert(occ5 && occ5->b == false);
}

// --- Bug 1b: DP101 illuminance, DP103 led_enable, DP102 keep_time, DP2 sens --
static void test_lgstepha_channels() {
    const auto& d = resolve("_TZE200_lgstepha");

    // DP101 illuminance (Numeric 0x02, s32 BE = 250)
    const std::uint8_t lux[] = { 0x00, 0x00, 0x00, 0xFA };
    const TuyaDpRecord rlux[] = { {101, 0x02, std::span<const std::uint8_t>(lux, 4)} };
    auto rl = dispatch_dp(d, std::span<const TuyaDpRecord>(rlux, 1));
    const Value* il = rl.merged.find("illuminance");
    assert(il && il->type == ValueType::Int && il->i == 250);

    // DP103 led_enable (Bool 0x01)
    const std::uint8_t on[] = { 0x01 };
    const TuyaDpRecord rled[] = { {103, 0x01, std::span<const std::uint8_t>(on, 1)} };
    auto rd = dispatch_dp(d, std::span<const TuyaDpRecord>(rled, 1));
    const Value* led = rd.merged.find("led_enable");
    assert(led && led->type == ValueType::Bool && led->b == true);

    // DP102 -> keep_time on this variant (NOT illuminance_calibration)
    const std::uint8_t kt[] = { 0x00, 0x00, 0x00, 0x03 };
    const TuyaDpRecord rkt[] = { {102, 0x02, std::span<const std::uint8_t>(kt, 4)} };
    auto rk = dispatch_dp(d, std::span<const TuyaDpRecord>(rkt, 1));
    const Value* keep = rk.merged.find("keep_time");
    assert(keep && keep->type == ValueType::Int && keep->i == 3);
    assert(rk.merged.find("illuminance_calibration") == nullptr);

    // DP2 sensitivity (Numeric)
    const std::uint8_t sv[] = { 0x00, 0x00, 0x00, 0x4B };  // 75
    const TuyaDpRecord rsv[] = { {2, 0x02, std::span<const std::uint8_t>(sv, 4)} };
    auto rs = dispatch_dp(d, std::span<const TuyaDpRecord>(rsv, 1));
    const Value* sens = rs.merged.find("sensitivity");
    assert(sens && sens->type == ValueType::Int && sens->i == 75);
}

// --- Bug 1c: _TZE200_kagkgk0i remaps DP102 -> illuminance_calibration -------
static void test_kagkgk0i_dp_remap() {
    const auto& d = resolve("_TZE200_kagkgk0i");
    // Must be the dedicated variant def (not the lgstepha one).
    assert(d.model && std::strcmp(d.model, "JS-MC-SENSOR-ZB") == 0);

    // DP102 -> illuminance_calibration on this variant (NOT keep_time)
    const std::uint8_t cal[] = { 0x00, 0x00, 0x00, 0x0A };  // 10
    const TuyaDpRecord rcal[] = { {102, 0x02, std::span<const std::uint8_t>(cal, 4)} };
    auto rc = dispatch_dp(d, std::span<const TuyaDpRecord>(rcal, 1));
    const Value* ic = rc.merged.find("illuminance_calibration");
    assert(ic && ic->type == ValueType::Int && ic->i == 10);
    assert(rc.merged.find("keep_time") == nullptr);

    // DP106 -> keep_time on this variant
    const std::uint8_t kt[] = { 0x00, 0x00, 0x00, 0x05 };
    const TuyaDpRecord rkt[] = { {106, 0x02, std::span<const std::uint8_t>(kt, 4)} };
    auto rk = dispatch_dp(d, std::span<const TuyaDpRecord>(rkt, 1));
    const Value* keep = rk.merged.find("keep_time");
    assert(keep && keep->type == ValueType::Int && keep->i == 5);

    // DP107 -> led_enable on this variant
    const std::uint8_t on[] = { 0x01 };
    const TuyaDpRecord rled[] = { {107, 0x01, std::span<const std::uint8_t>(on, 1)} };
    auto rd = dispatch_dp(d, std::span<const TuyaDpRecord>(rled, 1));
    const Value* led = rd.merged.find("led_enable");
    assert(led && led->type == ValueType::Bool && led->b == true);
}

// --- Bug 1d: sensor is NOT IAS — no phantom alarm/tamper exposes -------------
static void test_sensor_not_ias() {
    const auto& d = resolve("_TZE200_lgstepha");
    assert(!has_expose(d, "alarm"));
    assert(!has_expose(d, "tamper"));
    assert(!has_expose(d, "battery_low"));
    assert(has_expose(d, "occupancy"));
    assert(has_expose(d, "illuminance"));
    // A real motion-state alarm DP would never decode through IAS; assert the
    // device exposes the real sensor channels instead.
    assert(has_expose(d, "presence_state"));
}

// --- Bug 2: JS-SLK2-ZB lock is NOT a controllable on/off switch -------------
static void test_lock_not_onoff_switch() {
    const auto* d = find_definition_by_model("JAVISLOCK", javis_registry());
    assert(d && "JAVISLOCK must resolve");
    assert(d->model && std::strcmp(d->model, "JS-SLK2-ZB") == 0);

    // z2m toZigbee is empty — no controllable write path.
    assert(d->to_zigbee_count == 0);
    // No dead `state` on/off expose; role is action + battery.
    assert(!has_expose(*d, "state"));
    assert(has_expose(*d, "action"));
    assert(has_expose(*d, "battery"));
}

// Battery still decodes through the generic converter (genPowerCfg). Assert the
// battery fz converter is wired (its numeric decode is covered by the generic
// battery test); the old port additionally carried a genOnOff fz/tz which must
// now be gone.
static void test_lock_keeps_battery_drops_onoff() {
    const auto* d = find_definition_by_model("JAVISLOCK", javis_registry());
    assert(d);

    bool battery_fz = false;
    bool onoff_fz   = false;
    for (std::uint8_t i = 0; i < d->from_zigbee_count; ++i) {
        const auto* fz = d->from_zigbee[i];
        if (!fz || !fz->cluster) continue;
        if (std::strcmp(fz->cluster, "genPowerCfg") == 0) battery_fz = true;
        if (std::strcmp(fz->cluster, "genOnOff") == 0)    onoff_fz   = true;
    }
    assert(battery_fz && "lock must keep the genPowerCfg battery converter");
    assert(!onoff_fz && "lock must NOT carry a genOnOff on/off converter");
}

int main() {
    test_dp1_presence_and_occupancy();
    test_lgstepha_channels();
    test_kagkgk0i_dp_remap();
    test_sensor_not_ias();
    test_lock_not_onoff_switch();
    test_lock_keeps_battery_drops_onoff();
    return 0;
}
