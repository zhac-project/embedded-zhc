// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Colour: the decoder remembers each axis so a one-attribute report still
// yields the `color_xy` / `color_hs` pairs, and the encoder writes both axes
// (the other one from memory) instead of resetting it to centre. The colour
// mode is remembered the same way.
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <span>

#include "definitions/_generic/_shared.hpp"
#include "zhc/color_hex.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"

using namespace zhc;

static RuntimeStore<2> g_store;
static RuntimeContext ctx_with_store() {
    RuntimeContext c{};
    c.store = &g_store;
    c.store_get = &RuntimeStore<2>::get;
    c.device_index = 0;
    return c;
}
// A light the hub has not heard from: device 1, colour memory empty. Device 0
// carries one memory through the whole of main() and is in hs mode by the end,
// which would decide the colour of any later report.
static RuntimeContext ctx_fresh_light() {
    RuntimeContext c = ctx_with_store();
    c.device_index = 1;
    c.device_state()->scratch.fill(0);
    return c;
}
static Value uintv(std::uint64_t u) { Value v{}; v.type = ValueType::Uint; v.u = u; return v; }
static Value floatv(float f)        { Value v{}; v.type = ValueType::Float; v.f = f; return v; }
static Value strv(const char* s)    { Value v{}; v.type = ValueType::StringRef; v.str = s; return v; }
static bool str_is(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}
static std::uint16_t le16(const std::uint8_t* p) { return static_cast<std::uint16_t>(p[0] | (p[1] << 8)); }

