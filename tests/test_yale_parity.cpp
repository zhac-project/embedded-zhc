// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Yale smart-door-lock family.
// z2m-source: zigbee-herdsman-converters/src/devices/yale.ts.
//
// Bug fixed (each device graduated generated/ -> Tier-2 parent):
//
//  * Lost battery decoder. Every Yale lock in z2m decodes battery — via
//    the shared lockExtend() helper (which bundles fz.battery + e.battery()
//    + e.battery_low()) or via m.battery(). 23 of the 28 generated defs
//    wired only kFzLock (lock_state) and dropped battery entirely: the
//    genPowerCfg decoder, the "battery" expose, and the 0x0001 bind were
//    all missing, so a battery-powered lock never surfaced its charge
//    level. (The other 5 — SOLIS01, YMC420-W, YRL256 TS, YMI70A, YRM476 —
//    had already been hand-corrected.) All 23 graduated to Tier-2 parents
//    wiring the generic kFzBattery + a "battery" expose + the genPowerCfg
//    binding.
//
// The lock-state decode (kFzLock -> "lock_state") and the lock/unlock
// command encoder (kTzLock, key "state" -> closuresDoorLock cmd 0x00/0x01)
// were already correct on every def; these tests pin them as a regression
// guard alongside the restored battery channel. PIN/user-code management,
// auto-relock, sound-volume and the programming/operation-event "action"
// stream remain out of scope (bespoke converters; deferred).

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

namespace zhc::devices::yale {
// Battery-restored Tier-2 graduates (representative spread of the 23):
extern const PreparedDefinition kDef_YRD226HA2619;   // YRD series
extern const PreparedDefinition kDef_YDD_D4F0_TSDB;  // YDD series
extern const PreparedDefinition kDef_YDF40;          // YDF series
extern const PreparedDefinition kDef_YRL226_TS;      // YRL series
extern const PreparedDefinition kDef_YRD410_BLE;     // BLE variant
extern const PreparedDefinition kDef_LIA;            // standalone Lia
// Already-correct reference (battery present pre-fix) — regression anchor:
extern const PreparedDefinition kDef_SOLIS01;
}  // namespace zhc::devices::yale

namespace {

using namespace zhc;

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool u_eq(const Value* v, std::uint64_t expect) {
    return v && v->type == ValueType::Uint && v->u == expect;
}

// ── ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then records ─
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// closuresDoorLock (0x0101) attr 0x0000 LockState (enum8, type 0x30):
//   0 = not fully locked, 1 = locked, 2 = unlocked.
// kFzLock emits "lock_state" as the raw Uint.
void check_lock_state(const PreparedDefinition& def) {
    assert(def_exposes(def, "lock_state"));
    assert(def_binds(def, 0x0101));

    const std::uint8_t locked[] = {0x01};
    auto r1 = dispatch_zcl(def, 0x0101, "closuresDoorLock", 1,
                           attr_report(0x0000, 0x30, locked));
    assert(r1.any_matched);
    assert(u_eq(r1.merged.find("lock_state"), 1));

    const std::uint8_t unlocked[] = {0x02};
    auto r2 = dispatch_zcl(def, 0x0101, "closuresDoorLock", 1,
                           attr_report(0x0000, 0x30, unlocked));
    assert(r2.any_matched);
    assert(u_eq(r2.merged.find("lock_state"), 2));
}

// genPowerCfg (0x0001) attr 0x0021 batteryPercentageRemaining (u8, type
// 0x20): half-percent units. kFzBattery emits "battery" = raw/2. This is
// the channel that was dead on the 23 graduated defs.
void check_battery(const PreparedDefinition& def) {
    assert(def_exposes(def, "battery"));
    assert(def_binds(def, 0x0001));

    const std::uint8_t full[] = {0xC8};   // 200 half-% -> 100 %
    auto r = dispatch_zcl(def, 0x0001, "genPowerCfg", 1,
                          attr_report(0x0021, 0x20, full));
    assert(r.any_matched);
    assert(u_eq(r.merged.find("battery"), 100));
}

// kTzLock: key "state" -> closuresDoorLock (0x0101) lock (cmd 0x00) /
// unlock (cmd 0x01). Pins that the controllable lock expose is wired to
// a real encoder on the graduated defs.
void check_lock_tz(const PreparedDefinition& def) {
    std::array<std::uint8_t, 16> frame{};

    RuntimeContext ctx_lock{};
    Value lock_v{}; lock_v.type = ValueType::StringRef; lock_v.str = "LOCK";
    auto rl = dispatch_to_zigbee(def, "state", lock_v, ctx_lock, frame);
    assert(rl.ok);
    assert(rl.cluster_id == 0x0101);
    assert(rl.frame_size >= 3);
    assert(frame[2] == 0x00);              // lock command id

    RuntimeContext ctx_unlock{};
    Value unlock_v{}; unlock_v.type = ValueType::StringRef; unlock_v.str = "UNLOCK";
    auto ru = dispatch_to_zigbee(def, "state", unlock_v, ctx_unlock, frame);
    assert(ru.ok);
    assert(ru.cluster_id == 0x0101);
    assert(frame[2] == 0x01);              // unlock command id
}

using namespace zhc::devices::yale;

// ── lock-state decode (regression guard, every series) ───────────────
void test_yrd_lock()  { check_lock_state(kDef_YRD226HA2619); }
void test_ydd_lock()  { check_lock_state(kDef_YDD_D4F0_TSDB); }
void test_ydf_lock()  { check_lock_state(kDef_YDF40); }
void test_yrl_lock()  { check_lock_state(kDef_YRL226_TS); }
void test_ble_lock()  { check_lock_state(kDef_YRD410_BLE); }
void test_lia_lock()  { check_lock_state(kDef_LIA); }

// ── restored battery channel (the fix) ───────────────────────────────
void test_yrd_batt()  { check_battery(kDef_YRD226HA2619); }
void test_ydd_batt()  { check_battery(kDef_YDD_D4F0_TSDB); }
void test_ydf_batt()  { check_battery(kDef_YDF40); }
void test_yrl_batt()  { check_battery(kDef_YRL226_TS); }
void test_ble_batt()  { check_battery(kDef_YRD410_BLE); }
void test_lia_batt()  { check_battery(kDef_LIA); }
void test_solis_batt(){ check_battery(kDef_SOLIS01); }   // already-correct anchor

// ── lock/unlock command encoder (regression guard) ───────────────────
void test_yrd_tz()    { check_lock_tz(kDef_YRD226HA2619); }
void test_ble_tz()    { check_lock_tz(kDef_YRD410_BLE); }

}  // namespace

int main() {
    test_yrd_lock();
    test_ydd_lock();
    test_ydf_lock();
    test_yrl_lock();
    test_ble_lock();
    test_lia_lock();

    test_yrd_batt();
    test_ydd_batt();
    test_ydf_batt();
    test_yrl_batt();
    test_ble_batt();
    test_lia_batt();
    test_solis_batt();

    test_yrd_tz();
    test_ble_tz();
    return 0;
}
