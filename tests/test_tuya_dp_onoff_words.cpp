// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// A Tuya boolean datapoint takes z2m's words, not only JSON booleans.
//
// Bug: `device.attr.set {key:"state", value:"ON"}` on a TS0601 switch failed
// with "no zhc converter": tz_tuya_datapoints' Bool encoder took Bool / Uint /
// Int and refused every string. Phone scenes store "ON"/"OFF", so they did
// nothing on Tuya datapoint switches.
//
// z2m v26.105.0: tuya.valueConverter.onOff = lookup({ON: true, OFF: false}),
// lockUnlock = lookup({LOCK: true, UNLOCK: false}); lookup's to() is
// utils.getFromLookup, which tries the string as given, lower-cased, then
// upper-cased. So "on" is accepted, "TOGGLE" is not (the key is missing).
//
// The Tuya frames here are the full EF00 dataRequest the hub sends:
//   fc 0x01, tsn 0x00, cmd 0x00, seq 0x0001, dp, type, len (BE), value.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/alecto_registry.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefGen__TZE284_xnwxmj8z;   // MG-ZG01W 1-gang switch
extern const PreparedDefinition kDefGen__TZE200_vhy3iakz;   // 3-gang switch
extern const PreparedDefinition kDefGen__TZE200_hue3yfsn;   // TV02 TRV, child_lock = lockUnlock
extern const PreparedDefinition kDefGen__TZE204_rkbxtclc;   // 3-gang switch, child_lock = onOff
extern const PreparedDefinition kDefTS011F_okaz9tjs;        // ZCL genOnOff plug
}  // namespace zhc::devices::tuya

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> tuya_reg() {
    return {devices::tuya::kTuyaRegistry, devices::tuya::kTuyaRegistryCount};
}

const PreparedDefinition& tuya_def(const char* model, const char* manu,
                                   const PreparedDefinition& expected) {
    const PreparedDefinition* d = find_definition(model, manu, tuya_reg());
    assert(d == &expected);
    return *d;
}

struct Sent {
    bool          ok;
    std::uint16_t cluster;
    std::size_t   size;
    std::uint8_t  frame[64];
};

Sent send(const PreparedDefinition& def, const char* key, const Value& v) {
    Sent s{};
    RuntimeContext ctx{};
    const auto r = dispatch_to_zigbee(def, key, v, ctx, std::span<std::uint8_t>(s.frame, sizeof(s.frame)));
    s.ok = r.ok;
    s.cluster = r.cluster_id;
    s.size = r.frame_size;
    return s;
}

Value str(const char* s) { Value v{}; v.type = ValueType::StringRef; v.str = s; return v; }
Value boolean(bool b)    { Value v{}; v.type = ValueType::Bool; v.b = b; return v; }

// An EF00 write of Bool datapoint `dp` (type 0x01, length 1).
void expect_bool_dp(const PreparedDefinition& def, const char* key, const Value& v,
                    std::uint8_t dp, bool on) {
    const Sent s = send(def, key, v);
    const std::uint8_t want[] = {0x01, 0x00, 0x00, 0x00, 0x01, dp, 0x01, 0x00, 0x01,
                                 static_cast<std::uint8_t>(on ? 0x01 : 0x00)};
    assert(s.ok);
    assert(s.cluster == 0xEF00);
    assert(s.size == sizeof(want));
    assert(std::memcmp(s.frame, want, sizeof(want)) == 0);
}

void expect_refused(const PreparedDefinition& def, const char* key, const Value& v) {
    assert(!send(def, key, v).ok);
}

}  // namespace

// TS0601 switch: "ON"/"OFF" send the same bytes as true/false.
static void test_switch_words_match_booleans() {
    const auto& def = tuya_def("TS0601", "_TZE284_xnwxmj8z", devices::tuya::kDefGen__TZE284_xnwxmj8z);
    expect_bool_dp(def, "state", boolean(true),  1, true);    // unchanged
    expect_bool_dp(def, "state", boolean(false), 1, false);   // unchanged
    expect_bool_dp(def, "state", str("ON"),  1, true);
    expect_bool_dp(def, "state", str("OFF"), 1, false);
    // getFromLookup tries the upper-cased form, so z2m takes these too.
    expect_bool_dp(def, "state", str("on"),  1, true);
    expect_bool_dp(def, "state", str("off"), 1, false);
    expect_bool_dp(def, "state", str("On"),  1, true);
    // A second boolean datapoint of the same device, same words.
    expect_bool_dp(def, "backlight_mode", str("ON"), 16, true);
}

static void test_multi_gang_words() {
    const auto& def = tuya_def("TS0601", "_TZE200_vhy3iakz", devices::tuya::kDefGen__TZE200_vhy3iakz);
    expect_bool_dp(def, "state_l2", str("ON"),  2, true);
    expect_bool_dp(def, "state_l3", str("OFF"), 3, false);
}

