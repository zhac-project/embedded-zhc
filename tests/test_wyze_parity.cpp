// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Wyze WLCKG1 ("Ford") door lock.
//
// z2m wires fromZigbee: [fz.lock, fz.lock_operation_event, fz.battery].
// The auto-generated port wired only fz.lock + fz.battery and DROPPED
// fz.lock_operation_event — the closuresDoorLock (0x0101) cluster-specific
// command 0x21 (Operation Event Notification, ServerToClient) that reports
// HOW the lock was operated (manual / keypad / rf, lock vs unlock, user id).
// Fixed by wiring the generic kFzLockOperationEvent alongside the existing
// kFzLock + kFzBattery converters.
//
// Verified here:
//   * lock operation event (cmd 0x21) -> action / action_user /
//     action_source / action_source_name runtime keys.
//   * Regression: kFzLock still emits lock_state (attr 0x0000) and the
//     battery / lock_state exposes are intact.
//
// z2m-source: zigbee-herdsman-converters/src/devices/wyze.ts #WLCKG1.

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

namespace zhc::devices::wyze {
extern const PreparedDefinition kDef_WLCKG1;
}  // namespace zhc::devices::wyze

using namespace zhc;

namespace {

constexpr std::uint16_t CLOSURES_DOOR_LOCK = 0x0101;

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

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

// ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A=ReportAttributes.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// closuresDoorLock cluster-specific command, ServerToClient.
//   fc = 0x09 (bit0 cluster-specific, bit3 server->client)
//   tsn, cmd, then body.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> body) {
    std::vector<std::uint8_t> v{0x09, 0x42, cmd};
    for (auto b : body) v.push_back(b);
    return v;
}

const Value* str_val(const Value* v) {
    return (v && v->type == ValueType::StringRef) ? v : nullptr;
}

}  // namespace

// ── lock operation event (cmd 0x21) — the dropped decoder ────────────
static void test_operation_event() {
    const auto& def = devices::wyze::kDef_WLCKG1;

    // Body: src:u8 code:u8 user_id:u16le.
    // src=2 (manual), code=14 (manual_unlock), user_id=7.
    const std::uint8_t body[] = {0x02, 0x0E, 0x07, 0x00};
    auto frame = cmd_frame(0x21, body);
    auto r = dispatch_zcl(def, CLOSURES_DOOR_LOCK, "closuresDoorLock", 1, frame);
    assert(r.any_matched);

    const Value* action = str_val(r.merged.find("action"));
    assert(action && std::strcmp(action->str, "manual_unlock") == 0);

    const Value* user = r.merged.find("action_user");
    assert(user && user->type == ValueType::Uint && user->u == 7);

    const Value* src = r.merged.find("action_source");
    assert(src && src->type == ValueType::Uint && src->u == 2);

    const Value* src_name = str_val(r.merged.find("action_source_name"));
    assert(src_name && std::strcmp(src_name->str, "manual") == 0);

    // A different code maps to a different action ("key_lock" = code 8).
    const std::uint8_t body2[] = {0x00, 0x08, 0x00, 0x00};
    auto r2 = dispatch_zcl(def, CLOSURES_DOOR_LOCK, "closuresDoorLock", 1,
                           cmd_frame(0x21, body2));
    assert(r2.any_matched);
    const Value* a2 = str_val(r2.merged.find("action"));
    assert(a2 && std::strcmp(a2->str, "key_lock") == 0);
}

// ── regression: kFzLock still decodes lock_state (attr 0x0000) ───────
static void test_lock_state_regression() {
    const auto& def = devices::wyze::kDef_WLCKG1;

    // LockState attr 0x0000, enum8 (type 0x30), value 1 = locked.
    const std::uint8_t locked[] = {0x01};
    auto r = dispatch_zcl(def, CLOSURES_DOOR_LOCK, "closuresDoorLock", 1,
                          attr_report(0x0000, 0x30, locked));
    assert(r.any_matched);
    const Value* ls = r.merged.find("lock_state");
    assert(ls && ls->type == ValueType::Uint && ls->u == 1);

    // Exposes intact.
    assert(def_exposes(def, "lock_state"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));
}

int main() {
    test_operation_event();
    test_lock_state_regression();
    return 0;
}
