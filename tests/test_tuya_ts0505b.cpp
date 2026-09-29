// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Tuya TS0505B (z2m TS0505B_1, the zigbeeModel-wide RGB+CCT
// light). z2m v26.105.0: tuyaLight({colorTemp: {range: [153, 500]}, color:
// true}) → light with color_temp and color_xy, Tuya 0xF000 brightness reports,
// effect, do_not_disturb, color_power_on_behavior; no power_on_behavior. For
// six manufacturers z2m sets moveToLevelWithOnOffDisable, so brightness goes
// out as moveToLevel (0x00) instead of moveToLevelWithOnOff (0x04).
// The bulb could not be given a colour at all before: no colour converter,
// no colour expose. It then never reported `color_mode` (colorMode is not
// among its configured reports, as in z2m), so consumers could not tell xy,
// hs and colour-temperature mode apart.
//
// z2m-source: tuya.ts #TS0505B_1, lib/tuya.ts tuyaLight / tuyaTz.do_not_disturb
//             / tuyaTz.color_power_on_behavior, converters/toZigbee.ts tz.effect
//             / tz.light_color / tz.light_colortemp, converters/fromZigbee.ts
//             fz.color_colortemp, lib/color.ts syncColorState.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS0505B;
extern const PreparedDefinition kDefTS0505B_moveToLevel;
}  // namespace zhc::devices::tuya

using namespace zhc;
using devices::tuya::kDefTS0505B;
using devices::tuya::kDefTS0505B_moveToLevel;
using Bytes = std::vector<std::uint8_t>;

namespace {

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (std::strcmp(def.exposes[i].name, key) == 0) return &def.exposes[i];
    return nullptr;
}

// One light's runtime memory, kept across frames as the hub adapter keeps it
// (each frame still gets a fresh context, as in the adapter).
RuntimeStore<1> g_mem;
RuntimeContext new_ctx(bool mem) {
    RuntimeContext ctx{};
    if (mem) { ctx.store = &g_mem; ctx.store_get = &RuntimeStore<1>::get; }
    return ctx;
}

// ZCL global command on `cluster` (0x0A attribute report, 0x01 read
// attributes response) → the def's merged output.
DispatchResult report(const PreparedDefinition& def, std::uint16_t cluster, const Bytes& records,
                      bool mem = false, std::uint8_t cmd = 0x0A) {
    Bytes f = {0x18, 0x42, cmd};
    f.insert(f.end(), records.begin(), records.end());
    InboundApsFrame raw{};
    raw.cluster_id = cluster;
    raw.src_endpoint = raw.dst_endpoint = 1;
    raw.data = f;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster);
    RuntimeContext ctx = new_ctx(mem);
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

struct Sent { std::uint16_t cluster; Bytes frame; };
Sent send(const PreparedDefinition& def, const char* key, const Value& v, bool mem = false) {
    RuntimeContext ctx = new_ctx(mem);
    std::uint8_t frame[64]{};
    const auto r = dispatch_to_zigbee(def, key, v, ctx, frame);
    if (!r.ok) return {0, {}};
    return {r.cluster_id, Bytes(frame, frame + r.frame_size)};
}

Value str_v(const char* s) { Value v{}; v.type = ValueType::StringRef; v.str = s; return v; }
Value bool_v(bool b)       { Value v{}; v.type = ValueType::Bool; v.b = b; return v; }
Value uint_v(std::uint64_t u) { Value v{}; v.type = ValueType::Uint; v.u = u; return v; }
bool is_str(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}
const char* mode_of(const DispatchResult& r) {
    const Value* v = r.merged.find("color_mode");
    return v && v->type == ValueType::StringRef ? v->str : nullptr;
}

