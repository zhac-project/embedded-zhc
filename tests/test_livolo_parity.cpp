// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Livolo TI0001-* family. Pins the real gaps the
// auto-generated ports carried against the z2m wire shape:
//
//   * Every Livolo device speaks a bespoke raw protocol carried over
//     genPowerCfg (0x0001) whose body starts with the magic header
//     [0x7A 0xD1]. Those frames decode to MessageType::Raw (frame
//     control 0x7A is not cluster-specific), but the custom decoders
//     gated only on MessageType::Command, so NONE of them ever fired.
//     Fixed by adding type_bit(Raw) to every Livolo FzConverter (matches
//     the terncy / orvibo raw-cluster precedent).
//
//   * TI0001 (4-gang switch) decoded state_left/_right/_bottom_left/
//     _bottom_right but exposed only a bare "state" (no key had a home).
//     Exposes corrected to the four keyed switches.
//
//   * TI0001-pir carried a phantom IAS bundle (alarm/tamper/battery_low
//     + ssIasZone binding) while the decoder emits "occupancy". Fixed to
//     occupancy + genPowerCfg binding.
//
//   * TI0001-curtain-switch / -dimmer / -cover were wired to dead generic
//     on/off / brightness / cover-position converters (standard 0x0006 /
//     0x0008 / 0x0102) that never match the raw frame. Ported the custom
//     livolo_curtain_switch_state / livolo_dimmer_state / livolo_cover_state
//     decoders.
//
// z2m-source: zigbee-herdsman-converters/src/devices/livolo.ts +
//             src/converters/fromZigbee.ts (livolo_*_state).

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

namespace zhc::devices::livolo {
extern const PreparedDefinition kDef_TI0001;                // 4-gang switch
extern const PreparedDefinition kDef_TI0001_pir;            // occupancy
extern const PreparedDefinition kDef_TI0001_curtain_switch; // curtain
extern const PreparedDefinition kDef_TI0001_dimmer;         // dimmer
extern const PreparedDefinition kDef_TI0001_cover;          // roller blind
extern const PreparedDefinition kDef_TI0001_socket;         // socket (regression)
}  // namespace zhc::devices::livolo

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

// Dispatch a raw Livolo genPowerCfg (0x0001) frame. The bytes are the
// full z2m `msg.data` view (header [0x7A 0xD1] at offset 0). decode_frame
// copies them verbatim into raw_data; frame control 0x7A classifies as
// MessageType::Raw with direction = server->client.
DispatchResult dispatch_raw(const PreparedDefinition& def,
                            const std::vector<std::uint8_t>& bytes) {
    auto raw = make_frame(0x0001, 6,
                          std::span<const std::uint8_t>(bytes.data(), bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    assert(msg.type == MessageType::Raw);
    assert(msg.direction == Direction::ServerToClient);
    msg.cluster = "genPowerCfg";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// 8-byte filler IEEE address occupying data[2..9].
constexpr std::uint8_t kIeee[8] = {199, 147, 3, 24, 0, 75, 18, 0};

// Build a header+ieee prefix [0x7A 0xD1 <ieee*8>] then append `tail`.
std::vector<std::uint8_t> magic(std::initializer_list<std::uint8_t> tail) {
    std::vector<std::uint8_t> v{0x7A, 0xD1};
    for (auto b : kIeee) v.push_back(b);
    for (auto b : tail)  v.push_back(b);
    return v;
}

}  // namespace

// ── 4-gang switch: data[10]==7, status bits 0..3 -> 4 keyed switches ──
static void test_switch_4gang() {
    const auto& def = devices::livolo::kDef_TI0001;
    assert(def_exposes(def, "state_left"));
    assert(def_exposes(def, "state_right"));
    assert(def_exposes(def, "state_bottom_left"));
    assert(def_exposes(def, "state_bottom_right"));
    assert(!def_exposes(def, "state"));   // bare placeholder must be gone

    // data layout: [..hdr+ieee(0..9).., 10=7, 11=1, 12=6, 13=1, 14=status, 15=11]
    // status 0b0101 -> left ON, right OFF, bottom_left ON, bottom_right OFF.
    auto r = dispatch_raw(def, magic({7, 1, 6, 1, 0x05, 11}));
    assert(r.any_matched);
    assert(b_true(r.merged.find("state_left")));
    assert(b_false(r.merged.find("state_right")));
    assert(b_true(r.merged.find("state_bottom_left")));
    assert(b_false(r.merged.find("state_bottom_right")));
}

// ── PIR: occupancy from bit 0; no phantom IAS expose / binding ────────
static void test_pir() {
    const auto& def = devices::livolo::kDef_TI0001_pir;
    assert(def_exposes(def, "occupancy"));
    assert(!def_exposes(def, "alarm"));
    assert(!def_exposes(def, "tamper"));
    assert(!def_exposes(def, "battery_low"));
    assert(!def_binds(def, 0x0500));      // no ssIasZone
    assert(def_binds(def, 0x0001));       // genPowerCfg

    auto on = dispatch_raw(def, magic({7, 1, 7, 1, 0x01, 11}));
    assert(on.any_matched);
    assert(b_true(on.merged.find("occupancy")));

    auto off = dispatch_raw(def, magic({7, 1, 7, 1, 0x00, 11}));
    assert(off.any_matched);
    assert(b_false(off.merged.find("occupancy")));
}

// ── Curtain: data[10] in {5,2}; s==1 -> left ON, s==0 -> right ON ─────
static void test_curtain() {
    const auto& def = devices::livolo::kDef_TI0001_curtain_switch;
    assert(def_exposes(def, "state_left"));
    assert(def_exposes(def, "state_right"));
    assert(!def_exposes(def, "state"));

    // status 1 -> left ON, right OFF.
    auto left = dispatch_raw(def, magic({5, 1, 5, 0, 0x01, 11}));
    assert(left.any_matched);
    assert(str_eq(left.merged.find("state_left"),  "ON"));
    assert(str_eq(left.merged.find("state_right"), "OFF"));

    // status 0 -> left OFF, right ON.
    auto right = dispatch_raw(def, magic({5, 1, 5, 0, 0x00, 11}));
    assert(right.any_matched);
    assert(str_eq(right.merged.find("state_left"),  "OFF"));
    assert(str_eq(right.merged.find("state_right"), "ON"));
}

// ── Dimmer: dp 7/13 -> state; dp 5 -> brightness (val*10 -> 0..255) ───
static void test_dimmer() {
    const auto& def = devices::livolo::kDef_TI0001_dimmer;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));

    // dp 7, data[14] bit0 -> state ON.
    auto on = dispatch_raw(def, magic({7, 1, 0, 1, 0x01, 11}));
    assert(on.any_matched);
    assert(str_eq(on.merged.find("state"), "ON"));

    // dp 5, data[14]=100 -> value 1000 -> brightness 255.
    auto bri = dispatch_raw(def, magic({5, 1, 0, 0, 100, 11}));
    assert(bri.any_matched);
    assert(uint_eq(bri.merged.find("brightness"), 255));

    // dp 5, data[14]=50 -> value 500 -> brightness 127.
    auto half = dispatch_raw(def, magic({5, 1, 0, 0, 50, 11}));
    assert(half.any_matched);
    assert(uint_eq(half.merged.find("brightness"), 127));
}

