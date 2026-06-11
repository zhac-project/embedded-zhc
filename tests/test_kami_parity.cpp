// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the KAMI (Yi) N20 contact / motion sensor.
// z2m-source: zigbee-herdsman-converters/src/devices/kami.ts +
//             converters/fromZigbee.ts (KAMI_contact, KAMI_occupancy).
//
// Bug fixed (Kam_N20 graduated generated/ -> Tier-2 parent):
//
//  * The generated port was triply wrong for this sensor:
//      1. phantom on/off (generic kFzOnOff + kTzOnOff + genOnOff binding +
//         a controllable `state` expose) — the device has no genOnOff;
//      2. wrong decoder — generic kFzIasZone publishing alarm/tamper/
//         battery_low from the standard IAS bitmask;
//      3. wrong exposes — should be `contact` + `action(["motion"])`.
//    KAMI firmware is non-standard: z2m's fz.KAMI_contact reads the raw
//    ssIasZone frame byte 7 ({contact: data[7] === 0}) and
//    fz.KAMI_occupancy reads the raw msOccupancySensing frame byte 7
//    ({action: "motion"} when data[7] === 1). Now wired the vendor
//    kami::kFzKamiContact / kami::kFzKamiOccupancy converters.
//
// These tests pin the corrected wire-shape decode.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::kami {
extern const PreparedDefinition kDef_N20;   // contact + motion sensor
}  // namespace zhc::devices::kami

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool str_eq(const Value* v, const char* expect) {
    return v && v->type == ValueType::StringRef && v->str
           && std::strcmp(v->str, expect) == 0;
}

// ── 9-byte raw frame with a controllable absolute byte 7 ─────────────
//   fc(1)=0x09 (server->client)  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)=byte7  delay_hi(1)
// KAMI firmware ignores the standard field semantics — both converters
// key off the absolute frame byte 7, which lands on the `zoneId` slot.
struct RawFrame { std::array<std::uint8_t, 9> bytes; };
RawFrame raw_byte7(std::uint8_t byte7, std::uint8_t tsn = 0x42) {
    RawFrame f{};
    f.bytes = {0x09, tsn, 0x00, 0x00, 0x00, 0x00, 0x00, byte7, 0x00};
    return f;
}

DispatchResult dispatch_raw(const PreparedDefinition& def,
                            std::uint16_t cluster_id, const RawFrame& f) {
    auto raw = make_frame(cluster_id, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

// ── exposes / wiring shape ───────────────────────────────────────────
static void test_n20_shape() {
    const auto& def = devices::kami::kDef_N20;
    assert(def_exposes(def, "contact"));
    assert(def_exposes(def, "action"));
    // No phantom on/off state, no controllable TZ, no genOnOff binding.
    assert(!def_exposes(def, "state"));
    assert(!def_exposes(def, "alarm"));
    assert(!def_exposes(def, "tamper"));
    assert(!def_exposes(def, "battery_low"));
    assert(def.to_zigbee_count == 0);
    assert(!def_binds(def, 0x0006));
    assert(def_binds(def, 0x0500));
}

// ── KAMI_contact: {contact: data[7] === 0} ───────────────────────────
static void test_n20_contact() {
    const auto& def = devices::kami::kDef_N20;

    // byte7 == 0 -> contact true (closed).
    auto closed = dispatch_raw(def, 0x0500, raw_byte7(0x00));
    assert(closed.any_matched);
    assert(b_true(closed.merged.find("contact")));

    // byte7 != 0 -> contact false (open).
    auto open = dispatch_raw(def, 0x0500, raw_byte7(0x01));
    assert(open.any_matched);
    assert(b_false(open.merged.find("contact")));

    // The contact frame must NOT publish IAS alarm-bit booleans or action.
    assert(closed.merged.find("alarm") == nullptr);
    assert(closed.merged.find("tamper") == nullptr);
    assert(closed.merged.find("battery_low") == nullptr);
    assert(closed.merged.find("action") == nullptr);
}

// ── KAMI_occupancy: {action: "motion"} when data[7] === 1 ────────────
static void test_n20_motion() {
    const auto& def = devices::kami::kDef_N20;

    // byte7 == 1 -> action "motion".
    auto motion = dispatch_raw(def, 0x0406, raw_byte7(0x01));
    assert(motion.any_matched);
    assert(str_eq(motion.merged.find("action"), "motion"));
    assert(motion.merged.find("occupancy") == nullptr);   // z2m emits no boolean

    // byte7 != 1 -> converter declines: no action.
    auto idle = dispatch_raw(def, 0x0406, raw_byte7(0x00));
    assert(idle.merged.find("action") == nullptr);
}

int main() {
    test_n20_shape();
    test_n20_contact();
    test_n20_motion();
    return 0;
}
