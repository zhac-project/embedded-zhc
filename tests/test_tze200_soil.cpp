// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Regression: Tuya soil-sensor temperature DP (dp 5) must decode as
// z2m's `tuya.valueConverter.raw` (/1), NOT /10. z2m source: tuya.ts
// `TS0601_soil` -> `[5, "temperature", tuya.valueConverter.raw]` (~line 4031).
// A raw 25 must surface as 25.0 C, not 2.5 C.

#include <cassert>
#include <cstdint>
#include <span>

#include "definitions/tuya/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefTZE200_soil;
extern const PreparedDefinition kDefTZE200_soil_ec;
extern const PreparedDefinition kDefTZE200_soil_th;
}  // namespace zhc::devices::tuya

using namespace zhc;

// Build the canonical Tuya-DP DecodedMessage + InboundApsFrame, run the
// def's DP map, and assert dp 5 (raw 25) decodes to temperature == 25.0 (/1).
static void check_soil_temperature_raw(const PreparedDefinition& def) {
    // dp 5, value-type 0x02 (4-byte big-endian int), raw = 25 (0x00000019).
    const std::uint8_t kTemp25[] = {0x00, 0x00, 0x00, 0x19};

    const TuyaDpRecord recs[] = {
        {5, 0x02, std::span<const std::uint8_t>(kTemp25, 4)},
    };

    DecodedMessage msg{};
    msg.family       = FrameFamily::TuyaDp;
    msg.type         = MessageType::Command;
    msg.cluster      = "manuSpecificTuya";
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;

    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>(recs, 1), def, raw, ctx);
    assert(result.any_matched);

    // With divisor /1 the decoder emits an exact integer (ValueType::Int),
    // mirroring z2m's `raw` integer JSON. The OLD /10 bug would instead emit
    // a Float of 2.5. So: type must be Int and value must be 25 (not 2.5).
    const Value* t = result.merged.find("temperature");
    assert(t && t->type == ValueType::Int);
    assert(t->i == 25);
    // Belt-and-braces: if the bug regressed to /10, type would be Float 2.5.
    assert(t->type != ValueType::Float);
}

int main() {
    check_soil_temperature_raw(devices::tuya::kDefTZE200_soil);
    check_soil_temperature_raw(devices::tuya::kDefTZE200_soil_ec);
    check_soil_temperature_raw(devices::tuya::kDefTZE200_soil_th);
    return 0;
}
