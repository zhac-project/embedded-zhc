// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Tuya TS0052 dimmer modules (z2m v26.115.1):
//   TS0052   zigbeeModel, 1 channel (white label Tuya FS-05R, _TZ3000_mgusv51k)
//   TS0052_2 fingerprint TS0052 + _TZ3000_zjtxnoft / _kvwrdf47 / _sfibawtr,
//            2 channels: m.deviceEndpoints({l1: 1, l2: 2}) + configureMagicPacket
// Both are tuyaLight({powerOnBehavior: true, configureReporting: true,
// switchType: true, minBrightness: "attribute"}): a standard ZCL light (genOnOff
// + genLevelCtrl) with min_brightness (genLevelCtrl 0xFC00 = min << 8 | 0xFF),
// effect, do_not_disturb (lightingColorCtrl cmd 0xFA), power_on_behavior and
// switch_type (manuSpecificTuya3 0xE001 attrs 0xD010 / 0xD030), plus Tuya's
// 0xF000 brightness reports.
// The generated definitions carried none of it: TS0052 exposed only an
// `action` and had no to-zigbee converter at all, so the hub logged
// "key='brightness' no converter / encode failed"; the 2-channel modules were
// battery + on/off stubs, and _TZ3000_sfibawtr fell through to the 1-channel
// definition.
//
// z2m-source: tuya.ts #TS0052 / #TS0052_2, lib/tuya.ts tuyaLight,
//             tuyaFz/tuyaTz brightness, power_on_behavior_2, switch_type,
//             min_brightness_attribute, do_not_disturb, lib/modernExtend.ts
//             light(), converters/toZigbee.ts light_onoff_brightness / effect.

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;
using Bytes = std::vector<std::uint8_t>;

namespace {

const PreparedDefinition* lookup(const char* manu) {
    const std::span<const PreparedDefinition* const> reg(devices::tuya::kTuyaRegistry,
                                                         devices::tuya::kTuyaRegistryCount);
    return find_definition("TS0052", manu, reg);
}

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (std::strcmp(def.exposes[i].name, key) == 0) return &def.exposes[i];
    return nullptr;
}

bool enum_is(const Expose* e, std::initializer_list<const char*> values) {
    if (!e || e->type != ExposeType::Enum || e->enum_count != values.size()) return false;
    std::size_t i = 0;
    for (const char* v : values)
        if (std::strcmp(e->enum_values[i++], v) != 0) return false;
    return true;
}

// ZCL attribute report (0x0A) on `cluster` from endpoint `ep`.
DispatchResult report(const PreparedDefinition& def, std::uint16_t cluster, const Bytes& records,
                      std::uint8_t ep = 1) {
    Bytes f = {0x18, 0x42, 0x0A};
    f.insert(f.end(), records.begin(), records.end());
    InboundApsFrame raw{};
    raw.cluster_id = cluster;
    raw.src_endpoint = ep;
    raw.dst_endpoint = 1;
    raw.data = f;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

struct Sent { bool ok; std::uint16_t cluster; Bytes frame; };
Sent send(const PreparedDefinition& def, const char* key, const Value& v) {
    RuntimeContext ctx{};
    std::uint8_t frame[64]{};
    const auto r = dispatch_to_zigbee(def, key, v, ctx, frame);
    if (!r.ok) return {false, 0, {}};
    return {true, r.cluster_id, Bytes(frame, frame + r.frame_size)};
}

Value str_v(const char* s)      { Value v{}; v.type = ValueType::StringRef; v.str = s; return v; }
Value bool_v(bool b)            { Value v{}; v.type = ValueType::Bool; v.b = b; return v; }
Value uint_v(std::uint64_t u)   { Value v{}; v.type = ValueType::Uint; v.u = u; return v; }
Value float_v(float f)          { Value v{}; v.type = ValueType::Float; v.f = f; return v; }
bool is_str(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}
bool is_uint(const Value* v, std::uint64_t u) { return v && v->type == ValueType::Uint && v->u == u; }

bool binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}
bool reports(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.reports_count; ++i)
        if (def.reports[i].endpoint == ep && def.reports[i].cluster_id == cluster &&
            def.reports[i].attr_id == 0x0000) return true;
    return false;
}

