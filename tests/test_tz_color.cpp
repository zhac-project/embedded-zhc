// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Colour: the decoder remembers each axis so a one-attribute report still
// yields the `color_xy` / `color_hs` pairs, and the encoder writes both axes
// (the other one from memory) instead of resetting it to centre.
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/_generic/_shared.hpp"
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
    return 0;
}
