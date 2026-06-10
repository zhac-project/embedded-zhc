// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the TERNCY AduroSmart-cluster (0xFCCC) devices:
//   * TERNCY-PP01  awareness switch  — action + occupancy/motion-side +
//                                       temperature(scale 10) + illuminance
//   * TERNCY-SD01  knob smart dimmer — action + rotate(direction/number)
//   * TERNCY-LS01  smart light socket — on/off + single-press action
//   * TERNCY-DC01  temp & contact sensor — temperature(scale 10) + contact
//
// Bug fixed (shared terncy_raw decoder): z2m's fzLocal.terncy_raw reads
// the WHOLE raw frame buffer (msg.data, ZCL header included):
//   msg.data[4] discriminates action vs motion, msg.data[6] = action
//   value (1..4), msg.data[7] = motion-side value.
// The AduroSmart frame is manufacturer-specific, so its ZCL header is
// 5 bytes [fc, mfg_lo, mfg_hi, tsn, cmd] and ZHC's raw_body begins at
// full-frame index 5. The previous decoder re-used z2m's full-frame
// offsets against raw_body (raw_body[4]/[6]/[7]) AND required
// raw_body.size() >= 8 — so on real (~3-byte body) frames it read past
// the body and matched NOTHING. The entire action + motion surface was
// dead. Fixed to use msg.command_id as the discriminator (z2m's
// msg.data[4] IS the command id) and raw_body[1]/[2] for the values.
//
// These tests pin, at the dispatch_from_zigbee boundary, that:
//   * each single/double/triple/quadruple press emits the matching
//     `action` string (PP01, SD01, LS01),
//   * PP01's motion frame emits occupancy=true + action_side right/left,
//   * SD01's knob attribute emits rotate + direction + action_number,
//   * a frame on the wrong (server->client) direction does NOT match,
//   * the contact + scaled-temperature decoders fire (DC01),
//   * the on/off relay survives (LS01).
//
// z2m-source: zigbee-herdsman-converters/src/devices/terncy.ts
//             fzLocal.terncy_raw / fzLocal.terncy_knob / terncyExtend.contact.

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::terncy {
extern const PreparedDefinition kDef_TERNCY_PP01;
extern const PreparedDefinition kDef_TERNCY_SD01;
extern const PreparedDefinition kDef_TERNCY_LS01;
extern const PreparedDefinition kDef_TERNCY_DC01;
}  // namespace zhc::devices::terncy

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("  FAIL: %s\n", what);
        ++g_failures;
    }
}

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

DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster_id,
                        std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Manufacturer-specific raw frame on 0xFCCC:
//   [fc, mfg_lo, mfg_hi, tsn, cmd, b0, b1, b2]
// fc = 0x0D → cluster-specific (bit0) + manufacturer-specific (bit2) +
//             direction server->client (bit3) — matches the z2m wire byte
//             (`13` = 0x0D) for these device-originated frames.
std::vector<std::uint8_t> raw_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> body,
                                    std::uint8_t fc = 0x0D) {
    std::vector<std::uint8_t> v{fc, 0x28, 0x12, 0x42, cmd};
    for (auto b : body) v.push_back(b);
    return v;
}

// ZCL attribute report (fc=0x18 non-mfg, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool action_is(const DispatchResult& r, const char* expected) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