// The write path both definitions share; the hub strips an `_l1` / `_l2`
// suffix and routes to that endpoint before it gets here.
void check_writes(const PreparedDefinition& def) {
    // brightness on z2m's 0-254 scale: moveToLevelWithOnOff, level, no transition.
    for (unsigned level : {0u, 1u, 100u, 101u, 150u, 254u}) {
        const Sent b = send(def, "brightness", uint_v(level));
        assert(b.ok);
        assert(b.cluster == 0x0008 &&
               b.frame == (Bytes{0x11, 0x00, 0x04, static_cast<std::uint8_t>(level), 0x00, 0x00}));
    }
    // z2m: 255 (this light reports 0xF000 brightness on 0-255) goes out as
    // 254, more is refused; a decimal rounds to the nearest level first.
    assert(send(def, "brightness", uint_v(255)).frame == (Bytes{0x11, 0x00, 0x04, 254, 0x00, 0x00}));
    assert(!send(def, "brightness", uint_v(256)).ok);
    assert(send(def, "brightness", float_v(127.6f)).frame == (Bytes{0x11, 0x00, 0x04, 128, 0x00, 0x00}));

    const Sent on = send(def, "state", str_v("ON"));
    assert(on.ok && on.cluster == 0x0006 && on.frame == (Bytes{0x11, 0x00, 0x01}));
    const Sent off = send(def, "state", bool_v(false));
    assert(off.ok && off.cluster == 0x0006 && off.frame == (Bytes{0x11, 0x00, 0x00}));

    // min_brightness: genLevelCtrl write 0xFC00 (uint16) = min << 8 | 0xFF (max).
    const Sent mb = send(def, "min_brightness", uint_v(20));
    assert(mb.ok && mb.cluster == 0x0008 &&
           mb.frame == (Bytes{0x10, 0x00, 0x02, 0x00, 0xFC, 0x21, 0xFF, 0x14}));
    assert(!send(def, "min_brightness", uint_v(0)).ok);
    assert(!send(def, "min_brightness", uint_v(256)).ok);
    assert(send(def, "min_brightness", float_v(19.6f)).frame == mb.frame);

    // power_on_behavior / switch_type: manuSpecificTuya3 (0xE001) enum8 writes.
    const Sent pob = send(def, "power_on_behavior", str_v("previous"));
    assert(pob.ok && pob.cluster == 0xE001 &&
           pob.frame == (Bytes{0x10, 0x00, 0x02, 0x10, 0xD0, 0x30, 0x02}));
    const Sent st = send(def, "switch_type", str_v("momentary"));
    assert(st.ok && st.cluster == 0xE001 &&
           st.frame == (Bytes{0x10, 0x00, 0x02, 0x30, 0xD0, 0x30, 0x02}));

    // do_not_disturb → tuyaDoNotDisturb; effect → genIdentify triggerEffect
    // (no colour, so no colour loop).
    const Sent dnd = send(def, "do_not_disturb", bool_v(true));
    assert(dnd.ok && dnd.cluster == 0x0300 && dnd.frame == (Bytes{0x11, 0x00, 0xFA, 0x01}));
    const Sent blink = send(def, "effect", str_v("blink"));
    assert(blink.ok && blink.cluster == 0x0003 && blink.frame == (Bytes{0x11, 0x00, 0x40, 0x00, 0x00}));
    assert(!send(def, "effect", str_v("colorloop")).ok);
}

// The light's own exposes for one channel (suffix "" or "_l1" / "_l2").
void check_light_exposes(const PreparedDefinition& def, const char* sfx) {
    char k[32];
    std::snprintf(k, sizeof(k), "state%s", sfx);
    const Expose* state = find_expose(def, k);
    assert(state && state->type == ExposeType::Binary && state->access == Access::StateSet);
    std::snprintf(k, sizeof(k), "brightness%s", sfx);
    const Expose* br = find_expose(def, k);
    assert(br && br->type == ExposeType::Numeric && br->access == Access::StateSet);
    assert(br->value_min == 0 && br->value_max == 254);
    std::snprintf(k, sizeof(k), "min_brightness%s", sfx);
    const Expose* mb = find_expose(def, k);
    assert(mb && mb->type == ExposeType::Numeric && mb->access == Access::StateSet);
    assert(mb->value_min == 1 && mb->value_max == 255);
    std::snprintf(k, sizeof(k), "effect%s", sfx);
    const Expose* fx = find_expose(def, k);
    assert(enum_is(fx, {"blink", "breathe", "okay", "channel_change", "finish_effect", "stop_effect"}));
    assert(fx->access == Access::Set);
    std::snprintf(k, sizeof(k), "power_on_behavior%s", sfx);
    const Expose* pob = find_expose(def, k);
    assert(enum_is(pob, {"off", "previous", "on"}) && pob->access == Access::StateSet &&
           pob->category == ExposeCategory::Config);
}

}  // namespace