// color_mode, as z2m's fz.color_colortemp: lightingColorCtrl colorMode (0x0008,
// enum8) 0 → hs, 1 → xy, 2 → color_temp, from a report or a read response. A
// colour frame without colorMode carries the light's last known mode (z2m keeps
// it in state, syncColorState): the last colorMode it sent, or the mode of the
// last colour command — tz.light_color / tz.light_colortemp return color_mode
// as state. Nothing is guessed while no mode is known.
void check_color_mode(const PreparedDefinition& def) {
    const char* const names[] = {"hs", "xy", "color_temp"};
    for (std::uint8_t m = 0; m < 3; ++m)   // report: colorMode enum8 = m
        assert(is_str(report(def, 0x0300, {0x08, 0x00, 0x30, m}).merged.find("color_mode"), names[m]));
    // Read Attributes Response: currentX (u16, status 0) + colorMode = 1.
    const auto rr = report(def, 0x0300, {0x03, 0x00, 0x00, 0x21, 0x33, 0x53,
                                         0x08, 0x00, 0x00, 0x30, 0x01}, false, 0x01);
    assert(is_str(rr.merged.find("color_mode"), "xy") && rr.merged.find("color_x"));

    const Bytes ct300 = {0x07, 0x00, 0x21, 0x2C, 0x01};                  // colorTemperature 300
    const Bytes x_only = {0x03, 0x00, 0x21, 0x33, 0x53};                 // currentX
    g_mem = RuntimeStore<1>{};
    // No colorMode and no mode known yet: nothing guessed from the attributes.
    assert(!mode_of(report(def, 0x0300, ct300, true)));
    assert(!mode_of(report(def, 0x0300, x_only, true)));
    // The mode the light reported stays with its later colour reports …
    report(def, 0x0300, {0x08, 0x00, 0x30, 0x00}, true);                 // hs
    assert(is_str(report(def, 0x0300, x_only, true).merged.find("color_mode"), "hs"));
    assert(is_str(report(def, 0x0300, ct300, true).merged.find("color_mode"), "hs"));
    // … until a colour command: the next report carries the command's mode.
    assert(send(def, "color_temp", uint_v(300), true).cluster == 0x0300);
    assert(is_str(report(def, 0x0300, ct300, true).merged.find("color_mode"), "color_temp"));
    assert(is_str(report(def, 0x0300, x_only, true).merged.find("color_mode"), "color_temp"));
    assert(send(def, "color_xy", str_v("0.3,0.4"), true).cluster == 0x0300);
    assert(is_str(report(def, 0x0300, x_only, true).merged.find("color_mode"), "xy"));
    assert(send(def, "color_hs", str_v("120,50"), true).cluster == 0x0300);
    assert(is_str(report(def, 0x0300, {0x00, 0x00, 0x20, 0x55}, true).merged.find("color_mode"), "hs"));
    assert(send(def, "color_x", str_v("0.3,0.4"), true).frame.empty());   // refused: nothing sent …
    assert(is_str(report(def, 0x0300, x_only, true).merged.find("color_mode"), "hs"));   // … mode kept
    // A colorMode from the light wins over the command's.
    assert(send(def, "color_temp", uint_v(250), true).cluster == 0x0300);
    const Bytes ct_and_mode = {0x07, 0x00, 0x21, 0x2C, 0x01, 0x08, 0x00, 0x30, 0x01};
    assert(is_str(report(def, 0x0300, ct_and_mode, true).merged.find("color_mode"), "xy"));

    // Exposed read-only, with the three values it takes.
    const Expose* cm = find_expose(def, "color_mode");
    assert(cm && cm->type == ExposeType::Enum && cm->access == Access::State && cm->enum_count == 3);
    for (std::uint8_t m = 0; m < 3; ++m) assert(std::strcmp(cm->enum_values[m], names[m]) == 0);
}

