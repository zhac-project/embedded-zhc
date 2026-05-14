// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// T1.1: fz_tuya_datapoints meta-driven generic.
//
// Exit criterion from plan: port a pure-DP smart plug with
//   dp 1  = state
//   dp 18 = current (mA → A via /1000)
//   dp 19 = voltage (dV → V via /10)
// and prove every DP round-trips.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"

using namespace zhc;

namespace {

bool approx(float a, float b, float eps) { return std::fabs(a - b) <= eps; }

// Wire value bytes shared by several records.
const std::uint8_t bool_on[]  = { 0x01 };
const std::uint8_t bool_off[] = { 0x00 };
// 2000 big-endian = 0x000007D0
const std::uint8_t num_2000[]  = { 0x00, 0x00, 0x07, 0xD0 };
// 2300 big-endian = 0x000008FC
const std::uint8_t num_2300[]  = { 0x00, 0x00, 0x08, 0xFC };
// enum: 1
const std::uint8_t enum_1[]    = { 0x01 };

}  // namespace

static void test_pure_dp_plug() {
    // Device map — reused by all tests in this TU.
    static constexpr tuya::TuyaDpMapEntry kEntries[] = {
        { 1,  "state",   TuyaDpType::Bool,    1,    nullptr, 0 },
        { 18, "current", TuyaDpType::Numeric, 1000, nullptr, 0 },
        { 19, "voltage", TuyaDpType::Numeric, 10,   nullptr, 0 },
    };
    static constexpr tuya::TuyaDatapointMap kMap{ kEntries, 3 };

    // Build a FzConverter literal carrying our user_config.
    FzConverter cvt = tuya::kFzTuyaDatapoints;
    cvt.user_config = &kMap;

    const TuyaDpRecord recs[] = {
        { 1,  0x01, std::span<const std::uint8_t>(bool_on,  1) },
        { 18, 0x02, std::span<const std::uint8_t>(num_2000, 4) },
        { 19, 0x02, std::span<const std::uint8_t>(num_2300, 4) },
    };

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    DecodedMessage msg{};
    PreparedDefinition def{};
    RuntimeContext ctx{};

    assert(tuya::fz_tuya_datapoints(
        std::span<const TuyaDpRecord>(recs, 3),
        msg, cvt, def, ctx, out));

    const Value* s = out.find("state");
    assert(s && s->type == ValueType::Bool && s->b == true);

    const Value* cur = out.find("current");
    assert(cur && cur->type == ValueType::Float);
    assert(approx(cur->f, 2.0f, 0.001f));

    const Value* v = out.find("voltage");
    assert(v && v->type == ValueType::Float);
    assert(approx(v->f, 230.0f, 0.1f));
}

static void test_unknown_dp_silently_dropped() {
    static constexpr tuya::TuyaDpMapEntry kEntries[] = {
        { 1, "state", TuyaDpType::Bool, 1, nullptr, 0 },
    };
    static constexpr tuya::TuyaDatapointMap kMap{ kEntries, 1 };
    FzConverter cvt = tuya::kFzTuyaDatapoints;
    cvt.user_config = &kMap;

    const TuyaDpRecord recs[] = {
        { 1,   0x01, std::span<const std::uint8_t>(bool_off, 1) },
        { 99,  0x02, std::span<const std::uint8_t>(num_2000, 4) },  // unknown
    };

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    DecodedMessage msg{};
    PreparedDefinition def{};
    RuntimeContext ctx{};

    assert(tuya::fz_tuya_datapoints(
        std::span<const TuyaDpRecord>(recs, 2), msg, cvt, def, ctx, out));

    assert(out.find("state") != nullptr);
    // Unknown dp 99 must produce nothing; payload has exactly 1 entry.
    assert(out.count == 1);
}

static void test_enum_lookup() {
    static constexpr tuya::TuyaEnumEntry kModeTable[] = {
        { 0, "off" }, { 1, "cool" }, { 2, "heat" }, { 3, "auto" },
    };
    static constexpr tuya::TuyaDpMapEntry kEntries[] = {
        { 5, "mode", TuyaDpType::Enum, 1, kModeTable, 4 },
    };
    static constexpr tuya::TuyaDatapointMap kMap{ kEntries, 1 };
    FzConverter cvt = tuya::kFzTuyaDatapoints;
    cvt.user_config = &kMap;

    const TuyaDpRecord recs[] = {
        { 5, 0x04, std::span<const std::uint8_t>(enum_1, 1) },   // cool
    };

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    DecodedMessage msg{};
    PreparedDefinition def{};
    RuntimeContext ctx{};

    assert(tuya::fz_tuya_datapoints(
        std::span<const TuyaDpRecord>(recs, 1), msg, cvt, def, ctx, out));

    const Value* m = out.find("mode");
    assert(m && m->type == ValueType::StringRef);
    assert(std::strcmp(m->str, "cool") == 0);
}

