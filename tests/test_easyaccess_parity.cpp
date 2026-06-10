// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for EasyAccess EasyCode903G2.1 (EasyFinger V2) door lock.
// Pins the real gaps the auto-generated port carried vs the z2m wire
// shape (easyaccess.ts #EasyCode903G2.1):
//
//   * Phantom genOnOff bundle: the port wired kFzOnOff + kTzOnOff and a
//     {1,0x0006} binding. The device is a closuresDoorLock (0x0101) with
//     no on/off cluster. Removed — the lock toggle goes through the
//     generic kTzLock ("state" -> closuresDoorLock lock/unlock).
//
//   * Dropped event stream: z2m's fz.easycode_action decodes a RAW
//     closuresDoorLock command into action = lock / zigbee_unlock /
//     rfid_unlock / keypad_unlock. The port had no action decoder and no
//     action expose. Ported as kFzEasycodeAction.
//
//   * Dropped sound_volume read (closuresDoorLock soundVolume 0x0024,
//     z2m constants.lockSoundVolume labels) + sound_volume / auto_relock
//     writers. Ported as kFzEasycodeSoundVolume / kTzEasycodeSoundVolume
//     / kTzEasycodeAutoRelock.
//
//   * Bindings corrected to z2m: endpoint 11 closuresDoorLock (0x0101) +
//     genPowerCfg (0x0001); no genOnOff (0x0006).
//
// z2m-source: zigbee-herdsman-converters/src/devices/easyaccess.ts +
//             converters/fromZigbee.ts (easycode_action, lock).

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

namespace zhc::devices::easyaccess {
extern const PreparedDefinition kDef_EasyCode903G2_1;
}  // namespace zhc::devices::easyaccess

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

// RAW closuresDoorLock command. z2m's easycode_action reads the full
// frame buffer at index [3] and [4]; ez exposes the same bytes in
// raw_data. Header [fc, tsn, cmd] = [0x09, tsn, cmd], so data3/data4
// are the first two body bytes.
DispatchResult dispatch_action(std::uint8_t data3, std::uint8_t data4) {
    std::vector<std::uint8_t> bytes = { 0x09, 0x42, 0x10, data3, data4, 0x00 };
    auto raw = make_frame(0x0101, 11,
                          std::span<const std::uint8_t>(bytes.data(), bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "closuresDoorLock";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, zhc::devices::easyaccess::kDef_EasyCode903G2_1,
                               raw, ctx);
}

// Profile-wide Report Attributes (server->client) carrying one enum8:
//   fc(1)=0x18  tsn(1)  cmd(1)=0x0A  attrId_lo attrId_hi dataType(0x30) value(1)
DispatchResult dispatch_attr(std::uint16_t attr, std::uint8_t value) {
    std::vector<std::uint8_t> bytes = {
        0x18, 0x42, 0x0A,
        static_cast<std::uint8_t>(attr & 0xFF),
        static_cast<std::uint8_t>(attr >> 8),
        0x30, value,
    };
    auto raw = make_frame(0x0101, 11,
                          std::span<const std::uint8_t>(bytes.data(), bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "closuresDoorLock";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, zhc::devices::easyaccess::kDef_EasyCode903G2_1,
                               raw, ctx);
}

bool str_eq(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, s) == 0;
}
bool uint_eq(const Value* v, std::uint64_t u) {
    return v && v->type == ValueType::Uint && v->u == u;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}
bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster)
            return true;
    return false;
}

}  // namespace

// ── easycode_action: byte[4] in {13,14}; else byte[3] lookup ─────────
static void test_action() {
    // data[4] = 13 -> "lock" (takes priority over data[3]).
    auto lock = dispatch_action(/*d3=*/3, /*d4=*/13);
    assert(lock.any_matched);
    assert(str_eq(lock.merged.find("action"), "lock"));

    // data[4] = 14 -> "zigbee_unlock".
    auto zb = dispatch_action(/*d3=*/0, /*d4=*/14);
    assert(zb.any_matched);
    assert(str_eq(zb.merged.find("action"), "zigbee_unlock"));

    // data[4] not in {13,14} -> fall through to data[3]: 3 -> rfid_unlock.
    auto rfid = dispatch_action(/*d3=*/3, /*d4=*/99);
    assert(rfid.any_matched);
    assert(str_eq(rfid.merged.find("action"), "rfid_unlock"));

    // data[3] = 0 -> keypad_unlock.
    auto kp = dispatch_action(/*d3=*/0, /*d4=*/99);
    assert(kp.any_matched);
    assert(str_eq(kp.merged.find("action"), "keypad_unlock"));
}

// ── lock_state attribute (0x0000 enum8) via generic kFzLock ──────────
static void test_lock_state() {
    auto r = dispatch_attr(0x0000, 1);  // 1 = locked
    assert(r.any_matched);
    assert(uint_eq(r.merged.find("lock_state"), 1));
}

// ── sound_volume attribute (0x0024 enum8) -> z2m label ───────────────
static void test_sound_volume() {
    auto silent = dispatch_attr(0x0024, 0);
    assert(silent.any_matched);
    assert(str_eq(silent.merged.find("sound_volume"), "silent_mode"));

    auto high = dispatch_attr(0x0024, 2);
    assert(high.any_matched);
    assert(str_eq(high.merged.find("sound_volume"), "high_volume"));
}

// ── exposes + bindings shape (no phantom genOnOff) ───────────────────
static void test_shape() {
    const auto& def = zhc::devices::easyaccess::kDef_EasyCode903G2_1;
    assert(def_exposes(def, "state"));         // lock toggle (kTzLock)
    assert(def_exposes(def, "lock_state"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "sound_volume"));
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "auto_relock"));

    // bindings: ep11 closuresDoorLock + genPowerCfg, NO genOnOff.
    assert(def_binds(def, 11, 0x0101));
    assert(def_binds(def, 11, 0x0001));
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        assert(def.bindings[i].cluster_id != 0x0006);  // no phantom genOnOff
}

int main() {
    test_action();
    test_lock_state();
    test_sound_volume();
    test_shape();
    return 0;
}
