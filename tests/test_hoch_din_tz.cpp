// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Port-gap test for the HOCH DIN-rail RCBO (_TZE200_hkdl5fmv). z2m uses
// `legacy.toZigbee.hoch_din`; the ZHC parent def previously had
// `to_zigbee=nullptr`. This drives the real `kDefHochDin` write path and
// asserts the exact manuSpecificTuya (0xEF00 cmd setData) frames for each
// of the four round-trippable DPs:
//   state(DP1 bool)  countdown_timer(DP9 value)
//   child_lock(DP29 bool)  power_on_behavior(DP27 enum off/on/previous)

// Keep assert() live even under -DNDEBUG (Release).
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cstdint>
#include <cstring>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefHochDin;
}  // namespace zhc::devices::tuya

using namespace zhc;

namespace {

// Common header bytes for every setData frame:
//   fc=0x01 tsn=0x00 cmd=0x00 seq_be=0x00 0x01
//   then: dp_id, dp_type, len_be(2), value(N)

void test_state_bool_on() {
    const auto& def = devices::tuya::kDefHochDin;
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Bool; v.b = true;
    auto r = dispatch_to_zigbee(def, "state", v, ctx, frame);
    assert(r.ok);
    assert(r.cluster_id == 0xEF00);
    assert(r.command_id == 0x00);
    const std::uint8_t want[] = {
        0x01, 0x00, 0x00, 0x00, 0x01,  // fc tsn cmd seq
        0x01,                          // dp_id = 1 (state)
        0x01,                          // type Bool
        0x00, 0x01,                    // len = 1
        0x01,                          // value ON
    };
    assert(r.frame_size == sizeof(want));
    assert(std::memcmp(frame, want, r.frame_size) == 0);
}

void test_child_lock_bool_off() {
    const auto& def = devices::tuya::kDefHochDin;
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Bool; v.b = false;
    auto r = dispatch_to_zigbee(def, "child_lock", v, ctx, frame);
    assert(r.ok);
    const std::uint8_t want[] = {
        0x01, 0x00, 0x00, 0x00, 0x01,
        0x1D,                          // dp_id = 29 (child_lock)
        0x01,                          // type Bool
        0x00, 0x01,                    // len = 1
        0x00,                          // value OFF
    };
    assert(r.frame_size == sizeof(want));
    assert(std::memcmp(frame, want, r.frame_size) == 0);
}

void test_countdown_timer_value() {
    const auto& def = devices::tuya::kDefHochDin;
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    // 600 s → raw 600 (divisor 1) → big-endian s32 0x00000258.
    Value v{}; v.type = ValueType::Int; v.i = 600;
    auto r = dispatch_to_zigbee(def, "countdown_timer", v, ctx, frame);
    assert(r.ok);
    const std::uint8_t want[] = {
        0x01, 0x00, 0x00, 0x00, 0x01,
        0x09,                          // dp_id = 9 (countdown_timer)
        0x02,                          // type Numeric (s32 BE)
        0x00, 0x04,                    // len = 4
        0x00, 0x00, 0x02, 0x58,        // 600
    };
    assert(r.frame_size == sizeof(want));
    assert(std::memcmp(frame, want, r.frame_size) == 0);
}

void test_power_on_behavior_enum() {
    const auto& def = devices::tuya::kDefHochDin;
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::StringRef; v.str = "previous";
    auto r = dispatch_to_zigbee(def, "power_on_behavior", v, ctx, frame);
    assert(r.ok);
    const std::uint8_t want[] = {
        0x01, 0x00, 0x00, 0x00, 0x01,
        0x1B,                          // dp_id = 27 (hochRelayStatus)
        0x04,                          // type Enum (u8)
        0x00, 0x01,                    // len = 1
        0x02,                          // "previous" → 2
    };
    assert(r.frame_size == sizeof(want));
    assert(std::memcmp(frame, want, r.frame_size) == 0);
}

void test_unmapped_key_rejected() {
    const auto& def = devices::tuya::kDefHochDin;
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    // voltage is read-only — must not produce a write frame.
    Value v{}; v.type = ValueType::Float; v.f = 230.0f;
    auto r = dispatch_to_zigbee(def, "voltage", v, ctx, frame);
    assert(!r.ok);
}

}  // namespace

int main() {
    test_state_bool_on();
    test_child_lock_bool_off();
    test_countdown_timer_value();
    test_power_on_behavior_enum();
    test_unmapped_key_rejected();
    return 0;
}
