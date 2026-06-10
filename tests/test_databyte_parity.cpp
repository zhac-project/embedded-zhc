// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Databyte (databyte.ch) device family.
// z2m-source: zigbee-herdsman-converters/src/devices/databyte.ts
//             + converters/fromZigbee.ts (fz.DTB190502A1).
//
// Bugs fixed (each device graduated generated/ -> Tier-2 parent):
//
//  * Touch4 (DTB-ED2011-014): z2m wires fzLocal.DTB2011014 + fz.battery
//    and exposes key_1..key_4 (binary ON/OFF). The auto-port dropped the
//    custom decoder (battery-only), so the four touch keys never
//    surfaced. The decoder reads genOnOff manufacturer attributes
//    0xA191..0xA194 (decimal 41361..41364) -> key_1..key_4. (The
//    "4-gang endpoint_map / state_lN" suspicion was a false flag: z2m
//    uses four manufacturer attributes on ONE endpoint, not per-endpoint
//    genOnOff state.)
//
//  * DTB190502A1: z2m wires fz.DTB190502A1 and exposes led_state (binary,
//    read-only) + key_state (enum). The auto-port fabricated a generic
//    controllable `state` + a phantom battery/voltage bundle (this mains
//    IO board has no battery) and dropped the real decoder. Now wires the
//    vendor decoder (0xA191 cpu_temperature / 0xA192 key_state / 0xA193
//    led_state) and carries z2m's exposes.
//
// These tests pin the corrected wire-shape decode.

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

namespace zhc::devices::databyte {
extern const PreparedDefinition kDef_DTB190502A1;
extern const PreparedDefinition kDef_Touch4;
extern const PreparedDefinition kDef_ED2004_012;
}  // namespace zhc::devices::databyte

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

bool is_str(const Value* v, const char* expect) {
    return v && v->type == ValueType::StringRef && v->str
           && std::strcmp(v->str, expect) == 0;
}

bool f_near(const Value* v, float expect) {
    if (!v || v->type != ValueType::Float) return false;
    const float d = v->f - expect;
    return (d < 0.01f) && (d > -0.01f);
}

// ── ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then records ─
// (Manufacturer-specific bit is irrelevant to the decoder — it keys each
//  attribute by its decimal id regardless, which is what these vendor
//  converters read.)
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// Multi-record attribute report (concatenate several attr records).
std::vector<std::uint8_t> attr_report_multi(
        std::span<const std::tuple<std::uint16_t, std::uint8_t,
                                   std::vector<std::uint8_t>>> recs) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    for (const auto& [attr, type, val] : recs) {
        v.push_back(static_cast<std::uint8_t>(attr & 0xFF));
        v.push_back(static_cast<std::uint8_t>(attr >> 8));
        v.push_back(type);
        for (auto b : val) v.push_back(b);
    }
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

}  // namespace

// ── Touch4 — key_1..key_4 restored ───────────────────────────────────
static void test_touch4_keys() {
    const auto& def = devices::databyte::kDef_Touch4;
    assert(def_exposes(def, "key_1"));
    assert(def_exposes(def, "key_2"));
    assert(def_exposes(def, "key_3"));
    assert(def_exposes(def, "key_4"));
    assert(def_exposes(def, "battery"));

    // Attr 0xA191=ON(1), 0xA192=OFF(0), 0xA193=ON(1), 0xA194=OFF(0).
    // u8 (type 0x20).
    using Rec = std::tuple<std::uint16_t, std::uint8_t, std::vector<std::uint8_t>>;
    std::array<Rec, 4> recs = {
        Rec{0xA191, 0x20, {0x01}},
        Rec{0xA192, 0x20, {0x00}},
        Rec{0xA193, 0x20, {0x01}},
        Rec{0xA194, 0x20, {0x00}},
    };
    auto r = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                          attr_report_multi(std::span<const Rec>(recs.data(), recs.size())));
    assert(r.any_matched);
    assert(is_str(r.merged.find("key_1"), "ON"));
    assert(is_str(r.merged.find("key_2"), "OFF"));
    assert(is_str(r.merged.find("key_3"), "ON"));
    assert(is_str(r.merged.find("key_4"), "OFF"));
    // No phantom bare on/off "state".
    assert(r.merged.find("state") == nullptr);
}

// Touch4 battery still decodes via the generic converter.
static void test_touch4_battery() {
    const auto& def = devices::databyte::kDef_Touch4;
    assert(def_binds(def, 0x0001));
    const std::uint8_t val[] = {0xC8};   // 0xC8 = 200 half-percent -> 100 %
    auto r = dispatch_zcl(def, 0x0001, "genPowerCfg", 1,
                          attr_report(0x0021, 0x20, val));
    assert(r.any_matched);
    const Value* b = r.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);
}

// ── DTB190502A1 — led_state + key_state + cpu_temperature ────────────
static void test_dtb190502a1() {
    const auto& def = devices::databyte::kDef_DTB190502A1;
    // z2m schema: led_state + key_state, NOT a controllable state or battery.
    assert(def_exposes(def, "led_state"));
    assert(def_exposes(def, "key_state"));
    assert(!def_exposes(def, "state"));
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));
    assert(def.to_zigbee_count == 0);     // no fabricated on/off TZ
    assert(!def_binds(def, 0x0001));      // no phantom battery binding

    // Attr 0xA191 cpu_temperature (s16=42 -> 42.0), 0xA192 key_state=1
    // (KEY_UP), 0xA193 led_state=1 (ON).
    using Rec = std::tuple<std::uint16_t, std::uint8_t, std::vector<std::uint8_t>>;
    std::array<Rec, 3> recs = {
        Rec{0xA191, 0x29, {0x2A, 0x00}},   // s16 0x002A = 42
        Rec{0xA192, 0x20, {0x01}},         // u8 = 1 -> KEY_UP
        Rec{0xA193, 0x20, {0x01}},         // u8 = 1 -> ON
    };
    auto r = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                          attr_report_multi(std::span<const Rec>(recs.data(), recs.size())));
    assert(r.any_matched);
    assert(f_near(r.merged.find("cpu_temperature"), 42.0f));
    assert(is_str(r.merged.find("key_state"), "KEY_UP"));
    assert(is_str(r.merged.find("led_state"), "ON"));

    // led_state OFF path.
    std::array<Rec, 1> off = { Rec{0xA193, 0x20, {0x00}} };
    auto r2 = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                           attr_report_multi(std::span<const Rec>(off.data(), off.size())));
    assert(r2.any_matched);
    assert(is_str(r2.merged.find("led_state"), "OFF"));
}

// ── ED2004-012 — plain on/off (false-by-category, must stay correct) ─
static void test_ed2004_onoff() {
    const auto& def = devices::databyte::kDef_ED2004_012;
    assert(def_exposes(def, "state"));
    assert(def.to_zigbee_count == 1);    // controllable on/off
    const std::uint8_t val[] = {0x01};
    auto r = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                          attr_report(0x0000, 0x10, val));   // onOff bool=1
    assert(r.any_matched);
    const Value* s = r.merged.find("state");
    assert(s && s->type == ValueType::Bool && s->b);
}

int main() {
    test_touch4_keys();
    test_touch4_battery();
    test_dtb190502a1();
    test_ed2004_onoff();
    return 0;
}