bool str_is(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

bool has_key(const DispatchResult& r, const char* key) {
    return r.merged.find(key) != nullptr;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// z2m: msg.data[6] = action value → body byte at raw_body index 1.
//      action body is [b0, ACTION] (b0 is don't-care; z2m never reads it).
std::vector<std::uint8_t> action_body(std::uint8_t value) {
    return {0x08, value};
}
// z2m: msg.data[7] = side value → body byte at raw_body index 2.
std::vector<std::uint8_t> motion_body(std::uint8_t side) {
    return {0xA7, 0x00, side};  // [b0, b1, SIDE]; only SIDE is read
}

// ── TERNCY-PP01: action (cmd 0) + motion/occupancy (cmd 4) ──────────────
void test_pp01_action() {
    const auto& def = devices::terncy::kDef_TERNCY_PP01;

    check(action_is(dispatch(def, 0xFCCC, raw_frame(0, action_body(1))), "single"),
          "PP01 single");
    check(action_is(dispatch(def, 0xFCCC, raw_frame(0, action_body(2))), "double"),
          "PP01 double");
    check(action_is(dispatch(def, 0xFCCC, raw_frame(0, action_body(3))), "triple"),
          "PP01 triple");
    check(action_is(dispatch(def, 0xFCCC, raw_frame(0, action_body(4))), "quadruple"),
          "PP01 quadruple");

    // Unrecognised action value → no match, no action key.
    const auto bad = dispatch(def, 0xFCCC, raw_frame(0, action_body(9)));
    check(!has_key(bad, "action"), "PP01 unknown action no key");
}

void test_pp01_motion() {
    const auto& def = devices::terncy::kDef_TERNCY_PP01;

    auto right = dispatch(def, 0xFCCC, raw_frame(4, motion_body(7)));
    check(str_is(right, "action_side", "right"), "PP01 motion right side");
    check(str_is(right, "side", "right"),        "PP01 legacy side alias");
    const Value* occ = right.merged.find("occupancy");
    check(occ && occ->type == ValueType::Bool && occ->b, "PP01 motion occupancy true");

    auto left = dispatch(def, 0xFCCC, raw_frame(4, motion_body(40)));
    check(str_is(left, "action_side", "left"), "PP01 motion left side");

    // side value 5 also maps to right (z2m sidelookup).
    check(str_is(dispatch(def, 0xFCCC, raw_frame(4, motion_body(5))), "action_side", "right"),
          "PP01 motion side 5 = right");
    // Unrecognised side value → no match.
    check(!has_key(dispatch(def, 0xFCCC, raw_frame(4, motion_body(99))), "occupancy"),
          "PP01 unknown side no occupancy");
}

void test_pp01_sensors() {
    const auto& def = devices::terncy::kDef_TERNCY_PP01;

    // msTemperatureMeasurement 0x0000 (s16) = 235 → 23.5 °C (scale 10).
    const std::array<std::uint8_t, 2> t = {0xEB, 0x00};  // 235
    auto tr = dispatch(def, 0x0402,
                       attr_report(0x0000, 0x29,
                                   std::span<const std::uint8_t>(t.data(), t.size())));
    const Value* tv = tr.merged.find("temperature");
    check(tv && tv->type == ValueType::Float && std::fabs(tv->f - 23.5f) < 0.01f,
          "PP01 temperature scale10");

    check(def_exposes(def, "illuminance"), "PP01 exposes illuminance");
    check(def_exposes(def, "occupancy"),   "PP01 exposes occupancy");
}

// ── TERNCY-SD01: action (cmd 0) + knob rotation (attr 0x001b) ───────────
void test_sd01_action() {
    const auto& def = devices::terncy::kDef_TERNCY_SD01;
    check(action_is(dispatch(def, 0xFCCC, raw_frame(0, action_body(1))), "single"),
          "SD01 single");
    check(action_is(dispatch(def, 0xFCCC, raw_frame(0, action_body(2))), "double"),
          "SD01 double");

    // SD01 wires action-only — a motion (cmd 4) frame must NOT emit occupancy.
    check(!has_key(dispatch(def, 0xFCCC, raw_frame(4, motion_body(7))), "occupancy"),
          "SD01 action-only: no occupancy on motion frame");
}

void test_sd01_knob() {
    const auto& def = devices::terncy::kDef_TERNCY_SD01;

    // terncyRotation 0x001b (s16) = +12 → rotate clockwise, number 1.0.
    const std::array<std::uint8_t, 2> cw = {0x0C, 0x00};  // 12
    auto r = dispatch(def, 0xFCCC,
                      attr_report(0x001b, 0x29,
                                  std::span<const std::uint8_t>(cw.data(), cw.size())));
    check(action_is(r, "rotate"),                       "SD01 knob rotate action");
    check(str_is(r, "action_direction", "clockwise"),   "SD01 knob clockwise");
    const Value* n = r.merged.find("action_number");
    check(n && n->type == ValueType::Float && std::fabs(n->f - 1.0f) < 0.01f,
          "SD01 knob number 1.0");

    // -24 (0xFFE8) → counterclockwise, number 2.0.
    const std::array<std::uint8_t, 2> ccw = {0xE8, 0xFF};  // -24
    auto r2 = dispatch(def, 0xFCCC,
                       attr_report(0x001b, 0x29,
                                   std::span<const std::uint8_t>(ccw.data(), ccw.size())));
    check(str_is(r2, "action_direction", "counterclockwise"), "SD01 knob ccw");
    const Value* n2 = r2.merged.find("action_number");
    check(n2 && n2->type == ValueType::Float && std::fabs(n2->f - 2.0f) < 0.01f,
          "SD01 knob number 2.0");
}

// ── TERNCY-LS01: on/off relay + single-press action ─────────────────────
void test_ls01() {
    const auto& def = devices::terncy::kDef_TERNCY_LS01;

    check(action_is(dispatch(def, 0xFCCC, raw_frame(0, action_body(1))), "single"),
          "LS01 single action");

    // genOnOff onOff attr 0x0000 (bool) = 1 → state on.
    const std::array<std::uint8_t, 1> on = {0x01};
    auto r = dispatch(def, 0x0006,
                      attr_report(0x0000, 0x10,
                                  std::span<const std::uint8_t>(on.data(), on.size())));
    check(has_key(r, "state"), "LS01 onOff decodes state");
    check(def_exposes(def, "state"), "LS01 exposes state");
}

// ── TERNCY-DC01: contact (genBinaryInput) + scaled temperature ──────────
void test_dc01() {
    const auto& def = devices::terncy::kDef_TERNCY_DC01;

    // genBinaryInput presentValue 0x0055 (bool) = 1 → contact true (closed).
    const std::array<std::uint8_t, 1> closed = {0x01};
    auto rc = dispatch(def, 0x000F,
                       attr_report(0x0055, 0x10,
                                   std::span<const std::uint8_t>(closed.data(), closed.size())));
    const Value* cv = rc.merged.find("contact");
    check(cv && cv->type == ValueType::Bool && cv->b, "DC01 contact closed");

    const std::array<std::uint8_t, 1> open = {0x00};
    auto ro = dispatch(def, 0x000F,
                       attr_report(0x0055, 0x10,
                                   std::span<const std::uint8_t>(open.data(), open.size())));
    const Value* ov = ro.merged.find("contact");
    check(ov && ov->type == ValueType::Bool && !ov->b, "DC01 contact open");

    // msTemperatureMeasurement 0x0000 (s16) = 218 → 21.8 °C (scale 10).
    const std::array<std::uint8_t, 2> t = {0xDA, 0x00};  // 218
    auto rt = dispatch(def, 0x0402,
                       attr_report(0x0000, 0x29,
                                   std::span<const std::uint8_t>(t.data(), t.size())));
    const Value* tv = rt.merged.find("temperature");
    check(tv && tv->type == ValueType::Float && std::fabs(tv->f - 21.8f) < 0.01f,
          "DC01 temperature scale10");
}

// ── Direction guard: a client->server raw frame must NOT match ──────────
void test_direction_guard() {
    const auto& def = devices::terncy::kDef_TERNCY_PP01;
    // fc 0x05 = cluster-specific (bit0) + mfg-specific (bit2), bit3 CLEAR →
    //           client->server, the wrong direction for the action converter.
    auto r = dispatch(def, 0xFCCC, raw_frame(0, action_body(1), /*fc=*/0x05));
    check(!has_key(r, "action"), "PP01 wrong-direction raw frame no action");
}

}  // namespace

int main() {
    std::printf("test_terncy_parity\n");
    test_pp01_action();
    test_pp01_motion();
    test_pp01_sensors();
    test_sd01_action();
    test_sd01_knob();
    test_ls01();
    test_dc01();
    test_direction_guard();
    if (g_failures == 0) {
        std::printf("  OK\n");
        return 0;
    }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
