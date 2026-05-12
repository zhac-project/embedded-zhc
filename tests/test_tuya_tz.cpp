// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Unit tests for tz_tuya_datapoints — the write-path counterpart of
// fz_tuya_datapoints. Covers Bool + Numeric + Enum + invert-bool +
// negative-divisor (multiplier) paths.

#include <cassert>
#include <cstdint>
#include <cstring>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

constexpr tuya::TuyaEnumEntry kModeEnum[] = {
    { 0, "off" }, { 1, "on" }, { 2, "previous" },
};

constexpr tuya::TuyaDpMapEntry kEntries[] = {
    // dp_id, out_key, type, divisor, enum_table, enum_count, flags
    { 1,  "state",        TuyaDpType::Bool,    1, nullptr, 0, 0 },
    { 2,  "state_inv",    TuyaDpType::Bool,    1, nullptr, 0,
                          tuya::kTuyaDpFlagInvertBool },
    { 10, "current",      TuyaDpType::Numeric, 1000, nullptr, 0, 0 },
    { 11, "big_scaled",   TuyaDpType::Numeric, -10, nullptr, 0, 0 },
    { 20, "mode",         TuyaDpType::Enum,    1,
                          kModeEnum, sizeof(kModeEnum) / sizeof(kModeEnum[0]),
                          0 },
};
constexpr tuya::TuyaDatapointMap kMap{ kEntries, 5 };

constexpr TzConverter kTz{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &tuya::tz_tuya_datapoints,
    .user_config = &kMap,
};

const TzConverter* const kTzArr[] = { &kTz };

PreparedDefinition make_def() {
    PreparedDefinition d{};
    d.to_zigbee       = kTzArr;
    d.to_zigbee_count = 1;
    return d;
}

}  // namespace

static void test_bool_on() {
    auto def = make_def();
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Bool; v.b = true;
    auto r = dispatch_to_zigbee(def, "state", v, ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0xEF00);
    assert(r.command_id == 0x00);
    // fc=0x01 tsn=0x00 cmd=0x00 seq=0x0001 dp=1 type=1(Bool) len=0x0001 val=0x01
    const std::uint8_t want[] = {
        0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01,
    };
    assert(r.frame_size == sizeof(want));
    assert(std::memcmp(frame, want, r.frame_size) == 0);
}

static void test_bool_inverted_writes_zero_for_true() {
    auto def = make_def();
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Bool; v.b = true;
    auto r = dispatch_to_zigbee(def, "state_inv", v, ctx, frame);
    assert(r.ok);
    assert(frame[r.frame_size - 1] == 0x00);   // inverted → 0
}

static void test_numeric_divisor_reverse() {
    auto def = make_def();
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    // fz divides by 1000 → here we multiply by 1000 going out.
    // current=2.0 A → raw 2000 → big-endian 0x000007D0.
    Value v{}; v.type = ValueType::Float; v.f = 2.0f;
    auto r = dispatch_to_zigbee(def, "current", v, ctx, frame);
    assert(r.ok);
    // last 4 bytes are the value.
    const std::uint8_t* val = frame + r.frame_size - 4;
    assert(val[0] == 0x00 && val[1] == 0x00 && val[2] == 0x07 && val[3] == 0xD0);
}

static void test_numeric_multiplier_reverse() {
    auto def = make_def();
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    // divisor -10 on fz multiplies by 10; tz must divide by 10.
    // Input 250 → raw 25 → 0x00000019.
    Value v{}; v.type = ValueType::Int; v.i = 250;
    auto r = dispatch_to_zigbee(def, "big_scaled", v, ctx, frame);
    assert(r.ok);
    const std::uint8_t* val = frame + r.frame_size - 4;
    assert(val[0] == 0x00 && val[1] == 0x00 && val[2] == 0x00 && val[3] == 0x19);
}

static void test_enum_label_lookup() {
    auto def = make_def();
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::StringRef; v.str = "previous";
    auto r = dispatch_to_zigbee(def, "mode", v, ctx, frame);
    assert(r.ok);
    // Enum payload: 1 byte = mapped value (2 for "previous").
    assert(frame[r.frame_size - 1] == 0x02);
}

static void test_unknown_key_no_match() {
    auto def = make_def();
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Bool; v.b = true;
    auto r = dispatch_to_zigbee(def, "not_in_map", v, ctx, frame);
    assert(!r.ok);
}

int main() {
    test_bool_on();
    test_bool_inverted_writes_zero_for_true();
    test_numeric_divisor_reverse();
    test_numeric_multiplier_reverse();
    test_enum_label_lookup();
    test_unknown_key_no_match();
    return 0;
}
