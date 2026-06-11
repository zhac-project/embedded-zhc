// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Direct Signs (DIRECTSIGNS) device family.
// z2m-source: direct_signs.ts.
//
// Bug fixed: DS-Z-001DE is a full RGB + CCT LED controller in z2m —
//   m.light({colorTemp:{range:[158,500]}, color:{modes:["xy","hs"],
//   enhancedHue:true}}). The auto-port DROPPED the colour axis: it wired
//   only kFzOnOff + kFzBrightness (and the matching tz), exposed only
//   state + brightness, and bound only genOnOff (0x0006) + genLevelCtrl
//   (0x0008). lightingColorCtrl (0x0300) was never decoded, never
//   controllable, and never bound — color / color_temp were dead.
//
// The def was graduated to a Tier-2 parent override that adds
// kFzColorTemperature + kFzColor (+ kTzColorTemp / kTzColor), the
// color_temp (range [158,500] mired) + color_xy exposes, and the 0x0300
// binding. These tests pin the restored decode + surface.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/devices/direct_signs_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::direct_signs {
extern const PreparedDefinition kDef_DS_Z_001DE;  // RGB + CCT LED controller
}  // namespace zhc::devices::direct_signs

using namespace zhc;

namespace {

constexpr std::uint16_t LIGHTING_COLOR_CTRL = 0x0300;

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

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report (cluster-specific=false, Report=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

const Expose* def_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return &def.exposes[i];
    return nullptr;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// A tz converter that claims exactly `key` is present.
bool tz_handles_key(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* tz = def.to_zigbee[i];
        if (tz && tz->key && std::strcmp(tz->key, key) == 0) return true;
    }
    return false;
}

// A tz converter routing the given cluster is present (color uses a
// nullptr-key wildcard claimer, matched here by cluster id).
bool tz_routes_cluster(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* tz = def.to_zigbee[i];
        if (tz && tz->cluster_id == cluster) return true;
    }
    return false;
}

std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}

// ── DS-Z-001DE: full colour surface restored ─────────────────────────
void test_ds_z_001de_colour_surface() {
    const auto& def = devices::direct_signs::kDef_DS_Z_001DE;

    // Light base stays (regression).
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_binds(def, 0x0006));
    assert(def_binds(def, 0x0008));

    // Colour axis surface — the dropped half.
    assert(def_exposes(def, "color_temp"));
    assert(def_exposes(def, "color_xy"));
    assert(def_binds(def, LIGHTING_COLOR_CTRL) &&
           "RGB+CCT light must bind lightingColorCtrl (0x0300)");

    // colorTemp range mirrors z2m colorTemp.range [158,500].
    const Expose* ct = def_expose(def, "color_temp");
    assert(ct && ct->type == ExposeType::Numeric);
    assert(ct->unit && std::strcmp(ct->unit, "mired") == 0);
    assert(ct->value_min == 158 && ct->value_max == 500 &&
           "color_temp must carry the z2m [158,500] mired range");

    // Control path: color_temp + color writable (kTzColorTemp / kTzColor).
    assert(tz_handles_key(def, "color_temp"));
    assert(tz_handles_key(def, "state"));
    assert(tz_handles_key(def, "brightness"));
    // kTzColor is a nullptr-key wildcard claimer on lightingColorCtrl.
    assert(tz_routes_cluster(def, LIGHTING_COLOR_CTRL) &&
           "a tz converter must route lightingColorCtrl (colour control)");
}

// ── DS-Z-001DE: colorTemp decode (attr 0x0007 on 0x0300) ─────────────
void test_ds_z_001de_color_temp_decode() {
    const auto& def = devices::direct_signs::kDef_DS_Z_001DE;
    // colorTemperature attr 0x0007, u16 (type 0x21) = 370 mired.
    auto r = dispatch_zcl(def, LIGHTING_COLOR_CTRL, 1,
                          attr_report(0x0007, 0x21, u16(370)));
    assert(r.any_matched && "kFzColorTemperature must decode 0x0007");
    const Value* v = r.merged.find("color_temp");
    assert(v && v->type == ValueType::Uint && v->u == 370);
}

// ── DS-Z-001DE: color xy + hue/sat decode (kFzColor) ─────────────────
void test_ds_z_001de_color_decode() {
    const auto& def = devices::direct_signs::kDef_DS_Z_001DE;

    // currentHue 0x0000 (u8 0x20) = 200, currentSaturation 0x0001 = 150.
    {
        const std::uint8_t hue[] = {200};
        auto r = dispatch_zcl(def, LIGHTING_COLOR_CTRL, 1,
                              attr_report(0x0000, 0x20, hue));
        assert(r.any_matched && "kFzColor must decode currentHue 0x0000");
        const Value* h = r.merged.find("hue");
        assert(h && h->type == ValueType::Uint && h->u == 200);
    }
    {
        const std::uint8_t sat[] = {150};
        auto r = dispatch_zcl(def, LIGHTING_COLOR_CTRL, 1,
                              attr_report(0x0001, 0x20, sat));
        const Value* s = r.merged.find("saturation");
        assert(s && s->type == ValueType::Uint && s->u == 150);
    }
    // currentX 0x0003 (u16 0x21) = 26214 → 26214/65535 ≈ 0.4.
    {
        auto r = dispatch_zcl(def, LIGHTING_COLOR_CTRL, 1,
                              attr_report(0x0003, 0x21, u16(26214)));
        const Value* x = r.merged.find("color_x");
        assert(x && x->type == ValueType::Float && x->f > 0.39f && x->f < 0.41f);
    }
}

}  // namespace

int main() {
    test_ds_z_001de_colour_surface();
    test_ds_z_001de_color_temp_decode();
    test_ds_z_001de_color_decode();
    return 0;
}