static void test_no_user_config_returns_false() {
    FzConverter cvt = tuya::kFzTuyaDatapoints;  // user_config remains null
    const TuyaDpRecord recs[] = {
        { 1, 0x01, std::span<const std::uint8_t>(bool_on, 1) },
    };
    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    DecodedMessage msg{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(!tuya::fz_tuya_datapoints(
        std::span<const TuyaDpRecord>(recs, 1), msg, cvt, def, ctx, out));
}

// Multiple TuyaDpMapEntry rows can share a dp_id when the device reports
// several semantic fields off one wire DP (e.g. TRV60 DP36 carries both
// frost_protection and scale_protection). All matching entries must fire.
static void test_multi_row_fanout_same_dp() {
    static constexpr tuya::TuyaDpMapEntry kEntries[] = {
        { 36, "frost_protection", TuyaDpType::Bool, 1, nullptr, 0 },
        { 36, "scale_protection", TuyaDpType::Bool, 1, nullptr, 0 },
    };
    static constexpr tuya::TuyaDatapointMap kMap{ kEntries, 2 };
    FzConverter cvt = tuya::kFzTuyaDatapoints;
    cvt.user_config = &kMap;

    const TuyaDpRecord recs[] = {
        { 36, 0x01, std::span<const std::uint8_t>(bool_on, 1) },
    };
    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    DecodedMessage msg{};
    PreparedDefinition def{};
    RuntimeContext ctx{};

    assert(tuya::fz_tuya_datapoints(
        std::span<const TuyaDpRecord>(recs, 1), msg, cvt, def, ctx, out));

    const Value* a = out.find("frost_protection");
    const Value* b = out.find("scale_protection");
    assert(a && a->type == ValueType::Bool && a->b == true);
    assert(b && b->type == ValueType::Bool && b->b == true);
}

// Some devices reuse a single DP for two content-disjoint payloads — TRV26
// DP17 carries either open_window_time (4-byte numeric) or schedule_monday
// (12-byte schedule frame). emit_from_entry must abstain on shape mismatch
// so the sibling entry can claim the value.
static void test_schedule_overlap_content_disjoint() {
    static constexpr tuya::TuyaDpMapEntry kEntries[] = {
        { 17, "open_window_time", TuyaDpType::Numeric, 1, nullptr, 0, 0 },
        { 17, "schedule_monday",  TuyaDpType::Raw,     1, nullptr, 0,
            tuya::kTuyaDpFlagScheduleDay },
    };
    static constexpr tuya::TuyaDatapointMap kMap{ kEntries, 2 };
    FzConverter cvt = tuya::kFzTuyaDatapoints;
    cvt.user_config = &kMap;

    // A: short numeric frame — only open_window_time should emit.
    {
        const TuyaDpRecord recs[] = {
            { 17, 0x02, std::span<const std::uint8_t>(num_2000, 4) },
        };
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        DecodedMessage msg{};
        PreparedDefinition def{};
        RuntimeContext ctx{};
        assert(tuya::fz_tuya_datapoints(
            std::span<const TuyaDpRecord>(recs, 1), msg, cvt, def, ctx, out));
        const Value* o = out.find("open_window_time");
        assert(o && o->type == ValueType::Int && o->i == 2000);
        assert(out.find("schedule_monday") == nullptr);
    }

    // B: 12-byte schedule-day frame — only schedule_monday should emit.
    {
        const std::uint8_t sched12[] = {
            6,  0, 43,   // 06:00 / 21.5
            17, 20, 52,  // 17:20 / 26.0
            20, 0, 42,   // 20:00 / 21.0
            24, 0, 36,   // 24:00 / 18.0
        };
        const TuyaDpRecord recs[] = {
            { 17, 0x00, std::span<const std::uint8_t>(sched12, sizeof(sched12)) },
        };
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        DecodedMessage msg{};
        PreparedDefinition def{};
        RuntimeContext ctx{};
        assert(tuya::fz_tuya_datapoints(
            std::span<const TuyaDpRecord>(recs, 1), msg, cvt, def, ctx, out));
        const Value* s = out.find("schedule_monday");
        assert(s && s->type == ValueType::StringRef);
        assert(out.find("open_window_time") == nullptr);
    }
}

int main() {
    test_pure_dp_plug();
    test_unknown_dp_silently_dropped();
    test_enum_lookup();
    test_no_user_config_returns_false();
    test_multi_row_fanout_same_dp();
    test_schedule_overlap_content_disjoint();
    return 0;
}