void check_def(const PreparedDefinition& def, std::uint8_t brightness_cmd) {
    // Exposes: the light, colour, and tuyaLight's three extras; no power_on_behavior.
    for (const char* k : {"state", "brightness", "color_temp", "color_x", "color_y"})
        assert(find_expose(def, k) && find_expose(def, k)->access == Access::StateSet);
    const Expose* ct = find_expose(def, "color_temp");
    assert(ct->value_min == 153 && ct->value_max == 500);
    const Expose* fx = find_expose(def, "effect");
    assert(fx && fx->type == ExposeType::Enum && fx->access == Access::Set && fx->enum_count == 8);
    assert(std::strcmp(fx->enum_values[6], "colorloop") == 0 && std::strcmp(fx->enum_values[7], "stop_colorloop") == 0);
    const Expose* dnd = find_expose(def, "do_not_disturb");
    assert(dnd && dnd->type == ExposeType::Binary && dnd->category == ExposeCategory::Config);
    const Expose* cpob = find_expose(def, "color_power_on_behavior");
    assert(cpob && cpob->type == ExposeType::Enum && cpob->enum_count == 3 &&
           cpob->category == ExposeCategory::Config);
    assert(!find_expose(def, "power_on_behavior"));

    // Colour decodes (currentX 0x0003 / currentY 0x0004, u16) …
    const auto rc = report(def, 0x0300, {0x03, 0x00, 0x21, 0x33, 0x53,     // x = 0x5333
                                         0x04, 0x00, 0x21, 0x66, 0x66});   // y = 0x6666
    const Value* x = rc.merged.find("color_x");
    const Value* y = rc.merged.find("color_y");
    assert(x && y && std::fabs(x->f - 0x5333 / 65535.0f) < 1e-4f && std::fabs(y->f - 0.4f) < 1e-4f);
    // … and Tuya's 0xF000 brightness (0-1000) lands on the 0-255 scale.
    const auto rb = report(def, 0x0008, {0x00, 0xF0, 0x21, 0xF4, 0x01});  // 500
    const Value* br = rb.merged.find("brightness");
    assert(br && br->type == ValueType::Uint && br->u == 128);

    // Colour write: moveToColor.
    const Sent c = send(def, "color_xy", str_v("0.3,0.4"));
    assert(c.cluster == 0x0300 && c.frame.size() == 9 && c.frame[2] == 0x07);

    // Brightness: moveToLevelWithOnOff, or moveToLevel for the six.
    const Sent b = send(def, "brightness", uint_v(100));
    assert(b.cluster == 0x0008 && b.frame == (Bytes{0x11, 0x00, brightness_cmd, 100, 0x00, 0x00}));

    // effect: identify effects on genIdentify triggerEffect …
    const Sent blink = send(def, "effect", str_v("blink"));
    assert(blink.cluster == 0x0003 && blink.frame == (Bytes{0x11, 0x00, 0x40, 0x00, 0x00}));
    const Sent stop = send(def, "effect", str_v("stop_effect"));
    assert(stop.cluster == 0x0003 && stop.frame == (Bytes{0x11, 0x00, 0x40, 0xFF, 0x00}));
    // … colour loop as z2m's hue_move: moveHue up at 255/15 = 17, stop = moveHue stop rate 1.
    const Sent loop = send(def, "effect", str_v("colorloop"));
    assert(loop.cluster == 0x0300 && loop.frame == (Bytes{0x11, 0x00, 0x01, 0x01, 17, 0x00, 0x00}));
    const Sent unloop = send(def, "effect", str_v("stop_colorloop"));
    assert(unloop.cluster == 0x0300 && unloop.frame == (Bytes{0x11, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00}));
    assert(send(def, "effect", str_v("disco")).frame.empty());

    // do_not_disturb → tuyaDoNotDisturb (0x0300 cmd 0xFA); color_power_on_behavior → tuyaOnStartUp (0xF9).
    const Sent d = send(def, "do_not_disturb", bool_v(true));
    assert(d.cluster == 0x0300 && d.frame == (Bytes{0x11, 0x00, 0xFA, 0x01}));
    const Sent p = send(def, "color_power_on_behavior", str_v("previous"));
    assert(p.cluster == 0x0300 && p.frame.size() == 15 && p.frame[2] == 0xF9 && p.frame[3] == 0x00 && p.frame[4] == 0x01);
}

}  // namespace

int main() {
    check_def(kDefTS0505B, 0x04);
    check_def(kDefTS0505B_moveToLevel, 0x00);
    check_color_mode(kDefTS0505B);
    check_color_mode(kDefTS0505B_moveToLevel);

    // The six moveToLevelWithOnOffDisable manufacturers (z2m v26.105.0) get the
    // moveToLevel definition; every other TS0505B keeps the generic one.
    const std::span<const PreparedDefinition* const> reg(devices::tuya::kTuyaRegistry,
                                                         devices::tuya::kTuyaRegistryCount);
    for (const char* m : {"_TZB210_uoiqhjqe", "_TZB210_417ikxay", "_TZB210_qzsxaqqe",
                          "_TZB210_u3ri0968", "_TZB210_rs0ufzwg", "_TZ3210_mja6r5ix"})
        assert(find_definition("TS0505B", m, reg) == &kDefTS0505B_moveToLevel);
    for (const char* m : {"_TZ3210_r0xgkft5", "_TZ3000_qd7hej8u", "_TZ3210_sln7ah6r"})
        assert(find_definition("TS0505B", m, reg) == &kDefTS0505B);

    // z2m's TS0505B_1 white labels (v26.105.0 additions included).
    auto has_wl = [](const char* vendor, const char* model) {
        for (std::size_t i = 0; i < kDefTS0505B.white_labels_count; ++i)
            if (std::strcmp(kDefTS0505B.white_labels[i].vendor, vendor) == 0 &&
                std::strcmp(kDefTS0505B.white_labels[i].model, model) == 0) return true;
        return false;
    };
    assert(has_wl("Lidl", "HG08008") && has_wl("Hatsy", "SDL-312Z") && has_wl("Nous", "P8Z") &&
           has_wl("Emos", "GoSmart ZQZ516R") && has_wl("MiBoxer", "FUT037Z+"));
    return 0;
}
