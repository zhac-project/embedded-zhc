// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// T2.1-2.3: helper constructors produce the right TuyaDpMapEntry
// shape and round-trip through fz_tuya_datapoints.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "zhc/runtime/dispatch.hpp"

using namespace zhc;

static bool approx(float a, float b, float e) { return std::fabs(a - b) <= e; }

static void test_helper_shapes() {
    constexpr auto n = tuya::dp::numeric(5, "power", 10);
    assert(n.dp_id == 5 && n.type == TuyaDpType::Numeric);
    assert(n.divisor == 10 && n.enum_table == nullptr);
    assert(std::strcmp(n.out_key, "power") == 0);

    constexpr auto b = tuya::dp::binary(1, "state");
    assert(b.dp_id == 1 && b.type == TuyaDpType::Bool);

    constexpr auto t = tuya::dp::temperature(102);
    assert(t.dp_id == 102 && t.divisor == 10);
    assert(std::strcmp(t.out_key, "temperature") == 0);

    static constexpr tuya::TuyaEnumEntry tab[] = { {0,"off"},{1,"on"} };
    constexpr auto e = tuya::dp::enum_lookup(7, "mode", tab, 2);
    assert(e.enum_table == tab && e.enum_count == 2);

    constexpr auto a = tuya::dp::action(4, tab, 2);
    assert(std::strcmp(a.out_key, "action") == 0);
    assert(a.type == TuyaDpType::Enum);
}

// End-to-end: build a map via helpers and decode a synthetic frame.
static void test_end_to_end_helpers() {
    static constexpr tuya::TuyaEnumEntry kAct[] = {
        {1, "single"}, {2, "double"},
    };
    static constexpr tuya::TuyaDpMapEntry kEntries[] = {
        tuya::dp::action(101, kAct, 2),
        tuya::dp::numeric(18, "current", 1000),
        tuya::dp::binary(1, "state"),
        tuya::dp::temperature(102),
    };
    static constexpr tuya::TuyaDatapointMap kMap{ kEntries, 4 };

    FzConverter cvt = tuya::kFzTuyaDatapoints;
    cvt.user_config = &kMap;

    const std::uint8_t one[]    = { 0x01 };
    const std::uint8_t two[]    = { 0x02 };
    const std::uint8_t num_c[]  = { 0x00, 0x00, 0x07, 0xD0 };  // 2000
    const std::uint8_t num_t[]  = { 0x00, 0x00, 0x00, 0xFB };  // 251

    const TuyaDpRecord recs[] = {
        { 1,   0x01, std::span<const std::uint8_t>(one,   1) },
        { 18,  0x02, std::span<const std::uint8_t>(num_c, 4) },
        { 101, 0x04, std::span<const std::uint8_t>(two,   1) },
        { 102, 0x02, std::span<const std::uint8_t>(num_t, 4) },
    };

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    DecodedMessage msg{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(tuya::fz_tuya_datapoints(
        std::span<const TuyaDpRecord>(recs, 4), msg, cvt, def, ctx, out));

    const Value* s = out.find("state");
    assert(s && s->type == ValueType::Bool && s->b == true);

    const Value* c = out.find("current");
    assert(c && c->type == ValueType::Float && approx(c->f, 2.0f, 0.001f));

    const Value* a = out.find("action");
    assert(a && a->type == ValueType::StringRef);
    assert(std::strcmp(a->str, "double") == 0);

    const Value* t = out.find("temperature");
    assert(t && t->type == ValueType::Float && approx(t->f, 25.1f, 0.05f));
}

int main() {
    test_helper_shapes();
    test_end_to_end_helpers();
    return 0;
}
