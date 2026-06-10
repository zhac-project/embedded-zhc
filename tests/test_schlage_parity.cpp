// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Schlage Connect smart deadbolt (BE468).
// z2m-source: zigbee-herdsman-converters/src/devices/schlage.ts.
//
// Bug fixed (Sch_BE468 graduated generated/ -> Tier-2 parent):
//
//   * Dropped lock-action stream. z2m wires fz.lock_operation_event +
//     fz.lock_programming_event (closuresDoorLock cmds 0x21/0x20,
//     server->client) -> action / action_user / action_source /
//     action_source_name, plus tz.pincode_lock -> pin_code. The auto-port
//     wired only kFzLock + kFzBattery and exposed battery/voltage/lock_state,
//     so the BE468 reported lock-state + battery but never surfaced who/how
//     the deadbolt was operated, and could not program PIN codes. Restored
//     kFzLockOperationEvent + kFzLockProgrammingEvent + kTzLockPinCode and the
//     matching exposes (the kwikset/onesti/datek precedent — all already
//     exist in _generic/_shared).
//   * lock_state expose was Binary; z2m fz.lock emits the
//     not_fully_locked/locked/unlocked enum -> corrected to Enum.
//
// PIN/user-code *read* response (fz.lock_pin_code_response -> users map)
// has no generic converter and stays INFRA-deferred (kwikset precedent).

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

namespace zhc::devices::schlage {
extern const PreparedDefinition kDef_BE468;  // Connect smart deadbolt
}  // namespace zhc::devices::schlage

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

ExposeType expose_type(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return def.exposes[i].type;
    return ExposeType::Numeric;
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

using namespace zhc::devices::schlage;

// ── exposes / bindings shape (regression guard). ──
void test_shape() {
    assert(def_exposes(kDef_BE468, "battery"));
    assert(def_exposes(kDef_BE468, "voltage"));
    assert(def_exposes(kDef_BE468, "lock_state"));
    // lock_state must be Enum, not Binary (z2m emits the lock-state enum).
    assert(expose_type(kDef_BE468, "lock_state") == ExposeType::Enum);
    // restored action stream + PIN write.
    assert(def_exposes(kDef_BE468, "action"));
    assert(def_exposes(kDef_BE468, "action_user"));
    assert(def_exposes(kDef_BE468, "action_source"));
    assert(def_exposes(kDef_BE468, "action_source_name"));
    assert(def_exposes(kDef_BE468, "pin_code"));
    // z2m binds closuresDoorLock + genPowerCfg on endpoint 1 (endpoints[0]).
    assert(def_binds_ep(kDef_BE468, 1, 0x0101));
    assert(def_binds_ep(kDef_BE468, 1, 0x0001));
}

// ── lock-state decode (closuresDoorLock attr 0x0000, enum8). ──
void test_lock_state() {
    const std::uint8_t locked[] = {0x01};
    auto r1 = dispatch_zcl(kDef_BE468, 0x0101, "closuresDoorLock", 1,
                           attr_report(0x0000, 0x30, locked));
    assert(r1.any_matched);
    assert(u_eq(r1.merged.find("lock_state"), 1));

    const std::uint8_t unlocked[] = {0x02};
    auto r2 = dispatch_zcl(kDef_BE468, 0x0101, "closuresDoorLock", 1,
                           attr_report(0x0000, 0x30, unlocked));
    assert(r2.any_matched);
    assert(u_eq(r2.merged.find("lock_state"), 2));
}

// ── battery decode (genPowerCfg 0x0021, half-percent units -> raw/2). ──
void test_battery() {
    const std::uint8_t full[] = {0xC8};   // 200 half-% -> 100 %
    auto r = dispatch_zcl(kDef_BE468, 0x0001, "genPowerCfg", 1,
                          attr_report(0x0021, 0x20, full));
    assert(r.any_matched);
    assert(u_eq(r.merged.find("battery"), 100));
}

// ── operation-event "action" stream (closuresDoorLock cmd 0x21,
//    server->client). Body: src:u8 code:u8 user_id:u16le. ──
void test_operation_event() {
    // src=0 (keypad), code=2 (unlock), user_id=7.
    const std::uint8_t body[] = {0x00, 0x02, 0x07, 0x00};
    auto r = dispatch_zcl(kDef_BE468, 0x0101, "closuresDoorLock", 1,
                          cmd_frame(0x21, body));
    assert(r.any_matched);
    assert(s_eq(r.merged.find("action"), "unlock"));
    assert(s_eq(r.merged.find("action_source_name"), "keypad"));
    assert(u_eq(r.merged.find("action_user"), 7));

    // src=2 (manual), code=13 (manual_lock), user_id=0.
    const std::uint8_t body2[] = {0x02, 0x0D, 0x00, 0x00};
    auto r2 = dispatch_zcl(kDef_BE468, 0x0101, "closuresDoorLock", 1,
                           cmd_frame(0x21, body2));
    assert(r2.any_matched);
    assert(s_eq(r2.merged.find("action"), "manual_lock"));
    assert(s_eq(r2.merged.find("action_source_name"), "manual"));
}

// ── programming-event "action" stream (closuresDoorLock cmd 0x20). ──
void test_programming_event() {
    // src=0 (keypad), code=2 (pin_code_added), user_id=3.
    const std::uint8_t body[] = {0x00, 0x02, 0x03, 0x00};
    auto r = dispatch_zcl(kDef_BE468, 0x0101, "closuresDoorLock", 1,
                          cmd_frame(0x20, body));
    assert(r.any_matched);
    assert(s_eq(r.merged.find("action"), "pin_code_added"));
    assert(u_eq(r.merged.find("action_user"), 3));
}

// ── kTzLock: key "state" -> closuresDoorLock lock (0x00) / unlock (0x01). ──
void test_lock_tz() {
    std::array<std::uint8_t, 16> frame{};

    RuntimeContext ctx_lock{};
    Value lock_v{}; lock_v.type = ValueType::StringRef; lock_v.str = "LOCK";
    auto rl = dispatch_to_zigbee(kDef_BE468, "state", lock_v, ctx_lock, frame);
    assert(rl.ok);
    assert(rl.cluster_id == 0x0101);
    assert(rl.frame_size >= 3);
    assert(frame[2] == 0x00);              // lock command id

    RuntimeContext ctx_unlock{};
    Value unlock_v{}; unlock_v.type = ValueType::StringRef; unlock_v.str = "UNLOCK";
    auto ru = dispatch_to_zigbee(kDef_BE468, "state", unlock_v, ctx_unlock, frame);
    assert(ru.ok);
    assert(ru.cluster_id == 0x0101);
    assert(frame[2] == 0x01);              // unlock command id
}

// ── kTzLockPinCode: key "pin_code" -> setPinCode (cmd 0x05) "<user>:<pin>". ──
void test_pin_tz() {
    std::array<std::uint8_t, 24> frame{};
    RuntimeContext ctx{};
    Value pin_v{}; pin_v.type = ValueType::StringRef; pin_v.str = "3:1234";
    auto r = dispatch_to_zigbee(kDef_BE468, "pin_code", pin_v, ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0x0101);
    assert(frame[2] == 0x05);              // setPinCode command id
    assert(frame[3] == 0x03);              // user id LE low byte = 3
}

}  // namespace

int main() {
    test_shape();
    test_lock_state();
    test_battery();
    test_operation_event();
    test_programming_event();
    test_lock_tz();
    test_pin_tz();
    return 0;
}
