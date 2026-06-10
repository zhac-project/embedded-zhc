// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Kwikset SmartCode door-lock family.
// z2m-source: zigbee-herdsman-converters/src/devices/kwikset.ts.
//
// Bug fixed (each of the 7 defs graduated generated/ -> Tier-2 parent):
//
//   * Bindings on the WRONG endpoint. Every Kwikset def in z2m configures
//     reporting via `device.getEndpoint(2)` and binds closuresDoorLock
//     (0x0101) + genPowerCfg (0x0001) on endpoint 2. The generated defs
//     bound both clusters on endpoint 1 (the generator's default when z2m
//     uses a non-default getEndpoint(N)). run_configure() passes the bind
//     endpoint verbatim to configure_bind, so the coordinator bound a
//     wrong/absent endpoint and the lock never set up lock-state or
//     battery reporting. Re-endpointed all 7 to {2, 0x0001} + {2, 0x0101}
//     and added .default_endpoint=2 (the onesti S4RX-110 precedent).
//
// The fz/tz wiring (kFzLock -> lock_state, kFzBattery -> battery,
// kFzLockOperationEvent -> action stream, kTzLock key "state" lock/unlock,
// kTzLockPinCode -> pin_code) was already correct (Tier-2 hand-port,
// 2026-04-29a); these tests pin that surface as a regression guard
// alongside the corrected binding endpoint. PIN/user-code *management*
// beyond the setPinCode encoder remains INFRA-deferred.

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

namespace zhc::devices::kwikset {
extern const PreparedDefinition kDef_D66492_001;  // conversion kit (no PIN)
extern const PreparedDefinition kDef_D99100_006;  // 910 deadbolt (no PIN)
extern const PreparedDefinition kDef_D99100_045;  // 910 deadbolt (PIN)
extern const PreparedDefinition kDef_D99120_021;  // 912 lever (PIN)
extern const PreparedDefinition kDef_D99140_002;  // deadbolt (PIN)
extern const PreparedDefinition kDef_D99140_031;  // deadbolt (PIN, 3 models)
extern const PreparedDefinition kDef_D99140_139;  // conversion kit W3 (no PIN)
}  // namespace zhc::devices::kwikset

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

bool def_binds_ep(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool u_eq(const Value* v, std::uint64_t expect) {
    return v && v->type == ValueType::Uint && v->u == expect;
}

bool s_eq(const Value* v, const char* expect) {
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, expect) == 0;
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

// Cluster-specific command frame (server->client): fc=0x09, tsn, cmd, body.
// fc bit 0 = cluster-specific, bit 3 = direction server->client.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd_id,
                                    std::span<const std::uint8_t> body) {
    std::vector<std::uint8_t> v{0x09, 0x42, cmd_id};
    for (auto b : body) v.push_back(b);
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

// ── THE FIX: binding endpoint. z2m device.getEndpoint(2). ─────────────
void check_bindings_ep2(const PreparedDefinition& def) {
    // closuresDoorLock + genPowerCfg must bind on endpoint 2, NOT 1.
    assert(def_binds_ep(def, 2, 0x0101));   // closuresDoorLock
    assert(def_binds_ep(def, 2, 0x0001));   // genPowerCfg
    assert(!def_binds_ep(def, 1, 0x0101));  // regression: not ep1
    assert(!def_binds_ep(def, 1, 0x0001));
    assert(def.default_endpoint == 2);
}

// ── lock-state decode (regression guard). The fz uses WILDCARD_ENDPOINT,
//    so it decodes regardless of source endpoint — but the device reports
//    from endpoint 2, so we feed src_ep=2. ──
void check_lock_state(const PreparedDefinition& def) {
    assert(def_exposes(def, "lock_state"));

    const std::uint8_t locked[] = {0x01};
    auto r1 = dispatch_zcl(def, 0x0101, "closuresDoorLock", 2,
                           attr_report(0x0000, 0x30, locked));
    assert(r1.any_matched);
    assert(u_eq(r1.merged.find("lock_state"), 1));

    const std::uint8_t unlocked[] = {0x02};
    auto r2 = dispatch_zcl(def, 0x0101, "closuresDoorLock", 2,
                           attr_report(0x0000, 0x30, unlocked));
    assert(r2.any_matched);
    assert(u_eq(r2.merged.find("lock_state"), 2));
}

// ── battery decode (genPowerCfg 0x0021 batteryPercentageRemaining,
//    half-percent units → "battery" = raw/2). ──
void check_battery(const PreparedDefinition& def) {
    assert(def_exposes(def, "battery"));

    const std::uint8_t full[] = {0xC8};   // 200 half-% -> 100 %
    auto r = dispatch_zcl(def, 0x0001, "genPowerCfg", 2,
                          attr_report(0x0021, 0x20, full));
    assert(r.any_matched);
    assert(u_eq(r.merged.find("battery"), 100));
}

// ── operation-event "action" stream (closuresDoorLock cmd 0x21,
//    server->client). Body: src:u8 code:u8 user_id:u16le.
//    code 2 = unlock, src 0 = keypad. ──
void check_operation_event(const PreparedDefinition& def) {
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "action_source_name"));
    assert(def_exposes(def, "action_user"));

    // src=0 (keypad), code=2 (unlock), user_id=7.
    const std::uint8_t body[] = {0x00, 0x02, 0x07, 0x00};
    auto r = dispatch_zcl(def, 0x0101, "closuresDoorLock", 2,
                          cmd_frame(0x21, body));
    assert(r.any_matched);
    assert(s_eq(r.merged.find("action"), "unlock"));
    assert(s_eq(r.merged.find("action_source_name"), "keypad"));
    assert(u_eq(r.merged.find("action_user"), 7));

    // src=2 (manual), code=1 (lock), user_id=0.
    const std::uint8_t body2[] = {0x02, 0x01, 0x00, 0x00};
    auto r2 = dispatch_zcl(def, 0x0101, "closuresDoorLock", 2,
                           cmd_frame(0x21, body2));
    assert(r2.any_matched);
    assert(s_eq(r2.merged.find("action"), "lock"));
    assert(s_eq(r2.merged.find("action_source_name"), "manual"));
}

