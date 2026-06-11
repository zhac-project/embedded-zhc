// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the XAL "Just MOVE IT 25" track luminaires
// (050-0131558M spotlight, 050-0511558F cable-suspended spotlight,
// 050-1212558H opal floodlight). All three are z2m m.light() with NO
// color/colorTemp args, i.e. plain dimmable lights.
//
// Pins the gap fixed when the defs were graduated out of generated/:
//
//   * z2m's bare m.light() exposes state + brightness + power_on_behavior
//     (e.power_on_behavior(["off","on","toggle","previous"]) backed by
//     fz/tz.power_on_behavior on genOnOff attr 0x4003 startUpOnOff). The
//     auto-port dropped that decodable channel — power_on_behavior was a
//     missing decoder. Restored via the generic kFz/kTzPowerOnBehavior1
//     pair.
//   * The SUSPECT "dropped color/colorTemp axis" is a FALSE FLAG: bare
//     m.light() (no color/colorTemp args) emits neither expose nor the
//     fz.color_colortemp decoder, so NO color axis must exist. Asserted
//     negatively below.
//
// z2m-source: zigbee-herdsman-converters/src/devices/xal.ts +
//             lib/modernExtend.ts #light().

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::xal {
extern const PreparedDefinition kDef_D050_0131558M;
extern const PreparedDefinition kDef_D050_0511558F;
extern const PreparedDefinition kDef_D050_1212558H;
}  // namespace zhc::devices::xal

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

// ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then one record.
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
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool str_is(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}

// Exercises the shared on/off + brightness + power_on_behavior surface
// that every XAL m.light() def carries.
void check_light_def(const PreparedDefinition& def) {
    // Exposes: state + brightness + power_on_behavior; NO color axis.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "power_on_behavior"));
    assert(!def_exposes(def, "color"));
    assert(!def_exposes(def, "color_temp"));
    assert(!def_exposes(def, "color_hs"));
    assert(!def_exposes(def, "color_xy"));

    // genOnOff onOff (0x0000 bool) → state (emitted as Bool).
    {
        const std::uint8_t on[] = {0x01};
        auto r = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                              attr_report(0x0000, 0x10, on));
        assert(r.any_matched);
        const Value* st = r.merged.find("state");
        assert(st && st->type == ValueType::Bool && st->b);
    }
    // genLevelCtrl currentLevel (0x0000 u8) → brightness.
    {
        const std::uint8_t lvl[] = {0x7F};  // 127
        auto r = dispatch_zcl(def, 0x0008, "genLevelCtrl", 1,
                              attr_report(0x0000, 0x20, lvl));
        assert(r.any_matched);
        assert(r.merged.find("brightness") != nullptr);
    }
    // genOnOff startUpOnOff (0x4003 enum8) → power_on_behavior (the
    // restored channel). 0xFF → "previous", 0x00 → "off".
    {
        const std::uint8_t prev[] = {0xFF};
        auto r = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                              attr_report(0x4003, 0x30, prev));
        assert(r.any_matched);
        assert(str_is(r.merged.find("power_on_behavior"), "previous"));

        const std::uint8_t off[] = {0x00};
        auto r2 = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                               attr_report(0x4003, 0x30, off));
        assert(r2.any_matched);
        assert(str_is(r2.merged.find("power_on_behavior"), "off"));
    }
    // No color decoder: a lightingColorCtrl currentX report (0x0003) must
    // NOT produce any color key.
    {
        const std::uint8_t x[] = {0x00, 0x80};  // 0x8000
        auto r = dispatch_zcl(def, 0x0300, "lightingColorCtrl", 1,
                              attr_report(0x0003, 0x21, x));
        assert(r.merged.find("color") == nullptr);
        assert(r.merged.find("color_temp") == nullptr);
    }
}

}  // namespace

int main() {
    using namespace zhc::devices::xal;

    check_light_def(kDef_D050_0131558M);
    check_light_def(kDef_D050_0511558F);
    check_light_def(kDef_D050_1212558H);

    return 0;
}
