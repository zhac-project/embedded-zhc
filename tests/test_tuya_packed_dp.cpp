// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Packed-payload datapoint expanders (see TuyaDpExpandFn in
// definitions/tuya/_shared.hpp). These map ONE datapoint onto SEVERAL keys,
// and every one of them is byte-offset sensitive, so the layouts are pinned
// here against hand-built payloads.
//
// Covered:
//   * phaseVariant2WithPhase — 24-bit current/power reads and the
//     offset-encoded negative power. The 24-bit read is the whole point:
//     upstream's older narrow variant wraps current above 65.536 A, and the
//     high-current case below fails if the third byte is dropped.
//   * phaseVariant2 — the narrow variant, kept for parity. Pinned so nobody
//     "fixes" it into the wide one by accident.
//   * parseThresholds — 4-byte records, flag-only entries, unknown ids.
//   * circuitBreakerFaults1 — bit positions to a joined string.
//
// z2m-source: lib/tuya.ts valueConverter.phaseVariant2 /
//             phaseVariant2WithPhase / threshold_7 / threshold_8 /
//             circuitBreakerFaults1.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "zhc/runtime/dispatch.hpp"

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) { std::printf("  FAIL: %s\n", what); ++g_failures; }
}

bool approx(float a, float b, float eps = 0.001f) { return std::fabs(a - b) <= eps; }

float float_of(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& p, const char* key) {
    const Value* v = p.find(key);
    if (!v) return -1e9f;
    if (v->type == ValueType::Float) return v->f;
    if (v->type == ValueType::Int)   return static_cast<float>(v->i);
    return -1e9f;
}

std::int64_t int_of(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& p, const char* key) {
    const Value* v = p.find(key);
    if (!v) return INT64_MIN;
    if (v->type == ValueType::Int)  return v->i;
    if (v->type == ValueType::Uint) return static_cast<std::int64_t>(v->u);
    return INT64_MIN;
}

bool bool_of(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& p, const char* key, bool& out) {
    const Value* v = p.find(key);
    if (!v || v->type != ValueType::Bool) return false;
    out = v->b;
    return true;
}

const char* str_of(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& p, const char* key) {
    const Value* v = p.find(key);
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

// Run one record through the DP map machinery, as the real decode path does.
bool run(const tuya::TuyaDatapointMap& map, const TuyaDpRecord& rec,
         RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    FzConverter cvt = tuya::kFzTuyaDatapoints;
    cvt.user_config = &map;
    DecodedMessage msg{};
    PreparedDefinition def{};
    return tuya::fz_tuya_datapoints(std::span<const TuyaDpRecord>(&rec, 1),
                                     msg, cvt, def, ctx, out);
}

constexpr tuya::TuyaPhaseKeys kPhaseA{ "voltage_a", "current_a", "power_a" };

void test_phase_variant2_with_phase() {
    std::printf("phaseVariant2WithPhase\n");
    static constexpr tuya::TuyaDpMapEntry kE[] = {
        tuya::dp::phase_variant2_with_phase(6, &kPhaseA),
    };
    static constexpr tuya::TuyaDatapointMap kMap{ kE, 1 };

    // 230.0 V, 12.345 A, 2800 W.
    //   voltage be16 2300 = 0x08FC
    //   current be24 12345 = 0x003039
    //   power   be24 2800  = 0x000AF0
    {
        const std::uint8_t body[] = {0x08,0xFC, 0x00,0x30,0x39, 0x00,0x0A,0xF0};
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(kMap, {6, 0x00, std::span<const std::uint8_t>(body, 8)}, ctx, out),
              "decodes");
        check(approx(float_of(out, "voltage_a"), 230.0f), "voltage 230.0");
        check(approx(float_of(out, "current_a"), 12.345f), "current 12.345");
        check(int_of(out, "power_a") == 2800, "power 2800");
    }

    // 70 A — 70000 mA = 0x011170. A 16-bit read would give 0x1170 = 4.464 A,
    // which is exactly the wrap upstream fixed by widening this read.
    {
        const std::uint8_t body[] = {0x08,0xFC, 0x01,0x11,0x70, 0x00,0x00,0x64};
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(kMap, {6, 0x00, std::span<const std::uint8_t>(body, 8)}, ctx, out), "decodes");
        check(approx(float_of(out, "current_a"), 70.0f, 0.01f), "current 70.0 (not wrapped)");
    }

    // Negative power: the meter reports 0x19999A + power, NOT two's
    // complement. -100 W arrives as 0x199936.
    {
        const std::uint8_t body[] = {0x08,0xFC, 0x00,0x00,0x00, 0x19,0x99,0x36};
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(kMap, {6, 0x00, std::span<const std::uint8_t>(body, 8)}, ctx, out), "decodes");
        check(int_of(out, "power_a") == -100, "power -100 (offset-decoded)");
    }

    // A plausible large-but-positive power must NOT be treated as negative.
    // 0x0FFFFF = 1048575, one below the implausibility threshold.
    {
        const std::uint8_t body[] = {0x08,0xFC, 0x00,0x00,0x00, 0x0F,0xFF,0xFF};
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(kMap, {6, 0x00, std::span<const std::uint8_t>(body, 8)}, ctx, out), "decodes");
        check(int_of(out, "power_a") == 0x0FFFFF, "power stays positive below the threshold");
    }

    // Short payload must abstain rather than read past the end.
    {
        const std::uint8_t body[] = {0x08,0xFC, 0x00};
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        run(kMap, {6, 0x00, std::span<const std::uint8_t>(body, 3)}, ctx, out);
        check(out.find("voltage_a") == nullptr, "short payload emits nothing");
    }
}

