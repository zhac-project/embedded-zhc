// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Colour: the decoder remembers each axis so a one-attribute report still
// yields the `color_xy` / `color_hs` pairs, and the encoder writes both axes
// (the other one from memory) instead of resetting it to centre. The colour
// mode is remembered the same way.
#include <cassert>
#include <cstdint>
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
    return 0;
}
