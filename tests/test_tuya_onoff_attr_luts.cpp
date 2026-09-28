// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Regression test for the shared Tuya genOnOff attribute tables:
//   0x8002 moesStartUpOnOff → power_on_behavior {0 off, 1 on, 2 previous}
//   0x8001 tuyaBacklightMode → indicator_mode {0 off, 1 "off/on", 2 "on/off", 3 on}
// The shared tables had 2 = "toggle", 3 = "previous" and the indicator tz
// accepted off/on_when_off/on_when_on → 0..2, disagreeing with its own fz.
// Every def wired to kFz/kTzTuyaPowerOnBehavior or kFz/kTzTuyaIndicatorMode
// is pinned here, fz and tz.
//
// z2m-source: zigbee-herdsman-converters/src/lib/tuya.ts
//             tuyaFz.power_on_behavior_1 / tuyaTz.power_on_behavior_1,
//             tuyaFz.indicator_mode / tuyaTz.backlight_indicator_mode_1.
//   z2m counterparts: TS0001 tuyaOnOff(); TS011F_plug_1 (tuyaOnOff, generic
//   + _TZ3000_okaz9tjs); TS0001 _TZ3000_bzzgvet0
//   tuyaOnOff({powerOnBehavior2 …}) (the def keeps genOnOff 0x8002);
//   girier JR-ZDS01 tuyaOnOff({switchType}); moes ZP-LZ-FR2U
//   tuyaOnOff({powerOutageMemory, indicatorMode}) — power_outage_memory
//   {off, on, restore} on the same 0..2 values; TS0726 power_on_behavior_2
//   {off, on, previous} (same values, other cluster — see note in the def).

#include <cassert>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTS0001;
extern const PreparedDefinition kDef_TS0001_bzzgvet0;
extern const PreparedDefinition kDef_TS0726;
extern const PreparedDefinition kDefTS011F;
extern const PreparedDefinition kDefTS011F_okaz9tjs;
}  // namespace zhc::devices::tuya
namespace zhc::devices::girier {
extern const PreparedDefinition kDef_JR_ZDS01;
}  // namespace zhc::devices::girier
namespace zhc::devices::moes {
extern const PreparedDefinition kDefMoes__TZ3000_2xlvlnez;
extern const PreparedDefinition kDefMoes__TZ3000_cymsnfvf;
extern const PreparedDefinition kDefMoes__TZ3210_2uk4z8ce;
extern const PreparedDefinition kDef_ZP_LZ_FR2U;
}  // namespace zhc::devices::moes

using namespace zhc;

namespace {

const char* decode_attr(const PreparedDefinition& def, std::uint16_t attr,
                        std::uint8_t value, const char* key) {
    const std::uint8_t bytes[] = {0x18, 0x42, 0x0A,
                                  static_cast<std::uint8_t>(attr & 0xFF),
                                  static_cast<std::uint8_t>(attr >> 8),
                                  0x30, value};   // enum8
    InboundApsFrame raw{};
    raw.cluster_id   = 0x0006;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = bytes;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(0x0006);
    RuntimeContext ctx{};
    static DispatchResult r;   // keep StringRef storage alive past return
    r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    const Value* v = r.merged.find(key);
    return (v && v->type == ValueType::StringRef) ? v->str : nullptr;
}

bool is(const char* got, const char* want) {
    return got && std::strcmp(got, want) == 0;
}

// Returns the written enum8 byte, or -1 if the key/label is refused.
int encode(const PreparedDefinition& def, const char* key, const char* label) {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::StringRef; v.str = label;
    const auto r = dispatch_to_zigbee(def, key, v, ctx, frame);
    if (!r.ok) return -1;
    assert(r.cluster_id == 0x0006);
    // fc tsn cmd attr_lo attr_hi type value
    assert(r.frame_size == 7 && frame[2] == 0x02 && frame[5] == 0x30);
    return frame[6];
}

}  // namespace

int main() {
    const PreparedDefinition* pob_defs[] = {
        &devices::tuya::kDefTS0001,
        &devices::tuya::kDef_TS0001_bzzgvet0,
        &devices::tuya::kDef_TS0726,
        &devices::tuya::kDefTS011F,
        &devices::tuya::kDefTS011F_okaz9tjs,
        &devices::girier::kDef_JR_ZDS01,
        &devices::moes::kDefMoes__TZ3000_2xlvlnez,
        &devices::moes::kDefMoes__TZ3000_cymsnfvf,
        &devices::moes::kDefMoes__TZ3210_2uk4z8ce,
        &devices::moes::kDef_ZP_LZ_FR2U,
    };
    for (const auto* def : pob_defs) {
        assert(is(decode_attr(*def, 0x8002, 0, "power_on_behavior"), "off"));
        assert(is(decode_attr(*def, 0x8002, 1, "power_on_behavior"), "on"));
        assert(is(decode_attr(*def, 0x8002, 2, "power_on_behavior"), "previous"));
        assert(decode_attr(*def, 0x8002, 3, "power_on_behavior") == nullptr);
        assert(encode(*def, "power_on_behavior", "off") == 0);
        assert(encode(*def, "power_on_behavior", "on") == 1);
        assert(encode(*def, "power_on_behavior", "previous") == 2);
        assert(encode(*def, "power_on_behavior", "toggle") == -1);
    }

    const PreparedDefinition* ind_defs[] = {
        &devices::moes::kDefMoes__TZ3000_2xlvlnez,
        &devices::moes::kDefMoes__TZ3000_cymsnfvf,
        &devices::moes::kDefMoes__TZ3210_2uk4z8ce,
        &devices::moes::kDef_ZP_LZ_FR2U,
    };
    const char* ind_labels[] = {"off", "off/on", "on/off", "on"};
    for (const auto* def : ind_defs) {
        for (std::uint8_t raw = 0; raw < 4; ++raw) {
            assert(is(decode_attr(*def, 0x8001, raw, "indicator_mode"), ind_labels[raw]));
            assert(encode(*def, "indicator_mode", ind_labels[raw]) == raw);
        }
    }

    // TS0001 (z2m tuyaOnOff()): power_on_behavior is an enum the UI can offer.
    const auto& ts0001 = devices::tuya::kDefTS0001;
    bool found = false;
    for (std::size_t i = 0; i < ts0001.exposes_count; ++i) {
        const Expose& e = ts0001.exposes[i];
        if (std::strcmp(e.name, "power_on_behavior") != 0) continue;
        found = true;
        assert(e.type == ExposeType::Enum && e.enum_count == 3);
    }
    assert(found);
    return 0;
}
