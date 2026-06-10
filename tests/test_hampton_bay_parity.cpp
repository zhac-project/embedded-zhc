// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the two Hampton Bay definitions.
// z2m-source: hampton_bay.ts.
//
// #99432 — Universal ceiling fan premier remote (fromZigbee:[fz.fan]).
//   z2m's fz.fan decodes hvacFanCtrl attr 0x0000 (fanMode, enum8) and
//   publishes BOTH `fan_mode` (enum key) AND `fan_state` ("OFF"/"ON"). The
//   generated port wired the generic kFzFanMode (emits ONLY fan_mode) and
//   declared fan_mode as a bare Binary with NO fan_state expose, so the
//   derived on/off state was DEAD. Fixed with an inlined vendor converter
//   (kFzHamptonFan) that emits fan_mode (u8) + fan_state (Bool: off/0 ->
//   false) and Enum/Binary exposes.
//
// #54668161 — 12 in. LED smart puff (extend:[m.light({colorTemp})]).
//   The auto-port dropped the color_temp axis; added kFzColorTemperature
//   (lightingColorCtrl 0x0007 -> color_temp, mireds) + expose.

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::hampton_bay {
extern const PreparedDefinition kDef_D99432;     // ceiling fan + light
extern const PreparedDefinition kDef_D54668161;  // tunable-white puff light
}  // namespace zhc::devices::hampton_bay

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

bool u_eq(const Value* v, std::uint64_t want) {
    return v && v->type == ValueType::Uint && v->u == want;
}
bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return &def.exposes[i];
    return nullptr;
}

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) { std::printf("  FAIL: %s\n", what); ++g_failures; }
}

// hvacFanCtrl (0x0202) Report-Attributes for attr 0x0000 (fanMode, enum8 0x30).
DispatchResult dispatch_fan(const PreparedDefinition& def, std::uint8_t fan_mode) {
    const std::uint8_t report[] = {
        0x18, 0x42, 0x0A,   // fc, tsn, cmd=Report Attributes
        0x00, 0x00,         // attr 0x0000
        0x30,               // type enum8
        fan_mode,
    };
    auto raw = make_frame(0x0202, 1, report);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacFanCtrl";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// genOnOff (0x0006) Report-Attributes for attr 0x0000 (onOff, bool).
DispatchResult dispatch_onoff(const PreparedDefinition& def, bool on) {
    const std::uint8_t report[] = {
        0x18, 0x42, 0x0A,
        0x00, 0x00,
        0x10,               // type bool
        static_cast<std::uint8_t>(on ? 0x01 : 0x00),
    };
    auto raw = make_frame(0x0006, 1, report);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// lightingColorCtrl (0x0300) Report-Attributes for attr 0x0007
// (colorTemperature, u16 0x21).
DispatchResult dispatch_color_temp(const PreparedDefinition& def, std::uint16_t mireds) {
    const std::uint8_t report[] = {
        0x18, 0x42, 0x0A,
        0x07, 0x00,         // attr 0x0007
        0x21,               // type u16
        static_cast<std::uint8_t>(mireds & 0xFF),
        static_cast<std::uint8_t>(mireds >> 8),
    };
    auto raw = make_frame(0x0300, 1, report);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "lightingColorCtrl";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ── #99432: fan_mode + fan_state both decode from one hvacFanCtrl report ──
void test_fan_decode() {
    const auto& def = devices::hampton_bay::kDef_D99432;

    // off (0): fan_mode==0, fan_state==false
    auto off = dispatch_fan(def, 0);
    check(off.any_matched,                        "fan off matched");
    check(u_eq(off.merged.find("fan_mode"), 0),   "fan off fan_mode==0");
    check(b_false(off.merged.find("fan_state")),  "fan off fan_state==false");

    // medium (2): fan_mode==2, fan_state==true
    auto med = dispatch_fan(def, 2);
    check(med.any_matched,                        "fan medium matched");
    check(u_eq(med.merged.find("fan_mode"), 2),   "fan medium fan_mode==2");
    check(b_true(med.merged.find("fan_state")),   "fan medium fan_state==true");

    // smart (6): non-contiguous high mode, fan_state still true
    auto smart = dispatch_fan(def, 6);
    check(u_eq(smart.merged.find("fan_mode"), 6), "fan smart fan_mode==6");
    check(b_true(smart.merged.find("fan_state")), "fan smart fan_state==true");
}

// ── #99432: the on/off light relay still decodes; fan keys do not leak ──
void test_fan_onoff_still_works() {
    const auto& def = devices::hampton_bay::kDef_D99432;
    auto on = dispatch_onoff(def, true);
    check(on.any_matched,                         "fan onoff matched");
    check(b_true(on.merged.find("state")),        "fan onoff state==true");
    check(on.merged.find("fan_mode")  == nullptr, "fan onoff no fan_mode leak");
    check(on.merged.find("fan_state") == nullptr, "fan onoff no fan_state leak");
}

// ── #99432: exposes — fan_mode Enum + fan_state Binary (dead-key guard) ──
void test_fan_exposes() {
    const auto& def = devices::hampton_bay::kDef_D99432;
    const Expose* mode  = find_expose(def, "fan_mode");
    const Expose* state = find_expose(def, "fan_state");
    check(find_expose(def, "state") != nullptr,   "fan exposes state");
    check(mode  != nullptr,                       "fan exposes fan_mode");
    check(state != nullptr,                       "fan exposes fan_state");
    check(mode  && mode->type == ExposeType::Enum,    "fan_mode is Enum");
    check(mode  && mode->enum_count == 5,             "fan_mode has 5 modes");
    check(state && state->type == ExposeType::Binary, "fan_state is Binary");
}

// ── #54668161: color_temp decodes + is declared (CT axis restored) ──
void test_light_color_temp() {
    const auto& def = devices::hampton_bay::kDef_D54668161;
    auto ct = dispatch_color_temp(def, 370);
    check(ct.any_matched,                          "light CT matched");
    check(u_eq(ct.merged.find("color_temp"), 370), "light color_temp==370");

    const Expose* ce = find_expose(def, "color_temp");
    check(find_expose(def, "state")      != nullptr, "light exposes state");
    check(find_expose(def, "brightness") != nullptr, "light exposes brightness");
    check(ce != nullptr,                             "light exposes color_temp");
    check(ce && ce->type == ExposeType::Numeric,     "color_temp is Numeric");
}

}  // namespace

int main() {
    std::printf("test_hampton_bay_parity\n");
    test_fan_decode();
    test_fan_onoff_still_works();
    test_fan_exposes();
    test_light_color_temp();
    if (g_failures == 0) { std::printf("  OK\n"); return 0; }
    std::printf("  %d failure(s)\n", g_failures);
    return 1;
}
