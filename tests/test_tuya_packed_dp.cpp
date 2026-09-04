// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Packed-payload datapoint expanders (see TuyaDpExpandFn in
// definitions/tuya/_shared.hpp). These map ONE datapoint onto SEVERAL keys,
// and every one of them is byte-offset sensitive, so the layouts are pinned
// here against hand-built payloads.
//
// Covered:
//   * phaseVariant2WithPhase — 16-bit current/power reads and the
//     offset-encoded negative power. z2m v26.97.0 widened the reads to 24
//     bits (#12928) and v26.105.0 reverted that (52542ec); the port follows
//     upstream, so the 70 A case below pins the WRAP, not the wide read.
//   * phaseVariant2 — the plain variant, same narrow reads, no sign handling.
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

namespace zhc::devices::tuya {
extern const PreparedDefinition kDef_ZBN_DJ_63;
}  // namespace zhc::devices::tuya

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

    // 70 A — 70000 mA = 0x011170. Only b[3..4] = 0x1170 is read, so this
    // decodes as 4.464 A. Upstream briefly read the third byte (v26.97.0 ..
    // v26.104.0) and reverted; the wrap is pinned so the port tracks upstream
    // and nobody re-widens it by accident.
    {
        const std::uint8_t body[] = {0x08,0xFC, 0x01,0x11,0x70, 0x00,0x00,0x64};
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(kMap, {6, 0x00, std::span<const std::uint8_t>(body, 8)}, ctx, out), "decodes");
        check(approx(float_of(out, "current_a"), 4.464f, 0.001f), "current 4.464 (16-bit read, as upstream)");
    }

    // Negative power: a reading above 0x7FFF is 0x999A - power, NOT two's
    // complement. -100 W arrives as 0x9936.
    {
        const std::uint8_t body[] = {0x08,0xFC, 0x00,0x00,0x00, 0x00,0x99,0x36};
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(kMap, {6, 0x00, std::span<const std::uint8_t>(body, 8)}, ctx, out), "decodes");
        check(int_of(out, "power_a") == -100, "power -100 (offset-decoded)");
    }

    // The sign branch is taken strictly above 0x7FFF: 32767 W stays
    // positive, 0x8000 decodes as 0x8000 - 0x999A = -6554.
    {
        const std::uint8_t body[] = {0x08,0xFC, 0x00,0x00,0x00, 0x00,0x7F,0xFF};
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(kMap, {6, 0x00, std::span<const std::uint8_t>(body, 8)}, ctx, out), "decodes");
        check(int_of(out, "power_a") == 0x7FFF, "power 32767 stays positive");
        const std::uint8_t body2[] = {0x08,0xFC, 0x00,0x00,0x00, 0x00,0x80,0x00};
        RuntimeContext c2{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o2{};
        check(run(kMap, {6, 0x00, std::span<const std::uint8_t>(body2, 8)}, c2, o2), "decodes");
        check(int_of(o2, "power_a") == -6554, "power 0x8000 -> -6554");
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

    // Same 70 A payload as above: b[3..4] = 0x1170 -> 4.464 A. Unlike the
    // WithPhase variant there is no negative-power branch here.
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

// ── ZBN-DJ-63, end to end ────────────────────────────────────────────
//
// The breaker is the device that drove these expanders, so its own three
// packed datapoints are pinned against the real definition rather than a
// synthetic map.

void test_zbn_dj_63() {
    std::printf("ZBN-DJ-63 packed datapoints\n");
    const PreparedDefinition& def = ::zhc::devices::tuya::kDef_ZBN_DJ_63;

    // Locate the device's own DP map through its Fz list, so the test binds
    // to what the definition actually ships.
    const tuya::TuyaDatapointMap* map = nullptr;
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i) {
        const FzConverter* c = def.from_zigbee[i];
        if (c && c->family == FrameFamily::TuyaDp && c->user_config) {
            map = static_cast<const tuya::TuyaDatapointMap*>(c->user_config);
            break;
        }
    }
    check(map != nullptr, "definition ships a datapoint map");
    if (!map) return;

    // dp6 — the NARROW phaseVariant2. 230.0 V, current from b[3..4] only,
    // power from b[6..7]. Pinned to prove the device keeps upstream's narrow
    // read rather than silently getting the wide one.
    {
        const std::uint8_t body[] = {0x08,0xFC, 0x00,0x04,0xD2, 0x00,0x09,0x60};
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(*map, {6, 0x00, std::span<const std::uint8_t>(body, 8)}, ctx, out), "dp6 decodes");
        check(approx(float_of(out, "voltage"), 230.0f), "voltage 230.0");
        check(approx(float_of(out, "current"), 1.234f), "current 1.234");
        check(int_of(out, "power") == 2400, "power 2400");
    }

    // dp9 — fault is a LOOKUP over single-bit values, not a bitmap. 1<<3
    // is the leakage alarm; a two-bit value matches nothing, which is
    // upstream's behaviour and the reason this is worth pinning.
    {
        const std::uint8_t one[] = { 0x08 };          // 1 << 3
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(*map, {9, 0x04, std::span<const std::uint8_t>(one, 1)}, ctx, out), "dp9 decodes");
        const char* f = str_of(out, "fault");
        check(f && std::strcmp(f, "leakagecurr_alarm") == 0, "fault leakagecurr_alarm");

        const std::uint8_t clear[] = { 0x00 };
        RuntimeContext c2{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o2{};
        run(*map, {9, 0x04, std::span<const std::uint8_t>(clear, 1)}, c2, o2);
        const char* f2 = str_of(o2, "fault");
        check(f2 && std::strcmp(f2, "clear") == 0, "fault clear");

        const std::uint8_t multi[] = { 0x0C };        // two bits set
        RuntimeContext c3{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> o3{};
        run(*map, {9, 0x04, std::span<const std::uint8_t>(multi, 1)}, c3, o3);
        check(o3.find("fault") == nullptr, "multi-bit fault matches nothing (as upstream)");
    }

    // dp17 — threshold_2: overload(3) / leakage(4) / high_temperature(5).
    {
        const std::uint8_t body[] = {
            0x04, 0x01, 0x00, 0x1E,   // leakage on, 30 mA
            0x05, 0x00, 0x00, 0x50,   // high temperature off, 80 C
        };
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(*map, {17, 0x00, std::span<const std::uint8_t>(body, 8)}, ctx, out), "dp17 decodes");
        bool b = false;
        check(bool_of(out, "leakage_breaker", b) && b, "leakage_breaker ON");
        check(int_of(out, "leakage_threshold") == 30, "leakage_threshold 30");
        check(bool_of(out, "high_temperature_breaker", b) && !b, "high_temperature_breaker OFF");
        check(int_of(out, "high_temperature_threshold") == 80, "high_temperature_threshold 80");
    }

    // dp18 — threshold_3: over_current(1) / over_voltage(3) /
    // under_voltage(4) / insufficient_balance(8).
    {
        const std::uint8_t body[] = {
            0x01, 0x01, 0x00, 0x3F,   // over current on, 63 A
            0x03, 0x01, 0x01, 0x04,   // over voltage on, 260 V
            0x04, 0x01, 0x00, 0xC3,   // under voltage on, 195 V
        };
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(*map, {18, 0x00, std::span<const std::uint8_t>(body, 12)}, ctx, out), "dp18 decodes");
        check(int_of(out, "over_current_threshold") == 63, "over_current_threshold 63");
        check(int_of(out, "over_voltage_threshold") == 260, "over_voltage_threshold 260 (be16)");
        check(int_of(out, "under_voltage_threshold") == 195, "under_voltage_threshold 195");
    }

    // relay_power_on_state labels — the first port had off/on/memory, which
    // do not match upstream's {Off, On, Restore}.
    {
        const std::uint8_t two[] = { 0x02 };
        RuntimeContext ctx{}; FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
        check(run(*map, {134, 0x04, std::span<const std::uint8_t>(two, 1)}, ctx, out), "dp134 decodes");
        const char* v = str_of(out, "relay_power_on_state");
        check(v && std::strcmp(v, "Restore") == 0, "relay_power_on_state Restore");
    }
}

}  // namespace

int main() {
    std::printf("== Tuya packed-payload datapoint expanders ==\n");
    test_phase_variant2_with_phase();
    test_phase_variant2_narrow();
    test_thresholds();
    test_fault_bitmap();
    test_zbn_dj_63();
    if (g_failures) { std::printf("FAILED: %d check(s)\n", g_failures); return 1; }
    std::printf("all packed-DP checks passed\n");
    return 0;
}