void test_phase_variant2_narrow() {
    std::printf("phaseVariant2 (narrow, parity-preserving)\n");
    static constexpr tuya::TuyaDpMapEntry kE[] = {
        tuya::dp::phase_variant2(6, &tuya::kTuyaPhaseKeysPlain),
    };
    static constexpr tuya::TuyaDatapointMap kMap{ kE, 1 };

    // Same 70 A payload as above. This variant reads only b[3..4] = 0x1170,
    // so it yields 4.464 A. That is upstream's behaviour for this converter
    // and the test pins it deliberately — see the header.
    const std::uint8_t body[] = {0x08,0xFC, 0x01,0x11,0x70, 0x00,0x0A,0xF0};
    RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    check(run(kMap, {6, 0x00, std::span<const std::uint8_t>(body, 8)}, ctx, out), "decodes");
    check(approx(float_of(out, "voltage"), 230.0f), "voltage 230.0");
    check(approx(float_of(out, "current"), 4.464f), "current 4.464 (narrow read, as upstream)");
    check(int_of(out, "power") == 2800, "power 2800");
}

void test_thresholds() {
    std::printf("parseThresholds\n");
    static constexpr tuya::TuyaThresholdDef kDefs[] = {
        { 1, "over_current_alarm", "over_current_threshold" },
        { 5, "phase_loss_alarm",   nullptr                  },
    };
    static constexpr tuya::TuyaThresholdTable kTbl{ kDefs, 2 };
    static constexpr tuya::TuyaDpMapEntry kE[] = { tuya::dp::thresholds(18, &kTbl) };
    static constexpr tuya::TuyaDatapointMap kMap{ kE, 1 };

    // Three records: id 1 on/100, id 99 unknown, id 5 flag-only on.
    const std::uint8_t body[] = {
        0x01, 0x01, 0x00, 0x64,
        0x63, 0x01, 0x12, 0x34,
        0x05, 0x01, 0x00, 0x00,
    };
    RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    check(run(kMap, {18, 0x00, std::span<const std::uint8_t>(body, 12)}, ctx, out), "decodes");

    bool b = false;
    check(bool_of(out, "over_current_alarm", b) && b, "over_current_alarm ON");
    check(int_of(out, "over_current_threshold") == 100, "threshold 100 (be16)");
    check(bool_of(out, "phase_loss_alarm", b) && b, "phase_loss_alarm ON");
    check(out.find("phase_loss_threshold") == nullptr, "flag-only record emits no value");

    // An OFF flag must still be published — a cleared alarm is information.
    const std::uint8_t off[] = { 0x01, 0x00, 0x00, 0x00 };
    RuntimeContext ctx2{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out2{};
    check(run(kMap, {18, 0x00, std::span<const std::uint8_t>(off, 4)}, ctx2, out2), "decodes");
    check(bool_of(out2, "over_current_alarm", b) && !b, "over_current_alarm OFF");

    // A blob of only unknown ids emits nothing and abstains.
    const std::uint8_t unknown[] = { 0x63, 0x01, 0x00, 0x01 };
    RuntimeContext ctx3{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out3{};
    run(kMap, {18, 0x00, std::span<const std::uint8_t>(unknown, 4)}, ctx3, out3);
    check(out3.find("over_current_alarm") == nullptr, "unknown ids emit nothing");
}

void test_fault_bitmap() {
    std::printf("circuitBreakerFaults1\n");
    static constexpr const char* kNames[] = {
        "short_circuit", "surge", "overload", "leakage_current",
        "temperature", "fire", "high_power", "self_test",
        "over_current", "unbalance", "over_voltage", "under_voltage",
        "miss_phase", "outage", "negative_power", "credit", "no_balance",
    };
    static constexpr tuya::TuyaFaultTable kTbl{ kNames, 17 };
    static constexpr tuya::TuyaDpMapEntry kE[] = {
        tuya::dp::fault_bitmap(9, "faults", &kTbl),
    };
    static constexpr tuya::TuyaDatapointMap kMap{ kE, 1 };

    // Bits 0 and 14 → short_circuit + negative_power (bit 14 is "magnetism"
    // upstream, renamed in the device's table).
    const std::uint32_t bits = (1u << 0) | (1u << 14);
    const std::uint8_t body[] = {
        static_cast<std::uint8_t>(bits >> 24), static_cast<std::uint8_t>(bits >> 16),
        static_cast<std::uint8_t>(bits >> 8),  static_cast<std::uint8_t>(bits),
    };
    RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    check(run(kMap, {9, 0x05, std::span<const std::uint8_t>(body, 4)}, ctx, out), "decodes");
    const char* s = str_of(out, "faults");
    check(s && std::strcmp(s, "short_circuit,negative_power") == 0,
          "joined in bit order");

    // No bits set → empty string, so a fault CLEARING is observable.
    const std::uint8_t none[] = { 0x00, 0x00, 0x00, 0x00 };
    RuntimeContext ctx2{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out2{};
    check(run(kMap, {9, 0x05, std::span<const std::uint8_t>(none, 4)}, ctx2, out2), "decodes");
    const char* s2 = str_of(out2, "faults");
    check(s2 && s2[0] == '\0', "empty string when no faults");
}

}  // namespace

int main() {
    std::printf("== Tuya packed-payload datapoint expanders ==\n");
    test_phase_variant2_with_phase();
    test_phase_variant2_narrow();
    test_thresholds();
    test_fault_bitmap();
    if (g_failures) { std::printf("FAILED: %d check(s)\n", g_failures); return 1; }
    std::printf("all packed-DP checks passed\n");
    return 0;
}
