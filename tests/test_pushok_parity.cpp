// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for PushOk Hardware devices.
// z2m-source: pushok.ts.
//
// Bugs fixed (all were headline channels silently dropped by the generated
// Tier-1 ports):
//
//   * POK003 / POK010 (water level sensors) expose `contact` from z2m's
//     m.binary({cluster:"genBinaryInput", attribute:"presentValue", ...})
//     but wired NO decoder for it. Added vendor kFzPushokContact
//     (genBinaryInput 0x000F presentValue 0x0055 → bool).
//   * POK008 (thermostat relay) exposes+decodes `temperature` in z2m
//     (m.temperature on msTemperatureMeasurement) but the port dropped both
//     the expose and the decoder. Wired generic kFzTemperature + expose.
//   * POK016 (window opener) is an m.windowCovering cover with `position`
//     on closuresWindowCovering(0x0102) — a prior port misclassified it as
//     an m.onOff switch, dropping position entirely. Fixed to a cover.
//   * POK017 (greenhouse vent) is the same m.windowCovering cover — a prior
//     port removed the cover ENTIRELY (battery-only). Restored the cover.
//
// These tests pin, on real attribute-report wire shapes, that each restored
// channel decodes to the z2m-equivalent value, and regression-check the
// sibling channels (temperature/humidity/battery) still decode alongside.

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::pushok {
extern const PreparedDefinition kDef_POK003;
extern const PreparedDefinition kDef_POK008;
extern const PreparedDefinition kDef_POK010;
extern const PreparedDefinition kDef_POK016;
extern const PreparedDefinition kDef_POK017;
}  // namespace zhc::devices::pushok

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

bool near(float a, float b) { return std::fabs(a - b) < 0.01f; }

// Profile-wide attribute report (S->C):
//   fc(1)=0x18 | tsn(1) | cmd(1)=0x0A | attrId(2,LE) | type(1) | value(LE).

// uint16 measured value at attr 0x0000 (temperature/humidity/illuminance).
DispatchResult report_u16_attr0(const PreparedDefinition& def,
                                 std::uint16_t cluster, std::uint16_t value) {
    const std::array<std::uint8_t, 8> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 measuredValue
        0x21,                  // type 0x21 = uint16
        static_cast<std::uint8_t>(value & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF)};
    auto raw = make_frame(cluster,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// signed-int16 measured value at attr 0x0000 (sub-zero temperature).
DispatchResult report_s16_attr0(const PreparedDefinition& def,
                                 std::uint16_t cluster, std::int16_t value) {
    const std::array<std::uint8_t, 8> rep = {
        0x18, 0x01, 0x0A,
        0x00, 0x00,            // attr 0x0000 measuredValue
        0x29,                  // type 0x29 = int16
        static_cast<std::uint8_t>(static_cast<std::uint16_t>(value) & 0xFF),
        static_cast<std::uint8_t>((static_cast<std::uint16_t>(value) >> 8) & 0xFF)};
    auto raw = make_frame(cluster,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// genBinaryInput presentValue (attr 0x0055) as a bool (type 0x10).
DispatchResult report_binary_input(const PreparedDefinition& def,
                                    std::uint8_t value) {
    const std::array<std::uint8_t, 7> rep = {
        0x18, 0x01, 0x0A,
        0x55, 0x00,            // attr 0x0055 presentValue
        0x10,                  // type 0x10 = boolean
        value};
    auto raw = make_frame(0x000F /* genBinaryInput */,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// closuresWindowCovering currentPositionLiftPercentage (attr 0x0008, u8).
DispatchResult report_cover_position(const PreparedDefinition& def,
                                     std::uint8_t pct) {
    const std::array<std::uint8_t, 7> rep = {
        0x18, 0x01, 0x0A,
        0x08, 0x00,            // attr 0x0008 currentPositionLiftPercentage
        0x20,                  // type 0x20 = uint8
        pct};
    auto raw = make_frame(0x0102 /* closuresWindowCovering */,
                          std::span<const std::uint8_t>(rep.data(), rep.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// --- contact decode (was dropped on POK003 + POK010) ---------------------
void assert_contact(const PreparedDefinition& def) {
    // presentValue 0x01 → contact closed (true)
    {
        auto r = report_binary_input(def, 0x01);
        assert(r.any_matched);
        const Value* c = r.merged.find("contact");
        assert(c && c->type == ValueType::Bool && c->b == true);
    }
    // presentValue 0x00 → contact open (false)
    {
        auto r = report_binary_input(def, 0x00);
        assert(r.any_matched);
        const Value* c = r.merged.find("contact");
        assert(c && c->type == ValueType::Bool && c->b == false);
    }
}

// --- cover position decode (POK016 + POK017) -----------------------------
void assert_cover_position(const PreparedDefinition& def) {
    auto r = report_cover_position(def, 60);
    assert(r.any_matched);
    const Value* p = r.merged.find("position");
    assert(p && p->type == ValueType::Uint && p->u == 60);
}

}  // namespace

int main() {
    using namespace zhc::devices::pushok;

    // ---- POK003: contact (genBinaryInput) restored; temperature regress ----
    assert_contact(kDef_POK003);
    {
        auto r = report_s16_attr0(kDef_POK003, 0x0402 /* msTemperature */, 2150);
        assert(r.any_matched);
        const Value* t = r.merged.find("temperature");
        assert(t && t->type == ValueType::Float && near(t->f, 21.50f));
    }

    // ---- POK010: contact restored; temperature + humidity regress ----
    assert_contact(kDef_POK010);
    {
        auto r = report_s16_attr0(kDef_POK010, 0x0402, -550);  // -5.5 C
        assert(r.any_matched);
        const Value* t = r.merged.find("temperature");
        assert(t && t->type == ValueType::Float && near(t->f, -5.50f));
    }
    {
        auto r = report_u16_attr0(kDef_POK010, 0x0405 /* msRelativeHumidity */, 6789);
        assert(r.any_matched);
        const Value* h = r.merged.find("humidity");
        assert(h && h->type == ValueType::Float && near(h->f, 67.89f));
    }

    // ---- POK008: temperature (msTemperatureMeasurement) restored ----
    {
        auto r = report_s16_attr0(kDef_POK008, 0x0402, 2375);  // 23.75 C
        assert(r.any_matched);
        const Value* t = r.merged.find("temperature");
        assert(t && t->type == ValueType::Float && near(t->f, 23.75f));
    }

    // ---- POK016 + POK017: cover position (closuresWindowCovering) ----
    assert_cover_position(kDef_POK016);
    assert_cover_position(kDef_POK017);

    std::printf("pushok parity tests passed\n");
    return 0;
}