// z2m has no TOGGLE on a datapoint switch (onOff has no such key), and no
// other word; the numbers and JSON words as strings are not keys either.
static void test_other_strings_refused() {
    const auto& def = tuya_def("TS0601", "_TZE284_xnwxmj8z", devices::tuya::kDefGen__TZE284_xnwxmj8z);
    for (const char* s : {"TOGGLE", "toggle", "LOCK", "UNLOCK", "", "1", "0", "true", "false",
                          "ONN", "O", "OF", " ON", "ON ", "yes"}) {
        expect_refused(def, "state", str(s));
    }
    Value null_str{}; null_str.type = ValueType::StringRef; null_str.str = nullptr;
    expect_refused(def, "state", null_str);
}

// child_lock: z2m uses lockUnlock on most devices (TV02 here) and onOff on
// some (rkbxtclc). The generated map does not say which, so a child_lock
// datapoint takes both pairs; LOCK is true either way.
static void test_child_lock_words() {
    const auto& tv02 = tuya_def("TS0601", "_TZE200_hue3yfsn", devices::tuya::kDefGen__TZE200_hue3yfsn);
    expect_bool_dp(tv02, "child_lock", boolean(true), 40, true);   // unchanged
    expect_bool_dp(tv02, "child_lock", str("LOCK"),   40, true);
    expect_bool_dp(tv02, "child_lock", str("UNLOCK"), 40, false);
    expect_bool_dp(tv02, "child_lock", str("lock"),   40, true);
    expect_bool_dp(tv02, "child_lock", str("unlock"), 40, false);
    expect_refused(tv02, "child_lock", str("TOGGLE"));
    expect_refused(tv02, "child_lock", str("locked"));

    const auto& gang = tuya_def("TS0601", "_TZE204_rkbxtclc", devices::tuya::kDefGen__TZE204_rkbxtclc);
    expect_bool_dp(gang, "child_lock", str("ON"),     101, true);
    expect_bool_dp(gang, "child_lock", str("OFF"),    101, false);
    expect_bool_dp(gang, "child_lock", str("LOCK"),   101, true);
    // LOCK / UNLOCK belong to child_lock only.
    expect_refused(gang, "state_l1", str("LOCK"));
}

// A boolean datapoint with its own word table (kTuyaDpFlagBoolEnum) takes
// exactly those words. Alecto SMART-HEAT10 reports child_lock "LOCK"/"UNLOCK"
// and window_detection "ON"/"OFF"; z2m writes them with legacy converters
// that compare the word as written (`value === "LOCK"`), so "lock" is refused.
static void test_bool_enum_labels() {
    const std::span<const PreparedDefinition* const> reg(devices::alecto::kAlectoRegistry,
                                                         devices::alecto::kAlectoRegistryCount);
    const PreparedDefinition* heat10 = find_definition("TS0601", "_TZE200_8daqwrsj", reg);
    assert(heat10 && std::strcmp(heat10->model, "SMART-HEAT10") == 0);
    expect_bool_dp(*heat10, "child_lock", boolean(false), 7, false);   // unchanged
    expect_bool_dp(*heat10, "child_lock", str("LOCK"),    7, true);
    expect_bool_dp(*heat10, "child_lock", str("UNLOCK"),  7, false);
    expect_refused(*heat10, "child_lock", str("lock"));
    expect_refused(*heat10, "child_lock", str("ON"));   // not this datapoint's word
    expect_bool_dp(*heat10, "window_detection", str("ON"),  18, true);
    expect_bool_dp(*heat10, "window_detection", str("OFF"), 18, false);
    expect_refused(*heat10, "window_detection", str("LOCK"));
}

// The ZCL genOnOff path is not the datapoint path and keeps its frames.
static void test_zcl_on_off_unchanged() {
    const auto& plug = tuya_def("TS011F", "_TZ3000_okaz9tjs", devices::tuya::kDefTS011F_okaz9tjs);
    const struct { Value v; std::uint8_t cmd; } cases[] = {
        {str("ON"), 0x01}, {str("OFF"), 0x00}, {str("TOGGLE"), 0x02},
        {boolean(true), 0x01}, {boolean(false), 0x00},
    };
    for (const auto& c : cases) {
        const Sent s = send(plug, "state", c.v);
        const std::uint8_t want[] = {0x11, 0x00, c.cmd};
        assert(s.ok && s.cluster == 0x0006 && s.size == sizeof(want));
        assert(std::memcmp(s.frame, want, sizeof(want)) == 0);
    }
}

int main() {
    test_switch_words_match_booleans();
    test_multi_gang_words();
    test_other_strings_refused();
    test_child_lock_words();
    test_bool_enum_labels();
    test_zcl_on_off_unchanged();
    return 0;
}