// ── Cover: dp 0x0c/0x0f reportType 0x04 -> position/state/moving ──────
static void test_cover() {
    const auto& def = devices::livolo::kDef_TI0001_cover;
    assert(def_exposes(def, "position"));
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "moving"));
    assert(def_exposes(def, "motor_speed"));
    assert(def_exposes(def, "motor_direction"));

    // dp 0x0c (settled), reportType 0x04, data[13]=30 -> position 70, OPEN, not moving.
    auto pos = dispatch_raw(def, magic({0x0C, 0, 0x04, 30, 0, 11}));
    assert(pos.any_matched);
    assert(uint_eq(pos.merged.find("position"), 70));
    assert(str_eq(pos.merged.find("state"), "OPEN"));
    assert(b_false(pos.merged.find("moving")));

    // dp 0x0f (in motion), reportType 0x04, data[13]=100 -> position 0, CLOSE, moving.
    auto closed = dispatch_raw(def, magic({0x0F, 0, 0x04, 100, 0, 11}));
    assert(closed.any_matched);
    assert(uint_eq(closed.merged.find("position"), 0));
    assert(str_eq(closed.merged.find("state"), "CLOSE"));
    assert(b_true(closed.merged.find("moving")));

    // reportType 0x12 -> motor_speed.
    auto speed = dispatch_raw(def, magic({0x0C, 0, 0x12, 35, 0, 11}));
    assert(speed.any_matched);
    assert(uint_eq(speed.merged.find("motor_speed"), 35));

    // reportType 0x13, data[13] >= 0x80 -> REVERSE.
    auto rev = dispatch_raw(def, magic({0x0C, 0, 0x13, 0x80, 0, 11}));
    assert(rev.any_matched);
    assert(str_eq(rev.merged.find("motor_direction"), "REVERSE"));

    // reportType 0x13, data[13] < 0x80 -> FORWARD.
    auto fwd = dispatch_raw(def, magic({0x0C, 0, 0x13, 0x10, 0, 11}));
    assert(fwd.any_matched);
    assert(str_eq(fwd.merged.find("motor_direction"), "FORWARD"));
}

// ── Regression: socket 1-gang still decodes "state" on the Raw path ───
static void test_socket_regression() {
    const auto& def = devices::livolo::kDef_TI0001_socket;
    auto on = dispatch_raw(def, magic({7, 1, 6, 1, 0x01, 11}));
    assert(on.any_matched);
    assert(b_true(on.merged.find("state")));
}

int main() {
    test_switch_4gang();
    test_pir();
    test_curtain();
    test_dimmer();
    test_cover();
    test_socket_regression();
    return 0;
}