int main() {
    const PreparedDefinition def{};
    std::uint8_t frame[16];
    std::size_t n = 0;

    // ── decoder ──
    {   // x and y in one report: single axes as floats plus the pair
        RuntimeContext ctx = ctx_with_store();
        DecodedMessage msg{};
        msg.payload.put("3", uintv(20493));
        msg.payload.put("4", uintv(21563));
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));
        const Value* x = out.find("color_x");
        assert(x && x->type == ValueType::Float && x->f > 0.3126f && x->f < 0.3128f);
        assert(str_is(out.find("color_xy"), "0.3127,0.3290"));
    }
    {   // y alone later: the pair is completed from the remembered x
        RuntimeContext ctx = ctx_with_store();
        DecodedMessage msg{};
        msg.payload.put("4", uintv(32768));
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));
        assert(str_is(out.find("color_xy"), "0.3127,0.5000"));
        assert(out.find("color_x") == nullptr);   // only what the report carried
    }
    {   // hue and saturation: pair in zigbee2mqtt units (0-360, 0-100)
        RuntimeContext ctx = ctx_with_store();
        DecodedMessage msg{};
        msg.payload.put("0", uintv(127));
        msg.payload.put("1", uintv(127));
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));
        assert(str_is(out.find("color_hs"), "180.0,50.0"));
    }
    {   // no store: single axis still decodes, the pair needs both in one report
        RuntimeContext ctx{};
        DecodedMessage msg{};
        msg.payload.put("3", uintv(1000));
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));
        assert(out.find("color_x") && out.find("color_xy") == nullptr);
    }

    // ── encoder ──
    {   // color_x write keeps the remembered y (0.5 from the report above)
        RuntimeContext ctx = ctx_with_store();
        assert(generic::tz_color("color_x", floatv(0.25f), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(n == 9 && frame[2] == 0x07);
        assert(le16(frame + 3) == 16384 && le16(frame + 5) == 32768);
    }
    {   // and that write is now the memory: a y write keeps x = 0.25
        RuntimeContext ctx = ctx_with_store();
        assert(generic::tz_color("color_y", floatv(0.75f), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(le16(frame + 3) == 16384 && le16(frame + 5) == 49151);   // 0.75 * 65535 + 0.5, truncated
    }
    {   // color_xy "x,y": one frame with both axes
        RuntimeContext ctx = ctx_with_store();
        assert(generic::tz_color("color_xy", strv("0.5,0.25"), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(n == 9 && frame[2] == 0x07 && le16(frame + 3) == 32768 && le16(frame + 5) == 16384);
        assert(!generic::tz_color("color_xy", strv("0.5"), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(!generic::tz_color("color_xy", strv("1.5,0.2"), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(!generic::tz_color("color_xy", strv("abc,0.2"), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(!generic::tz_color("color_xy", floatv(0.5f), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
    }
    {   // color_hs "h,s" -> moveToHueAndSaturation with device units
        RuntimeContext ctx = ctx_with_store();
        assert(generic::tz_color("color_hs", strv("180,50"), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(n == 7 && frame[2] == 0x06 && frame[3] == 127 && frame[4] == 127);
        assert(!generic::tz_color("color_hs", strv("400,50"), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
    }
    {   // no store: color_x alone still writes, the other axis is centre
        RuntimeContext ctx{};
        assert(generic::tz_color("color_x", floatv(0.0f), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(le16(frame + 3) == 0 && le16(frame + 5) == 0x8000);
    }

    // ── colour mode ──
    {   // the last command's mode (color_hs above) rides a report without
        // colorMode; not on a multi-endpoint light, whose endpoints would
        // share the one memory
        RuntimeContext ctx = ctx_with_store();
        DecodedMessage msg{};
        msg.payload.put("3", uintv(1000));
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));
        assert(str_is(out.find("color_mode"), "hs"));
        static constexpr EndpointLabel kEps[] = {{"l1", 1}, {"l2", 2}};
        PreparedDefinition multi{};
        multi.endpoint_map = kEps;
        multi.endpoint_map_count = 2;
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out2{};
        assert(generic::fz_color(msg, generic::kFzColor, multi, ctx, out2));
        assert(out2.find("color_x") && !out2.find("color_mode"));
    }

    // ── color_hex ──
    {   // an xy report also yields color_hex (0.64, 0.33 = pure red)
        RuntimeContext ctx = ctx_fresh_light();
        DecodedMessage msg{};
        msg.payload.put("3", uintv(41943));
        msg.payload.put("4", uintv(21627));
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));
        assert(str_is(out.find("color_hex"), "#ff0000"));
    }
    {   // a hue/saturation report in hs mode yields color_hex from hue + sat
        RuntimeContext ctx = ctx_fresh_light();
        DecodedMessage msg{};
        msg.payload.put("0", uintv(85));    // 120 degrees
        msg.payload.put("1", uintv(254));   // full saturation
        msg.payload.put("8", uintv(0));     // colorMode hs
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));
        zhc::color::Rgb want = zhc::color::hs_to_rgb(85 * 360.0f / 254.0f, 1.0f);
        char w[8]; zhc::color::format_hex(want, w);
        assert(str_is(out.find("color_hex"), w));
    }
    {   // one axis and no memory: no pair, so no color_hex
        RuntimeContext ctx{};
        DecodedMessage msg{};
        msg.payload.put("3", uintv(41943));
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));
        assert(out.find("color_hex") == nullptr);
    }
    {   // a light last set to hs is in hs mode: an xy report leaves color_hex on
        // the remembered hue + sat (device 0, color_hs 180,50 above)
        RuntimeContext ctx = ctx_with_store();
        DecodedMessage msg{};
        msg.payload.put("3", uintv(41943));
        msg.payload.put("4", uintv(21627));
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));
        assert(str_is(out.find("color_hex"), "#80ffff"));   // hue 180, saturation 50 %
    }
    {   // a light that knows both pairs shows the one its mode names, and a
        // colorMode report alone is enough to switch
        RuntimeContext ctx = ctx_fresh_light();
        DecodedMessage msg{};
        msg.payload.put("0", uintv(127));   // hue 180, saturation 50 %: #80ffff
        msg.payload.put("1", uintv(127));
        msg.payload.put("3", uintv(41943)); // xy: #ff0000
        msg.payload.put("4", uintv(21627));
        msg.payload.put("8", uintv(1));     // colorMode xy
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));
        assert(str_is(out.find("color_hex"), "#ff0000"));
        DecodedMessage mode_only{};
        mode_only.payload.put("8", uintv(0));   // colorMode hs
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out2{};
        assert(generic::fz_color(mode_only, generic::kFzColor, def, ctx, out2));
        assert(str_is(out2.find("color_hex"), "#80ffff"));
    }
    {   // a pair needs both axes: a hue alone shows no colour, the saturation that
        // follows completes it, and an x alone does not pull color_hex to xy
        RuntimeContext ctx = ctx_fresh_light();
        auto feed = [&](const char* attr, std::uint64_t v, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
            DecodedMessage msg{};
            msg.payload.put(attr, uintv(v));
            return generic::fz_color(msg, generic::kFzColor, def, ctx, out);
        };
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> hue{}, sat{}, x{};
        assert(feed("0", 127, hue) && hue.find("color_hex") == nullptr);
        assert(feed("1", 127, sat) && str_is(sat.find("color_hex"), "#80ffff"));
        assert(feed("3", 41943, x) && str_is(x.find("color_hex"), "#80ffff"));
    }
    {   // an xy that is no colour (x = y = 1, so x + y > 1) and no hue/sat to fall
        // back on: nothing is published, in particular not black
        RuntimeContext ctx = ctx_fresh_light();
        DecodedMessage msg{};
        msg.payload.put("3", uintv(65535));
        msg.payload.put("4", uintv(65535));
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        assert(generic::fz_color(msg, generic::kFzColor, def, ctx, out));
        assert(out.find("color_hex") == nullptr);
    }

    // ── color_hex: write ──
    {   // color_hex on an xy light: moveToColor at the colour's chromaticity
        RuntimeContext ctx = ctx_fresh_light();
        assert(generic::tz_color("color_hex", strv("#ff0000"), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(n == 9 && frame[2] == 0x07);
        assert(std::abs(int(le16(frame + 3)) - 41942) < 70 && std::abs(int(le16(frame + 5)) - 21627) < 70);
        // upper case is the same colour: the same frame
        std::uint8_t lower[16];
        std::memcpy(lower, frame, n);
        assert(generic::tz_color("color_hex", strv("#FF0000"), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(n == 9 && std::memcmp(frame, lower, n) == 0);
        assert(!generic::tz_color("color_hex", strv("ff0000"), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n) && n == 0);
        assert(!generic::tz_color("color_hex", strv("#ff00"), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n) && n == 0);
        assert(!generic::tz_color("color_hex", strv("#000000"), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n) && n == 0);
        assert(!generic::tz_color("color_hex", uintv(5), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n) && n == 0);
        // the refusals left the colour memory alone: a y write still pairs with the red's x
        assert(generic::tz_color("color_y", floatv(0.5f), generic::kTzColor, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(std::abs(int(le16(frame + 3)) - 41942) < 70 && le16(frame + 5) == 32768);
    }
    {   // color_hex on an HS-only light (hue/saturation rows only): moveToHueAndSaturation
        static constexpr Expose kHs[] = {
            {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
            {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0}};
        PreparedDefinition hs{}; hs.exposes = kHs; hs.exposes_count = 2;
        RuntimeContext ctx = ctx_fresh_light();
        assert(generic::tz_color("color_hex", strv("#00ff00"), generic::kTzColor, hs, ctx, std::span<std::uint8_t>(frame), n));
        assert(n == 7 && frame[2] == 0x06 && frame[3] == 85 && frame[4] == 254);
        // black is refused here too (rgb_to_hs would take it as hue 0, saturation 0)
        assert(!generic::tz_color("color_hex", strv("#000000"), generic::kTzColor, hs, ctx, std::span<std::uint8_t>(frame), n) && n == 0);
    }
    {   // what a definition does with color_hex
        static const FzConverter* const kFz[] = {&generic::kFzColor};
        static const TzConverter* const kTz[] = {&generic::kTzColor};
        static constexpr Expose kRows[] = {
            {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
            {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0}};
        PreparedDefinition d{};
        d.from_zigbee = kFz; d.from_zigbee_count = 1; d.to_zigbee = kTz; d.to_zigbee_count = 1;
        d.exposes = kRows; d.exposes_count = 2;
        generic::ColorHexSupport s = generic::color_hex_support(d);
        assert(s.read && s.write && std::strcmp(s.mode, "xy") == 0);
        PreparedDefinition ro{}; ro.from_zigbee = kFz; ro.from_zigbee_count = 1;
        s = generic::color_hex_support(ro);
        assert(s.read && !s.write);
        assert(!generic::color_hex_support(PreparedDefinition{}).read);
        // the paired rows count as well: color_hs alone is an HS light, color_xy beside hue is xy
        static constexpr Expose kHsRow[] = {
            {"color_hs", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0}};
        static constexpr Expose kXyRows[] = {
            {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
            {"color_xy", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0}};
        PreparedDefinition hp{}; hp.exposes = kHsRow; hp.exposes_count = 1;
        PreparedDefinition xp{}; xp.exposes = kXyRows; xp.exposes_count = 2;
        assert(std::strcmp(generic::color_hex_support(hp).mode, "hs") == 0);
        assert(std::strcmp(generic::color_hex_support(xp).mode, "xy") == 0);
    }

    // ── colour state: what the hub mirrors after a write ──
    // A pick shows as the light will report it: the colour at full brightness
    // the frame carries, not the text that was picked. Every colour key leaves
    // the colour and its mode, not only color_hex.
    {
        // what a report carrying exactly the frame's axes publishes (both axes in
        // one report need no memory)
        auto reported = [&](const char* want) {
            RuntimeContext rc{};
            DecodedMessage msg{};
            if (frame[2] == 0x07) {
                msg.payload.put("3", uintv(le16(frame + 3)));
                msg.payload.put("4", uintv(le16(frame + 5)));
            } else {
                msg.payload.put("0", uintv(frame[3]));
                msg.payload.put("1", uintv(frame[4]));
            }
            FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
            return generic::fz_color(msg, generic::kFzColor, def, rc, out) && str_is(out.find("color_hex"), want);
        };
        auto is = [](const generic::ColorState& s, const char* hex, const char* mode) {
            return std::strcmp(s.hex, hex) == 0 && s.mode && std::strcmp(s.mode, mode) == 0;
        };
        auto send = [&](RuntimeContext& c, const char* key, const Value& v, const PreparedDefinition& d) {
            return generic::tz_color(key, v, generic::kTzColor, d, c, std::span<std::uint8_t>(frame), n);
        };
        RuntimeContext ctx = ctx_fresh_light();
        generic::ColorState s = generic::color_state(ctx);
        assert(s.hex[0] == '\0' && s.mode == nullptr);               // nothing known yet
        assert(send(ctx, "color_hex", strv("#34C759"), def));          // the Green preset
        s = generic::color_state(ctx);
        assert(is(s, "#45ff74", "xy") && reported(s.hex));
        assert(send(ctx, "color_hex", strv("#400000"), def));          // a dark red is red
        s = generic::color_state(ctx);
        assert(is(s, "#ff0000", "xy") && reported(s.hex));
        assert(send(ctx, "color_hs", strv("120,100"), def));           // Home Assistant's hs
        s = generic::color_state(ctx);
        assert(s.mode && std::strcmp(s.mode, "hs") == 0 && reported(s.hex));
        assert(send(ctx, "hue", uintv(0), def));                       // a rule's hue: red at that saturation
        assert(is(generic::color_state(ctx), "#ff0000", "hs"));
        assert(send(ctx, "color_xy", strv("0.15,0.06"), def));         // Home Assistant's xy
        s = generic::color_state(ctx);
        assert(is(s, "#0000ff", "xy") && reported(s.hex));
        assert(send(ctx, "color_x", floatv(0.64f), def));              // one axis: the other from memory
        s = generic::color_state(ctx);
        assert(s.mode && std::strcmp(s.mode, "xy") == 0 && std::strcmp(s.hex, "#0000ff") != 0 && reported(s.hex));
        const generic::ColorState last = s;
        // a white pick: white mode, the colour stays the last one
        assert(generic::tz_color_temp("color_temp", uintv(300), generic::kTzColorTemp, def, ctx, std::span<std::uint8_t>(frame), n));
        assert(is(generic::color_state(ctx), last.hex, "color_temp"));
        // a refused write changes nothing
        assert(!send(ctx, "color_hex", strv("#000000"), def));
        assert(is(generic::color_state(ctx), last.hex, "color_temp"));
        // an HS-only light: the colour its hue and saturation bytes give
        static constexpr Expose kHsOnly[] = {
            {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
            {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0}};
        PreparedDefinition hs{}; hs.exposes = kHsOnly; hs.exposes_count = 2;
        RuntimeContext hctx = ctx_fresh_light();
        assert(send(hctx, "color_hex", strv("#34c759"), hs) && frame[2] == 0x06);
        s = generic::color_state(hctx);
        assert(s.mode && std::strcmp(s.mode, "hs") == 0 && std::strcmp(s.hex, "#34c759") != 0 && reported(s.hex));
        // no store: nothing to tell
        RuntimeContext bare{};
        s = generic::color_state(bare);
        assert(s.hex[0] == '\0' && s.mode == nullptr);
    }
    return 0;
}