// ── kTzLock: key "state" -> closuresDoorLock lock (0x00) / unlock (0x01). ─
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

// ── kTzLockPinCode: key "pin_code" -> closuresDoorLock setPinCode
//    (cmd 0x05) for "<user>:<pin>". Only the PIN-capable defs expose it. ─
void check_pin_tz(const PreparedDefinition& def) {
    assert(def_exposes(def, "pin_code"));
    std::array<std::uint8_t, 24> frame{};
    RuntimeContext ctx{};
    Value pin_v{}; pin_v.type = ValueType::StringRef; pin_v.str = "3:1234";
    auto r = dispatch_to_zigbee(def, "pin_code", pin_v, ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0x0101);
    assert(frame[2] == 0x05);              // setPinCode command id
    assert(frame[3] == 0x03);              // user id LE low byte = 3
}

using namespace zhc::devices::kwikset;

// All 7 defs carry the endpoint-2 binding fix.
void test_binds_66492() { check_bindings_ep2(kDef_D66492_001); }
void test_binds_99100_006() { check_bindings_ep2(kDef_D99100_006); }
void test_binds_99100_045() { check_bindings_ep2(kDef_D99100_045); }
void test_binds_99120_021() { check_bindings_ep2(kDef_D99120_021); }
void test_binds_99140_002() { check_bindings_ep2(kDef_D99140_002); }
void test_binds_99140_031() { check_bindings_ep2(kDef_D99140_031); }
void test_binds_99140_139() { check_bindings_ep2(kDef_D99140_139); }

// Core lock + battery + action surface (representative spread).
void test_lock_state_66492() { check_lock_state(kDef_D66492_001); }
void test_lock_state_99140_031() { check_lock_state(kDef_D99140_031); }
void test_battery_66492() { check_battery(kDef_D66492_001); }
void test_battery_99100_045() { check_battery(kDef_D99100_045); }
void test_action_99140_002() { check_operation_event(kDef_D99140_002); }
void test_action_99120_021() { check_operation_event(kDef_D99120_021); }

// Lock/unlock encoder (regression guard).
void test_tz_66492() { check_lock_tz(kDef_D66492_001); }
void test_tz_99140_139() { check_lock_tz(kDef_D99140_139); }

// PIN encoder on the PIN-capable defs only.
void test_pin_99100_045() { check_pin_tz(kDef_D99100_045); }
void test_pin_99140_031() { check_pin_tz(kDef_D99140_031); }

}  // namespace

int main() {
    test_binds_66492();
    test_binds_99100_006();
    test_binds_99100_045();
    test_binds_99120_021();
    test_binds_99140_002();
    test_binds_99140_031();
    test_binds_99140_139();

    test_lock_state_66492();
    test_lock_state_99140_031();
    test_battery_66492();
    test_battery_99100_045();
    test_action_99140_002();
    test_action_99120_021();

    test_tz_66492();
    test_tz_99140_139();

    test_pin_99100_045();
    test_pin_99140_031();
    return 0;
}
