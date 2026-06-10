// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Danalock V3-BTZB/V3-BTZBE BT/ZB smartlock.
//
// The generated port wired only kFzLock + kFzBattery and exposed
// battery/voltage/lock_state. z2m's danalock def also wires
// fz.lock_operation_event + fz.lock_programming_event (the lock `action`
// stream → action / action_user / action_source / action_source_name) and
// tz.pincode_lock (the `pin_code` write). The generic closuresDoorLock
// helpers kFzLockOperationEvent (cmd 0x21) / kFzLockProgrammingEvent
// (cmd 0x20) / kTzLockPinCode already exist (used by datek/onesti/kwikset)
// and are now wired into the danalock def.
//
// Verified here at the dispatch_from_zigbee boundary:
//   * operation-event cmd 0x21 → action="lock", action_user, action_source,
//     action_source_name="manual".
//   * programming-event cmd 0x20 → action="pin_code_added".
//   * Regression: battery / lock_state still decode + are exposed; the
//     closuresDoorLock + genPowerCfg binds + pin_code expose are present.
//
// z2m-source: zigbee-herdsman-converters/src/devices/danalock.ts
//             #V3-BTZB/V3-BTZBE.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::danalock {
extern const PreparedDefinition kDef_V3_BTZB_V3_BTZBE;
}  // namespace zhc::devices::danalock

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

// Cluster-specific, server→client command frame:
//   fc(1)=0x09 (cluster-specific + direction S→C)  tsn(1)  cmd(1)  body...
// closuresDoorLock operation/programming event body is:
//   src:u8  code:u8  user_id:u16le  (rest unused).
std::vector<std::uint8_t> lock_event(std::uint8_t cmd, std::uint8_t src,
                                     std::uint8_t code, std::uint16_t user_id) {
    return {0x09, 0x42, cmd, src, code,
            static_cast<std::uint8_t>(user_id & 0xFF),
            static_cast<std::uint8_t>(user_id >> 8)};
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool def_binds_ep(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster)
            return true;
    return false;
}

constexpr std::uint16_t GEN_POWER_CFG     = 0x0001;
constexpr std::uint16_t CLOSURES_DOORLOCK = 0x0101;

std::vector<std::uint8_t> u8v(std::uint8_t x) { return {x}; }

void check_v3() {
    const auto& def = devices::danalock::kDef_V3_BTZB_V3_BTZBE;

    // ── The fix: lock operation event (cmd 0x21). src=2 (manual),
    //    code=1 (lock), user_id=10. → action="lock", action_user=10,
    //    action_source=2, action_source_name="manual". ──
    {
        auto ev = lock_event(0x21, /*src*/2, /*code*/1, /*user*/10);
        auto r  = dispatch_zcl(def, CLOSURES_DOORLOCK, "closuresDoorLock", 1, ev);
        assert(r.any_matched);

        const Value* a = r.merged.find("action");
        assert(a && a->type == ValueType::StringRef &&
               std::strcmp(a->str, "lock") == 0);

        const Value* au = r.merged.find("action_user");
        assert(au && au->type == ValueType::Uint && au->u == 10);

        const Value* as = r.merged.find("action_source");
        assert(as && as->type == ValueType::Uint && as->u == 2);

        const Value* asn = r.merged.find("action_source_name");
        assert(asn && asn->type == ValueType::StringRef &&
               std::strcmp(asn->str, "manual") == 0);
    }

    // ── Lock programming event (cmd 0x20). code=2 → "pin_code_added". ──
    {
        auto ev = lock_event(0x20, /*src*/0, /*code*/2, /*user*/7);
        auto r  = dispatch_zcl(def, CLOSURES_DOORLOCK, "closuresDoorLock", 1, ev);
        assert(r.any_matched);
        const Value* a = r.merged.find("action");
        assert(a && a->type == ValueType::StringRef &&
               std::strcmp(a->str, "pin_code_added") == 0);
    }

    // ── Regression: battery (genPowerCfg 0x0021 BatteryPercentageRemaining,
    //    u8) still decodes via the generic kFzBattery. ──
    {
        auto r = dispatch_zcl(def, GEN_POWER_CFG, "genPowerCfg", 1,
                              attr_report(0x0021, 0x20, u8v(150)));
        assert(r.any_matched);
        assert(r.merged.find("battery") != nullptr);
    }

    // ── Regression: lock_state (closuresDoorLock 0x0000 LockState enum8)
    //    still decodes via the generic kFzLock. ──
    {
        auto r = dispatch_zcl(def, CLOSURES_DOORLOCK, "closuresDoorLock", 1,
                              attr_report(0x0000, 0x30, u8v(1)));
        assert(r.any_matched);
        assert(r.merged.find("lock_state") != nullptr);
    }

    // ── Exposes / bindings present. ──
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "lock_state"));
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "action_user"));
    assert(def_exposes(def, "action_source_name"));
    assert(def_exposes(def, "pin_code"));
    assert(def_binds_ep(def, 1, GEN_POWER_CFG));
    assert(def_binds_ep(def, 1, CLOSURES_DOORLOCK));
}

}  // namespace

int main() {
    check_v3();
    return 0;
}