int main() {
    // ── TS0052, 1 channel (_TZ3000_mgusv51k, the owner's FS-05R) ─────────
    const PreparedDefinition* one = lookup("_TZ3000_mgusv51k");
    assert(one && std::strcmp(one->model, "TS0052") == 0);
    check_writes(*one);

    check_light_exposes(*one, "");
    const Expose* dnd = find_expose(*one, "do_not_disturb");
    assert(dnd && dnd->type == ExposeType::Binary && dnd->category == ExposeCategory::Config);
    const Expose* st = find_expose(*one, "switch_type");
    assert(enum_is(st, {"toggle", "state", "momentary"}) && st->access == Access::StateSet &&
           st->category == ExposeCategory::Config);
    assert(!find_expose(*one, "action"));

    // currentLevel (0x0000, uint8) is brightness as is; Tuya's 0xF000 (0-1000)
    // lands on the 0-255 scale.
    assert(is_uint(report(*one, 0x0008, {0x00, 0x00, 0x20, 200}).merged.find("brightness"), 200));
    assert(is_uint(report(*one, 0x0008, {0x00, 0xF0, 0x21, 0xF4, 0x01}).merged.find("brightness"), 128));
    const auto rmb = report(*one, 0x0008, {0x00, 0xFC, 0x21, 0xFF, 0x14});   // 0x14FF
    assert(is_uint(rmb.merged.find("min_brightness"), 20) && !rmb.merged.find("brightness"));
    const Value* on = report(*one, 0x0006, {0x00, 0x00, 0x10, 0x01}).merged.find("state");
    assert(on && on->type == ValueType::Bool && on->b);
    assert(is_str(report(*one, 0xE001, {0x10, 0xD0, 0x30, 0x02}).merged.find("power_on_behavior"), "previous"));
    assert(is_str(report(*one, 0xE001, {0x30, 0xD0, 0x30, 0x02}).merged.find("switch_type"), "momentary"));

    // Configure: bind + report genOnOff / genLevelCtrl, as tuyaLight's
    // configureReporting; no Tuya MCU (0xEF00) on this module.
    assert(one->bindings_count == 2 && binds(*one, 1, 0x0006) && binds(*one, 1, 0x0008));
    assert(reports(*one, 1, 0x0006) && reports(*one, 1, 0x0008));
    assert(one->white_labels_count == 1 && std::strcmp(one->white_labels[0].model, "FS-05R") == 0);

    // ── TS0052_2, 2 channels ─────────────────────────────────────────────
    const PreparedDefinition* two = lookup("_TZ3000_kvwrdf47");
    assert(two && two != one && std::strcmp(two->model, "TS0052_2") == 0);
    assert(lookup("_TZ3000_zjtxnoft") == two && lookup("_TZ3000_sfibawtr") == two);
    check_writes(*two);

    assert(two->endpoint_map_count == 2);
    assert(std::strcmp(two->endpoint_map[0].label, "l1") == 0 && two->endpoint_map[0].endpoint == 1);
    assert(std::strcmp(two->endpoint_map[1].label, "l2") == 0 && two->endpoint_map[1].endpoint == 2);
    check_light_exposes(*two, "_l1");
    check_light_exposes(*two, "_l2");
    assert(find_expose(*two, "do_not_disturb") && find_expose(*two, "switch_type"));
    assert(!find_expose(*two, "state") && !find_expose(*two, "battery"));

    // Reports land on the channel they come from.
    assert(is_uint(report(*two, 0x0008, {0x00, 0x00, 0x20, 77}, 2).merged.find("brightness_l2"), 77));
    const Value* on1 = report(*two, 0x0006, {0x00, 0x00, 0x10, 0x01}, 1).merged.find("state_l1");
    assert(on1 && on1->type == ValueType::Bool && on1->b);
    assert(is_str(report(*two, 0xE001, {0x10, 0xD0, 0x30, 0x00}, 2).merged.find("power_on_behavior_l2"), "off"));
    assert(is_uint(report(*two, 0x0008, {0x00, 0xFC, 0x21, 0xFF, 0x0A}, 1).merged.find("min_brightness_l1"), 10));

    // Both channels bound and reporting; the Tuya magic packet (genBasic read
    // of 4, 0, 1, 5, 7, 0xFFFE) as z2m's configureMagicPacket.
    for (std::uint8_t ep : {std::uint8_t{1}, std::uint8_t{2}})
        assert(binds(*two, ep, 0x0006) && binds(*two, ep, 0x0008) &&
               reports(*two, ep, 0x0006) && reports(*two, ep, 0x0008));
    bool magic = false;
    for (std::size_t i = 0; i < two->config_steps_count; ++i) {
        const ConfigStep& s = two->config_steps[i];
        const std::uint8_t want[] = {0x04, 0x00, 0x00, 0x00, 0x01, 0x00,
                                     0x05, 0x00, 0x07, 0x00, 0xFE, 0xFF};
        magic |= s.op == ConfigStepOp::Read && s.cluster_id == 0x0000 &&
                 s.payload_len == sizeof(want) && std::memcmp(s.payload, want, sizeof(want)) == 0;
    }
    assert(magic);
    return 0;
}
